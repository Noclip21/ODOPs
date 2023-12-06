#pragma once

#include <cstdint>
#include <cmath>


template<typename T,uint channels> struct Pixel;

using pixel   = Pixel<char,0>;

using r8      = Pixel<uint8_t,1>;
using rg8     = Pixel<uint8_t,2>;
using rgb8    = Pixel<uint8_t,3>;
using rgba8   = Pixel<uint8_t,4>;

using r32f    = Pixel<float,1>;
using rg32f   = Pixel<float,2>;
using rgb32f  = Pixel<float,3>;
using rgba32f = Pixel<float,4>;
