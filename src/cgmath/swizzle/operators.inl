#pragma once

#include <type_traits>

#include "Swizzle.h"

#include "../mat/operators.inl"

////////////////////////////////////////////////////////////////////////////////
template<typename T1,uint r,uint mask>
template<typename T2>
Swizzle<T1,r,mask>& Swizzle<T1,r,mask>::operator = (const Mat<T2,r,1>& v)
{ for(uint i=0; i<r; ++i) (*this)[i] = v[i]; return *this; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,uint r,uint mask>
template<typename T2,uint mask2>
Swizzle<T1,r,mask>& Swizzle<T1,r,mask>::operator = (const Swizzle<T2,r,mask2>& v)
{ for(uint i=0; i<r; ++i) (*this)[i] = v[i]; return *this; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
Swizzle<T,r,mask>& Swizzle<T,r,mask>::operator = (const Swizzle<T,r,mask>& s)
{ for(uint i=0; i<r; ++i) data[i] = s.data[i]; return *this; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
const T Swizzle<T,r,mask>::operator [] (const uint i) const
{ return data[(mask >> (i << 1)) & 0x3]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
T& Swizzle<T,r,mask>::operator [] (const uint i)
{ return data[(mask >> (i << 1)) & 0x3]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
Swizzle<T,r,mask>::operator const Mat<T,r,1>() const
{
	Mat<T,r,1> v;
	for(uint i=0; i<v.n; ++i)
		v.data[i] = (*this)[i];
	return v;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define SWIZZLE_AR(OP)\
template<typename T1,typename T2,uint r,uint mask1,uint mask2>\
const Mat<std::common_type_t<T1,T2>,r,1> operator OP (const Swizzle<T1,r,mask1>& a,const Swizzle<T2,r,mask2>& b)\
{ return (const Mat<T1,r,1>)a OP (const Mat<T2,r,1>)b; }\
template<typename T1,typename T2,uint r,uint mask>\
const Mat<std::common_type_t<T1,T2>,r,1> operator OP (const Swizzle<T1,r,mask>& a,const Mat<T2,r,1>& b)\
{ return (const Mat<T1,r,1>)a OP b; }\
template<typename T1,typename T2,uint r,uint mask>\
const Mat<std::common_type_t<T1,T2>,r,1> operator OP (const Mat<T1,r,1>& a,const Swizzle<T2,r,mask>& b)\
{ return a OP (const Mat<T2,r,1>)b; }\
template<typename T1,typename T2,uint r,uint mask>\
const Mat<std::common_type_t<T1,T2>,r,1> operator OP (const Swizzle<T1,r,mask>& a,const T2 b)\
{ return (const Mat<T1,r,1>)a OP b; }\
template<typename T1,typename T2,uint r,uint mask>\
const Mat<std::common_type_t<T1,T2>,r,1> operator OP (const T1 a,const Swizzle<T2,r,mask>& b)\
{ return a OP (const Mat<T2,r,1>)b; }
////////////////////////////////////////////////////////////////////////////////

SWIZZLE_AR(+)
SWIZZLE_AR(-)
SWIZZLE_AR(*)
SWIZZLE_AR(/)
SWIZZLE_AR(&&)
SWIZZLE_AR(||)
SWIZZLE_AR(==)
SWIZZLE_AR(!=)
SWIZZLE_AR(<)
SWIZZLE_AR(>)
SWIZZLE_AR(<=)
SWIZZLE_AR(>=)
SWIZZLE_AR(&)
SWIZZLE_AR(|)
SWIZZLE_AR(^)
#undef SWIZZLE_AR

////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask1,uint mask2>
const Mat<std::common_type_t<T1,T2>,r,1> compMulti(const Swizzle<T1,r,mask1>& a,const Swizzle<T2,r,mask2>& b)
{ return (const Mat<T1,r,1>)a * (const Mat<T2,r,1>)b; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask>
const Mat<std::common_type_t<T1,T2>,r,1> compMulti(const Swizzle<T1,r,mask>& a,const Mat<T2,r,1>& b)
{ return (const Mat<T1,r,1>)a * b; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint mask>
const Mat<std::common_type_t<T1,T2>,r,1> compMulti(const Mat<T1,r,1>& a,const Swizzle<T2,r,mask>& b)
{ return a * (const Mat<T2,r,1>)b; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint n,uint mask>
const Mat<std::common_type_t<T1,T2>,r,1> operator * (const Mat<T1,r,n>& a,const Swizzle<T2,n,mask>& b)
{ return a * (const Mat<T2,n,1>)b; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c,uint mask>
const Mat<std::common_type_t<T1,T2>,r,c> operator * (const Swizzle<T1,r,mask>& a,const Mat<T2,1,c>& b)
{ return (const Mat<T1,r,1>)a * b; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define SWIZZLE_AS(OP)\
template<typename T1,typename T2,uint r,uint mask1,uint mask2>\
Swizzle<T1,r,mask1>& operator OP (Swizzle<T1,r,mask1>& a,const Swizzle<T2,r,mask2>& b)\
{\
	for(uint i=0; i<r; ++i)\
		a[i] OP b[i];\
	return a;\
}\
template<typename T1,typename T2,uint r,uint mask>\
Swizzle<T1,r,mask>& operator OP (Swizzle<T1,r,mask>& a,const Mat<T2,r,1>& b)\
{\
	for(uint i=0; i<r; ++i)\
		a[i] OP b[i];\
	return a;\
}\
template<typename T1,typename T2,uint r,uint mask>\
Swizzle<T1,r,mask>& operator OP (Swizzle<T1,r,mask>& a,const T2 b)\
{\
	for(uint i=0; i<r; ++i)\
		a[i] OP b;\
	return a;\
}\
template<typename T1,typename T2,uint r,uint mask>\
Mat<T1,r,1>& operator OP (Mat<T1,r,1>& a,const Swizzle<T1,r,mask>& b)\
{\
	for(uint i=0; i<r; ++i)\
		a[i] = b[i];\
	return a;\
}
////////////////////////////////////////////////////////////////////////////////

SWIZZLE_AS(+=)
SWIZZLE_AS(-=)
SWIZZLE_AS(*=)
SWIZZLE_AS(/=)
SWIZZLE_AS(&=)
SWIZZLE_AS(|=)
SWIZZLE_AS(^=)
#undef SWIZZLE_AS

////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
const Mat<T,r,1> operator - (const Swizzle<T,r,mask>& a)
{
	Mat<T,r,1> m;
	for(uint i=0; i<r; ++i)
		m[i] = -a[i];
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
const Mat<bool,r,1> operator ! (const Swizzle<T,r,mask>& a)
{
	Mat<bool,r,1> m;
	for(uint i=0; i<r; ++i)
		m[i] = !a[i];
	return m;
}
////////////////////////////////////////////////////////////////////////////////
