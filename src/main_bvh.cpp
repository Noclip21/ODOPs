#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>
#include <limits>
#include <chrono>

#include "Mesh.h"


////////////////////////////////////////////////////////////////////////////////
const Scene* getScene(const std::string& filepath)
{
	static std::map<std::string,Scene*> scenes;
	if(scenes.find(filepath) == scenes.end())
		scenes[filepath] = new Scene(filepath.c_str());
	return scenes[filepath];
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr bool is_KDOP(typename T::StaticData*){ return true; };
template<typename T>
constexpr bool is_KDOP(...){ return false; };
////////////////////////////////////////////////////////////////////////////////
template<typename T>
using BVH_t = std::conditional_t<is_KDOP<typename T::Volume>(0),BVH<OBB,T::radius>,T>;
////////////////////////////////////////////////////////////////////////////////
template<typename BVH>
const BVH_t<BVH>* getBVH(const std::string& filepath)
{
	static std::map<std::string,BVH_t<BVH>*> bvhs;
	if(bvhs.find(filepath) == bvhs.end())
	{
		using Volume = typename BVH::Volume;
		std::vector<Volume> volumes;
		const Scene& scene = *getScene(filepath);
		for(auto& m: scene.meshes)
		{
			for(uint i=0; i<m.indices.size();)
			{
				if(m.type == Scene::Mesh::Type::TRIANGLES)
				{
					Triangle t(
						vec3(scene.vertices[m.indices[i  ]].position),
						vec3(scene.vertices[m.indices[i+1]].position),
						vec3(scene.vertices[m.indices[i+2]].position));

					volumes.push_back(Volume(t));

					i += 3;
				}else if(m.type == Scene::Mesh::Type::LINES)
				{
					const ivec2* lines = (const ivec2*)&m.indices[0];

					uint j = i+1;
					if(j >= m.indices.size()/2) break;
					while(lines[j].x == lines[j-1].y)
						if(++j >= m.indices.size()/2) break;

					std::vector<vec3> lineVerts(j-i+1);
					for(uint k=0; k<lineVerts.size()-1; ++k)
						lineVerts[k] = vec3(scene.vertices[lines[i+k].x].position);
					lineVerts[j-i] = vec3(scene.vertices[lines[j-1].y].position);

					volumes.push_back(Volume(&lineVerts[0],lineVerts.size()));

					i = j;
				}
			}
		}

		const PrimitiveArrayLayout<Volume> primLayout = {
			.data = &volumes[0],
			.count = (uint)volumes.size(),
		};
		if constexpr (is_KDOP<Volume>(0))
			bvhs[filepath] = new BVH_t<BVH>(BVH(primLayout));
		else
			bvhs[filepath] = new BVH(primLayout);
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
template<typename BVH>
const Bitmap* getHits(const std::string& filepath)
{
	static std::map<std::string,Bitmap*> hits;
	if(hits.find(filepath) == hits.end())
	{
		const BVH_t<BVH>& bvh = *getBVH<BVH>(filepath);

		const std::string filePos = filepath+"-position0001.exr";
		const std::string fileNormal = filepath+"-normal0001.exr";

		const Bitmap bitmapPos(filePos.c_str(),Bitmap::RGB32F);
		const Bitmap bitmapNormal(fileNormal.c_str(),Bitmap::RGB32F);

		Bitmap bitmap = bitmapPos;
		Compute compute(bitmap.width,bitmap.height);
		compute.dispatch([&](const ComputeInput<2>& input)
		{
			const ivec2 coords(input.id[0],input.id[1]);

			const vec3 p = *(const vec3*)bitmapPos(coords.x,coords.y);
			const vec3 n = *(const vec3*)bitmapNormal(coords.x,coords.y);;

			float hits = 0;
			static const uint nSamples = 32;//1024;
			for(uint i=0; i<nSamples; ++i)
			{
				vec3 d = normalize(hemisphereSample(samples<nSamples>(i),normalize(n)));
				Ray ray;
				ray.o = p+n*5e-5;
				ray.d = d;
				bvh.traverse(
					[&](const auto& volume,const int id)->bool
					{
						bool hit = intersect<Hitb>(ray,volume).isHit;
						if(hit) hits++;
						return hit;
					},[&](const int id)->bool { return true; });
			}
			hits = ceil(hits/float(nSamples));

			const vec3 color = hits;
			bitmap(&color,coords.x,coords.y);
		});
		compute.sync();

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
template<typename BVH_T>
void profile(const std::string& filepath)
{
	getScene(filepath);
	std::chrono::steady_clock::time_point buildBegin = std::chrono::steady_clock::now();
	const BVH_t<BVH_T>& bvh = *getBVH<BVH_T>(filepath);
	std::chrono::steady_clock::time_point buildEnd = std::chrono::steady_clock::now();
	std::cout << "===========" << std::endl;
	std::cout << "model = " << filepath << std::endl;
	std::cout << "primitives = " << bvh.nodes.size()-1 << std::endl;
	using Volume = typename BVH_T::Volume;
	std::string volumestr;
	if(std::is_same_v<Volume,AABB>) volumestr = "AABB";
	if(std::is_same_v<Volume,OBB>) volumestr = "OBB-PCA";
	if(std::is_same_v<Volume,KDOP<1>>) volumestr = "OBB-KDOP<1>";
	if(std::is_same_v<Volume,KDOP<2>>) volumestr = "OBB-KDOP<2>";
	if(std::is_same_v<Volume,KDOP<3>>) volumestr = "OBB-KDOP<3>";
	if(std::is_same_v<Volume,KDOP<4>>) volumestr = "OBB-KDOP<4>";
	if(std::is_same_v<Volume,KDOP<5>>) volumestr = "OBB-KDOP<5>";
	std::cout << "BVH type = " << volumestr << std::endl;
	std::cout << volumestr << " size = " << sizeof(Volume) << " bytes" << std::endl;
	std::cout << "BVH size = " << bvh.nodes.size()*sizeof(Volume) << " bytes" << std::endl;
	std::cout << "BVH build time = " << std::chrono::duration_cast<std::chrono::milliseconds>(buildEnd - buildBegin).count() << "ms" << std::endl;
	std::cout << "SAH = " << bvhSAH(bvh) << std::endl;
	std::chrono::steady_clock::time_point traceBegin = std::chrono::steady_clock::now();
	const Bitmap& bitmap = *getHits<BVH_T>(filepath);
	std::chrono::steady_clock::time_point traceEnd = std::chrono::steady_clock::now();
	auto [minHits,maxHits,meanHits,devHits] = distribution(bitmap);
	std::cout << "min hits = " << minHits << std::endl;
	std::cout << "max hits = " << maxHits << std::endl;
	std::cout << "mean hits = " << meanHits << std::endl;
	std::cout << "standard deviation = " << devHits << std::endl;
	std::cout << "BVH trace time = " << std::chrono::duration_cast<std::chrono::milliseconds>(traceEnd - traceBegin).count() << "ms" << std::endl;

	heatmap(bitmap,meanHits-2*devHits,meanHits+2*devHits,vec3(1,0,0),vec3(0,1,0),vec3(0,0,1))
		.write((filepath+std::string("-heatmap-")+volumestr+std::string(".png")).c_str(),Bitmap::RGB8);

	if constexpr (std::is_same_v<Volume,AABB>) return;

	const Bitmap& bitmapAABB = *getHits<BVH<AABB,BVH_T::radius>>(filepath);

	Bitmap bitmapDifference = difference(bitmap,bitmapAABB);
	auto [minDifference,maxDifference,meanDifference,devDifference] =
		distribution(bitmapDifference);
	std::cout << "comparison between AABB and " << volumestr << " for model = " << filepath << std::endl;
	std::cout << "min difference = " << minDifference << std::endl;
	std::cout << "max difference = " << maxDifference << std::endl;
	std::cout << "mean difference = " << meanDifference << std::endl;
	std::cout << "standard deviation = " << devDifference << std::endl;

	heatmap(
		bitmapDifference,meanDifference-2*devDifference,meanDifference+2*devDifference,
		vec3(0.34,0,0.1),vec3(1,1,1),vec3(0.1,0,0.34))
		.write((filepath+std::string("-heatmapDifference-AABB-")+volumestr+std::string(".png")).c_str(),Bitmap::RGB8);

	std::cout << "===========" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const char* model = "./res/bunny.obj";

	profile<BVH<AABB,16>>(model);
	profile<BVH<KDOP<2>,16>>(model);
	return 0;

	const char* sanmiguel = "./res/sanmiguel.obj";
	//const Camera sanmiguelCamera(
	//	1280,720,
	//	80.0f*M_PI/180.0f,
	//	vec3(22.9106,1.21172,11.3856),
	//	normalize(vec3(-0.6470741629600525,-0.03091871738433838,-0.7617999315261841)));

	profile<BVH<AABB,16>>(sanmiguel);
	profile<BVH<KDOP<1>,16>>(sanmiguel);
	profile<BVH<KDOP<2>,16>>(sanmiguel);
	profile<BVH<KDOP<3>,16>>(sanmiguel);
	profile<BVH<KDOP<4>,16>>(sanmiguel);
	//profile<BVH<KDOP<5>,16>>(sanmiguel);


	const char* bunny = "./res/bunny.obj";
	//const Camera bunnyCamera(
	//	1280,720,
	//	45.0f*M_PI/180.0f,
	//	vec3(0.14261,2.00613,4.27206),
	//	normalize(vec3(-0.08793334662914276,-0.28418299555778503,-0.9547291398048401)));

	profile<BVH<AABB,16>>(bunny);
	profile<BVH<KDOP<1>,16>>(bunny);
	profile<BVH<KDOP<2>,16>>(bunny);
	profile<BVH<KDOP<3>,16>>(bunny);
	profile<BVH<KDOP<4>,16>>(bunny);
	//profile<BVH<KDOP<5>,16>>(bunny);

	return 0;
}
