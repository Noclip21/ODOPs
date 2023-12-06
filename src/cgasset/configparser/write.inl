#pragma once

#include "ConfigParser.h"

#include "../file/File.h"
#include "../file/constructors.inl"
#include "../file/write.inl"


bool ConfigParser::write(const char* filepath)
{
	std::vector<char> data;
	for(auto [k,va]: _vars)
	{
		if(data.size() > 0) data.push_back('\n');
		data.insert(data.end(),k.begin(),k.end());
		for(auto v: va)
		{
			data.push_back(' ');
			data.insert(data.end(),v.begin(),v.end());
		}
	}
	return File(&data[0],data.size()).write(filepath);
}
