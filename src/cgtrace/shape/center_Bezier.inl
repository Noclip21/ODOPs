#pragma once

#include "Bezier.h"

#include "constructors_AABB.inl"
#include "center_AABB.inl"


const vec3 Bezier::center() const { return AABB(*this).center(); }
