#pragma once

#include "Quaternion.h"

#include "../common/defaults.inl"
#include "../common/transform.inl"
#include "../common/functions.inl"

template<typename T1,typename T2 = decltype(cg::up)::Type>
inline const auto lookRotationq(const Mat<T1,3,1>& direction,const Mat<T2,3,1>& up = cg::up)
{
	const auto m = lookRotation(direction,up);
	const auto c = (m(0,0)+m(1,1)+m(2,2)-1.0f)/2.0f;
	const auto u = Mat<T1,3,1>(m(1,2)-m(2,1),m(2,0)-m(0,2),m(0,1)-m(1,0));
	const auto angle = acos(c);
	return make(u,angle);
}
