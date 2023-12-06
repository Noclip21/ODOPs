#pragma once

#include "dot.inl"


template<typename T1,typename T2>
inline auto reflect(const T1& i,const T2& n)
{ return i-2*dot(n,i)*n; }
