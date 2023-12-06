#pragma once

#include <sstream>

#include "../../file/constructors.inl"
#include "../../file/operators.inl"

#include "Tokenizer.h"


codec::obj::Tokenizer::Tokenizer(const char* filepath)
{
	File file(filepath);
	good = (bool)file;

	std::stringstream stream;
	stream.rdbuf()->pubsetbuf(file.data,file.size);

	std::string line,word;
	while(std::getline(stream,line))
	{
		std::stringstream ss(line);
		std::vector<std::string> words;
		while(ss >> word)
			words.push_back(word);
		if(words.size() > 1) tokens.push_back(words);
	}
}
