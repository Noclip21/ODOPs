#pragma once

#include <cmath>

#include "bilinear.inl"


////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T trilinear(
	const T* data0,const T* data1,
	const int w0,const int h0,
	const int w1,const int h1,
	const float u,const float v,float s,
	const T& border)
{
	     if(s <= 0) return bilinear<W,T>(data0,w0,h0,u,v,border);
	else if(s >= 1) return bilinear<W,T>(data1,w1,h1,u,v,border);

	const T
		p0 = bilinear<W,T>(data0,w0,h0,u,v,border),
		p1 = bilinear<W,T>(data1,w1,h1,u,v,border);

	return p0*(1-s)+p1*s;
}
////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T trilinear(const TextureLod<T>& tex,const float u,const float v,float s)
{
	int k0 = std::floor(s), k1 = k0+1;

	k0 = (k0<0)?0:((k0>=tex.levels)?tex.levels-1:k0);
	k1 = (k1<0)?0:((k1>=tex.levels)?tex.levels-1:k1);

	auto [w0,h0] = tex.dimensions(k0);
	auto [w1,h1] = tex.dimensions(k1);

	return trilinear<W,T>(tex(k0),tex(k1),w0,h0,w1,h1,u,v,s-k0,tex.border);
}
////////////////////////////////////////////////////////////////////////////////
