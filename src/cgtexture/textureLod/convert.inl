#pragma once

#include "operators.inl"
#include "../texture/constructors.inl"


template<typename T>
const Texture<T> TextureLod<T>::texture(const uint k) const
{ 
	auto [w,h] = dimensions(k);
	return Texture<T>((*this)(k),w,h,border);
}
