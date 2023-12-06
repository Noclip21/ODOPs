#pragma once

#include <cmath>

#include "nearest.inl"


////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T bilinear(
	const T* data,
	const int w,const int h,
	const float u,const float v,
	const T& border)
{
	const float i = u*(w-1), j = v*(h-1);

	const int i0 = std::floor(i), j0 = std::floor(j),
	          i1 = std::ceil(i),  j1 = std::ceil(j);

	const T
		p00 = nearest<W,T>(data,w,h,i0,j0,border), p01 = nearest<W,T>(data,w,h,i0,j1,border),
		p10 = nearest<W,T>(data,w,h,i1,j0,border), p11 = nearest<W,T>(data,w,h,i1,j1,border);

	float s = j-j0,
	      t = i-i0;

	s = s*s*(3-2*s);
	t = t*t*(3-2*t);

	const float st = s*t;

	return p00*(1-s-t+st)+p01*(s-st)+p10*(t-st)+p11*st;
}
////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T bilinear(const Texture<T>& tex,const float u,const float v)
{
	return bilinear<W,T>(tex.data,tex.w,tex.h,u,v,tex.border);
}
////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T bilinear(const TextureLod<T>& tex,const float u,const float v,int k)
{
	k = (k<0)?0:((k>=tex.levels)?tex.levels-1:k);
	auto [w,h] = tex.dimensions(k);
	return bilinear<W,T>(tex(k),w,h,u,v,tex.border);
}
////////////////////////////////////////////////////////////////////////////////
