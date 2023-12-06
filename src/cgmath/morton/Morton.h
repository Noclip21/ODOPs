#pragma once

#include "../common/types.h"

template<typename T,uint N,class = uint_t<T>>
struct Morton
{
	T u;

	Morton():u(0){}
	Morton(const T& u):u(u){}
};
