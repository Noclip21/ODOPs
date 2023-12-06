#pragma once

#include <algorithm>

#include "../common/types.h"


template<typename T1,uint C1,typename T2,uint C2>
const Pixel<T2,C2> convert(const Pixel<T1,C1>& p1)
{
	if constexpr (std::is_same_v<T1,T2> && C1 == C2)
		return p1;
	else
	if constexpr (C1 == C2)
	{
		Pixel<T2,C2> p2;
		for(uint i=0; i<C1; ++i)
			// 32f -> byte
			if constexpr (std::is_floating_point_v<T1> && std::is_integral_v<T2>)
				p2[i] = std::clamp(T2(p1[i]*255),T2(0),T2(255));
			// byte -> 32f
			else if constexpr (std::is_integral_v<T1> && std::is_floating_point_v<T2>)
				p2[i] = std::clamp(T2(p1[i])/T2(255),T2(0),T2(1));
			else
				p2[i] = T2(p1[i]);
		return p2;
	}
	else
	if constexpr (std::is_same_v<T1,T2>)
	{
		Pixel<T2,C2> p2;
		// r(gba) to grayscale
		if constexpr (C2 == 1)
		{
			float mean = 0;
			for(uint i=0; i<C1; ++i)
				mean += p1[i];
			return T2(mean/C1);
		}else
		// r(gba) to r(gba)
			for(uint i=0; i<C2; ++i)
				p2[i] = p1[i%C1];
		return p2;
	}
	else
	return convert<T2,C1,T2,C2>(convert<T1,C1,T2,C1>(p1));
}
