#pragma once

#include "ConfigParser.h"


////////////////////////////////////////////////////////////////////////////////
inline ConfigParser::operator bool() const
{ return (_vars.size() > 0); }
////////////////////////////////////////////////////////////////////////////////
inline const std::vector<std::string>& ConfigParser::operator [] (const char* key)
{ return _vars[key]; }
////////////////////////////////////////////////////////////////////////////////
template<>
inline const int ConfigParser::get<int>(const char* key,const int defV,const int i)
{
	const std::vector<std::string>& v = _vars[key];
	return (v.size()>size_t(i))?atoi(v[i].c_str()):defV;
}
////////////////////////////////////////////////////////////////////////////////
template<>
inline const float ConfigParser::get<float>(const char* key,const float defV,const int i)
{
	const std::vector<std::string>& v = _vars[key];
	return (v.size()>size_t(i))?atof(v[i].c_str()):defV;
}
////////////////////////////////////////////////////////////////////////////////
template<>
inline const std::string ConfigParser::get<std::string>(const char* key,const std::string defV,const int i)
{
	const std::vector<std::string>& v = _vars[key];
	return (v.size()>size_t(i))?v[i]:defV;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void ConfigParser::set(const char* key,const T value,const int i)
{
	set<std::string>(key,std::to_string(value),i);
}
////////////////////////////////////////////////////////////////////////////////
template<>
inline void ConfigParser::set(const char* key,const std::string value,const int i)
{
	auto& va = _vars[key];
	if(int(va.size())-1 < i) va.resize(i);
	va[i] = value;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,int N>
inline const T* ConfigParser::getArray(const char* key,const T* defV,const int i)
{
	static T data[N];
	for(uint j=0; j<N; ++j)
		data[j] = get<T>(key,(defV)?defV[j]:T(),i+j);
	return data;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,int N>
inline void ConfigParser::setArray(const char* key,const T* value,const int i)
{
	for(int j=0; j<N; ++j)
		set<T>(key,value[j],i+j);
}
////////////////////////////////////////////////////////////////////////////////
