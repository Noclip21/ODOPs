#pragma once

#include <fstream>
#include <istream>
#include <sstream>

#include "ConfigParser.h"

#include "../file/File.h"
#include "../file/constructors.inl"


////////////////////////////////////////////////////////////////////////////////
ConfigParser::ConfigParser(){}
////////////////////////////////////////////////////////////////////////////////
ConfigParser::ConfigParser(const char* filepath)
{
	File file(filepath);
	*this = ConfigParser(file.data,file.size);
}
////////////////////////////////////////////////////////////////////////////////
ConfigParser::ConfigParser(const void* data,const int size)
{
	std::istringstream buffer;
	buffer.rdbuf()->pubsetbuf((char*)data,size);
	buffer.seekg(0,buffer.beg);
	std::string line,key,value;
	while(std::getline(buffer,line))
	{
		std::istringstream iss(line);
		iss >> key;
		if(key[0] == '#') continue;
		while(iss >> value && value[0] != '#')
		{
			_vars[key].push_back(value);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
