#pragma once

#include <cgmath>

#include "common/Ray.h"


struct Camera
{
private:

	mat3 _mvp;

public:

	int width,height;
	float fov;
	vec3 o,d,up;

	Camera(const float,const float,const float,const vec3&,const vec3&,const vec3& = cg::up);

	template<typename RAY>
	requires(std::is_base_of_v<RayDataAttributeBase,RAY>)
	const RAY createRay(const int,const int) const;
};
