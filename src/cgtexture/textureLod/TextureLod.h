#pragma once

#include <tuple>

#include <cgasset>

#include "../common/types.h"


template<typename T>
struct TextureLod
{
	static float maxAniso;

	using pixel = T;

	union{
		struct { uint width,height,levels; };
		struct { uint w,h,l; };
	};
	pixel  border;
	uint*  levelOffsets;
	pixel* data;
	uint   size;


	inline TextureLod();
	inline TextureLod(const uint,const uint,const pixel& = pixel());
	inline TextureLod(const void*,const uint,const uint,const pixel& = pixel());
	inline TextureLod(const Texture<T>&);
	inline TextureLod(const TextureLod&);
	inline TextureLod(const Bitmap& bitmap,const pixel& = pixel());
	~TextureLod() { delete[] levelOffsets; delete[] data; }

	
	inline TextureLod& operator = (const TextureLod&);


	// Get bitmap dimensions at level k
	inline const std::tuple<int,int> dimensions(const int) const;
	inline const int dimensionW(const int) const;

	// Get bitmap texture at level k
	inline const Texture<T> texture(const uint) const;


	// No Bounds checking ====
	
	// Get pointer to bitmap at mip level k
	inline pixel* operator () (const uint);
	inline const pixel* operator () (const uint) const;
	
	// Get/set pixel at mip level k
	inline pixel& operator () (const uint,const uint,const uint);
	inline const pixel& operator () (const uint,const uint,const uint) const;

	// =======================
};
