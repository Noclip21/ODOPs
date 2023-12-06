#pragma once

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/operators.inl"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
requires(r == 1 || c == 1)
inline const std::common_type_t<T1,T2> dot(const Mat<T1,r,c>& a,const Mat<T2,r,c>& b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r*c; ++i)
		d += a[i] * b[i];
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask>
inline const std::common_type_t<T1,T2> dot(const Mat<T1,r,1>& a,const Swizzle<T2,r,mask>& b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r; ++i)
		d += a[i] * b[i];
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask>
inline const std::common_type_t<T1,T2> dot(const Swizzle<T1,r,mask>& a,const Mat<T2,r,1>& b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r; ++i)
		d += a[i] * b[i];
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask1,uint mask2>
inline const std::common_type_t<T1,T2> dot(const Swizzle<T1,r,mask1>& a,const Swizzle<T2,r,mask2>& b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r; ++i)
		d += a[i] * b[i];
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
requires(r == 1 || c == 1)
inline const std::common_type_t<T1,T2> dot(const Mat<T1,r,c>& a,const T2 b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r*c; ++i)
		d += a[i] * b;
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
requires(r == 1 || c == 1)
inline const std::common_type_t<T1,T2> dot(const T1 a,const Mat<T2,r,c>& b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r*c; ++i)
		d += a * b[i];
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask>
inline const std::common_type_t<T1,T2> dot(const Swizzle<T1,r,mask>& a,const T2 b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r; ++i)
		d += a[i] * b;
	return d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask>
inline const std::common_type_t<T1,T2> dot(const T1 a,const Swizzle<T2,r,mask>& b)
{
	std::common_type_t<T1,T2> d = 0;
	for(uint i=0; i<r; ++i)
		d += a * b[i];
	return d;
}
////////////////////////////////////////////////////////////////////////////////
