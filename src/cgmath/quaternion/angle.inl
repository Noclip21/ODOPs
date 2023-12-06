#pragma once

#include "Quaternion.h"

template<typename T>
inline const T angle(const Quaternion<T>& q)
{
	return acos(q.w)*2;
}
