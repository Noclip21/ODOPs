#pragma once

#include <cstring>
#include <fstream>
#include <iostream>

#include "File.h"


////////////////////////////////////////////////////////////////////////////////
File::File():data(nullptr),size(0){}
////////////////////////////////////////////////////////////////////////////////
File::File(const File& file):File(file.data,file.size){}
////////////////////////////////////////////////////////////////////////////////
File::File(const int s):size(s)
{ data = new char[size]; }
////////////////////////////////////////////////////////////////////////////////
File::File(const void* d,const int s):File(s)
{ memcpy(data,d,size); }
////////////////////////////////////////////////////////////////////////////////
File::File(const char* filepath):File()
{
	std::ifstream handle(filepath,std::ifstream::in | std::ifstream::binary);
	if(verbose) std::cout << filepath << ": ";
	if(!handle.good())
	{
		if(verbose) std::cout << "Error opening file" << std::endl;
		return;
	}

	handle.seekg(0,handle.end);
	size = handle.tellg();
	handle.seekg(0,handle.beg);
	data = new char[size];
	handle.read(data,size);
	if(verbose) std::cout << "Reading successful" << std::endl;
	handle.close();
}
////////////////////////////////////////////////////////////////////////////////
