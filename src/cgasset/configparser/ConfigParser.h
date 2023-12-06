#pragma once

#include <iostream> 

#include <unordered_map>
#include <string>
#include <vector>


class ConfigParser
{
private:

	std::unordered_map<std::string,std::vector<std::string>> _vars;

public:

	inline ConfigParser();
	inline ConfigParser(const char*);
	inline ConfigParser(const void*,const int);


	inline operator bool() const;


	inline const std::vector<std::string>& operator [] (const char*);

	template<typename T>
	inline const T get(const char*,const T = T(),const int = 0);

	template<typename T>
	inline void set(const char*,const T,const int = 0);

	template<typename T,int N>
	inline const T* getArray(const char*,const T* = nullptr,const int = 0);

	template<typename T,int N>
	inline void setArray(const char*,const T*,const int = 0);


	inline bool write(const char*);
};
