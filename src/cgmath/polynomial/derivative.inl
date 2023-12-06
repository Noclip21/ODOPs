#pragma once

#include "Polynomial.h"


template<typename T,uint N>
const Polynomial<T,N-1> derivative(const Polynomial<T,N>& p)
{
	Polynomial<T,N-1> pd;
	for(uint i=1; i<N; ++i)
		pd.coefs[i-1] = p.coefs[i]*i;
	return pd;
}
