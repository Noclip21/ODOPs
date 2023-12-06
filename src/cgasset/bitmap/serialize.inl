#pragma once

#include <iostream>

#include "Bitmap.h"

////////////////////////////////////////////////////////////////////////////////
inline std::istream& read(std::istream& is,Bitmap* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		Bitmap& bitmap = data[i];
		int format,width,height;
		is.read((char*)&format,sizeof(int));
		is.read((char*)&width,sizeof(int));
		is.read((char*)&height,sizeof(int));
		bitmap = Bitmap(width,height,format);
		is.read((char*)bitmap.data,bitmap.size);
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
inline std::ostream& write(std::ostream& os,const Bitmap* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const Bitmap& bitmap = data[i];
		os.write((const char*)&bitmap.format,sizeof(bitmap.format));
		os.write((const char*)&bitmap.width,sizeof(bitmap.width));
		os.write((const char*)&bitmap.height,sizeof(bitmap.height));
		os.write(bitmap.data,bitmap.size);
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////
