#pragma once

#include "../common/Ray.h"
#include "../common/Hit.h"
#include "../shape/Triangle.h"


////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY>
const HIT intersect(const RAY& ray,const Triangle& triangle,const bool backface = true)
{
	const vec3 p1p2 = triangle.p2-triangle.p1,
	           p1p3 = triangle.p3-triangle.p1,
	           rop1 = triangle.p1-ray.o,
	           n    = cross(p1p2,p1p3);

	const float t = 1.0/dot(n,ray.d);

	bool isHit = (t <= 0);
	float u = 0,v = 0;

	if(backface || isHit)
	{
		const vec3 q = cross(ray.d,rop1);

		u = dot(-q,p1p3)*t;
		v = dot( q,p1p2)*t;

		isHit = (min(u,v) >= 0 && u+v <= 1);
	}

	HIT hit;

	if constexpr (is_Hitb<HIT>)
		hit.ishit = isHit;

	if constexpr (is_Hitf<HIT>)
		hit.t = (isHit)?t*dot(n,rop1):-1;

	if constexpr (is_Hitn<HIT>)
		hit.n = (isHit)?normalize(n):0;

	if constexpr (is_Hituv<HIT>)
		hit.uv = vec2(u,v);

	if constexpr (is_Hitduv<HIT>)
	{
		// TODO
		if(isHit)
		{
    		const float k = t*dot(n,rop1);

			const vec3 qdx = cross(p1p3,ray.d), qdy = cross(ray.d,p1p2);
			const float udx = dot(qdx,k*ray.ddx)*t, vdx = dot(qdy,k*ray.ddx)*t,
		            	udy = dot(qdx,k*ray.ddy)*t, vdy = dot(qdy,k*ray.ddy)*t;

			hit.uvdx = vec2(udx,vdx);
			hit.uvdy = vec2(udy,vdy);
		}
	}

	return hit;
}
////////////////////////////////////////////////////////////////////////////////
