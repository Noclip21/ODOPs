#pragma once

#include "types.h"
#include "../bitmap/Bitmap.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,uint c1>
struct Pixel
{
	T1 data[c1];

	Pixel() = default;
	Pixel(const Pixel&) = default;
	Pixel(const T1 a)
	{ for(uint i=0; i<c1; ++i) data[i] = a; }
	template<typename ...T2>
	requires(sizeof...(T2) == c1 && sizeof...(T2) > 1)
	Pixel(const T2...args):data{T1(args)...}{};

	inline T1& operator [] (const uint);
	inline const T1& operator [] (const uint) const;

	inline operator const T1*() const;
	inline explicit operator const T1() const;

	template<typename T2,uint c2>
	requires(c2 > 0)
	inline operator Pixel<T2,c2>() const;

};
////////////////////////////////////////////////////////////////////////////////
template<>
struct Pixel<char,0>
{
	union{
		uint8_t data8[4];
		float data32f[4];
	};
	const uint format;

	inline Pixel(const Bitmap::Format);
	inline Pixel(const void*,const uint);
	inline Pixel(const Pixel& p);
	template<typename T,uint c>
	Pixel(const Pixel<T,c>&);
	
	inline operator const uint8_t() const;
	inline operator const float() const;

	inline pixel& operator = (const pixel&);
	template<typename T,uint c>
	inline pixel& operator = (const Pixel<T,c>&);

	template<typename T,uint c>
	inline operator const Pixel<T,c>() const;
};
////////////////////////////////////////////////////////////////////////////////
