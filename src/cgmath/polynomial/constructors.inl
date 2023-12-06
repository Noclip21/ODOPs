#pragma once

#include "Polynomial.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
Polynomial<T,N>::Polynomial()
{
	for(uint i=0; i<nCoefs; ++i) coefs[i] = 0;
	for(uint i=0; i<nRoots; ++i) roots[i] = 0;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
template<typename T2>
Polynomial<T,N>::Polynomial(const Polynomial<T2,N>& p)
{
	for(uint i=0; i<nCoefs; ++i) coefs[i] = T(p.coefs[i]);
	for(uint i=0; i<nRoots; ++i) roots[i] = T(p.roots[i]);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
template<typename ...Args>
Polynomial<T,N>::Polynomial(const Args&... args):Polynomial()
{
	static_assert(sizeof...(Args) == nCoefs,"size mismatch");
	uint i = 0;
	((coefs[i++] = T(args)),...);
}
////////////////////////////////////////////////////////////////////////////////
