#pragma once

#include <cmath>

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/operators.inl"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename F,typename ... T,uint r,uint c>
inline const Mat<std::common_type_t<T...>,r,c> _foreach(F f,const Mat<T,r,c>& ... v)
{
	Mat<std::common_type_t<T...>,r,c> m;
	for(uint i=0; i<r*c; ++i)
		m[i] = f(v[i]...);
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename F,typename ... T>
requires(std::is_arithmetic_v<T> && ...)
inline std::common_type_t<T...> _foreach(F f,T ... v)
{ return f(v...); }
////////////////////////////////////////////////////////////////////////////////
template<typename F,typename ... T>
requires(!std::is_arithmetic_v<T> || ...)
inline auto _foreach(F f,const T& ... v)
{ return _foreach(f,decltype((mat_t<T>(v)+...))(v)...); }
////////////////////////////////////////////////////////////////////////////////


// Trigonometric
template<typename T> inline const T radians(const T& a) { return (M_PI/180.0f)*a; }
template<typename T> inline const T degrees(const T& a) { return (180.0f/M_PI)*a; }
template<typename T> inline auto sin(const T& a)   { return _foreach(::sinf,a); }
template<typename T> inline auto cos(const T& a)   { return _foreach(::cosf,a); }
template<typename T> inline auto tan(const T& a)   { return _foreach(::tanf,a); }
template<typename T> inline auto asin(const T& a)  { return _foreach(::asinf,a); }
template<typename T> inline auto acos(const T& a)  { return _foreach(::acosf,a); }
template<typename T> inline auto atan(const T& a)  { return _foreach(::atanf,a); }
template<typename T> inline auto sinh(const T& a)  { return _foreach(::sinhf,a); }
template<typename T> inline auto cosh(const T& a)  { return _foreach(::coshf,a); }
template<typename T> inline auto tanh(const T& a)  { return _foreach(::tanhf,a); }
template<typename T> inline auto asinh(const T& a) { return _foreach(::asinhf,a); }
template<typename T> inline auto acosh(const T& a) { return _foreach(::acoshf,a); }
template<typename T> inline auto atanh(const T& a) { return _foreach(::atanhf,a); }
template<typename T1,typename T2> inline auto atan2(const T1& a,const T2& b) { return _foreach(::atan2f,a,b); }

// Exponential
template<typename T1,typename T2> inline auto pow(const T1& a,const T2& b) { return _foreach(::powf,a,b); }
template<typename T> inline auto exp(const T& a)  { return _foreach(::expf,a); }
template<typename T> inline auto log(const T& a)  { return _foreach(::logf,a); }
template<typename T> inline auto exp2(const T& a) { return _foreach(::exp2f,a); }
template<typename T> inline auto log2(const T& a) { return _foreach(::log2f,a); }
template<typename T> inline auto sqrt(const T& a) { return _foreach(::sqrtf,a); }
template<typename T> inline auto inversesqrt(const T& a) { return 1.0f/sqrt(a); } // TODO


// Common
static inline const int _sign(const float a) { return ((a>0)-(a<0)); }
static inline const float _fract(const float a)
{ return (a-floor(a)); }
static inline const float _step(const float edge,const float x)
{ return (x>=edge); }
static inline const float _clamp(const float x,const float minVal,const float maxVal)
{ return ::fminl(::fmaxl(x,minVal),maxVal); }
static inline const float _mix(const float x,const float y,const float a)
{ return (x*(1.0f-a)+y*a); }
static inline const float _smoothstep(const float edge0,const float edge1,const float x)
{ float t = _clamp((x-edge0)/(edge1-edge0),0.0f,1.0f); return (t*t*(3.0f-2.0f*t)); }


template<typename T> inline auto abs(const T& a) { return _foreach(::fabsf,a); }
template<typename T> inline auto sign(const T& a)  { return _foreach(_sign,a); }
template<typename T> inline auto floor(const T& a) { return _foreach(::floorf,a); }
template<typename T> inline auto trunc(const T& a) { return _foreach(::truncf,a); }
template<typename T> inline auto round(const T& a) { return _foreach(::roundf,a); }
template<typename T> inline auto roundEven(const T& a) { return _foreach(::roundevenf,a); }
template<typename T> inline auto ceil(const T& a)  { return _foreach(::ceilf,a); }
template<typename T> inline auto fract(const T& a) { return _foreach(_fract,a); }
template<typename T1,typename T2> inline auto mod(const T1& a,const T2& b) { return _foreach(::fmod,a,b); }
template<typename T1,typename T2> inline auto min(const T1& a,const T2& b) { return _foreach(::fminl,a,b); }
template<typename T1,typename T2> inline auto max(const T1& a,const T2& b) { return _foreach(::fmaxl,a,b); }
template<typename T1,typename T2> inline auto step(const T1& a,const T2& b) { return _foreach(_step,a,b); }
template<typename T1,typename T2,typename T3> inline auto clamp(const T1& a,const T2& b,const T3& c) { return _foreach(_clamp,a,b,c); }
template<typename T1,typename T2,typename T3> inline auto mix(const T1& a,const T2& b,const T3& c) { return _foreach(_mix,a,b,c); }
template<typename T1,typename T2,typename T3> inline auto smoothstep(const T1& a,const T2& b,const T3& c) { return _foreach(_smoothstep,a,b,c); }
