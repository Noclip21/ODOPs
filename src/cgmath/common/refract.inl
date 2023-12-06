#pragma once

#include "dot.inl"


template<typename T1,typename T2,typename T3>
inline auto refract(const T1& i,const T2& n,const T3 eta)
{
	const auto d = dot(n,i);
	const auto k = 1.0f-eta*eta*(1.0f-d*d);
	return (k<0)?0:eta*i-(eta*d+sqrt(k))*n;
}
