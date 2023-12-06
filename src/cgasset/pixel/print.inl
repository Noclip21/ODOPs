#pragma once

#include <iostream>
#include <iomanip> // setprecision

#include "../bitmap/Bitmap.h"

#include "Pixel.h"
#include "operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
requires(c > 0)
inline std::ostream& operator << (std::ostream& out,const Pixel<T,c>& p)
{
	if constexpr (std::is_same_v<T,float>)
	{
		std::ios_base::fmtflags flags(out.flags());
		int precision = out.precision();
		out << std::fixed << std::setprecision(3);
		for(uint i=0; i<c; ++i)
			out << ((p[i] < 0)?" ":"  ") << p[i];
		out.flags(flags);
		out.precision(precision);
	}else{
		for(uint i=0; i<c; ++i)
			out << (int)p[i] << ((i<c-1)?" ":"");
	}
	return out;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
requires(c == 0)
inline std::ostream& operator << (std::ostream& out,const Pixel<T,c>& p)
{
	const uint size = Bitmap::numChannels(p.format);
	if(Bitmap::bytesPerChannel(p.format) == 4)
	{
		std::ios_base::fmtflags flags(out.flags());
		int precision = out.precision();
		out << std::fixed << std::setprecision(3);
		for(uint i=0; i<size; ++i)
			out << ((p.data32f[i] < 0)?" ":"  ") << p.data32f[i];
		out.flags(flags);
		out.precision(precision);
	}else{
		for(uint i=0; i<size; ++i)
			out << (int)p.data8[i] << ((i<c-1)?" ":"");
	}
	return out;
}
////////////////////////////////////////////////////////////////////////////////
