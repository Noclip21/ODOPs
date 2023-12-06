#pragma once

#include <limits>
#include <cstdarg>

#include "Volume.h"


struct AABB: public Volume
{
	vec3 pmin;
	vec3 pmax;

	virtual void init(std::initializer_list<const vec3> ps)
	{
		pmin = +std::numeric_limits<float>::infinity();
		pmax = -std::numeric_limits<float>::infinity();
		for(auto& p: ps)
		{
			pmin = min(pmin,p);
			pmax = max(pmax,p);
		}
		sah = SAH();
	}
	virtual void init(const Volume& _a,const Volume& _b)
	{
		auto a = *static_cast<const AABB*>(&_a);
		auto b = *static_cast<const AABB*>(&_b);
		pmin = min(a.pmin,b.pmin);
		pmax = max(a.pmax,b.pmax);
		sah = SAH();
	};
	virtual float SAH() const
	{
		vec3 v = pmax-pmin;
		return v.x*v.y + v.x*v.z + v.y*v.z;
	}
	virtual const vec3 center() const
	{
		return (pmin+pmax)*0.5;
	}

	virtual ~AABB(){}
};
