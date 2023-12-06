#pragma once

#include <type_traits>

#include <algorithm>
#include <cstring>

#include "Pixel.h"
#include "constructors.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
T& Pixel<T,c>::operator [] (const uint i) { return data[i]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
const T& Pixel<T,c>::operator [] (const uint i) const { return data[i]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
Pixel<T,c>::operator const T*() const
{ return &data[0]; }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,uint c1>
Pixel<T1,c1>::operator const T1() const
{
	if constexpr (c1 == 1)
		return data[0];
	else{
		float mean = 0;
		for(uint i=0; i<c1; ++i)
			mean += data[i];
		return T1(mean/c1);
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,uint c1>
template<typename T2,uint c2>
requires(c2 > 0)
Pixel<T1,c1>::operator Pixel<T2,c2>() const
{
	if constexpr (c1 == c2)
	{
		Pixel<T2,c2> p;
		for(uint i=0; i<c1; ++i)
			// 32f -> byte
			if constexpr (std::is_floating_point<T1>::value && std::is_integral<T2>::value)
				p.data[i] = std::clamp(T2(data[i]*255),T2(0),T2(255));
			// byte -> 32f
			else if constexpr (std::is_integral<T1>::value && std::is_floating_point<T2>::value)
				p.data[i] = std::clamp(T2(data[i])/T2(255),T2(0),T2(1));
			else
				p.data[i] = T2(data[i]);
		return p;
	}else if constexpr (std::is_same<T1,T2>::value)
	{
		Pixel<T2,c2> p;
		// r(gba) to grayscale
		if constexpr (c2 == 1)
			p.data[0] = (T1)(*this);
		else
		// r(gba) to r(gba)
			for(uint i=0; i<c2; ++i)
				p.data[i] = data[i%c1];
		return p;
	}else
		return (Pixel<T2,c2>)(Pixel<T2,c1>)(*this);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
pixel::operator const uint8_t() const
{
	const uint t = Bitmap::bytesPerChannel(format);
	const uint c = Bitmap::numChannels(format);
	if(t == 1)
	{
		if(c == 1)      return (uint8_t)*(const r8*)this;
		else if(c == 2) return (uint8_t)*(const rg8*)this;
		else if(c == 3) return (uint8_t)*(const rgb8*)this;
		else if(c == 4) return (uint8_t)*(const rgba8*)this;
	}else if(t == 4){
		if(c == 1)      return (uint8_t)(const r8)*(const r32f*)this;
		else if(c == 2) return (uint8_t)(const rg8)*(const rg32f*)this;
		else if(c == 3) return (uint8_t)(const rgb8)*(const rgb32f*)this;
		else if(c == 4) return (uint8_t)(const rgba8)*(const rgba32f*)this;
	}
	return data8[0];
}
////////////////////////////////////////////////////////////////////////////////
pixel::operator const float() const
{
	const uint t = Bitmap::bytesPerChannel(format);
	const uint c = Bitmap::numChannels(format);
	if(t == 1)
	{
		if(c == 1)      return (float)(const r32f)*(const r8*)this;
		else if(c == 2) return (float)(const rg32f)*(const rg8*)this;
		else if(c == 3) return (float)(const rgb32f)*(const rgb8*)this;
		else if(c == 4) return (float)(const rgba32f)*(const rgba8*)this;
	}else if(t == 4){
		if(c == 1)      return (float)*(const r32f*)this;
		else if(c == 2) return (float)*(const rg32f*)this;
		else if(c == 3) return (float)*(const rgb32f*)this;
		else if(c == 4) return (float)*(const rgba32f*)this;
	}
	return data32f[0];
}
////////////////////////////////////////////////////////////////////////////////
pixel& pixel::operator = (const pixel& p)
{
	if(p.format == format)
	{
		memcpy(this,&p,Bitmap::bytesPerPixel(format));
		return *this;
	}

	const uint t1 = Bitmap::bytesPerChannel(format);
	const uint t2 = Bitmap::bytesPerChannel(p.format);
	const uint c1 = Bitmap::numChannels(format);
	const uint c2 = Bitmap::numChannels(p.format);

	if(c1 == c2)
	{
		if(t1 == 1 && t2 == 4)
			for(uint i=0; i<c1; ++i)
				data8[i] = std::clamp(int(p.data32f[i]*255),0,255);
		else
			for(uint i=0; i<c1; ++i)
				data32f[i] = std::clamp(float(p.data8[i])/255.0f,0.0f,1.0f);
		return *this;
	}else if(t1 == t2)
	{
		if(t1 == 1)
		{
			if(c1 == 1) data8[0] = (uint8_t)p;
			else
				for(uint i=0; i<c1; ++i)
					data8[i] = p.data8[i%c2];
		}else{
			if(c1 == 1) data32f[0] = (float)p;
			else
				for(uint i=0; i<c1; ++i)
					data32f[i] = p.data32f[i%c2];
		}
		return *this;
	}
	
	return *this = (pixel(Bitmap::genFormat(t1,c2)) = p);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
pixel& pixel::operator = (const Pixel<T,c>& pp)
{ return *this = pixel(pp); }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
pixel::operator const Pixel<T,c>() const
{ return *(const Pixel<T,c>*)&(pixel(Bitmap::genFormat(sizeof(T),c)) = *this); }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define VEC_OP(OP)\
template<typename T,uint c> \
inline const Pixel<T,c> operator OP (const Pixel<T,c>& a,const Pixel<T,c>& b)\
{ Pixel<T,c> v; for(uint i=0; i<c; ++i) v[i] = a[i] OP b[i]; return v; } \
template<typename T1,typename T2,uint c> \
requires(std::is_convertible_v<T2,T1>)\
inline const Pixel<T1,c> operator OP (const Pixel<T1,c>& a,const T2 b)\
{ Pixel<T1,c> v; for(uint i=0; i<c; ++i) v[i] = a[i] OP b; return v; }\
template<typename T1,typename T2,uint c> \
requires(std::is_convertible_v<T2,T1>)\
inline const Pixel<T1,c> operator OP (const T2 a,const Pixel<T1,c>& b)\
{ Pixel<T1,c> v; for(uint i=0; i<c; ++i) v[i] = a OP b[i]; return v; } \

VEC_OP(+)
VEC_OP(-)
VEC_OP(*)
VEC_OP(/)
#undef VEC_OP
////////////////////////////////////////////////////////////////////////////////
