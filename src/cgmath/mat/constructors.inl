#pragma once

#include "Mat.h"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>::Mat(const Mat<T,r,c>& m)
{
	for(uint i=0; i<n; ++i)
		data[i] = m[i];
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>::Mat()
{
	for(uint i=0; i<n; ++i)
		data[i] = 0;
	if constexpr (rows == cols)
		for(uint i=0; i<rows; ++i)
			data[i+i*rows] = 1;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
Mat<T,r,c>::Mat(const T* d)
{
	for(uint i=0; i<n; ++i)
		data[i] = d[i];
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<typename T2>
Mat<T,r,c>::Mat(const T2 a)
{
	for(uint i=0; i<n; ++i)
		if constexpr (rows == cols)
			data[i] = 0;
		else
			data[i] = a;
	if constexpr (rows == cols)
		for(uint i=0; i<rows; ++i)
			data[i+i*rows] = a;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<typename T2>
Mat<T,r,c>::Mat(const Mat<T2,r,c>& m)
{
	for(uint i=0; i<n; ++i)
		data[i] = T(m.data[i]);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<uint mask>
Mat<T,r,c>::Mat(const Swizzle<T,r,mask>& s)
{
	for(uint i=0; i<n; ++i)
		data[i] = s[i];
};
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<typename T2,uint r2,uint c2>
requires(r2 < r || c2 < c)
Mat<T,r,c>::Mat(const Mat<T2,r2,c2>& m):Mat()
{
	for(uint i=0; i<c2; ++i)
	{
		T* v = data+i*r;
		const T2* mv = m.data+i*r2;
		for(uint j=0; j<r2; ++j)
			v[j] = mv[j];
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<typename T2,uint r2,uint c2>
requires(r2 > r || c2 > c)
Mat<T,r,c>::Mat(const Mat<T2,r2,c2>& m)
{
	for(uint i=0; i<c; ++i)
	{
		T* v = data+i*r;
		const T2* mv = m.data+i*r2;
		for(uint j=0; j<r; ++j)
			v[j] = mv[j];
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<typename ... T2>
requires(sizeof...(T2) == c)
Mat<T,r,c>::Mat(const Mat<T2,r,1>& ... cols)
{
	uint i = 0;
	([&](auto& m)
	{
		(*this)(i++) = m;
	}(cols),...);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
static constexpr auto to_mat(Mat<T,r,c>*) -> Mat<T,r,c>;
template<typename T,uint r,uint mask>
static constexpr auto to_mat(Swizzle<T,r,mask>*) -> Mat<T,r,1>;
template<typename T>
static constexpr auto to_mat(T*)-> Mat<T,1,1>;
template<typename T>
using mat_t = decltype(to_mat((T*)nullptr));
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
template<typename ... Args>
Mat<T,r,c>::Mat(const Args& ... args)
{
	const uint size = (mat_t<Args>::n + ...);
	static_assert(size == n,"size mismatch");

	if constexpr (sizeof...(Args) == r*c)
	{
		uint i = 0;
		((data[i++] = T(args)),...);
	}else{
		uint i = 0;
		([&](const mat_t<Args>& m)
		{
			for(uint k=0; k<mat_t<Args>::n; ++k)
				data[i++] = m[k];
		}(mat_t<Args>(args)),...);
	}
}
////////////////////////////////////////////////////////////////////////////////
