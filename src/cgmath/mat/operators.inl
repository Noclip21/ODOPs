#pragma once

#include <type_traits>

#include "Mat.h"

////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
const Mat<T,r,c>& Mat<T,r,c>::operator = (const Mat<T,r,c>& m) const
{ for(uint i=0; i<n; ++i) data[i] = m[i]; return *this; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>& Mat<T,r,c>::operator = (const Mat<T,r,c>& m)
{ for(uint i=0; i<n; ++i) data[i] = m[i]; return *this; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>::operator T() const requires(r*c == 1)
{ return data[0]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>::operator const T*() const
{ return &data[0]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
const T& Mat<T,r,c>::operator [] (const uint i) const
{ return data[i]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
const T& Mat<T,r,c>::operator () (const uint i,const uint j) const
{ return data[i+j*r]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
const Mat<T,r,1>& Mat<T,r,c>::operator () (const uint i) const
{ return *((const Mat<T,r,1>*)(&data[i*r])); }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>::operator T*()
{ return &data[0]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
T& Mat<T,r,c>::operator [] (const uint i)
{ return data[i]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
T& Mat<T,r,c>::operator () (const uint i,const uint j)
{ return data[i+j*r]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,1>& Mat<T,r,c>::operator () (const uint i)
{ return *((Mat<T,r,1>*)(&data[i*r])); }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
const T* Mat<T,r,c>::begin() const { return &data[0]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
const T* Mat<T,r,c>::end() const { return &data[r*c]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
T* Mat<T,r,c>::begin() { return &data[0]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
T* Mat<T,r,c>::end() { return &data[r*c]; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define MAT_AR(OP)\
template<typename T1,typename T2,uint r,uint c>\
inline const Mat<std::common_type_t<T1,T2>,r,c> operator OP (const Mat<T1,r,c>& a,const Mat<T2,r,c>& b)\
{\
	Mat<std::common_type_t<T1,T2>,r,c> m;\
	for(uint i=0; i<a.n; ++i)\
		m[i] = a[i] OP b[i];\
	return m;\
}\
template<typename T1,typename T2,uint r,uint c>\
inline const Mat<std::common_type_t<T1,T2>,r,c> operator OP (const Mat<T1,r,c>& a,const T2 b)\
{\
	Mat<std::common_type_t<T1,T2>,r,c> m;\
	for(uint i=0; i<a.n; ++i)\
		m[i] = a[i] OP b;\
	return m;\
}\
template<typename T1,typename T2,uint r,uint c>\
inline const Mat<std::common_type_t<T1,T2>,r,c> operator OP (const T1 a,const Mat<T2,r,c>& b)\
{\
	Mat<std::common_type_t<T1,T2>,r,c> m;\
	for(uint i=0; i<b.n; ++i)\
		m[i] = a OP b[i];\
	return m;\
}
////////////////////////////////////////////////////////////////////////////////

MAT_AR(+)
MAT_AR(-)
MAT_AR(/)
MAT_AR(&&)
MAT_AR(||)
MAT_AR(==)
MAT_AR(!=)
MAT_AR(<)
MAT_AR(>)
MAT_AR(<=)
MAT_AR(>=)
MAT_AR(&)
MAT_AR(|)
MAT_AR(^)
#undef MAT_AR

////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
inline const Mat<std::common_type_t<T1,T2>,r,c> compMulti(const Mat<T1,r,c>& a,const Mat<T2,r,c>& b)
{
	Mat<std::common_type_t<T1,T2>,r,c> m;
	for(uint i=0; i<a.n; ++i)
		m[i] = a[i] * b[i];
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
requires(r == 1 || c == 1)
inline const Mat<std::common_type_t<T1,T2>,r,c> operator * (const Mat<T1,r,c>& a,const Mat<T2,r,c>& b)
{ return compMulti(a,b); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
inline const Mat<std::common_type_t<T1,T2>,r,c> operator * (const Mat<T1,r,c>& a,const T2 b)
{
	Mat<std::common_type_t<T1,T2>,r,c> m;
	for(uint i=0; i<a.n; ++i)
		m[i] = a[i] * b;
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
inline const Mat<std::common_type_t<T1,T2>,r,c> operator * (const T1 a,const Mat<T2,r,c>& b)
{
	Mat<std::common_type_t<T1,T2>,r,c> m;
	for(uint i=0; i<b.n; ++i)
		m[i] = a * b[i];
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint n,uint c>
inline const Mat<std::common_type_t<T1,T2>,r,c> operator * (const Mat<T1,r,n>& a,const Mat<T2,n,c>& b)
{
	Mat<std::common_type_t<T1,T2>,r,c> m;
	for(uint i=0; i<c; ++i)
	{
		auto& mv = m(i);
		const auto& bv = b(i);
		for(uint j=0; j<r; ++j)
		{
			mv[j] = 0;
			for(uint k=0; k<n; ++k)
				mv[j] += a(j,k)*bv[k];
		}
	}
	return m;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define MAT_AS(OP)\
template<typename T1,typename T2,uint r,uint c>\
inline Mat<T1,r,c>& operator OP (Mat<T1,r,c>& a,const Mat<T2,r,c>& b)\
{\
	for(uint i=0; i<a.n; ++i)\
		a[i] OP b[i];\
	return a;\
}\
template<typename T1,typename T2,uint r,uint c>\
inline Mat<T1,r,c>& operator OP (Mat<T1,r,c>& a,const T2 b)\
{\
	for(uint i=0; i<a.n; ++i)\
		a[i] OP b;\
	return a;\
}
////////////////////////////////////////////////////////////////////////////////

MAT_AS(+=)
MAT_AS(-=)
MAT_AS(/=)
MAT_AS(&=)
MAT_AS(|=)
MAT_AS(^=)
#undef MAT_AS

////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
requires(r == 1 || c == 1)
inline Mat<T1,r,c>& operator *= (Mat<T1,r,c>& a,const Mat<T2,r,c> b)
{ 
	for(uint i=0; i<a.n; ++i)
		a[i] *= b[i];
	return a;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint r,uint c>
inline Mat<T1,r,c>& operator *= (Mat<T1,r,c>& a,const T2 b)
{
	for(uint i=0; i<a.n; ++i)
		a[i] *= b;
	return a;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline Mat<T1,n,n>& operator *= (Mat<T1,n,n>& a,const Mat<T2,n,n> b)
{ return a = a*b; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
inline const Mat<T,r,c> operator - (const Mat<T,r,c>& a)
{
	Mat<T,r,c> m;
	for(uint i=0; i<m.n; ++i)
		m[i] = -a[i];
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
inline const Mat<bool,r,c> operator ! (const Mat<T,r,c>& a)
{
	Mat<bool,r,c> m;
	for(uint i=0; i<m.n; ++i)
		m[i] = !a[i];
	return m;
}
////////////////////////////////////////////////////////////////////////////////
