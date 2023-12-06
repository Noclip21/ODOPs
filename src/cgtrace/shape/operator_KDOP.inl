#pragma once

#include "KDOP.h"


template<uint K>
const mat3x4 KDOP<K>::m(const uint i) const
{
	const float* ds = ko+6*i;

	vec3 s(ds[1]-ds[0],ds[3]-ds[2],ds[5]-ds[4]);
	     s = 1/vec3(
	    (abs(s.x) > 5e-5)?s.x:5e-5,
	    (abs(s.y) > 5e-5)?s.y:5e-5,
	    (abs(s.z) > 5e-5)?s.z:5e-5);

	const vec3 t(-ds[0],-ds[2],-ds[4]);

	mat3 R = transpose(kn.m[i]);

	return mat3x4(R(0)*s,R(1)*s,R(2)*s,t*s);
}
