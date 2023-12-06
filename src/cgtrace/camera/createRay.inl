#pragma once

#include "Camera.h"

#include "../common/Ray.h"


template<typename RAY>
requires(std::is_base_of_v<RayDataAttributeBase,RAY>)
const RAY Camera::createRay(const int i,const int j) const
{
	RAY ray;
	ray.o = o;
	ray.d = normalize(_mvp*vec3(float(i+0.5),float(j+0.5),1));

	if constexpr (std::is_base_of_v<RayDataCone,RAY>)
	{
		// TODO
	}
	if constexpr (std::is_base_of_v<RayDataDifferential,RAY>)
	{
		// TODO
		//ray.odx = o;
		//ray.ody = o;
		//ray.ddx = normalize(_mvp*vec3(i+1,j,1));
		//ray.ddy = normalize(_mvp*vec3(i,j+1,1));
	}

	return ray;
}
