#pragma once

#include <cgmath>


enum Wrap
{
	CLAMP_EDGE,
	CLAMP_BORDER,
	REPEAT,
	REPEAT_MIRRORED,
};

template<typename T> struct Texture;
template<typename T> struct TextureLod;

using Tex_r8    = Texture<Pixel<uint8,1>>;
using Tex_rg8   = Texture<Pixel<uint8,2>>;
using Tex_rgb8  = Texture<Pixel<uint8,3>>;
using Tex_rgba8 = Texture<Pixel<uint8,4>>;

using Tex_r32f    = Texture<Pixel<float,1>>;
using Tex_rg32f   = Texture<Pixel<float,2>>;
using Tex_rgb32f  = Texture<Pixel<float,3>>;
using Tex_rgba32f = Texture<Pixel<float,4>>;

using TexLod_r8    = TextureLod<Pixel<uint8,1>>;
using TexLod_rg8   = TextureLod<Pixel<uint8,2>>;
using TexLod_rgb8  = TextureLod<Pixel<uint8,3>>;
using TexLod_rgba8 = TextureLod<Pixel<uint8,4>>;

using TexLod_r32f    = TextureLod<Pixel<float,1>>;
using TexLod_rg32f   = TextureLod<Pixel<float,2>>;
using TexLod_rgb32f  = TextureLod<Pixel<float,3>>;
using TexLod_rgba32f = TextureLod<Pixel<float,4>>;
