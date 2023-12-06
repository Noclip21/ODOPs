#pragma once

#include <vector>
#include <string>


namespace codec
{
	namespace obj
	{
		struct Tokenizer 
		{
			std::vector<std::vector<std::string>> tokens;

			bool good;

			Tokenizer(const char*);
		};
	};
};
