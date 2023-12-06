#pragma once

#include <execution>
#include <algorithm>

#include <cgthread>
#include <limits>

#include "BVH.h"
#include "Node.h"


////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<Volume,V>)
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
requires(std::is_base_of_v<Volume,V>)
template<typename V2,uint R2>
BVH<V,R>::BVH(const BVH<V2,R2>& bvh)
{ *this = bvh; }
////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<Volume,V>)
BVH<V,R>::BVH(const vec3* verts,const uint nVertices)
{
	std::vector<int> indices(nVertices);
	for(uint i=0; i<nVertices; ++i)
		indices[i] = i;
	*this = BVH(verts,&indices[0],nVertices);
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<Volume,V>)
BVH<V,R>::BVH(const vec3* verts,const int* indices,const uint nIndices)
{
	// Compute leaf nodes
	std::vector<Volume> volumes(nIndices/3);
	for(int i=0; i<volumes.size(); ++i)
		volumes[i].init({
			verts[indices[3*i]],
			verts[indices[3*i+1]],
			verts[indices[3*i+2]]});

	*this = BVH(&volumes[0],volumes.size());
}
////////////////////////////////////////////////////////////////////////////////
template<typename V,uint R>
requires(std::is_base_of_v<Volume,V>)
BVH<V,R>::BVH(const Volume* volumes,const uint nVolumes)
{
	// Compute centroids
	std::vector<vec3> centers(nVolumes);
	vec3 cmax = -std::numeric_limits<float>::infinity(),
		 cmin = +std::numeric_limits<float>::infinity();
	for(uint i=0; i<nVolumes; ++i)
	{
		centers[i] = volumes[i].center();
		cmax = max(cmax,centers[i]);
		cmin = min(cmin,centers[i]);
	}

	// Compute morton codes
	vec3 csize = cmax-cmin;
	std::vector<std::pair<morton3,int>> codes(nVolumes);
	for(uint i=0; i<nVolumes; ++i)
	{
		codes[i].first  = encode(uvec3(1024*(centers[i]+cmin)/csize));
		codes[i].second = i;
	}

	// Sort by morton code
	std::sort(std::execution::par_unseq,codes.begin(),codes.end());

	// Compute leaf nodes
	std::vector<Node> nodeBuffer(nVolumes);
	for(uint i=0; i<nVolumes; ++i)
	{
		uint id = codes[i].second;
		nodeBuffer[i] = {(id | leafMask),id,id,volumes[id]};
	}

	nodes = std::vector<Node>(nVolumes-1);
	uint nodeId = nodes.size()-1;

	std::cout << "Building BVH..."; std::cout.flush();
	// Compute inner nodes
	uint numNodes = nodeBuffer.size();
	while(numNodes > 1)
	{
		// Compute smallest SAH for each node
		std::vector<Node> nodeCandidates(numNodes);

		Compute compute(numNodes);
		compute.dispatch([&](const ComputeInput<1>& input)->void
		//for(uint i=0; i<numNodes; ++i)
		{
			const uint i = input.id[0];
			const int j0 = (i<R)?0:i-R;
			const int jR = (i+R+1>numNodes)?numNodes:i+R+1;
			auto& node = nodeCandidates[i];
			for(uint j=j0; j<jR; ++j)
			{
				if(i == j) continue;
				Volume v; v.init(nodeBuffer[i].volume,nodeBuffer[j].volume);
				if(node.volume > v)
					node = {0,i,j,v};
			}
		//}
		});
		compute.sync();

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
	std::cout << "done" << std::endl;
}
