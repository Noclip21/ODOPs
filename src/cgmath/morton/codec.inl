#pragma once

#include "Morton.h"

#include "../mat/Mat.h"
#include "../mat/operators.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N,class = uint_t<T>>
const T expandBits(const T v)
{
	T u = 0;
	constexpr uint n = (sizeof(T) << 3)/N;
	for(uint i=0; i<n; ++i)
		u |= (v >> i & 0b1) << i*N;
	return u;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N,class = uint_t<T>>
const T packBits(const T v)
{
	T u = 0;
	constexpr uint n = (sizeof(T) << 3)/N;
	for(uint i=0; i<n; ++i)
		u |=  (v >> i*N & 0b1) << i;
	return u;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
const Morton<T,N> encode(Mat<T,N,1> v)
{
	Morton<T,N> morton;
	for(uint i=0; i<N; ++i)
		morton.u |= expandBits<T,N>(v[i]) << i;
	return morton;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
const Mat<T,N,1> decode(const Morton<T,N>& morton)
{
	Mat<T,N,1> v;
	for(uint i=0; i<N; ++i)
		v[i] = packBits<T,N>(morton.u >> i);
	return v;
}
////////////////////////////////////////////////////////////////////////////////
