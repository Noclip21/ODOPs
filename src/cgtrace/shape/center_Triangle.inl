#pragma once

#include "Triangle.h"


const vec3 Triangle::center() const { return (p1+p2+p3)/3; }
