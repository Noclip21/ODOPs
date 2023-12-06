#pragma once

#include <vector>

#include "Scene.h"


struct Scene::Mesh
{
	enum Type
	{
		POINTS    = 1,
		LINES     = 2,
		TRIANGLES = 3,
	};

	int type;
	int material;
	std::vector<int> indices;
};
