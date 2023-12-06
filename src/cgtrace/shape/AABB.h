#pragma once

#include "ShapeBase.h"


struct AABB: ShapeBase,VolumeBase
{ 
	vec3 pmin,pmax;

	AABB();
	AABB(const AABB&) = default;
	AABB(const vec3&,const vec3&);
	AABB(const vec3*,const uint);
	AABB(const AABB&,const AABB&);
	template<typename T>
	requires(std::is_base_of_v<ShapeBase,T>)
	AABB(const T&);
	template<uint N>
	AABB(const Bezier&,const Polynomial<float,N>&);
	template<uint K>
	AABB(const KDOP<K>&);

	inline const vec3 center() const;
	inline const float SAH() const;
};
