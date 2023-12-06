#pragma once

#include "../common/types.h"

template<typename T,uint r,uint c>
struct MatBase
{
	static constexpr uint rows = r;
	static constexpr uint cols = c;
	static constexpr uint n = r*c;
	using Type = T;
};
