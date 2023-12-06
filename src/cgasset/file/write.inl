#pragma once

#include <iostream>
#include <fstream>

#include "File.h"

////////////////////////////////////////////////////////////////////////////////
bool File::write(const char* filepath,const void* data,const int size)
{
	std::ofstream handle(filepath,std::ifstream::out | std::ifstream::binary);
	if(verbose) std::cout << filepath << ": ";
	if(!handle.good())
	{
		if(verbose) std::cout << "Error opening file" << std::endl;
		return false;
	}

	handle.write((const char*)data,size);
	handle.close();
	if(verbose) std::cout << "writing sucessful" << std::endl;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
bool File::write(const char* filepath)
{ return write(filepath,data,size); }
////////////////////////////////////////////////////////////////////////////////
