#pragma once

#include "Morton.h"

#define MORTON_OP(OP)\
template<typename T,uint N>\
inline const bool operator OP (const Morton<T,N>& a,const Morton<T,N>& b)\
{ return a.u OP b.u; }

MORTON_OP(==)
MORTON_OP(!=)
MORTON_OP(>)
MORTON_OP(<)
MORTON_OP(>=)
MORTON_OP(<=)

#undef MORTON_OP
