#pragma once

#include "ShapeBase.h"


struct Bezier: ShapeBase
{ 
	vec3 a,b,c,d;

	Bezier() = default;
	Bezier(const Bezier&) = default;
	Bezier(const vec3&,const vec3&,const vec3&,const vec3&);
	Bezier(const vec3*,const uint);

	template<typename T>
	inline const vec3 interpolate(const T&) const;
	template<typename T>
	inline const vec3 interpolatedt(const T&) const;

	inline const vec3 center() const;
};
