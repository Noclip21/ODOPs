#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>

#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const Scene scene("res/dummy.obj");

	static const int layoutsPositions[] = {
		VertStream::COORDS,3,0
	};

	std::vector<VertStream> vsPositions(scene.meshes.size());
	for(uint i=0; i<scene.meshes.size(); ++i)
		vsPositions[i] = VertStream(scene.meshes[i],scene.vertices,layoutsPositions);

	std::vector<AABB>    leavesAABB(vsPositions.size());
	std::vector<OBB>     leavesOBBPCA(vsPositions.size());
	std::vector<KDOP<2>> leavesKDOP(vsPositions.size());
	for(uint i=0; i<vsPositions.size(); ++i)
	{
		const vec3* verts = (const vec3*)&vsPositions[i].vertexArray[0];
		const int nVerts  = vsPositions[i].vertexArray.size()/3;
		leavesAABB[i] = AABB(verts,nVerts);
		leavesOBBPCA[i]  = OBB(verts,nVerts);
		leavesKDOP[i] = KDOP<2>(verts,nVerts);
	}

	const AABBArrayLayout AABBLayout = {
		.data  = &leavesAABB[0],
		.count = (uint)leavesAABB.size(),
	};
	const OBBArrayLayout OBBPCALayout = {
		.data  = &leavesOBBPCA[0],
		.count = (uint)leavesOBBPCA.size(),
	};
	const KDOPArrayLayout<2> KDOPLayout = {
		.data  = &leavesKDOP[0],
		.count = (uint)leavesKDOP.size(),
	};

	BVH<AABB,16>    bvhAABB(AABBLayout);
	BVH<OBB,16>     bvhOBBPCA(OBBPCALayout);
	BVH<KDOP<2>,16> bvhKDOP(KDOPLayout);
	BVH<OBB,16>     bvhOBBKDOP(bvhKDOP);
	

	std::cout << "o BVH_AABB" << std::endl;
	for(auto& v: leavesAABB)
	{
		mat4 m = mat4(scale(mat3(),v.pmax-v.pmin));
		m = translate(m,v.pmin);
		std::cout << m*BOX << std::endl;
	}
	for(auto& node: bvhAABB.nodes)
	{
		mat4 m = mat4(scale(mat3(),node.volume.pmax-node.volume.pmin));
		m = translate(m,node.volume.pmin);
		std::cout << m*BOX << std::endl;
	}

	std::cout << "o BVH_OBB_PCA" << std::endl;
	for(auto& v: leavesOBBPCA)
		std::cout << inverse(mat4(v.m))*BOX << std::endl;
	for(auto& node: bvhOBBPCA.nodes)
		std::cout << inverse(mat4(node.volume.m))*BOX << std::endl;

	std::cout << "o BVH_OBB_KDOP" << std::endl;
	for(auto& v: leavesKDOP)
		std::cout << inverse(mat4(OBB(v).m))*BOX << std::endl;
	for(auto& node: bvhOBBKDOP.nodes)
		std::cout << inverse(mat4(node.volume.m))*BOX << std::endl;


	uint i = 0;
	for(auto& node: bvhKDOP.nodes)
	{
		for(uint j=0; j<node.volume.numMatrices; ++j)
		{
			std::cout << "o kdop_" << i << "_" << j << std::endl;
			std::cout << inverse(mat4(node.volume.m(j)))*BOX << std::endl;
		}
		i++;
	}
	for(auto& v: leavesKDOP)
	{
		for(uint j=0; j<v.numMatrices; ++j)
		{
			std::cout << "o kdop_" << i << "_" << j << std::endl;
			std::cout << inverse(mat4(v.m(j)))*BOX << std::endl;
		}
		i++;
	}

	return 0;
}
