#pragma once

#include "Camera.h"


Camera::Camera(
	const float w,const float h,const float fov,
	const vec3& o,const vec3& d,const vec3& up):
	width(w),height(h),fov(fov),o(o),d(d),up(up)
{
	const float a = tan(fov*0.5f);
	const mat3 V = lookRotation(-d,up);
	const mat3 P = transpose(
		mat3(2*a/w,      0,    -a,
		     0,     -2*a/w, a*h/w,
		     0,          0,    -1));
	//const mat3 P = transpose(
	//	mat3(2*a/h,      0, -a*w/h,
	//	     0,     -2*a/h,      a,
	//	     0,          0,     -1));

	_mvp = V*P;
}
