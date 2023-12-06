#pragma once

#include "Quaternion.h"

#include "../mat/operators.inl"
#include "../common/dot.inl"
#include "../common/cross.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline const Quaternion<std::common_type_t<T1,T2>> operator * (const Quaternion<T1>& q1,const Quaternion<T2>& q2)
{
	return Quaternion<std::common_type_t<T1,T2>>(
		cross(q1.xyz,q2.xyz) + q1.xyz*q2.w + q2.xyz*q1.w,
		q1.w*q2.w - dot(q1.xyz,q2.xyz));
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline const Mat<std::common_type_t<T1,T2>,3,1> operator * (const Quaternion<T1>& q,const Mat<T2,3,1>& a)
{
	const auto t = cross(q.xyz,a) + a*q.w;
	return cross(q.xyz,t) + q.xyz*dot(a,q.xyz) + t*q.w;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint mask>
inline const Mat<std::common_type_t<T1,T2>,3,1> operator * (const Quaternion<T1>& q,const Swizzle<T2,3,mask>& s)
{
	return q*Mat<T2,3,1>(s);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline const Mat<std::common_type_t<T1,T2>,4,1> operator * (const Quaternion<T1>& q,const Mat<T2,4,1>& a)
{
	return Mat<std::common_type_t<T1,T2>,4,1>(q*a.xyz,a.w);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline const Mat<std::common_type_t<T1,T2>,3,3> operator * (const Quaternion<T1>& q,const Mat<T2,3,3>& m)
{
	return tomat3(q)*m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline const Mat<std::common_type_t<T1,T2>,4,4> operator * (const Quaternion<T1>& q,const Mat<T2,4,4>& m)
{
	return tomat4(q)*m;
}
////////////////////////////////////////////////////////////////////////////////
