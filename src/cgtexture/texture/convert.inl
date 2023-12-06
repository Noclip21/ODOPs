#pragma once

#include <cgasset>

#include "Texture.h"


template<typename T>
inline Texture<T>::operator const Bitmap() const
{
	Bitmap bitmap(data,width,height,Bitmap::genFormat(sizeof(typename T::Type),T::n));
	for(int i=0; i<height; ++i)
		memcpy((pixel*)bitmap.data+i*width,data+(height-i-1)*width,sizeof(pixel)*width);
	return bitmap;
}
