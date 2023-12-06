#pragma once

#include "../mat/constructors.inl"

namespace cg
{
	inline static const vec3 left    (-1, 0, 0);
	inline static const vec3 right   (+1, 0, 0);
	inline static const vec3 down    ( 0,-1, 0);
	inline static const vec3 up      ( 0,+1, 0);
	inline static const vec3 backward( 0, 0,+1);
	inline static const vec3 forward ( 0, 0,-1);
};
