#pragma once

#include "../common/Ray.h"
#include "../common/Hit.h"
#include "../shape/Sphere.h"


////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY>
const HIT intersect(const RAY& ray,const Sphere& sphere)
{
	const vec3  w = ray.o-sphere.o;
	const float b = dot(w,ray.d);
	const float c = dot(w,w)-sphere.r*sphere.r;
	const float d = b*b-c;

	const bool isHit = (d >= 0);

	HIT hit;

	if constexpr (is_Hitb<HIT>)
		hit.isHit = isHit;

	if constexpr (is_Hitf<HIT>)
	{
		hit.tmin = (d < 0)?-1:-b-sqrt(d);
		hit.tmax = (d < 0)?-1:-b+sqrt(d);
	}

	if constexpr (is_Hitn<HIT>)
		hit.n = (isHit)?(ray.o + hit.tmin*ray.d - sphere.o)/sphere.r:0;

	static auto uv = [](const vec3& n) -> const vec2
	{ return vec2(0.5+atan2(n.x,n.z)/(2*M_PI),acos(-n.y)/M_PI); };

	if constexpr (is_Hituv<HIT>)
		hit.uv = (isHit)?uv(hit.n):0;

	if constexpr (is_Hitduv<HIT>)
	{
		if(isHit)
		{
			// TODO
			const float tdx = dot(hit.n,sphere.o+sphere.r*hit.n-ray.odx)/dot(hit.n,ray.ddx);
			const float tdy = dot(hit.n,sphere.o+sphere.r*hit.n-ray.ody)/dot(hit.n,ray.ddy);
			hit.uvdx = uv((ray.odx + tdx*ray.ddx - sphere.o)/sphere.r)-hit.uv;
			hit.uvdy = uv((ray.ody + tdy*ray.ddy - sphere.o)/sphere.r)-hit.uv;
		}
	}

	return hit;
}
////////////////////////////////////////////////////////////////////////////////
