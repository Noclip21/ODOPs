#pragma once

#include <cmath>

#include "TextureLod.h"

////////////////////////////////////////////////////////////////////////////////
template<typename T>
const std::tuple<int,int> TextureLod<T>::dimensions(int i) const
{ 
	return {(w+(1 << i)-1) >> i,(h+(1 << i)-1) >> i};
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
const int TextureLod<T>::dimensionW(const int i) const
{
	return (w+(1 << i)-1) >> i;
}
////////////////////////////////////////////////////////////////////////////////
