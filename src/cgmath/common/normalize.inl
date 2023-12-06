#pragma once

#include "norm.inl"

template<typename T>
inline auto normalize(const T& v){ return v/norm(v); }
