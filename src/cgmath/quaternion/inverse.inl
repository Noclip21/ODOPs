#pragma once

#include "Quaternion.h"
#include "conjugate.inl"

template<typename T>
inline const Quaternion<T> inverse(const Quaternion<T>& q)
{
	return conjugate(q)/dot(q,q);
}
