#pragma once

#include "AABB.h"


const vec3 AABB::center() const
{
	return (pmin+pmax)*0.5;
}
