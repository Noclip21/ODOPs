#pragma once

#include <iostream>
#include <filesystem>
#include <cstring>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Scene.h"
#include "Material.h"
#include "Vert.h"
#include "Mesh.h"
#include "../bitmap/Bitmap.h"
#include "../bitmap/constructors.inl"
#include "../bitmap/operators.inl"
#include "../file/operators.inl"


Scene::Scene(const char* filepath)
{
	/* Reading File */
	Assimp::Importer importer;
	const aiScene* assimpScene = 
		importer.ReadFile(filepath,
        	aiProcess_GenNormals            |
        	aiProcess_GenUVCoords           |
        	aiProcess_JoinIdenticalVertices |
        	aiProcess_Triangulate           |
        	aiProcess_SortByPType);

	good = (bool)assimpScene;
	if(File::verbose) std::cout << filepath << ": ";
	if(!good)
	{
		if(File::verbose)
		{
			std::cout << "Error parsing file" << std::endl;
			std::cout << importer.GetErrorString() << std::endl;
		}
		return;
	}
	if(File::verbose) std::cout << "Reading successful" << std::endl;


	auto assimpMaterials = assimpScene->mMaterials;
	auto assimpMeshes    = assimpScene->mMeshes;
	int  numMaterials    = assimpScene->mNumMaterials;
	int  numMeshes       = assimpScene->mNumMeshes;

	/* Reading textures */
	int numTextures = 0;
	std::string dirpath = std::filesystem::path(filepath).parent_path();
	dirpath += ((dirpath.size() > 0)?"/":"");
	std::unordered_map<std::string,int> textureMap;
	auto loadTexture = [&](const aiMaterial& m,const aiTextureType& texType,const int i)->int
	{
		if(m.GetTextureCount(texType) <= 0) return -1;

		aiString texFile;
		m.GetTexture(texType,i,&texFile);
		std::string texStr = texFile.C_Str();
		if(texStr.size() <= 0) return -1;

		std::string fullpath = dirpath+texStr;
		int& texId = textureMap[fullpath];
		if(texId) return texId;

		Bitmap bitmap(fullpath.c_str());
		if(!bitmap) return -1;

		texId = textures.size();
		textures.push_back(bitmap);
		numTextures++;
		return texId;
	};

	/* Reading materials */
	for(int i=0; i<numMaterials; ++i)
	{
		const aiMaterial& m = *assimpMaterials[i];

		Material mat;
			m.Get(AI_MATKEY_COLOR_EMISSIVE,mat.colorEmissive[0]);
			m.Get(AI_MATKEY_COLOR_AMBIENT,mat.colorAmbient[0]);
			m.Get(AI_MATKEY_COLOR_DIFFUSE,mat.colorDiffuse[0]);
			m.Get(AI_MATKEY_COLOR_SPECULAR,mat.colorSpecular[0]);
			mat.textureEmissive  = loadTexture(m,aiTextureType_EMISSIVE,0);
			mat.textureAmbient   = loadTexture(m,aiTextureType_AMBIENT,0);
			mat.textureDiffuse   = loadTexture(m,aiTextureType_DIFFUSE,0);
			mat.textureSpecular  = loadTexture(m,aiTextureType_SPECULAR,0);
			mat.textureShininess = loadTexture(m,aiTextureType_SHININESS,0);
			mat.textureOpacity   = loadTexture(m,aiTextureType_OPACITY,0);
			mat.textureNormal    = loadTexture(m,aiTextureType_HEIGHT,0);
			m.Get(AI_MATKEY_SHININESS,mat.shininess);
			m.Get(AI_MATKEY_OPACITY,mat.opacity);
			m.Get(AI_MATKEY_REFLECTIVITY,mat.reflectivity);
			m.Get(AI_MATKEY_REFRACTI,mat.refractivity);

		materials.push_back(mat);
	}

	/* Reading geometry */
	int nFaces = 0,nVerts = 0;
	int baseVertex = vertices.size();
	//int baseMatId  = materials.size();
	for(int i=0; i<numMeshes; ++i)
	{
		const aiMesh& m  = *assimpMeshes[i];
		const float* vt  = (const float*)(&m.mVertices[0]);
        const float* vc  = (const float*)(&m.mColors[0]);
        const float* vn  = (const float*)(&m.mNormals[0]);
        const float* vtg = (const float*)(&m.mTangents[0]);
        const float* vbg = (const float*)(&m.mBitangents[0]);
        const float* uv  = (const float*)(&m.mTextureCoords[0][0]);
        size_t matId     = m.mMaterialIndex;
        size_t vtCount   = m.mNumVertices;
        size_t faceCount = m.mNumFaces;
        size_t viPerFace = m.mFaces[0].mNumIndices;

		Mesh mesh;
        int drawMode;
        switch(viPerFace)
        {
            case(2):{ drawMode = Mesh::Type::LINES;     break; }
            case(3):{ drawMode = Mesh::Type::TRIANGLES; break; }
            default:{ drawMode = Mesh::Type::POINTS;    break; }
        }

        for(size_t j=0; j<faceCount; ++j)
        	for(size_t k=0; k<viPerFace; ++k)
            	mesh.indices.push_back(m.mFaces[j].mIndices[k]+baseVertex);

        for(size_t j=0; j<vtCount; ++j)
        {
        	Vert vert;
            if(m.HasPositions())             memcpy(vert.position, vt+3*j, sizeof(vert.position));
            if(m.HasVertexColors(0))         memcpy(vert.color,    vc+4*j, sizeof(vert.color));
            if(m.HasNormals())               memcpy(vert.normal,   vn+3*j, sizeof(vert.normal));
            if(m.HasTangentsAndBitangents()) memcpy(vert.tangent,  vtg+3*j,sizeof(vert.tangent));
            if(m.HasTangentsAndBitangents()) memcpy(vert.bitangent,vbg+3*j,sizeof(vert.bitangent));
            if(m.HasTextureCoords(0))        memcpy(vert.texcoord, uv+3*j, sizeof(vert.texcoord));
            vertices.push_back(vert);
        }

        mesh.type     = drawMode;
        mesh.material = matId;//+baseMatId;
		meshes.push_back(mesh);

        nVerts     += vtCount;
        nFaces     += faceCount;
        baseVertex += vtCount;
	}

	if(File::verbose && numMaterials)
		std::cout << "Computed materials: " << numMaterials << std::endl;
	if(File::verbose && numTextures)
		std::cout << "Computed textures:  " << numTextures << std::endl;
	if(File::verbose && numMeshes)
	{
		std::cout << "Computed meshes:    " << numMeshes << std::endl;
		std::cout << "Computed vertices:  " << nVerts    << std::endl;
		std::cout << "Computed faces:     " << nFaces    << std::endl;
	}
}
