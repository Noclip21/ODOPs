#pragma once

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T determinant(const Mat<T,2,2>& a)
{ return a[0]*a[3]-a[2]*a[1]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T determinant(const Mat<T,3,3>& a)
{
	return
		+ a[0]*(a[4]*a[8] - a[7]*a[5])
		- a[3]*(a[1]*a[8] - a[7]*a[2])
		+ a[6]*(a[1]*a[5] - a[4]*a[2]);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T determinant(const Mat<T,4,4>& a)
{
	const T a2323 = a[10]*a[15]- a[14]*a[11];
	const T a1323 = a[ 6]*a[15]- a[14]*a[ 7];
	const T a1223 = a[ 6]*a[11]- a[10]*a[ 7];
	const T a0323 = a[ 2]*a[15]- a[14]*a[ 3];
	const T a0223 = a[ 2]*a[11]- a[10]*a[ 3];
	const T a0123 = a[ 2]*a[ 7]- a[ 6]*a[ 3];

	return
	  	+ a[ 0]*(a[5]*a2323 - a[9]*a1323 + a[13]*a1223) 
		- a[ 4]*(a[1]*a2323 - a[9]*a0323 + a[13]*a0223) 
		+ a[ 8]*(a[1]*a1323 - a[5]*a0323 + a[13]*a0123) 
		- a[12]*(a[1]*a1223 - a[5]*a0223 + a[ 9]*a0123);
}
////////////////////////////////////////////////////////////////////////////////
