#pragma once

#include "ShapeBase.h"


struct OBB: ShapeBase,VolumeBase
{ 
	mat3x4 m;

	OBB();
	OBB(const OBB&) = default;
	OBB(const mat3x4&);
	OBB(const vec3*,const uint);
	OBB(const OBB&,const OBB&);
	template<typename T>
	requires(std::is_base_of_v<ShapeBase,T>)
	OBB(const T&);
	template<uint K>
	OBB(const KDOP<K>&);

	inline const vec3 center() const;
	inline const float SAH() const;
};
