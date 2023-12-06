#pragma once

#include <ostream>

#include "Swizzle.h"

#include "../mat/Mat.h"
#include "../mat/print.inl"


template<typename T,uint r,uint mask>
inline std::ostream& operator << (std::ostream& out,const Swizzle<T,r,mask>& s)
{ return out << (const Mat<T,r,1>)s; }
