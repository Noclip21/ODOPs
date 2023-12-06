#pragma once

#include <unordered_map>
#include <cstdlib>

#include "Parser.h"
#include "Tokenizer_constructor.inl"


codec::obj::Parser::Parser(const codec::obj::Tokenizer& tokenizer,const int lineId)
{
	const std::unordered_map<std::string,KEY> kvmap =
	{
		{"v",       KEY_FLOAT3_POSITION},
		{"vn",      KEY_FLOAT3_NORMAL},
		{"vt",      KEY_FLOAT3_TEXCOORD},
		{"f",       KEY_INT_FACE},
		{"l",       KEY_INT_LINE},
		{"o",       KEY_CSTR_MESH},
		{"mtllib",  KEY_CSTR_MAT_FILE},
		{"newmtl",  KEY_CSTR_MAT_DEF},
		{"usemtl",  KEY_CSTR_MAT_USE},
		{"ke",      KEY_FLOAT3_MAT_EMISSIVE},    // Emissive color
		{"ka",      KEY_FLOAT3_MAT_AMBIENT},     // Ambient color
		{"kd",      KEY_FLOAT3_MAT_DIFFUSE},     // Diffuse color
		{"ks",      KEY_FLOAT3_MAT_SPECULAR},    // Specular color
		{"ns",      KEY_FLOAT_MAT_SHININESS},    // Shininess
		{"d",       KEY_FLOAT_MAT_OPACITY},      // Opacity
		{"tr",      KEY_FLOAT_MAT_TRANSPARENCY}, // Opacity Tr = 1-d
		{"tf",      KEY_FLOAT_MAT_TRANSMISSION}, // Transmission TODO
		{"ni",      KEY_FLOAT_MAT_REFRACTION},   // Refraction index
		{"map_ke",  KEY_CSTR_MAT_MAP_EMISSIVE},  // Texture emissive color
		{"map_ka",  KEY_CSTR_MAT_MAP_AMBIENT},   // Texture ambient color
		{"map_kd",  KEY_CSTR_MAT_MAP_DIFFUSE},   // Texture diffuse color
		{"map_ks",  KEY_CSTR_MAT_MAP_SPECULAR},  // Texture specular color
		{"map_ns",  KEY_CSTR_MAT_MAP_SHININESS}, // Texture shininess
		{"map_d",   KEY_CSTR_MAT_MAP_OPACITY},   // Texture opacity
		{"map_bump",KEY_CSTR_MAT_MAP_NORMAL},    // Texture normal
	};

	const auto& line = tokenizer.tokens[lineId];
	std::string keystr = line[0];
	for(auto& c: keystr) c = std::tolower(c);
	auto it = kvmap.find(keystr);
	key = (it != kvmap.end())?it->second:KEY_UNKNOWN;

	switch(key)
	{
		case(KEY_FLOAT3_POSITION):
		case(KEY_FLOAT3_NORMAL):
		case(KEY_FLOAT3_TEXCOORD):
		case(KEY_FLOAT3_MAT_EMISSIVE):
		case(KEY_FLOAT3_MAT_AMBIENT):
		case(KEY_FLOAT3_MAT_DIFFUSE):
		case(KEY_FLOAT3_MAT_SPECULAR):
		case(KEY_FLOAT_MAT_SHININESS):
		case(KEY_FLOAT_MAT_OPACITY):
		case(KEY_FLOAT_MAT_TRANSPARENCY):
		case(KEY_FLOAT_MAT_TRANSMISSION):
		case(KEY_FLOAT_MAT_REFRACTION):

		for(uint i=1; i<line.size(); ++i)
		{
			float v = std::atof(line[i].c_str());
			data.insert(data.end(),(const char*)&v,((const char*)&v)+sizeof(v));
		}
		break;

		case(KEY_INT_FACE):

		for(uint i=1; i<line.size(); ++i)
		{
			int pos = 0,prevPos = 0,j=0,v[3] = {0,0,0};
			while(pos < line[i].size())
			{
				while(pos < line[i].size() && line[i][pos] != '/') pos++;
				v[j++%3] = std::atoi(line[i].substr(prevPos,pos-prevPos).c_str());
				pos = prevPos = pos+1;
			}
			data.insert(data.end(),(const char*)&v[0],((const char*)&v[0])+sizeof(v));
		}
		break;

		case(KEY_INT_LINE):

		for(uint i=1; i<line.size(); ++i)
		{
			int v = std::atoi(line[i].c_str());
			data.insert(data.end(),(const char*)&v,((const char*)&v)+sizeof(v));
		}
		break;

		case(KEY_CSTR_MESH):
		case(KEY_CSTR_MAT_FILE):
		case(KEY_CSTR_MAT_DEF):
		case(KEY_CSTR_MAT_USE):
		case(KEY_CSTR_MAT_MAP_EMISSIVE):
		case(KEY_CSTR_MAT_MAP_AMBIENT):
		case(KEY_CSTR_MAT_MAP_DIFFUSE):
		case(KEY_CSTR_MAT_MAP_SPECULAR):
		case(KEY_CSTR_MAT_MAP_SHININESS):
		case(KEY_CSTR_MAT_MAP_OPACITY):
		case(KEY_CSTR_MAT_MAP_NORMAL):

		data.insert(data.end(),line[line.size()-1].begin(),line[line.size()-1].end());
		break;

		default:

		key = KEY_UNKNOWN;
		break;
	}
}
