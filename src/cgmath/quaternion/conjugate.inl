#pragma once

#include "Quaternion.h"
#include "constructors.inl"

template<typename T>
inline const Quaternion<T> conjugate(const Quaternion<T>& q)
{
	return Quaternion(-q.xyz,q.w);
}
