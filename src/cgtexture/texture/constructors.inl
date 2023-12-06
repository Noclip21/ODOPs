#pragma once

#include <cstring>

#include "Texture.h"
#include "operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T>
Texture<T>::Texture():width(0),height(0),border(0),data(nullptr){}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
Texture<T>::Texture(const uint width,const uint height,
	const Texture<T>::pixel& border):
	width(width),height(height),border(border),data(new pixel[width*height]){}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
Texture<T>::Texture(const void* src,const uint width,const uint height,
	const Texture<T>::pixel& border):
	Texture(width,height,border)
{ memcpy(data,src,sizeof(pixel)*width*height); }
////////////////////////////////////////////////////////////////////////////////
template<typename T>
Texture<T>::Texture(const Texture<T>& t):
	Texture(t.data,t.width,t.height,t.border){}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
Texture<T>::Texture(const Bitmap& bitmap,
	const Texture<T>::pixel& border):
	Texture(bitmap.width,bitmap.height,border)
{
	for(int i=0; i<height; ++i)
		memcpy(data+i*width,(pixel*)bitmap.data+(height-i-1)*width,sizeof(pixel)*width);
}
////////////////////////////////////////////////////////////////////////////////
