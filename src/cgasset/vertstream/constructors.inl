#pragma once

#include <cstring>
#include <unordered_map>


#include "VertStream.h"
#include "Command.h"

#include "../scene/Mesh.h"
#include "../scene/Vert.h"


////////////////////////////////////////////////////////////////////////////////
inline VertStream::VertStream(const Scene& scene,const int* layout):
	VertStream(scene.meshes,scene.vertices,layout){}
////////////////////////////////////////////////////////////////////////////////
inline VertStream::VertStream(
	const Scene::Mesh&              mesh,
	const std::vector<Scene::Vert>& vertices,
	const int*                      layout):
	VertStream(std::vector<Scene::Mesh>{mesh},vertices,layout){}
////////////////////////////////////////////////////////////////////////////////
inline VertStream::VertStream(
		const std::vector<Scene::Mesh>& meshes,
		const std::vector<Scene::Vert>& vertices,
		const int*                      layout)
{
	// Compute vertex array attribute layout
	int nCoords     = 0;
	int nColors     = 0;
	int nNormals    = 0;
	int nTangents   = 0;
	int nBitangents = 0;
	int nTexcoords  = 0;
	while(int l = *layout++)
	{
		attributeLayout.push_back(l);
		switch(l)
		{
			case(COORDS):     nCoords     = *layout++; break;
			case(COLORS):     nColors     = *layout++; break;
			case(NORMALS):    nNormals    = *layout++; break;
			case(TANGENTS):   nTangents   = *layout++; break;
			case(BITANGENTS): nBitangents = *layout++; break;
			case(TEXCOORDS):  nTexcoords  = *layout++; break;
		}
	}
	nCoords     = (nCoords>3)?    3:(nCoords<0)?    0:nCoords;
	nColors     = (nColors>4)?    4:(nColors<0)?    0:nColors;
	nNormals    = (nNormals>3)?   3:(nNormals<0)?   0:nNormals;
	nTangents   = (nTangents>3)?  3:(nTangents<0)?  0:nTangents;
	nBitangents = (nBitangents>3)?3:(nBitangents<0)?0:nBitangents;
	nTexcoords  = (nTexcoords>2)? 2:(nTexcoords<0)? 0:nTexcoords;
	for(size_t i=0; i<attributeLayout.size(); ++i)
		switch(attributeLayout[i])
		{
			case(COORDS):     nComponents.push_back(nCoords);     break;
			case(COLORS):     nComponents.push_back(nColors);     break;
			case(NORMALS):    nComponents.push_back(nNormals);    break;
			case(TANGENTS):   nComponents.push_back(nTangents);   break;
			case(BITANGENTS): nComponents.push_back(nBitangents); break;
			case(TEXCOORDS):  nComponents.push_back(nTexcoords);  break;
		}


	// Discard vertices not being indexed
	// Merge vertices with the same attributes
	// Recompute indices
	// Discard unused attributes
	std::unordered_map<std::string,int> vertMap;
	for(size_t i=0; i<meshes.size(); ++i)
	{
		const Scene::Mesh& m = meshes[i];
		Command cmd = {
    		.count        = (int)m.indices.size(),
    		.primCount    = 1,
    		.firstIndex   = (int)indices.size(),
    		.baseVertex   = 0,
    		.baseInstance = (int)i,
    	};
		drawCommands.push_back(cmd);	
		for(size_t j=0; j<m.indices.size(); ++j)
		{
			int vIdMesh = m.indices[j];
			Scene::Vert v = vertices[vIdMesh];
			// Zero unused attributes
			memset(((float*)&v.position)+nCoords,     0,(3-nCoords)*sizeof(float));
			memset(((float*)&v.color)+nColors,        0,(4-nColors)*sizeof(float));
			memset(((float*)&v.normal)+nNormals,      0,(3-nNormals)*sizeof(float));
			memset(((float*)&v.tangent)+nTangents,    0,(3-nTangents)*sizeof(float));
			memset(((float*)&v.bitangent)+nBitangents,0,(3-nBitangents)*sizeof(float));
			memset(((float*)&v.texcoord)+nTexcoords,  0,(2-nTexcoords)*sizeof(float));
			int& vIdMap = vertMap[std::string((const char*)&v,sizeof(v))];
			if(!vIdMap)
			{
				//  Write vertex attribue to array
				float* vp  = (float*)&v.position;
				float* vc  = (float*)&v.color;
				float* vn  = (float*)&v.normal;
				float* vtg = (float*)&v.tangent;
				float* vbg = (float*)&v.bitangent;
				float* vt  = (float*)&v.texcoord;
				auto&  va  = vertexArray;
				for(size_t i=0; i<attributeLayout.size(); ++i)
					switch(attributeLayout[i])
					{
						case(COORDS):     va.insert(va.end(),vp,vp+nCoords);       break;
						case(COLORS):     va.insert(va.end(),vc,vc+nColors);       break;
						case(NORMALS):    va.insert(va.end(),vn,vn+nNormals);      break;
						case(TANGENTS):   va.insert(va.end(),vtg,vtg+nTangents);   break;
						case(BITANGENTS): va.insert(va.end(),vbg,vbg+nBitangents); break;
						case(TEXCOORDS):  va.insert(va.end(),vt,vt+nTexcoords);    break;
					}
				vIdMap = vertexArray.size()/
					(nCoords+nColors+nNormals+nTangents+nBitangents+nTexcoords);
			}
			// Write vertex attribute Id to array
			indices.push_back(vIdMap-1);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
