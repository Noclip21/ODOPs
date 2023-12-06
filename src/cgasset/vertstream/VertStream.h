#pragma once

#include <string>

#include "../scene/Scene.h"


struct VertStream
{
	struct Command;

	enum Attributes
	{
		END        = 0,
		COORDS     = 1,
		COLORS     = 2,
		NORMALS    = 3,
		TANGENTS   = 4,
		BITANGENTS = 5,
		TEXCOORDS  = 6,
	};

	std::vector<int>     attributeLayout;
	std::vector<int>     nComponents;
	std::vector<Command> drawCommands;
	std::vector<int>     indices;
	std::vector<float>   vertexArray;

	VertStream() = default;
	VertStream(const Scene&,const int*);
	VertStream(const Scene::Mesh&,const std::vector<Scene::Vert>&,const int*);
	VertStream(const std::vector<Scene::Mesh>&,const std::vector<Scene::Vert>&,const int*);
};
