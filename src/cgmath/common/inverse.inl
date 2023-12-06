#pragma once

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/operators.inl"



////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Mat<T,2,2> inverse(const Mat<T,2,2>& a)
{
	return Mat<T,2,2>(
		+a[3],-a[1],
		-a[2],+a[0]
	)/determinant(a);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Mat<T,3,3> inverse(const Mat<T,3,3>& a)
{
	return Mat<T,3,3>(
		+(a[4]*a[8]-a[7]*a[5]), -(a[1]*a[8]-a[7]*a[2]), +(a[1]*a[5]-a[4]*a[2]),
		-(a[3]*a[8]-a[6]*a[5]), +(a[0]*a[8]-a[6]*a[2]), -(a[0]*a[5]-a[3]*a[2]),
		+(a[3]*a[7]-a[6]*a[4]), -(a[0]*a[7]-a[6]*a[1]), +(a[0]*a[4]-a[3]*a[1])
	)/determinant(a);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Mat<T,4,4> inverse(const Mat<T,4,4>& a)
{
	const T a2323 = a[10]*a[15] - a[14]*a[11];
	const T a1323 = a[ 6]*a[15] - a[14]*a[ 7];
	const T a1223 = a[ 6]*a[11] - a[10]*a[ 7];
	const T a0323 = a[ 2]*a[15] - a[14]*a[ 3];
	const T a0223 = a[ 2]*a[11] - a[10]*a[ 3];
	const T a0123 = a[ 2]*a[ 7] - a[ 6]*a[ 3];
	const T a2313 = a[ 9]*a[15] - a[13]*a[11];
	const T a1313 = a[ 5]*a[15] - a[13]*a[ 7];
	const T a1213 = a[ 5]*a[11] - a[ 9]*a[ 7];
	const T a2312 = a[ 9]*a[14] - a[13]*a[10];
	const T a1312 = a[ 5]*a[14] - a[13]*a[ 6];
	const T a1212 = a[ 5]*a[10] - a[ 9]*a[ 6];
	const T a0313 = a[ 1]*a[15] - a[13]*a[ 3];
	const T a0213 = a[ 1]*a[11] - a[ 9]*a[ 3];
	const T a0312 = a[ 1]*a[14] - a[13]*a[ 2];
	const T a0212 = a[ 1]*a[10] - a[ 9]*a[ 2];
	const T a0113 = a[ 1]*a[ 7] - a[ 5]*a[ 3];
	const T a0112 = a[ 1]*a[ 6] - a[ 5]*a[ 2];

	T det =
		+ a[ 0]*(a[5]*a2323 - a[9]*a1323 + a[13]*a1223) 
		- a[ 4]*(a[1]*a2323 - a[9]*a0323 + a[13]*a0223) 
		+ a[ 8]*(a[1]*a1323 - a[5]*a0323 + a[13]*a0123) 
		- a[12]*(a[1]*a1223 - a[5]*a0223 + a[ 9]*a0123);
	det = T(1)/det;

	return Mat<T,4,4>(
		+ det*(a[5]*a2323 - a[9]*a1323 + a[13]*a1223),
		- det*(a[1]*a2323 - a[9]*a0323 + a[13]*a0223),
		+ det*(a[1]*a1323 - a[5]*a0323 + a[13]*a0123),
		- det*(a[1]*a1223 - a[5]*a0223 + a[ 9]*a0123),
		- det*(a[4]*a2323 - a[8]*a1323 + a[12]*a1223),
		+ det*(a[0]*a2323 - a[8]*a0323 + a[12]*a0223),
		- det*(a[0]*a1323 - a[4]*a0323 + a[12]*a0123),
		+ det*(a[0]*a1223 - a[4]*a0223 + a[ 8]*a0123),
		+ det*(a[4]*a2313 - a[8]*a1313 + a[12]*a1213),
		- det*(a[0]*a2313 - a[8]*a0313 + a[12]*a0213),
		+ det*(a[0]*a1313 - a[4]*a0313 + a[12]*a0113),
		- det*(a[0]*a1213 - a[4]*a0213 + a[ 8]*a0113),
		- det*(a[4]*a2312 - a[8]*a1312 + a[12]*a1212),
		+ det*(a[0]*a2312 - a[8]*a0312 + a[12]*a0212),
		- det*(a[0]*a1312 - a[4]*a0312 + a[12]*a0112),
		+ det*(a[0]*a1212 - a[4]*a0212 + a[ 8]*a0112)
	);
}
