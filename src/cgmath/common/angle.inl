#pragma once

#include <cmath>

#include "norm.inl"
#include "cross.inl"

////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline auto spherical(const T& cartesian) -> decltype(cross(cartesian,cartesian))
{
	using V3 = decltype(cross(cartesian,cartesian));
	typename V3::Type r = length(cartesian);
	typename V3::Type y = acos(cartesian.y/r);
	typename V3::Type z = atan2(cartesian.x,cartesian.z);
	return V3(r,y,z);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline auto cartesian(const T& spherical) -> decltype(cross(spherical,spherical))
{
	using V3 = decltype(cross(spherical,spherical));
	return V3(
		spherical.x*sin(spherical.y)*sin(spherical.z),
		spherical.x*cos(spherical.y),
		spherical.x*sin(spherical.y)*cos(spherical.z));
}
////////////////////////////////////////////////////////////////////////////////
