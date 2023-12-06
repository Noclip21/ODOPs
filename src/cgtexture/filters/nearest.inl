#pragma once

#include <cmath>

#include "../common/types.h"

#include "../texture/operators.inl"
#include "../textureLod/operators.inl"


template<Wrap W,typename T>
inline const T nearest(const T* data,const int w,const int h,int i,int j,const T& border)
{
	if constexpr (W == Wrap::CLAMP_EDGE)
		return data[w*((j<0)?0:((j>=h)?h-1:j)) + ((i<0)?0:((i>=w)?w-1:i))];
	if constexpr (W == Wrap::CLAMP_BORDER)
		return (i >= 0 && i < w && j >= 0 && j < h)?data[w*j+i]:border;
	if constexpr (W == Wrap::REPEAT)
		return data[w*((j%h+h)%h) + (i%w+w)%w];
	if constexpr (W == Wrap::REPEAT_MIRRORED)
	{
		const int w2 = 2*w, h2 = 2*h;
		i = (i%w2+w2)%w2 - w; j = (j%h2+h2)%h2 - h;
		i = (i>=0)?i:-(1+i);  j = (j>=0)?j:-(1+j); 
		i = w-i-1;            j = h-j-1;
		return data[w*j+i];
	}
}
////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T nearest(const Texture<T>& tex,int i,int j)
{
	return nearest<W,T>(tex.data,tex.w,tex.h,i,j,tex.border);
}
////////////////////////////////////////////////////////////////////////////////
template<Wrap W,typename T>
inline const T nearest(const TextureLod<T>& tex,int i,int j,int k)
{
	k = (k<0)?0:((k>=tex.levels)?tex.levels-1:k);
	auto [w,h] = tex.dimensions(k);
	return nearest<W,T>(tex(k),w,h,i,j,tex.border);
}
////////////////////////////////////////////////////////////////////////////////
