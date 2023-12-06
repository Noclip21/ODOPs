#pragma once

#include <iostream>

#include "TextureLod.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline std::istream& read(std::istream& is,TextureLod<T>* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		TextureLod<T>& t = data[i];
		int width,height;
		T border;
		is.read((char*)&width,sizeof(t.width));
		is.read((char*)&height,sizeof(t.height));
		is.read((char*)&t.border,sizeof(t.border));
		t = TextureLod<T>(width,height,border);
		const int size = t.levelOffsets[t.levels-1]+1;
		is.read((char*)t.data,size*sizeof(decltype(t.data[0])));
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline std::ostream& write(std::ostream& os,const TextureLod<T>* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const TextureLod<T>& t = data[i];
		const int size = t.levelOffsets[t.levels-1]+1;
		os.write((const char*)&t.width,sizeof(t.width));
		os.write((const char*)&t.height,sizeof(t.height));
		os.write((const char*)&t.border,sizeof(t.border));
		os.write((const char*)t.data,size*sizeof(decltype(t.data[0])));
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////
