#pragma once

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/constructors.inl"
#include "../mat/operators.inl"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
requires(r == 1 || c == 1)
inline const Mat<T,c,r> transpose(const Mat<T,r,c>& a)
{ return *((const Mat<T,c,r>*)&a); }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint mask>
inline const Mat<T,1,r> transpose(const Swizzle<T,r,mask>& a)
{ return transpose((const Mat<T,r,1>)a); }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
inline const Mat<T,c,r> transpose(const Mat<T,r,c>& a)
{
	Mat<T,c,r> m;
	for(uint i=0; i<a.n; ++i)
		m[i] = a(i/c,i%c);
	return m;
}
////////////////////////////////////////////////////////////////////////////////
