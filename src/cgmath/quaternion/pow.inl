#pragma once

#include "Quaternion.h"
#include "make.inl"
#include "angle.inl"

template<typename T1,typename T2>
inline const Quaternion<std::common_type_t<T1,T2>> pow(const Quaternion<T1>& q,const T2 t)
{
	return make(q.xyz,angle(q)*t);
}
