#pragma once

#include <vector>

#include "../bitmap/Bitmap.h"

struct Scene
{
	struct Material;
	struct Vert;
	struct Mesh;

	std::vector<Bitmap>   textures;
	std::vector<Material> materials;
	std::vector<Vert>     vertices;
	std::vector<Mesh>     meshes;

	bool good;

	inline Scene() = delete;
	inline Scene(const Scene&) = delete;
	inline Scene(const char*);

	inline operator bool() const
	{ return good; }
};
