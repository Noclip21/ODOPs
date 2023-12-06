#pragma once

#include <cstdint>
#include <type_traits>

using uint8  = uint8_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using uint   = uint32_t;

template<typename T>
using uint_t = typename std::enable_if_t<std::is_unsigned_v<T> && std::is_integral_v<T>>;

template<typename,uint,uint> struct MatBase;
template<typename,uint,uint> struct MatData;
template<typename,uint,uint> struct Mat;
template<typename,uint,uint> struct Swizzle;
template<typename,uint,class> struct Morton;
template<typename> struct Quaternion;

template<typename T,uint rows>
using Vec = Mat<T,rows,1>;

using vec2  = Mat<float,2,1>;
using vec3  = Mat<float,3,1>;
using vec4  = Mat<float,4,1>;
using dvec2 = Mat<double,2,1>;
using dvec3 = Mat<double,3,1>;
using dvec4 = Mat<double,4,1>;
using ivec2 = Mat<int,2,1>;
using ivec3 = Mat<int,3,1>;
using ivec4 = Mat<int,4,1>;
using uvec2 = Mat<uint,2,1>;
using uvec3 = Mat<uint,3,1>;
using uvec4 = Mat<uint,4,1>;
using cvec2 = Mat<uint8,2,1>;
using cvec3 = Mat<uint8,3,1>;
using cvec4 = Mat<uint8,4,1>;
using bvec2 = Mat<bool,2,1>;
using bvec3 = Mat<bool,3,1>;
using bvec4 = Mat<bool,4,1>;

using mat2    = Mat<float,2,2>;
using mat3    = Mat<float,3,3>;
using mat4    = Mat<float,4,4>;
using mat2x3  = Mat<float,2,3>;
using mat2x4  = Mat<float,2,4>;
using mat3x2  = Mat<float,3,2>;
using mat3x4  = Mat<float,3,4>;
using mat4x2  = Mat<float,4,2>;
using mat4x3  = Mat<float,4,3>;
using dmat2   = Mat<double,2,2>;
using dmat3   = Mat<double,3,3>;
using dmat4   = Mat<double,4,4>;
using dmat2x3 = Mat<double,2,3>;
using dmat2x4 = Mat<double,2,4>;
using dmat3x2 = Mat<double,3,2>;
using dmat3x4 = Mat<double,3,4>;
using dmat4x2 = Mat<double,4,2>;
using dmat4x3 = Mat<double,4,3>;
using imat2   = Mat<int,2,2>;
using imat3   = Mat<int,3,3>;
using imat4   = Mat<int,4,4>;
using imat2x3 = Mat<int,2,3>;
using imat2x4 = Mat<int,2,4>;
using imat3x2 = Mat<int,3,2>;
using imat3x4 = Mat<int,3,4>;
using imat4x2 = Mat<int,4,2>;
using imat4x3 = Mat<int,4,3>;
using umat2   = Mat<uint,2,2>;
using umat3   = Mat<uint,3,3>;
using umat4   = Mat<uint,4,4>;
using umat2x3 = Mat<uint,2,3>;
using umat2x4 = Mat<uint,2,4>;
using umat3x2 = Mat<uint,3,2>;
using umat3x4 = Mat<uint,3,4>;
using umat4x2 = Mat<uint,4,2>;
using umat4x3 = Mat<uint,4,3>;
using cmat2   = Mat<uint8,2,2>;
using cmat3   = Mat<uint8,3,3>;
using cmat4   = Mat<uint8,4,4>;
using cmat2x3 = Mat<uint8,2,3>;
using cmat2x4 = Mat<uint8,2,4>;
using cmat3x2 = Mat<uint8,3,2>;
using cmat3x4 = Mat<uint8,3,4>;
using cmat4x2 = Mat<uint8,4,2>;
using cmat4x3 = Mat<uint8,4,3>;
using bmat2   = Mat<bool,2,2>;
using bmat3   = Mat<bool,3,3>;
using bmat4   = Mat<bool,4,4>;
using bmat2x3 = Mat<bool,2,3>;
using bmat2x4 = Mat<bool,2,4>;
using bmat3x2 = Mat<bool,3,2>;
using bmat3x4 = Mat<bool,3,4>;
using bmat4x2 = Mat<bool,4,2>;
using bmat4x3 = Mat<bool,4,3>;

using morton2    = Morton<uint,2,uint_t<uint>>;
using morton3    = Morton<uint,3,uint_t<uint>>;
using morton4    = Morton<uint,4,uint_t<uint>>;
using morton2u8  = Morton<uint8,2,uint_t<uint8>>;
using morton3u8  = Morton<uint8,3,uint_t<uint8>>;
using morton4u8  = Morton<uint8,4,uint_t<uint8>>;
using morton2u32 = Morton<uint32,2,uint_t<uint32>>;
using morton3u32 = Morton<uint32,3,uint_t<uint32>>;
using morton4u32 = Morton<uint32,4,uint_t<uint32>>;
using morton2u64 = Morton<uint64,2,uint_t<uint64>>;
using morton3u64 = Morton<uint64,3,uint_t<uint64>>;
using morton4u64 = Morton<uint64,4,uint_t<uint64>>;

using quaternion  = Quaternion<float>;
using quaterniond = Quaternion<double>;

template<typename T,uint C>
using Pixel = Vec<T,C>;

using r32f    = Pixel<float,1>;
using rg32f   = Pixel<float,2>;
using rgb32f  = Pixel<float,3>;
using rgba32f = Pixel<float,4>;
using r8      = Pixel<uint8,1>;
using rg8     = Pixel<uint8,2>;
using rgb8    = Pixel<uint8,3>;
using rgba8   = Pixel<uint8,4>;
