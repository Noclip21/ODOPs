#pragma once

#include "Scene.h"


struct Scene::Material
{
	/* std430 alignment */
	float colorEmissive[4];
	float colorAmbient[4];
	float colorDiffuse[4];
	float colorSpecular[4];

	int   textureEmissive;
	int   textureAmbient;
	int   textureDiffuse;
	int   textureSpecular;

	int   textureShininess;
	int   textureOpacity;
	int   textureNormal;
	float shininess;

	float opacity;
	float reflectivity;
	float refractivity;
	float padding;
};
