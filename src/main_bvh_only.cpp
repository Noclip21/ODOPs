#include <iostream>
#include <chrono>

#include <algorithm>
#include <execution>
#include <vector>

#include <cgmath>
#include <cgasset>
#include <cgtrace>

#include "Mesh.h"
#include "common/PrimimitiveArrayLayout.h"


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
template<typename BVH_T>
const std::string getVolumestr()
{
	using Volume = typename BVH_T::Volume;
	if(std::is_same_v<Volume,AABB>) return "AABB";
	if(std::is_same_v<Volume,KDOP<1>>) return "OBB-KDOP<1>";
	if(std::is_same_v<Volume,KDOP<2>>) return "OBB-KDOP<2>";
	if(std::is_same_v<Volume,KDOP<3>>) return "OBB-KDOP<3>";
	if(std::is_same_v<Volume,KDOP<4>>) return "OBB-KDOP<4>";
}
////////////////////////////////////////////////////////////////////////////////
template<typename BVH_T>
void writeBVH(const std::string& filepath)
{
	const std::string volumestr = getVolumestr<BVH_T::Volume>();
	const std::string filepathbvh = filepath+"-"+volumestr+".bvh";

	const BVH_t<BVH_T>& bvh = *getBVH<BVH_T>(filepath);

	std::ofstream os(filepathbvh);	
	write(os,&bvh,1);
	os.close();
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
	std::string volumestr = getVolumestr<BVH_T>();
	std::cout << "BVH type = " << volumestr << std::endl;
	std::cout << volumestr << " size = " << sizeof(Volume) << " bytes" << std::endl;
	std::cout << "BVH size = " << bvh.nodes.size()*sizeof(Volume) << " bytes" << std::endl;
	std::cout << "BVH build time = " << std::chrono::duration_cast<std::chrono::milliseconds>(buildEnd - buildBegin).count() << "ms" << std::endl;
	std::cout << "SAH = " << bvhSAH(bvh) << std::endl;
	std::cout << "===========" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,typename VP,uint R,typename LAYOUT>
const BVH<V,R> partitionedBVHConstruction(const LAYOUT& layout)
{
	const uint nParts = 8;

	std::vector<BVH<V,R>> bBVHs(nParts);
	std::vector<VP> bBVHRoots(nParts);

	int smallCountMin = layout.count/nParts;
	int smallCountExtra = layout.count%(smallCountMin*nParts);
	int offset = 0;

	for(uint i=0; i<nParts; ++i)
	{
		std::cout << i+1 << "/" << nParts << " ";
		auto layoutOffset = layout;
		layoutOffset.indices += offset;
		layoutOffset.count = smallCountMin + (smallCountExtra-- > 0);
		offset += layoutOffset.count;

		BVH<VP,R> bvhKDOP(layoutOffset);
		bBVHRoots[i] = bvhKDOP.nodes[i].volume;
		bBVHs[i] = BVH<V,R>(bvhKDOP);
	}

	const PrimitiveArrayLayout<VP> bLayout = {
		.data = &bBVHRoots[0],
		.count = (uint)bBVHRoots.size(),
	};

	const BVH<VP,R> tbvh(bLayout);
	BVH<V,R> bvh(tbvh);

	std::vector<uint> bBVHOffsets(nParts);
	for(uint i=0; i<nParts; ++i)
	{
		auto& bbvh = bBVHs[i];
		bBVHOffsets[i] = bvh.nodes.size();
		for(auto& node: bbvh.nodes)
		{
			node.parent += bBVHOffsets[i];
			node.left   += (node.left & bbvh.leafMask)?0:bBVHOffsets[i];
			node.right  += (node.right & bbvh.leafMask)?0:bBVHOffsets[i];
		}
		bvh.nodes.insert(bvh.nodes.end(),bbvh.nodes.begin(),bbvh.nodes.end());
	}
	for(uint i=0; i<bBVHOffsets[0]; ++i)
	{
		auto& node  = bvh.nodes[i];
		node.left   = (node.left & bvh.leafMask)?bBVHOffsets[node.left & ~bvh.leafMask]:node.left;
		node.right  = (node.right & bvh.leafMask)?bBVHOffsets[node.right & ~bvh.leafMask]:node.right;
	}

	return bvh;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	VertStream vs(Scene("./res/monkey.obj"),(const int[]){VertStream::COORDS,3,0});

	TriangleIndicesArray layout;
	layout.data = (vec3*)&vs.vertexArray[0];
	layout.indices = (ivec3*)&vs.indices[0];
	layout.count = (uint)vs.indices.size()/3;

	BVH<AABB,4> bvh(layout);
	return 0;

	// Compute centroids
	std::vector<vec3> centers(layout.count);
	vec3 cmax = -std::numeric_limits<float>::infinity(),
		 cmin = +std::numeric_limits<float>::infinity();
	for(uint i=0; i<layout.count; ++i)
	{
		centers[i] = 
			(layout.data[layout.indices[i][0]]+
			layout.data[layout.indices[i][1]]+
			layout.data[layout.indices[i][2]])/3;
		cmax = max(cmax,centers[i]);
		cmin = min(cmin,centers[i]);
	}

	// Compute morton codes
	vec3 csize = cmax-cmin;
	std::vector<std::pair<morton3,int>> codes(layout.count);
	for(uint i=0; i<layout.count; ++i)
	{
		codes[i].first  = encode(uvec3(1024*(centers[i]+cmin)/csize));
		codes[i].second = i;
	}

	// Sort by morton code
	std::sort(std::execution::par_unseq,codes.begin(),codes.end());

	std::vector<ivec3> sortedIndices(layout.count);
	for(uint i=0; i<layout.count; ++i)
		sortedIndices[i] = layout.indices[codes[i].second];

	layout.indices = &sortedIndices[0];


	partitionedBVHConstruction<AABB,AABB,8>(layout);

	return 0;



	/*
	const std::vector<std::string> scenes = {
		"./res/bunny.obj",
		"./res/hairball.obj",
		"./res/bistro.obj",
		"./res/rungholt.obj",
		"./res/sanmiguel.obj",
	};

	for(const std::string& scene: scenes)
	{
		profile<BVH<AABB,4>>(scene);
		profile<BVH<KDOP<1>,4>>(scene);
		profile<BVH<KDOP<2>,4>>(scene);
		profile<BVH<KDOP<3>,4>>(scene);
		profile<BVH<KDOP<4>,4>>(scene);
	}

	return 0;
	*/
}
