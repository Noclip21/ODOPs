#pragma once

#include <limits>
#include <algorithm>
#include <execution>

#include <cgmath>

#include "../common/PrimimitiveArrayLayout.h"

#include "../shape/center_AABB.inl"
#include "../shape/center_OBB.inl"
#include "../shape/center_KDOP.inl"
#include "../shape/center_Sphere.inl"
#include "../shape/center_Triangle.inl"
#include "../shape/center_Bezier.inl"


template<typename LAYOUT>
requires(is_PrimitiveArray<LAYOUT>)
void spatialSort(LAYOUT& layout)
{
	// Compute centroids
	std::vector<vec3> centers(layout.count);
	vec3 cmax = -std::numeric_limits<float>::infinity(),
		 cmin = +std::numeric_limits<float>::infinity();

	std::for_each(
		std::execution::par_unseq,
		centers.begin(),centers.end(),
		[&](vec3& c)
	{
		const uint i = &c-&centers[0];

		if constexpr (is_VertexArray<LAYOUT>)
			c = layout.data[i];
		else if constexpr (is_IndexedVertexArray<LAYOUT>)
			c = layout.data[layout.indices[i]];
		else if constexpr (is_TriangleIndicesArray<LAYOUT>)
		{
			const ivec3& ids = layout.indices[i];
			const Triangle triangle(
				layout.data[ids[0]],
				layout.data[ids[1]],
				layout.data[ids[2]]);
			c = triangle.center();
		}else if constexpr (is_PrimitiveArrayIndexed<LAYOUT>)
			c = layout.data[layout.indices[i]].center();
		else
			c = layout.data[i].center();
	});

	for(uint i=0; i<layout.count; ++i)
	{
		cmax = max(cmax,centers[i]);
		cmin = min(cmin,centers[i]);
	}

	// Compute morton codes
	const vec3 csize = cmax-cmin;
	std::vector<std::pair<morton3u64,int>> codes(layout.count);
	std::transform(
		std::execution::par_unseq,
		centers.begin(),centers.end(),
		codes.begin(),
		[&](const vec3& c) -> const std::pair<morton3u64,int>
	{
		const int i = &c-&centers[0];
		return {encode(Mat<uint64,3,1>(((1 << 21)-1)*(c-cmin)/csize)),i};
	});

	// Sort by morton code
	std::sort(std::execution::par_unseq,codes.begin(),codes.end());

	if constexpr (is_PrimitiveArrayIndexed<LAYOUT>)
	{
		std::vector<typename LAYOUT::IndexType> sortedArray(layout.count);
		for(uint i=0; i<layout.count; ++i)
			sortedArray[i] = layout.indices[codes[i].second];
		for(uint i=0; i<layout.count; ++i)
			layout.indices[i] = sortedArray[i];
	}else{
		std::vector<typename LAYOUT::PrimType> sortedArray(layout.count);
		for(uint i=0; i<layout.count; ++i)
			sortedArray[i] = layout.data[codes[i].second];
		for(uint i=0; i<layout.count; ++i)
			layout.data[i] = sortedArray[i];
	}
}
