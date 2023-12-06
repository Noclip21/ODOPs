#pragma once

#include "TextureLod.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T>
TextureLod<T>& TextureLod<T>::operator = (const TextureLod<T>& t)
{
	if(width != t.width || height != t.height)
	{
		width  = t.width;
		height = t.height;
		levels = t.levels;
		size   = t.size;

		delete[] levelOffsets;
		delete[] data;

		levelOffsets = new uint[levels];
		memcpy(levelOffsets,t.levelOffsets,sizeof(uint)*levels);

		data = new pixel[size];
	}

	border = t.border;

	memcpy(data,t.data,sizeof(pixel)*size);

	return *this;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T>
typename TextureLod<T>::pixel*
TextureLod<T>::operator () (const uint k)
{ return data+levelOffsets[k]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T>
const typename TextureLod<T>::pixel*
TextureLod<T>::operator () (const uint k) const
{ return data+levelOffsets[k]; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T>
typename TextureLod<T>::pixel&
TextureLod<T>::operator () (const uint i,const uint j,const uint k)
{ return (*this)(k)[dimensionW(k)*j+i]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T>
const typename TextureLod<T>::pixel&
TextureLod<T>::operator () (const uint i,const uint j,const uint k) const
{ return (*this)(k)[dimensionW(k)*j+i]; }
////////////////////////////////////////////////////////////////////////////////
