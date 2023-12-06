#pragma once

#include <iostream>

#include "AABB.h"
#include "KDOP.h"
#include "OBB.h"


////////////////////////////////////////////////////////////////////////////////
inline std::istream& read(std::istream& is,AABB* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		AABB& aabb = data[i];
		is.read((char*)&aabb.sah,sizeof(aabb.sah));
		is.read((char*)&aabb.pmin,sizeof(aabb.pmin));
		is.read((char*)&aabb.pmax,sizeof(aabb.pmax));
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
inline std::ostream& write(std::ostream& os,const AABB* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const AABB& aabb = data[i];
		os.write((const char*)&aabb.sah,sizeof(aabb.sah));
		os.write((const char*)&aabb.pmin,sizeof(aabb.pmin));
		os.write((const char*)&aabb.pmax,sizeof(aabb.pmax));
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
template<uint K>
inline std::istream& read(std::istream& is,KDOP<K>* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		KDOP<K>& kdop = data[i];
		is.read((char*)&kdop.sah,sizeof(kdop.sah));
		is.read((char*)&kdop.ko[0],sizeof(kdop.ko));
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
inline std::ostream& write(std::ostream& os,const KDOP<K>* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const KDOP<K>& kdop = data[i];
		os.write((const char*)&kdop.sah,sizeof(kdop.sah));
		os.write((const char*)&kdop.ko[0],sizeof(kdop.ko));
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
inline std::istream& read(std::istream& is,OBB* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		OBB& obb = data[i];
		is.read((char*)&obb.sah,sizeof(obb.sah));
		is.read((char*)&obb.m,sizeof(obb.m));
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
inline std::ostream& write(std::ostream& os,const OBB* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const OBB& obb = data[i];
		os.write((const char*)&obb.sah,sizeof(obb.sah));
		os.write((const char*)&obb.m,sizeof(obb.m));
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////
