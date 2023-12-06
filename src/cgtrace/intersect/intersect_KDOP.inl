#pragma once

#include "../common/Ray.h"
#include "../common/Hit.h"

#include "../shape/OBB.h"
#include "../shape/KDOP.h"
#include "../shape/constructors_OBB.inl"
#include "../shape/operator_KDOP.inl"

#include "intersect_OBB.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY,uint K>
const HIT intersect(const RAY& ray,const KDOP<K>& kdop)
{
	HIT hitNone;
	if constexpr (is_Hitb<HIT>) hitNone.isHit = false;
	if constexpr (is_Hitf<HIT>) hitNone.t = -1;

	HIT hit = hitNone;

	bool isHit = true;
	for(uint i=0; i<kdop.numMatrices; ++i)
	{
		OBB obb(kdop.m(i));

		HIT hit2 = intersect<HIT,RAY>(ray,obb);

		if constexpr (is_Hitb<HIT>)
		{
			isHit &= hit2.isHit;
			hit.isHit = isHit;
		}
		if constexpr (is_Hitf<HIT>)
		{
			isHit &= hit2.t > 0;
			hit = (hit2.t > hit.t)?hit2:hit;
			// TODO tmax
		}
	}

	return (isHit)?hit:hitNone;
}
////////////////////////////////////////////////////////////////////////////////
