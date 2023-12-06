#pragma once

#include <cstring>

#include "TextureLod.h"

#include "../texture/constructors.inl"
#include "../filters/downscale.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>::TextureLod():width(0),height(0),levels(0),border(0),levelOffsets(nullptr),data(nullptr){}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>::TextureLod(const uint width,const uint height,
	const TextureLod<T>::pixel& border):
	width(width),height(height),levels(1+std::ceil(std::log2(std::max(width,height)))),
	border(border),levelOffsets(nullptr),data(nullptr)
{
	size = 0;
	levelOffsets = new uint[levels];
	for(int i=0; i<levels; ++i)
	{
		levelOffsets[i] = size;
		auto [w,h] = dimensions(i);
		size += w*h;
	}
	data = new pixel[size];
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>::TextureLod(const void* src,const uint width,const uint height,
	const TextureLod<T>::pixel& border):
	TextureLod(width,height,border)
{
	memcpy(data,src,sizeof(pixel)*width*height);
	Texture<T> tex0(data,width,height);
	for(int i=1; i<levels; ++i)
	{
		auto [w1,h1] = dimensions(i);
		const Texture<T> tex1 = downscale(tex0,w1,h1);
		memcpy((*this)(i),tex1.data,sizeof(pixel)*w1*h1);
		tex0 = tex1;
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>::TextureLod(const Texture<T>& t):
	TextureLod(t.data,t.width,t.height,t.border){}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>::TextureLod(const TextureLod<T>& t):
	TextureLod(t.width,t.height,t.border)
{ memcpy(data,t.data,sizeof(pixel)*size); }
////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>::TextureLod(const Bitmap& bitmap,
	const TextureLod<T>::pixel& border):
	TextureLod(Texture(bitmap,border)){}
////////////////////////////////////////////////////////////////////////////////
