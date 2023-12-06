#pragma once

#include "AABB.h"


const float AABB::SAH() const
{
	const vec3 v = pmax-pmin;
	return v.x*v.y + v.x*v.z + v.y*v.z;
}
