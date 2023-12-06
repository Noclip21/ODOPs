#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>

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
template<typename V,typename VP,uint R>
const Bitmap* getRayHit(const std::string& filepath,const Camera camera)
{
	static std::map<std::string,Bitmap*> hits;
	if(hits.find(filepath) == hits.end())
	{
		const auto& bvh = *getBVH<V,VP,R>(filepath);

		Bitmap bitmap(camera.width,camera.height,Bitmap::RGB32F);

		Compute compute(camera.width,camera.height);
		compute.dispatch([&](const ComputeInput<2>& input)
		{
			const ivec2 coords(input.id[0],input.id[1]);
			const Ray ray = camera.createRay<Ray>(coords.x,coords.y);

			float hits = 0;
			bvh.traverse(
				[&ray,&hits](const V& v,const int)->bool
				{
					const bool isHit = intersect<Hitb>(ray,v).isHit;
					hits += (isHit)?1:0;
					return isHit;
				},[](const int){ return false; });

			const vec3 color(hits);
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
template<typename V,typename VP,uint R>
void profile(const std::string& filepath,const Camera camera)
{
	std::string volumestr;
	if(std::is_same_v<VP,AABB>) volumestr = "AABB";
	if(std::is_same_v<VP,KDOP<1>>) volumestr = "OBB-KDOP<1>";
	if(std::is_same_v<VP,KDOP<2>>) volumestr = "OBB-KDOP<2>";
	if(std::is_same_v<VP,KDOP<3>>) volumestr = "OBB-KDOP<3>";
	if(std::is_same_v<VP,KDOP<4>>) volumestr = "OBB-KDOP<4>";
	if(std::is_same_v<VP,KDOP<5>>) volumestr = "OBB-KDOP<5>";

	const Bitmap& bitmap = *getRayHit<V,VP,R>(filepath,camera);
	const Bitmap& bitmapAABB = *getRayHit<AABB,AABB,R>(filepath,camera);
	auto [minHitsAABB,maxHitsAABB,meanHitsAABB,devHitsAABB] = distribution(bitmapAABB);

	heatmap(bitmap,meanHitsAABB-2*devHitsAABB,meanHitsAABB+2*devHitsAABB,vec3(1,0,0),vec3(0,1,0),vec3(0,0,1))
		.write((filepath+std::string("-heatmapPrimaryRay-")+volumestr+std::string(".png")).c_str(),Bitmap::RGB8);

	if constexpr (std::is_same_v<V,AABB>) return;

	Bitmap bitmapDifference = difference(bitmap,bitmapAABB);
	auto [minDifference,maxDifference,meanDifference,devDifference] =
		distribution(bitmapDifference);

	heatmap(
		bitmapDifference,meanDifference-2*devDifference,meanDifference+2*devDifference,
		vec3(0.34,0,0.1),vec3(1,1,1),vec3(0.1,0,0.34))
		.write((filepath+std::string("-heatmapPrimaryRayDifference-AABB-")+volumestr+std::string(".png")).c_str(),Bitmap::RGB8);
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const std::vector<std::pair<std::string,Camera>> models = {
		//{"./res/cornellbox.obj",Camera(512,512,28.1*M_PI/180.0f,vec3(0,0,5),vec3(0,0,-1))},
		//"./res/rungholt.obj",
		//"./res/sanmiguel.obj",
		{"./res/bistro.obj",Camera(2048,2048,39.5978*M_PI/180.0f,vec3(-110.188,25.5509,14.7136),vec3(0.9904,-0.1343,-0.0312))},
		//"./res/bunny.obj",
		{"./res/hairball.obj",Camera(2048,2048,39.5978*M_PI/180.0f,vec3(-0.098916,4.63786,13.2517),vec3(0.0071221645921468735,-0.32797425985336304,-0.9446598887443542))},
	};

	for(auto model: models)
	{
		profile<AABB,AABB,8>(model.first,model.second);
		profile<OBB,KDOP<1>,8>(model.first,model.second);
		profile<OBB,KDOP<2>,8>(model.first,model.second);
		profile<OBB,KDOP<3>,8>(model.first,model.second);
		profile<OBB,KDOP<4>,8>(model.first,model.second);
	}

	return 0;
}
