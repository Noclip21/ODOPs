#pragma once

#include "Polynomial.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint N1,uint N2>
using polynomial_sum_t = Polynomial<std::common_type_t<T1,T2>,(N1>N2)?N1:N2>;
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint N1,uint N2>
using polynomial_multi_t = Polynomial<std::common_type_t<T1,T2>,N1+N2-1>;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
inline const Polynomial<T,N> operator - (const Polynomial<T,N>& p)
{
	Polynomial<T,N> pn;
	for(uint i=0; i<p.nCoefs; ++i)
		pn.coefs[i] = -p.coefs[i];
	return pn;
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint N1,uint N2>
inline const polynomial_sum_t<T1,T2,N1,N2>
operator + (const Polynomial<T1,N1>& p1,const Polynomial<T2,N2>& p2)
{
	polynomial_sum_t<T1,T2,N1,N2> p;
	for(uint i=0; i<p.nCoefs; ++i)
		p.coefs[i] = ((i<p1.nCoefs)?p1.coefs[i]:0) + ((i<p2.nCoefs)?p2.coefs[i]:0);
	return p;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint N1,uint N2>
inline const polynomial_sum_t<T1,T2,N1,N2>
operator - (const Polynomial<T1,N1>& p1,const Polynomial<T2,N2>& p2)
{
	polynomial_sum_t<T1,T2,N1,N2> p;
	for(uint i=0; i<p.nCoefs; ++i)
		p.coefs[i] = ((i<p1.nCoefs)?p1.coefs[i]:0) - ((i<p2.nCoefs)?p2.coefs[i]:0);
	return p;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint N1,uint N2>
inline const polynomial_multi_t<T1,T2,N1,N2>
operator * (const Polynomial<T1,N1>& p1,const Polynomial<T2,N2>& p2)
{
	polynomial_multi_t<T1,T2,N1,N2> p;
	for(uint i=0; i<p.nCoefs; ++i)
		p.coefs[i] = 0;

	for(uint i=0; i<p1.nCoefs; ++i)
	for(uint j=0; j<p2.nCoefs; ++j)
		p.coefs[i+j] += p1.coefs[i]*p2.coefs[j];

	return p;
}
////////////////////////////////////////////////////////////////////////////////
