#pragma once

#include "../mat/Mat.h"
#include "../mat/constructors.inl"
#include "../mat/operators.inl"
#include "norm.inl"
#include "reflect.inl"
#include "functions.inl"


template<typename T,uint N>
inline const Mat<T,N,N> eigs(Mat<T,N,N> m,int i = 300,T tol = T(5e-8))
{
	// Tridiagonal matrix
	Mat<T,N,N> u;
	Mat<T,N,1> v;
	for(uint i=1; i<N-1; ++i)
	{
		v = 0;
		memcpy(&v[i],&m(i,i-1),sizeof(T)*(N-i));

		T n;
		n     = norm(v);
		if(abs(v[i]-n) < 5e-5) v *= -1;
		v[i] -= n;
		n     = norm(v);
		
		if(n == 0) continue;

		v /= n;
		u  = reflect(Mat<T,N,N>(),v);
		m  = u*m*u;
	}

	// QR iterative matrix diagonalization
	Mat<T,N,N> q;
	float rtol = 0.0,atol = 1.0;
	while(i-- > 0 && atol > tol)
	{
		auto [q,r] = qr(m);
		u = u*q;
		m = transposeMulti(q,m*q);

		// Gershgorin circle approximation
		T diagonal = 0,length = 0;
		for(uint j=0; j<N; ++j)
		{
			diagonal += m(j,j)*m(j,j);
		    for(uint k=0; k<N; ++k)
				length += m(k,j)*m(k,j);
		}

		atol = rtol;
		rtol = abs(length-diagonal);
		atol = abs(atol-rtol);
	}

	return u;
}
