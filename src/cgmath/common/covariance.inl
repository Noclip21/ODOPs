#pragma once

#include <tuple>

#include "../mat/Mat.h"
#include "../mat/constructors.inl"
#include "../mat/operators.inl"
#include "transpose.inl"
#include "norm.inl"
#include "transform.inl"


template<typename T,uint N>
inline const std::tuple<Mat<T,N,1>,Mat<T,N,N>>
covariance(const Mat<T,N,1>* vs,const uint n)
{
	Mat<T,N,1> mean(0);
	for(uint i=0; i<n; ++i)
		mean += vs[i];
	mean /= n;

	Mat<T,N,N> cov(0);
	for(uint i=0; i<n; ++i)
	{
		const Mat<T,N,1> v = vs[i]-mean;
		cov += v*transpose(v);
	}
	cov /= n-1;

	return {mean,cov};
}
