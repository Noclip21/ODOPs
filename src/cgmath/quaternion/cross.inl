#pragma once
#include "Quaternion.h"

#include "constructors.inl"

template<typename T1,typename T2>
inline const Quaternion<std::common_type_t<T1,T2>> cross(const Quaternion<T1>& a,const Quaternion<T2>& b)
{
	return Quaternion<std::common_type_t<T1,T2>>(
		a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
		a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
		a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z,
		a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x);
}
