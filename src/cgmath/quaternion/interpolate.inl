#pragma once

#include "pow.inl"

#include "../common/dot.inl"
#include "../common/normalize.inl"

////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,typename T3>
inline const Quaternion<std::common_type_t<T1,T2,T3>>
lerp(const Quaternion<T1>& q1,const Quaternion<T2>& q2,const T3 t)
{
	return pow(q2,t)*pow(q1,T3(1)-t);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,typename T3>
inline const Quaternion<std::common_type_t<T1,T2,T3>>
slerp(const Quaternion<T1>& q1,Quaternion<T2> q2,const T3 t)
{
	auto c = dot(q1,q2);
	if(c >= 0) return lerp(q1,q2,t);
	q2 *= -1;
	if(abs(c)-1 <= 5e-5)
		return normalize(q1+(q2-q1)*t);
	return lerp(q1,q2,t);
}
////////////////////////////////////////////////////////////////////////////////
