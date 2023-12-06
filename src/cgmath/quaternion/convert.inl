#pragma once

#include "Quaternion.h"

////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Mat<T,3,3> tomat3(const Quaternion<T>& q)
{
	Mat<T,3,3> m;
	const T xw = q.x*q.w, xx = q.x*q.x, yy = q.y*q.y,
	        yw = q.y*q.w, xy = q.x*q.y, yz = q.y*q.z,
	        zw = q.z*q.w, xz = q.x*q.z, zz = q.z*q.z;
	m(0) = Mat<T,3,1>(1-2*(yy+zz),  2*(xy+zw),  2*(xz-yw));
	m(1) = Mat<T,3,1>(  2*(xy-zw),1-2*(xx+zz),  2*(yz+xw));
	m(2) = Mat<T,3,1>(  2*(xz+yw),  2*(yz-xw),1-2*(xx+yy));
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Mat<T,4,4> tomat4(const Quaternion<T>& q)
{
	return Mat<T,4,4>(tomat3(q));
}
////////////////////////////////////////////////////////////////////////////////
