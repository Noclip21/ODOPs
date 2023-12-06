#pragma once

#include <iostream>

#include "Morton.h"


template<typename T,uint N>
inline std::ostream& operator << (std::ostream& out,const Morton<T,N>& morton)
{
	constexpr uint n = sizeof(T) << 3;
	for(uint i=1; i<=n; ++i)
		std::cout << ((morton.u >> (n-i)) & 0b1) << (((n-i)%N || !(n-i))?"":"|");
	return out;
}
