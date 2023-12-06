#pragma once

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/constructors.inl"
#include "../mat/operators.inl"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline const Mat<std::common_type_t<T1,T2>,3,1> cross(const Mat<T1,3,1>& a,const Mat<T2,3,1>& b)
{
	return Mat<std::common_type_t<T1,T2>,3,1>(
		a[1]*b[2]-a[2]*b[1],
		a[2]*b[0]-a[0]*b[2],
		a[0]*b[1]-a[1]*b[0]);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint mask>
inline const Mat<std::common_type_t<T1,T2>,3,1> cross(const Swizzle<T1,3,mask>& a,const Mat<T2,3,1>& b)
{
	return Mat<std::common_type_t<T1,T2>,3,1>(
		a[1]*b[2]-a[2]*b[1],
		a[2]*b[0]-a[0]*b[2],
		a[0]*b[1]-a[1]*b[0]);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint mask>
inline const Mat<std::common_type_t<T1,T2>,3,1> cross(const Mat<T1,3,1>& a,const Swizzle<T2,3,mask>& b)
{
	return Mat<std::common_type_t<T1,T2>,3,1>(
		a[1]*b[2]-a[2]*b[1],
		a[2]*b[0]-a[0]*b[2],
		a[0]*b[1]-a[1]*b[0]);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint mask1,uint mask2>
inline const Mat<std::common_type_t<T1,T2>,3,1> cross(const Swizzle<T1,3,mask1>& a,const Swizzle<T2,3,mask2>& b)
{
	return Mat<std::common_type_t<T1,T2>,3,1>(
		a[1]*b[2]-a[2]*b[1],
		a[2]*b[0]-a[0]*b[2],
		a[0]*b[1]-a[1]*b[0]);
}
////////////////////////////////////////////////////////////////////////////////
