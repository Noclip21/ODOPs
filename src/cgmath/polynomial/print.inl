#pragma once

#include <iostream>

#include "../common/functions.inl"
#include "Polynomial.h"


template<typename T,uint N>
inline std::ostream& operator << (std::ostream& out,const Polynomial<T,N>& p)
{
	for(uint i=0; i<N; ++i)
	{
		std::cout << ((p.coefs[i] >= 0)?"+":"-") << abs(p.coefs[i]) << ((i > 0)?"x":"");
		if(i > 1) std::cout << "^" << i;
		if(i<N-1) std::cout << " ";
	}
	return out;
}
