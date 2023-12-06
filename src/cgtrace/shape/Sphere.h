#pragma once

#include "ShapeBase.h"


struct Sphere: ShapeBase,VolumeBase
{ 
	vec3 o; float r;

	Sphere() = default;
	Sphere(const Sphere&) = default;
	Sphere(const vec3&,const float);
	Sphere(const vec3*,const uint);
	Sphere(const Sphere&,const Sphere&);
	template<typename T>
	requires(std::is_base_of_v<ShapeBase,T>)
	Sphere(const T&);
	template<uint K>
	Sphere(const KDOP<K>&);

	inline const vec3 center() const;
	inline const float SAH() const;
};
