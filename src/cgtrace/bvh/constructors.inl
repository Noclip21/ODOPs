#pragma once

#include <execution>
#include <algorithm>
#include <limits>

#include <cgthread>

#include "BVH.h"

#include "../sort/spatialSort.inl"

#include "../shape/constructors_AABB.inl"
#include "../shape/constructors_OBB.inl"
#include "../shape/constructors_KDOP.inl"
#include "../shape/constructors_Sphere.inl"
#include "../shape/constructors_Bezier.inl"
#include "../shape/constructors_Triangle.inl"

#include "../shape/SAH_AABB.inl"
#include "../shape/SAH_OBB.inl"
#include "../shape/SAH_KDOP.inl"
#include "../shape/SAH_Sphere.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<VolumeBase,V>)
template<typename V2,uint R2>
const BVH<V,R>& BVH<V,R>::operator = (const BVH<V2,R2>& bvh)
{
	nodes.resize(bvh.nodes.size());
	for(uint i=0; i<bvh.nodes.size(); ++i)
		nodes[i] = Node(bvh.nodes[i]);
	return *this;
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<VolumeBase,V>)
template<typename V2,uint R2>
BVH<V,R>::BVH(const BVH<V2,R2>& bvh)
{ *this = bvh; }
////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<VolumeBase,V>)
template<typename LAYOUT>
requires(is_PrimitiveArray<LAYOUT>)
BVH<V,R>::BVH(const LAYOUT& layout,const bool sorted)
{
	std::vector<Volume> volumes(layout.count);
	std::vector<int> indices(layout.count);
	const uint nVolumes = layout.count;

	for(uint i=0; i<layout.count; ++i)
	{
		if constexpr (is_TriangleIndicesArray<LAYOUT>)
		{
			const ivec3& ids = layout.indices[i];
			const Triangle triangle(
				layout.data[ids[0]],
				layout.data[ids[1]],
				layout.data[ids[2]]);
			volumes[i] = Volume(triangle);
		}else if constexpr (is_BezierArray<LAYOUT>)
			volumes[i] = Volume(layout.data[i],layout.pr);
		else if constexpr (is_BezierArray<LAYOUT>)
			volumes[i] = Volume(layout.data[layout.indices[i]],layout.pr);
		else if constexpr (is_IndexedVertexArray<LAYOUT>)
			volumes[i] = Volume(layout.data[layout.indices[i]]);
		else
			volumes[i] = Volume(layout.data[i]);

		indices[i] = i;
	}

	PrimitiveArrayPayload<PrimitiveArrayDataBase<Volume>,PrimitiveArrayDataIndexed<int>> leavesLayout;
	leavesLayout.data = &volumes[0];
	leavesLayout.count = nVolumes;
	leavesLayout.indices = &indices[0];

	if(!sorted) spatialSort(leavesLayout);

	std::vector<Node> nodeBuffer(nVolumes);
	for(uint i=0; i<nVolumes; ++i)
	{
		const uint id = indices[i];
		nodeBuffer[i] = {(id | leafMask),id,id,volumes[id]};
	}

	nodes = std::vector<Node>(nVolumes-1);
	uint nodeId = nodes.size()-1;

	// Compute inner nodes
	uint numNodes = nodeBuffer.size();
	while(numNodes > 1)
	{
		// Compute smallest SAH for each node
		std::vector<Node> nodeCandidates(numNodes);

		std::for_each(
			std::execution::par_unseq,
			nodeCandidates.begin(),
			nodeCandidates.end(),
			[&](Node& node)
		{
			const uint i = &node-&nodeCandidates[0];
			const int j0 = (i<R)?0:i-R;
			const int jR = (i+R+1>numNodes)?numNodes:i+R+1;
			float sahi = std::numeric_limits<float>::infinity();
			for(uint j=j0; j<jR; ++j)
			{
				if(i == j) continue;
				const Volume v(nodeBuffer[i].volume,nodeBuffer[j].volume);
				const float sahj = v.SAH();
				if(sahi > sahj)
				{
					node = {0,i,j,v};
					sahi = sahj;
				}
			}
		});

		// Merge nodes
		uint k = 0;
		for(uint i=0; i<numNodes; ++i)
		{
			const uint j = nodeCandidates[i].right;
			if(nodeCandidates[j].right == i)
			{
				if(j > i)
				{
					auto& node  = nodes[nodeId] = nodeCandidates[i];
					auto& left  = nodeBuffer[node.left];
					auto& right = nodeBuffer[node.right];
					node.left   = left.parent;
					node.right  = right.parent;
					node.parent = nodeId--;
					if(!(node.left & leafMask))
						nodes[node.left].parent = node.parent;
					if(!(node.right & leafMask))
						nodes[node.right].parent = node.parent;
					nodeBuffer[k++] = node;
				}
			}else
				nodeBuffer[k++] = nodeBuffer[i];
		}
		numNodes = k;
	}
}
////////////////////////////////////////////////////////////////////////////////
