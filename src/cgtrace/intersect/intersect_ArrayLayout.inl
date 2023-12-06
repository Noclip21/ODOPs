#pragma once

#include "intersect_AABB.inl"
#include "intersect_OBB.inl"
#include "intersect_KDOP.inl"
#include "intersect_Bezier.inl"
#include "intersect_Sphere.inl"
#include "intersect_Triangle.inl"

#include "../common/PrimimitiveArrayLayout.h"


template<typename HIT,typename RAY,typename LAYOUT>
requires(is_PrimitiveArray<LAYOUT>)
const HIT intersect(const RAY& ray,const LAYOUT& layout,const uint id)
{
	HIT hit;

	if constexpr (is_TriangleIndicesArray<LAYOUT>)
	{
		const ivec3& ids = layout.indices[id];
		const Triangle triangle(
			layout.data[ids[0]],
			layout.data[ids[1]],
			layout.data[ids[2]]);
		hit = intersect<HIT>(ray,triangle,layout.backface);
	}else if constexpr (is_BezierArray<LAYOUT>)
		hit = intersect<HIT>(ray,layout.data[id],layout.pr);
	else if constexpr (is_IndexedBezierArray<LAYOUT>)
		hit = intersect<HIT>(ray,layout.data[layout.indices[id]],layout.pr);
	else if constexpr (is_TriangleArray<LAYOUT>)
		hit = intersect<HIT>(ray,layout.data[id],layout.backface);
	else if constexpr (is_IndexedTriangleArray<LAYOUT>)
		hit = intersect<HIT>(ray,layout.data[layout.indices[id]],layout.backface);
	else if constexpr (is_IndexedVertexArray<LAYOUT>)
		hit = intersect<HIT>(ray,layout.data[layout.indices[id]]);
	else
		hit = intersect<HIT>(ray,layout.data[id]);

	if constexpr (is_Hitb<HIT>)    { if(hit.isHit)  hit.id = id; }
	else if constexpr(is_Hitf<HIT>){ if(hit.t >= 0) hit.id = id; }

	return hit;
}
