#pragma once

#include "Scene.h"


struct Scene::Vert
{
	union{
		char data[72];
		struct{
			float position[3];
			float normal[3];
			float tangent[3];
			float bitangent[3];
			float texcoord[2];
			float color[4];
		};
	};
};
