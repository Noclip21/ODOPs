#pragma once

#include <iostream>
#include <iomanip> // setprecision

#include "Mat.h"
#include <type_traits>


template<typename T,uint r,uint c>
inline std::ostream& operator << (std::ostream& out,const Mat<T,r,c>& m)
{
	std::ios_base::fmtflags flags;
	int precision;
	if constexpr (std::is_floating_point_v<T>)
	{
		flags = out.flags();
		precision = out.precision();
		out << std::fixed << std::setprecision(4);
	}

	for(uint i=0; i<r; ++i)
	{
		for(uint j=0; j<c; ++j)
		{
			const T v = m(i,j);
			if constexpr (std::is_signed_v<T>)
				out << ((v < 0)?"":" ");
			out << +v << ((j<c-1)?" ":"");
		}
		if(i < r-1) std::cout << std::endl;
	}

	if constexpr (std::is_floating_point_v<T>)
	{
		out.flags(flags);
		out.precision(precision);
	}

	return out;
}
