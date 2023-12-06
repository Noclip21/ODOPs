#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>
#include <cgcompute>

#include <limits>
#include <chrono>

#include "Mesh.h"


////////////////////////////////////////////////////////////////////////////////
const VertStream* getScene(const std::string& filepath)
{
	static std::map<std::string,VertStream*> scenes;
	if(scenes.find(filepath) == scenes.end())
		scenes[filepath] = new VertStream(Scene(filepath.c_str()),(const int[]){VertStream::COORDS,3,0});
	return scenes[filepath];
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,typename VP,uint R>
const BVH<V,R>* getBVH(const std::string& filepath)
{
	static std::map<std::string,BVH<V,R>*> bvhs;
	if(bvhs.find(filepath) == bvhs.end())
	{
		const VertStream& vs = *getScene(filepath);

		TriangleIndicesArray layout;
		layout.data = (vec3*)&vs.vertexArray[0];
		layout.indices = (ivec3*)&vs.indices[0];
		layout.count = (uint)vs.indices.size()/3;

		bvhs[filepath] = new BVH<V,R>();
		*bvhs[filepath] = bvhs[filepath]->template partitionedBuild<VP>(layout,32);
	}
	return bvhs[filepath];
}
////////////////////////////////////////////////////////////////////////////////
float bvhSAH(const auto& bvh)
{
	float sah = 0;
	for(const auto&node: bvh.nodes)
		sah += node.volume.SAH();
	return sah;
}
////////////////////////////////////////////////////////////////////////////////
template<uint N>
const vec3 samples(const uint i)
{
	static vec3 s[N];
	static bool once = true;
	for(uint i=0; once && i<N; ++i)
		s[i] = cosineDistribution(genSample2());
	once = false;
	return s[i];
}
////////////////////////////////////////////////////////////////////////////////
void initDevice()
{
	static bool once = true;
	if(once)
	{
		once = false;
		static int layout[] =
		{
			Device::DEBUG_GLVERSION,
			Device::DEBUG_GLSLVERSION,
			Device::DEBUG_GLVERBOSE,Device::GLVERBOSE_LOW,
			0,
		};
		static Device device;
		device.createWindow("test",layout);
		device.bindContext();
		device.init(layout);
	}
}
////////////////////////////////////////////////////////////////////////////////
const Shader* getShaderAABB()
{
	initDevice();
	static Shader* shader = nullptr;
	if(!shader)
		shader = new Shader(true,"res/traceAABB.comp");
	return shader;
}
////////////////////////////////////////////////////////////////////////////////
const Shader* getShaderOBB()
{
	initDevice();
	static Shader* shader = nullptr;
	if(!shader)
		shader = new Shader(true,"res/traceOBB.comp");
	return shader;
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,typename VP,uint R>
const Bitmap* getHitsGPU(const std::string& filepath)
{
	static std::map<std::string,Bitmap*> hits;
	if(hits.find(filepath) == hits.end())
	{
		const auto& bvh = *getBVH<V,VP,R>(filepath);

		const std::string filePos = filepath+"-position0001.exr";
		const std::string fileNormal = filepath+"-normal0001.exr";

		const Bitmap bitmapPos(filePos.c_str(),Bitmap::RGB32F);
		const Bitmap bitmapNormal(fileNormal.c_str(),Bitmap::RGB32F);

		const Shader* shader;
		if(std::is_same_v<V,AABB>)
			shader = getShaderAABB();
		else
			shader = getShaderOBB();
		shader->bind();

		std::vector<uvec3> nodes(bvh.nodes.size());
		std::vector<V>     volumes(bvh.nodes.size());
		for(uint i=0; i<nodes.size(); ++i)
		{
			nodes[i]   = uvec3(bvh.nodes[i].parent,bvh.nodes[i].left,bvh.nodes[i].right);
			volumes[i] = bvh.nodes[i].volume;
			if constexpr(std::is_same_v<V,OBB>)
				*(mat4x3*)(&volumes[i].m) = transpose(volumes[i].m);
		}

		const uint width = bitmapPos.width,height = bitmapPos.height;
		struct Vert { vec3 p,n; };
		std::vector<Vert> verts(width*height);
		for(uint i=0; i<width; ++i)
			for(uint j=0; j<height; ++j)
			{
				const vec3 p = *(const vec3*)bitmapPos(i,j);
				const vec3 n = normalize(*(const vec3*)bitmapNormal(i,j));
				verts[i+width*j] = {p,n};
			}

		static const uint nSamples = 1024;//4096;
		std::vector<vec3> samples3d(nSamples);
		for(uint i=0; i<nSamples; ++i)
			samples3d[i] = normalize(samples<nSamples>(i));
		
		Buffer bufferOut(NULL,sizeof(float)*verts.size(),Buffer::MAP_PERSISTENT);
		Buffer bufferVerts(&verts[0],sizeof(Vert)*verts.size());
		Buffer bufferSamples(&samples3d[0],sizeof(vec3)*samples3d.size());
		Buffer bufferNodes(&nodes[0],sizeof(uvec3)*nodes.size());
		Buffer bufferVolumes(&volumes[0],sizeof(V)*volumes.size());
		bufferOut.bind(1);
		bufferVerts.bind(2);
		bufferSamples.bind(3);
		bufferNodes.bind(4);
		bufferVolumes.bind((std::is_same_v<V,AABB>)?5:6);

    	GLuint timerQuery;
    	glGenQueries(1,&timerQuery);
        glBeginQuery(GL_TIME_ELAPSED,timerQuery);

		float* p;
		p = (float*)bufferOut.map();
		for(uint i=0; i<verts.size(); ++i) p[i] = 0;
		bufferOut.unmap();

		shader->uniform1i(shader->getUniform("nSamples"),1);
		for(uint i=0; i<nSamples; i += 1)
		{
			shader->uniform1i(shader->getUniform("sampleOffset"),i);
			glDispatchCompute(verts.size(),1,1);
    		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    	}

        glEndQuery(GL_TIME_ELAPSED);

    	int done = 0;
    	GLuint64 timer;
    	while(!done)
        	glGetQueryObjectiv(timerQuery,GL_QUERY_RESULT_AVAILABLE,&done);
    	glGetQueryObjectui64v(timerQuery,GL_QUERY_RESULT,&timer);

    	float timef = (timer/1000000.0f);
		std::cout << "GPU nSamples = " << nSamples << std::endl;
		std::cout << "GPU timer = " << timef << "ms" << std::endl;

    	glDeleteQueries(1,&timerQuery);

		Bitmap bitmap(width,height,Bitmap::RGB32F);
		p = (float*)bufferOut.map();
		for(uint i=0; i<width; ++i)
			for(uint j=0; j<height; ++j)
			{
				const vec3 color = ceil(p[i+width*j]/nSamples);
				bitmap(&color,i,j);
			}
		bufferOut.unmap();

		hits[filepath] = new Bitmap(bitmap);
	}
	return hits[filepath];
}
////////////////////////////////////////////////////////////////////////////////
const std::tuple<float,float,float,float> distribution(const Bitmap& bitmap)
{
	float meanV = 0;
	float minV = +std::numeric_limits<float>::infinity(),
		  maxV = -std::numeric_limits<float>::infinity();
	for(uint i=0; i<bitmap.width*bitmap.height; ++i)
	{
		float hit = (*(const vec3*)bitmap[i]).x;
		minV = min(minV,hit);
		maxV = max(maxV,hit);
		meanV += hit;
	}
	meanV /= bitmap.width*bitmap.height;

	double devV = 0;
	for(uint i=0; i<bitmap.width*bitmap.height; ++i)
	{
		float hit = (*(const vec3*)bitmap[i]).x;
		devV += (hit-meanV)*(hit-meanV);
	}
	devV = sqrt(devV/(bitmap.width*bitmap.height));

	return {minV,maxV,meanV,float(devV)};
}
////////////////////////////////////////////////////////////////////////////////
const Bitmap heatmap(Bitmap bitmap,const float minV,const float maxV,const vec3& rc,const vec3& gc,const vec3& bc)
{
	for(uint i=0; i<bitmap.width*bitmap.height; ++i)
	{
		float v = (*(const vec3*)bitmap[i]).x;
    	float t = clamp(2*(v-minV)/(maxV-minV)-1,-1,1);
		vec3 color = (t<0)?mix(bc,gc,vec3(1+t)):mix(gc,rc,vec3(t));
    	bitmap(&color,i);
	}

	return bitmap;
}
////////////////////////////////////////////////////////////////////////////////
const Bitmap difference(const Bitmap& a,const Bitmap& b)
{
	Bitmap c(a.width,a.height,Bitmap::RGB32F);

	for(uint i=0; i<c.width*c.height; ++i)
	{
		float va = (*(const vec3*)a[i]).x;
		float vb = (*(const vec3*)b[i]).x;

		vec3 color(va-vb);
		c(&color,i);
	}

	return c;
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,typename VP,uint R>
void profile(const std::string& filepath)
{
	getScene(filepath);
	std::chrono::steady_clock::time_point buildBegin = std::chrono::steady_clock::now();
	const auto& bvh = *getBVH<V,VP,R>(filepath);
	std::chrono::steady_clock::time_point buildEnd = std::chrono::steady_clock::now();
	std::cout << "===========" << std::endl;
	std::cout << "model = " << filepath << std::endl;
	std::cout << "primitives = " << bvh.nodes.size()-1 << std::endl;
	std::string volumestr;
	if(std::is_same_v<VP,AABB>) volumestr = "AABB";
	if(std::is_same_v<VP,KDOP<1>>) volumestr = "1-ODOP";
	if(std::is_same_v<VP,KDOP<2>>) volumestr = "2-ODOP";
	if(std::is_same_v<VP,KDOP<3>>) volumestr = "3-ODOP";
	if(std::is_same_v<VP,KDOP<4>>) volumestr = "4-ODOP";
	if(std::is_same_v<VP,KDOP<5>>) volumestr = "5-ODOP";
	std::cout << "BVH type = " << volumestr << std::endl;
	std::cout << volumestr << " size = " << sizeof(VP) << " bytes" << std::endl;
	std::cout << "BVH size = " << bvh.nodes.size()*sizeof(VP) << " bytes" << std::endl;
	std::cout << "BVH build time = " << std::chrono::duration_cast<std::chrono::milliseconds>(buildEnd - buildBegin).count() << "ms" << std::endl;
	std::cout << "SAH = " << bvhSAH(bvh) << std::endl;
	std::chrono::steady_clock::time_point traceBegin = std::chrono::steady_clock::now();
	const Bitmap& bitmap = *getHitsGPU<V,VP,R>(filepath);
	std::chrono::steady_clock::time_point traceEnd = std::chrono::steady_clock::now();
	auto [minHits,maxHits,meanHits,devHits] = distribution(bitmap);
	std::cout << "min hits = " << minHits << std::endl;
	std::cout << "max hits = " << maxHits << std::endl;
	std::cout << "mean hits = " << meanHits << std::endl;
	std::cout << "standard deviation = " << devHits << std::endl;
	std::cout << "BVH trace time = " << std::chrono::duration_cast<std::chrono::milliseconds>(traceEnd - traceBegin).count() << "ms" << std::endl;

	const Bitmap& bitmapAABB = *getHitsGPU<AABB,AABB,R>(filepath);
	auto [minHitsAABB,maxHitsAABB,meanHitsAABB,devHitsAABB] = distribution(bitmapAABB);

	heatmap(bitmap,
		max(minHitsAABB,meanHitsAABB-devHitsAABB),
		min(maxHitsAABB,meanHitsAABB+devHitsAABB),
		vec3(1,0,0),vec3(0,1,0),vec3(0,0,1))
		.write((filepath+std::string("-heatmap-")+volumestr+std::string(".png")).c_str(),Bitmap::RGB8);

	if constexpr (std::is_same_v<V,AABB>) return;

	Bitmap bitmapDifference = difference(bitmap,bitmapAABB);
	auto [minDifference,maxDifference,meanDifference,devDifference] =
		distribution(bitmapDifference);
	std::cout << "comparison between AABB and " << volumestr << " for model = " << filepath << std::endl;
	std::cout << "min difference = " << minDifference << std::endl;
	std::cout << "max difference = " << maxDifference << std::endl;
	std::cout << "mean difference = " << meanDifference << std::endl;
	std::cout << "standard deviation = " << devDifference << std::endl;

	heatmap(bitmapDifference,
		max(minDifference,meanDifference-devDifference),
		min(maxDifference,meanDifference+devDifference),
		vec3(0.34,0,0.1),vec3(1,1,1),vec3(0.1,0,0.34))
		.write((filepath+std::string("-heatmapDifference-AABB-")+volumestr+std::string(".png")).c_str(),Bitmap::RGB8);

	std::cout << "===========" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const std::vector<std::string> models = {
		"./res/cornellbox.obj",
		"./res/rungholt.obj",
		"./res/sanmiguel.obj",
		"./res/bistro.obj",
		"./res/bunny.obj",
		"./res/hairball.obj",
	};

	for(auto model: models)
	{
		profile<AABB,AABB,8>(model);
		profile<OBB,KDOP<1>,8>(model);
		profile<OBB,KDOP<2>,8>(model);
		profile<OBB,KDOP<3>,8>(model);
		profile<OBB,KDOP<4>,8>(model);
	}

	return 0;
}
