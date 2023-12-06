#pragma once

#include <limits>

#include "KDOP.h"
#include "SAH_KDOP.inl"


template<uint K>
const vec3 KDOP<K>::center() const
{
	uint mi = 0;
	float sah = std::numeric_limits<float>::infinity();
	for(uint i=0; i<numMatrices; ++i)
	{
		const float sah2 = SAH(i);
		if(sah2 > sah) continue;
		sah = sah2;
		mi = i;
	}
	return
	     ((ko[6*mi  ]+ko[6*mi+1])*0.5)*kn.m[mi](0)
		+((ko[6*mi+2]+ko[6*mi+3])*0.5)*kn.m[mi](1)
		+((ko[6*mi+4]+ko[6*mi+5])*0.5)*kn.m[mi](2);
}
