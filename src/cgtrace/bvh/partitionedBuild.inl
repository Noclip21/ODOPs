#pragma once

#include "constructors.inl"


template<typename V,uint R>
requires(std::is_base_of_v<VolumeBase,V>)
template<typename VP,typename LAYOUT>
requires(std::is_base_of_v<VolumeBase,VP> && is_PrimitiveArray<LAYOUT>)
const BVH<V,R> BVH<V,R>::partitionedBuild(const LAYOUT& layout,const uint nParts,const bool sorted)
{
	// Set primitive array layout
	PrimitiveArray_makeIndexed_t<LAYOUT> leavesLayout;

	std::vector<typename decltype(leavesLayout)::IndexType> leavesIndices(layout.count);
	for(uint i=0; i<layout.count; ++i)
		if constexpr (is_PrimitiveArrayIndexed<LAYOUT>)
			leavesIndices[i] = layout.indices[i];
		else
			leavesIndices[i] = i;

	leavesLayout.data    = layout.data;
	leavesLayout.count   = layout.count;
	leavesLayout.indices = &leavesIndices[0];

	// Sort it
	if(!sorted) spatialSort(leavesLayout);

	// Allocate bottom BVH buffers
	std::vector<BVH<V,R>> bottomBVHs(nParts);
	std::vector<VP> bottomBVHRoots(nParts);

	// Compute partition size
	int smallCountMin   = leavesLayout.count/nParts;
	int smallCountExtra = leavesLayout.count%(smallCountMin*nParts);

	// Compute partition BVHs
	std::vector<int> indexOffsets(nParts);
	int indexOffset = 0;
	for(uint i=0; i<nParts; ++i)
	{
		indexOffsets[i]  = indexOffset;
		int count        = smallCountMin + (smallCountExtra-- > 0);
		indexOffset     += count;

		auto layoutOffset = leavesLayout;
		layoutOffset.indices += indexOffsets[i];
		layoutOffset.count    = count;

		const BVH<VP,R> partBVH(layoutOffset,true);
		bottomBVHRoots[i] = partBVH.nodes[0].volume;
		bottomBVHs[i]     = BVH<V,R>(partBVH);
	}

	// Set up top BVH array layout
	PrimitiveArrayPayload<PrimitiveArrayDataBase<VP>> bottomLayout;
	bottomLayout.data  = &bottomBVHRoots[0];
	bottomLayout.count = (uint)bottomBVHRoots.size();

	// Compute top BVH
	BVH<V,R> bvh = BVH<V,R>(BVH<VP,R>(bottomLayout));

	// Compute node offsets
	std::vector<int> nodeOffsets(nParts);
	nodeOffsets[0] = bvh.nodes.size();
	for(uint i=1; i<nParts; ++i)
		nodeOffsets[i] = nodeOffsets[i-1] + bottomBVHs[i-1].nodes.size();

	// Apply node offset to leaf nodes of top BVH
	for(uint i=0; i<bvh.nodes.size(); ++i)
	{
		auto& n = bvh.nodes[i];
		if(n.left & leafMask)
		{
			n.left &= ~leafMask;
			bottomBVHs[n.left].nodes[0].parent = i;
			n.left = nodeOffsets[n.left];
		}
		if(n.right & leafMask)
		{
			n.right &= ~leafMask;
			bottomBVHs[n.right].nodes[0].parent = i;
			n.right = nodeOffsets[n.right];
		}
	}

	// Apply primitive index / node offset to bottom BVHs
	for(uint i=0; i<bottomBVHs.size(); ++i)
	{
		auto& bottomBVH = bottomBVHs[i];
		for(uint j=0; j<bottomBVH.nodes.size(); ++j)
		{
			auto& n = bottomBVH.nodes[j];
			if(j > 0) n.parent += nodeOffsets[i];
			n.left   += (n.left  & leafMask)?indexOffsets[i]:nodeOffsets[i];
			n.right  += (n.right & leafMask)?indexOffsets[i]:nodeOffsets[i];
		}
	}

	// Merge top and bottom BVH nodes
	for(auto& bottomBVH: bottomBVHs)
		bvh.nodes.insert(bvh.nodes.end(),bottomBVH.nodes.begin(),bottomBVH.nodes.end());

	return bvh;
}
