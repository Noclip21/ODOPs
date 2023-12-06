#pragma once

#include "ShapeBase.h"


struct Triangle: ShapeBase
{ 
	vec3 p1,p2,p3;

	Triangle() = default;
	Triangle(const Triangle&) = default;
	Triangle(const vec3&,const vec3&,const vec3&);

	inline const vec3 center() const;
};
