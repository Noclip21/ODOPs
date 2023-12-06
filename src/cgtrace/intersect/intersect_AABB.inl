#pragma once

#include "../common/Ray.h"
#include "../common/Hit.h"
#include "../shape/AABB.h"


////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY>
inline const HIT intersect(const RAY& ray,const AABB& aabb)
{
	/* depends on IEE754 - revision 2008 min/max handling of NaNs */
    const vec3 ta   = (aabb.pmin-ray.o)/ray.d;
    const vec3 tb   = (aabb.pmax-ray.o)/ray.d;
    const vec3 tmin = min(ta,tb);
    const vec3 tmax = max(ta,tb);
    const float t0  = max(max(tmin.x,tmin.y),tmin.z);
    const float t1  = min(min(tmax.x,tmax.y),tmax.z);
	//const float inf = 1.0f/0.0f;
    //t0 = (t0==-inf)?inf:t0;
    //t1 = (t1==-inf)?inf:t1;
    const bool isHit = (t1 >= t0 && t1 > 0);

	HIT hit;

	if constexpr (is_Hitb<HIT>)
		hit.isHit = isHit;

	if constexpr (is_Hitf<HIT>)
	{
		hit.tmin = (isHit)?t0:-1;
		hit.tmax = (isHit)?t1:-1;
	}

	if constexpr (is_Hitn<HIT> || is_Hituv<HIT> || is_Hitduv<HIT>)
	{
    	const vec3 n = (isHit)?-sign(ray.d)*step(tmin.yzx,tmin)*step(tmin.zxy,tmin):0;

		if constexpr (is_Hitn<HIT>)
			hit.n = n;

		if constexpr (is_Hituv<HIT>)
			if(isHit)
			{
				const vec3 s = 1.0/(aabb.pmax-aabb.pmin);

    			const vec3 uv3 = (ray.o + t0*ray.d - aabb.pmin)*s;
    			vec2 uv2 = mix(vec2(uv3.x,uv3.y),vec2(uv3.z,uv3.z),abs(vec2(n.x,n.y)));
			         uv2 = mix(uv2,vec2(1.0-uv2.x,uv2.y),vec2(max(n.x,max(n.y,-n.z))));
				hit.uv = clamp(uv2,vec2(0),vec2(1));

				if constexpr (is_Hitduv<HIT>)
				{
					// TODO
					const int i = (n.x != 0)?0:((n.y != 0)?1:2);
					const float tdx = (((n[i]>0)?aabb.pmax[i]:aabb.pmin[i])-ray.odx[i])/ray.ddx[i];
					const float tdy = (((n[i]>0)?aabb.pmax[i]:aabb.pmin[i])-ray.ody[i])/ray.ddy[i];

					const vec3 uv3dx = (ray.odx + tdx*ray.ddx - aabb.pmin)*s;
					const vec3 uv3dy = (ray.odx + tdy*ray.ddy - aabb.pmin)*s;

    				vec2 uv2dx = mix(vec2(uv3dx.x,uv3dx.y),vec2(uv3dx.z,uv3dx.z),abs(vec2(n.x,n.y)));
			             uv2dx = mix(uv2dx,vec2(1.0-uv2dx.x,uv2dx.y),vec2(max(n.x,max(n.y,-n.z))));
    				vec2 uv2dy = mix(vec2(uv3dy.x,uv3dy.y),vec2(uv3dy.z,uv3dy.z),abs(vec2(n.x,n.y)));
			             uv2dy = mix(uv2dy,vec2(1.0-uv2dy.x,uv2dy.y),vec2(max(n.x,max(n.y,-n.z))));

			        hit.uvdx = uv2dx-uv2;
			        hit.uvdy = uv2dy-uv2;
				}
			}
    }

	return hit;
}
////////////////////////////////////////////////////////////////////////////////
