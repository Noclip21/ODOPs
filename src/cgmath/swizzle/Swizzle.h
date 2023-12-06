#pragma once

#include "../common/types.h"

template<typename T,uint r,uint mask>
struct Swizzle
{
	enum{
		ROW0 = (mask >> 0) & 0x3,
		ROW1 = (mask >> 2) & 0x3,
		ROW2 = (mask >> 4) & 0x3,
		ROW3 = (mask >> 6) & 0x3,
	};

	T data[1];

	template<typename T2>
	inline Swizzle& operator = (const Mat<T2,r,1>&);
	template<typename T2,uint mask2>
	inline Swizzle& operator = (const Swizzle<T2,r,mask2>&);
	inline Swizzle& operator = (const Swizzle&);

	inline const T operator [] (const uint) const;
	inline T& operator [] (const uint);

	inline operator const Mat<T,r,1>() const;

};
