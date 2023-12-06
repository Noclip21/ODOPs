#pragma once

#include "Polynomial.h"


template<typename T,uint N>
const T polyeval(const T& x,const Polynomial<T,N>& p)
{
	T xx = 1;
	T y  = p.coefs[0];;
	for(uint i=1; i<N; ++i)
		y += p.coefs[i]*(xx = xx*x);
	return y;
}
