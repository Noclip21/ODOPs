#pragma once

#include <tuple>
#include <cstring>

#include "../mat/Mat.h"
#include "../mat/constructors.inl"

#include "norm.inl"
#include "functions.inl"
#include "reflect.inl"


////////////////////////////////////////////////////////////////////////////////
// QR decomposition using Householder reflections
template <typename T,uint rows,uint cols>
requires(rows <= cols)
inline const std::tuple<Mat<T,rows,rows>,Mat<T,rows,cols>>
qr(const Mat<T,rows,cols>& m)
{
	Mat<T,rows,1>    v;
	Mat<T,rows,rows> h;
	Mat<T,rows,rows> q;
	Mat<T,rows,cols> r = m;

	for(uint i=0; i<rows-1; ++i)
	{
		v = 0;
		memcpy(&v[i],&r(i,i),sizeof(T)*(rows-i));

		T n;
		n     = norm(v);
		if(abs(v[i]-n) < 5e-5) v *= -1;
		v[i] -= n;
		n     = norm(v);
		
		if(n == 0) continue;

		v /= n;
		h  = reflect(Mat<T,rows,rows>(),v);
		q  = q*h;
		r  = h*r;
	}

	return {q,r};
}
////////////////////////////////////////////////////////////////////////////////
template <typename T,uint rows,uint cols>
requires(rows > cols)
inline const std::tuple<Mat<T,rows,cols>,Mat<T,cols,cols>>
qr(const Mat<T,rows,cols>& m)
{
	Mat<T,rows,1>    v;
	Mat<T,rows,cols> r2 = m;
	Mat<T,rows,rows> q2;
	Mat<T,rows,cols> q;
	Mat<T,cols,cols> r;

	for(uint i=0; i<cols; ++i)
	{
		v = 0;
		memcpy(&v[i],&r2(i,i),sizeof(T)*(rows-i));

		T n = 0;
		n     = norm(v);
		if(abs(v[i]-n) < 5e-5) v *= -1;
		v[i] -= n;
		n     = norm(v);
		
		if(n == 0) continue;

		v /= n;

		// q2 = q2*h; (transpose)
		for(uint j=0; j<rows; ++j)
			q2(j) -= 2*v*dot(v,q2(j));

		// h*r2
		for(uint j=i; j<cols; ++j)
			r2(j) -= 2*v*dot(v,r2(j));
	}

	q = Mat<T,rows,cols>(transpose(q2));
	r = Mat<T,cols,cols>(r2);

	return {q,r};
}
////////////////////////////////////////////////////////////////////////////////
