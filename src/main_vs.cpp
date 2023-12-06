#include <iostream>

#include <string>

#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgbvh>


int main(int argc,char* argv[])
{
	std::string configFile = "config";
	if(argc > 1) configFile = std::string(argv[1]);

	ConfigParser config(configFile.c_str());
	if(!config)
	{
		std::cout << "Error loading config file" << std::endl;
		return 1;
	}

	std::string filepathScene = config.get<std::string>("scene");
	if(filepathScene.size() <= 0)
	{
		std::cout << "Error no scenes to load" << std::endl;
		return 1;
	}

	std::string filepathCacheScene = config.get<std::string>("cache_scene");
	if(filepathCacheScene.size() <= 0)
	{
		std::cout << "Error no scene cache file specified" << std::endl;
		return 1;
	}


	const Scene scene(filepathScene.c_str());

	static const int layoutsPositions[] = {
		VertStream::COORDS,3,0
	};
	static const int layoutsAttributes[] = {
		VertStream::NORMALS,3,
		VertStream::TEXCOORDS,2,0
	};

	const VertStream vsPositions(scene,layoutsPositions);
	const VertStream vsAttributes(scene,layoutsAttributes);

	std::vector<std::pair<int,int>> attrIdMaterialId(scene.meshes.size());
	for(uint i=0; i<vsPositions.drawCommands.size(); ++i)
		attrIdMaterialId[i] = {vsPositions.drawCommands[i].firstIndex,scene.meshes[i].material};

	const BVH<OBB> bvh(
		BVH<KDOP<2>,8>(
			(const vec3*)&vsPositions.vertexArray[0],
			&vsPositions.indices[0],
			vsPositions.indices.size()));


	std::ofstream handle(filepathCacheScene);
	std::cout << filepathCacheScene << ": ";
	if(!handle.good())
	{
		std::cout << "Error opening file" << std::endl;
		return 1;
	}else
		std::cout << "Opened" << std::endl;

	std::cout << "Writing materials" << std::endl;

	int numMaterials = scene.materials.size();
	handle.write((const char*)&numMaterials,sizeof(int));
	handle.write((const char*)&scene.materials[0],sizeof(Scene::Material)*numMaterials);

	std::cout << "Writing textures" << std::endl;
	int numTextures = scene.textures.size();
	std::vector<TexLod_rgb32f> textures(numTextures);
	for(uint i=0; i<numTextures; ++i)
		textures[i] = TexLod_rgb32f(scene.textures[i].convert(Bitmap::RGB32F));
	handle.write((const char*)&numTextures,sizeof(int));
	write(handle,&textures[0],numTextures);

	std::cout << "Writing mesh / material indexing" << std::endl;
	int numAttrIdMaterialId = attrIdMaterialId.size();
	handle.write((const char*)&numAttrIdMaterialId,sizeof(int));
	handle.write((const char*)&attrIdMaterialId[0],sizeof(attrIdMaterialId[0])*numAttrIdMaterialId);

	std::cout << "Writing vertex data" << std::endl;
	write(handle,&vsPositions);

	std::cout << "Writing vertex attribute data" << std::endl;
	write(handle,&vsAttributes);

	std::cout << "Writing BVH" << std::endl;
	write(handle,&bvh);

	handle.close();

	return 0;
}
