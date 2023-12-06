#pragma once

#include <cgasset>

#include "../common/types.h"


template<typename T>
struct Texture
{
	using pixel = T;

	union{
		struct { uint width,height; };
		struct { uint w,h; };
	};
	pixel  border;
	pixel* data;


	inline Texture();
	inline Texture(const uint,const uint,const pixel& = pixel());
	inline Texture(const void*,const uint,const uint,const pixel& = pixel());
	inline Texture(const Texture&);
	inline Texture(const Bitmap& bitmap,const pixel& = pixel());
	~Texture() { delete[] data; }


	// TODO
	inline Texture& operator = (const Texture&);


	inline operator const Bitmap() const;


	// No bounds checking ====
	inline pixel& operator [] (const uint);
	inline const pixel& operator [] (const uint) const;

	inline pixel& operator () (const uint,const uint);
	inline const pixel& operator () (const uint,const uint) const;
	// =======================
};
