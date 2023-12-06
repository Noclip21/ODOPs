#pragma once

#include <iostream>

#include "AABB.h"
#include "OBB.h"
#include "KDOP.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Bezier.h"


////////////////////////////////////////////////////////////////////////////////
template<typename V>
requires(std::is_base_of_v<ShapeBase,V>)
inline std::istream& read(std::istream& is,V* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		V& volume = data[i];
		is.read((char*)&volume,sizeof(V));
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
template<typename V>
requires(std::is_base_of_v<ShapeBase,V>)
inline std::ostream& write(std::ostream& os,const V* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const V& volume = data[i];
		os.write((const char*)&volume,sizeof(V));
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////
