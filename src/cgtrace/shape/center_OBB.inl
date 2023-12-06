#pragma once

#include "OBB.h"


const vec3 OBB::center() const
{
	const vec3 s(
		1.0/norm(vec3(m(0,0),m(0,1),m(0,2))),
		1.0/norm(vec3(m(1,0),m(1,1),m(1,2))),
		1.0/norm(vec3(m(2,0),m(2,1),m(2,2))));
	const mat3 R = transpose(mat3(m(0)*s,m(1)*s,m(2)*s));
	return R*((0.5-m(3))*s);
}
