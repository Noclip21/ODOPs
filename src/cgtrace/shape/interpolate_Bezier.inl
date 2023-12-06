#pragma once

#include "Bezier.h"
#include "shape/ShapeBase.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T>
const vec3 Bezier::interpolate(const T& t) const
{
	const T tt = t*t,ttt = tt*t,s = (T(1)-t),ss = s*s,sss = ss*s;
	return sss*a + 3*ss*t*b + 3*s*tt*c + ttt*d;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
const vec3 Bezier::interpolatedt(const T& t) const
{
	const T t3 = t*3,tt3 = t3*t,s = (T(1)-t),ss3 = s*s*3,st6 = s*t*6;
	return -ss3*a + (ss3-st6)*b - (tt3-st6)*c + tt3*d;
}
////////////////////////////////////////////////////////////////////////////////
