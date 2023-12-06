#pragma once

#include <limits>

#include "KDOP.h"


////////////////////////////////////////////////////////////////////////////////
template<uint K>
const float KDOP<K>::SAH(const uint i) const
{
	const float* ds = ko+6*i;
	const vec3 v(ds[1]-ds[0],ds[3]-ds[2],ds[5]-ds[4]);
	return v.x*v.y + v.x*v.z + v.y*v.z;
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
const float KDOP<K>::SAH() const
{
	float s = std::numeric_limits<float>::infinity();
	for(uint i=0; i<numMatrices; ++i)
		s = min(s,SAH(i));
	return s;
}
////////////////////////////////////////////////////////////////////////////////
