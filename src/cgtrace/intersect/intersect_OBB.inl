#pragma once

#include "../common/Ray.h"
#include "../common/Hit.h"
#include "../shape/OBB.h"

#include "intersect_AABB.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY>
inline const HIT intersect(const RAY& ray,const OBB& obb)
{
	RAY ray2;
	ray2.o = obb.m*vec4(ray.o,1);
	ray2.d = obb.m*vec4(ray.d,0);
	if constexpr (is_Rayd<RAY>)
	{
		ray2.odx = obb.m*vec4(ray.odx,1);
		ray2.ody = obb.m*vec4(ray.ody,1);
		ray2.ddx = obb.m*vec4(ray.ddx,0);
		ray2.ddy = obb.m*vec4(ray.ddy,0);
	}

	static const AABB aabb(vec3(0),vec3(1));
	HIT hit = intersect<HIT>(ray2,aabb);

	if constexpr (is_Hitn<HIT>)
		hit.n = inverse(mat3(obb.m))*hit.n;

	return hit;
}
////////////////////////////////////////////////////////////////////////////////
