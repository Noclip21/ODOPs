#pragma once

#include "OBB.h"

const float OBB::SAH() const
{
	const float x = 1.0/norm(vec3(m(0,0),m(0,1),m(0,2)));
	const float y = 1.0/norm(vec3(m(1,0),m(1,1),m(1,2)));
	const float z = 1.0/norm(vec3(m(2,0),m(2,1),m(2,2)));
	return x*y + x*z + y*z;
}
