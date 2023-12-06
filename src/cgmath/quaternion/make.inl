#pragma once

#include "Quaternion.h"

#include "convert.inl"

#include "../mat/operators.inl"
#include "../common/normalize.inl"
#include "../common/cross.inl"
#include "../common/dot.inl"
#include "../common/functions.inl"

////////////////////////////////////////////////////////////////////////////////
// Make quaternion from axix-angle
template<typename T1,typename T2>
inline const Quaternion<std::common_type_t<T1,T2>> make(const Mat<T1,3,1>& v,const T2 w)
{
	Quaternion<std::common_type_t<T1,T2>> q;
  	q.xyz  = v; // Assumes normalized
  	q.xyz *= sin(w*std::common_type_t<T1,T2>(0.5));
  	q.w    = cos(w*std::common_type_t<T1,T2>(0.5));
  	return q;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint mask>
inline const auto make(const Swizzle<T1,3,mask>& s,const T2 w)
{ return make(Mat<T1,3,1>(s),w); }
////////////////////////////////////////////////////////////////////////////////
// Based on "A Robust Method to Extract the Rotational Part of Deformations"
// https://matthias-research.github.io/pages/publications/stablePolarDecomp.pdf
template<typename T>
inline const Quaternion<T> make(const Mat<T,3,3>& m,uint i = 16,const float tol = 5e-5)
{
	Quaternion<T> q;
	while(i-- > 0)
	{
		const Mat<T,3,3> r = tomat3(q);
		Mat<T,3,1> omega = cross(r(0),m(0)) + cross(r(1),m(1)) + cross(r(2),m(2));
		omega /= abs(dot(r(0),m(0)) + dot(r(1),m(1)) + dot(r(2),m(2)) + tol);
		float w = length(omega);
		if(w < tol) break;
		q = make(omega/w,w)*q;
		normalize(q);
	}
	return q;
}
////////////////////////////////////////////////////////////////////////////////
// Based on "Quaternion between two vectors"
// http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
template<typename T1,typename T2>
inline const Quaternion<std::common_type_t<T1,T2>>
make(const Mat<T1,3,1>& from,const Mat<T2,3,1>& to,const float tol = 5e-5)
{
	auto v  = cross(from,to);
	float c = dot(from,to);
	if(length(v) < 5e-5 && c < 0)
	{
		v = (abs(from.x)>abs(from.z))?Mat<T1,3,1>(-from.y,from.x,0):Mat<T1,3,1>(0,-from.z,from.y);
		return normalize(Quaternion<std::common_type_t<T1,T2>>(v,0));
	}
	Quaternion<std::common_type_t<T1,T2>> q(v,c);
	q.w += length(q);
	return normalize(q);
}
////////////////////////////////////////////////////////////////////////////////
