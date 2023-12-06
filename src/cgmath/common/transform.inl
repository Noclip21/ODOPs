#pragma once

#include <cmath>

#include "types.h"
#include "defaults.inl"
#include "dot.inl"
#include "normalize.inl"
#include "cross.inl"
#include "transpose.inl"

#include "../mat/Mat.h"
#include "../mat/constructors.inl"
#include "../mat/operators.inl"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline const Mat<T1,n,n> scale(Mat<T1,n,n> m,const Mat<T2,n,1>& s)
{ for(uint i=0; i<n; ++i) m(i,i) *= s[i]; return m; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask>
inline const Mat<T1,n,n> scale(Mat<T1,n,n> m,const Swizzle<T2,n,mask>& s)
{ for(uint i=0; i<n; ++i) m(i,i) *= s[i]; return m; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline const Mat<T1,n,n> scale(Mat<T1,n,n> m,const T2 s)
{ for(uint i=0; i<n; ++i) m(i,i) *= s; return m; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline const Mat<T1,n+1,n+1> translate(Mat<T1,n+1,n+1> m,const Mat<T2,n,1>& t)
{ *((Mat<T1,n,1>*)&m(n)) += t*m(n,n); return m; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask>
inline const Mat<T1,n+1,n+1> translate(Mat<T1,n+1,n+1> m,const Swizzle<T2,n,mask>& t)
{ *((Mat<T1,n,1>*)&m(n)) += t*m(n,n); return m; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask>
inline const Mat<T1,n+1,n+1> translate(Mat<T1,n+1,n+1> m,const T2 t)
{ *((Mat<T1,n,1>*)&m(n)) += t*m(n,n); return m; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline const Mat<T1,n,n> reflect(const Mat<T1,n,n>& m,const Mat<T2,n,1>& v)
{
	Mat<T1,n,n> r;
	for(uint i=0; i<n; ++i)
	{
		r(i,i) = 1-2*v[i]*v[i];
		for(uint j=0; j<i; ++j)
			r(i,j) = r(j,i) = -2*v[i]*v[j];
	}
	return r*m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask>
inline const Mat<T1,n,n> reflect(const Mat<T1,n,n>& m,const Swizzle<T2,n,mask>& v)
{
	Mat<T1,n,n> r;
	for(uint i=0; i<n; ++i)
	{
		r(i,i) = 1-2*v[i]*v[i];
		for(uint j=0; j<i; ++j)
			r(i,j) = r(j,i) = -2*v[i]*v[j];
	}
	return r*m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline const Mat<T1,n,n> reflect(const Mat<T1,n,n>& m,const T2 v)
{
	const T2 vv = -2*v*v;
	Mat<T1,n,n> r;
	for(uint i=0; i<m.n; ++i)
		r[i] = vv;
	return r*m;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define TRANSFORM_ROTATE(NAME,L1,L2)\
template<typename T1,typename T2,uint n>\
requires(n == 3 || n == 4)\
inline const Mat<T1,n,n> NAME(Mat<T1,n,n> m,const T2 angle)\
{\
	const T1 s = sin(angle),c = cos(angle);\
	for(uint i=0; i<n; ++i)\
	{\
		T1 &a = m(L1,i),&b = m(L2,i),tmp = a;\
		a = c*a   - s*b;\
		b = s*tmp + c*b;\
	}\
	return m;\
}
TRANSFORM_ROTATE(rotateX,1,2)
TRANSFORM_ROTATE(rotateY,2,0)
TRANSFORM_ROTATE(rotateZ,0,1)
#undef TRANSFORM_ROTATE
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2 = decltype(cg::up)::Type>
inline const Mat<std::common_type_t<T1,T2>,3,3>
	lookRotation(const Mat<T1,3,1>& direction,
	             const Mat<T2,3,1>& up = cg::up)
{
	const auto z = normalize(direction);
	const auto x = normalize(cross(up,z));
	const auto y = cross(z,x);
	return Mat<std::common_type_t<T1,T2>,3,3>(x,y,z);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline auto lookRotation(const T1& direction,const T2& up)
{ return lookRotation(mat_t<T1>(direction),mat_t<T2>(up)); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1>
inline auto lookRotation(const T1& direction)
{ return lookRotation(mat_t<T1>(direction),cg::up); }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,typename T3 = decltype(cg::up)::Type>
inline const Mat<std::common_type_t<T1,T2,T3>,4,4>
	lookAt(const Mat<T1,3,1>& eye,
	       const Mat<T2,3,1>& center,
	       const Mat<T3,3,1>& up = cg::up)
{
	using T = std::common_type_t<T1,T2,T3>;
	Mat<T,3,3> m3(transpose(lookRotation(eye-center,up)));
	Mat<T,4,4> m4(m3);
	m4(3) = Mat<T,4,1>(m3*(-eye),1);
	return m4;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,typename T3>
inline auto lookAt(const T1& eye,const T2& center,const T3& up)
{ return lookAt(mat_t<T1>(eye),mat_t<T2>(center),mat_t<T3>(up)); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2>
inline auto lookAt(const T1& eye,const T2& center)
{ return lookAt(mat_t<T1>(eye),mat_t<T2>(center)); }
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
inline const mat4 ortho(
	const float left, const float right,
	const float top,  const float bottom,
	const float near, const float far)
{
	const float 
		rl = right - left,
		tb = top   - bottom,
		fn = far   - near;
	return transpose(
		mat4(2/rl, 0,    0,    (right+left)/(-rl),
		     0,    2/tb, 0,    (top+bottom)/(-tb),
		     0,    0,    2/fn, (near+far)/fn,
		     0,    0,    0,    1));
}
////////////////////////////////////////////////////////////////////////////////
inline const mat4 ortho(
	const float aspect,const float scale,
	const float near,  const float far)
{
		// Orthogonal projection matrix
	const float fn = far - near;
	return transpose(
		mat4(1/(aspect*scale), 0,       0,    0,
		     0,                1/scale, 0,    0,
		     0,                0,       2/fn, (near+far)/fn,
		     0,                0,       0,    1));
}
////////////////////////////////////////////////////////////////////////////////
inline const mat4 proj(
	const float aspect,const float fov,
	const float near,  const float far)
{
		// Perspective projection matrix
	const float 
		scale = 1.0f/tan(fov*0.5f),
		fn    = far - near;
	return transpose(
		mat4(scale/aspect,  0,      0,              0,
		     0,             scale,  0,              0,
		     0,             0,     -(far+near)/fn, -2*far*near/fn,
		     0,             0,     -1,              0));
}
////////////////////////////////////////////////////////////////////////////////
inline const mat4 proj(
	const float aspect,const float fov,
	const float near)
{
		// Infinite perspective projection matrix
	const float scale = 1.0f/tan(fov*0.5f);
	return transpose(
		mat4(scale/aspect,  0,      0,  0,
		     0,             scale,  0,  0,
		     0,             0,     -1, -2*near,
		     0,             0,     -1,  0));
}
////////////////////////////////////////////////////////////////////////////////
