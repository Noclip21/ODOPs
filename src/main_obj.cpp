#include <iostream>
#include <cgasset>


namespace codec
{
	namespace obj
	{
		struct Scene
		{
			std::vector<std::vector<float>> positions;
			std::vector<std::vector<float>> normals;
			std::vector<std::vector<float>> texcoords;

			std::unordered_map<std::string,std::vector<int>> meshMap;
			std::unordered_map<std::string,std::unordered_map<int,std::vector<char>>> materialMap;

			std::unordered_map<std::string,std::string> meshMaterialMap;

			bool good;

			Scene(const char*);
		};
	};
};

codec::obj::Scene::Scene(const char* filepath)
{
	Tokenizer tokenizer(filepath);
	good = tokenizer.good;
	if(!good) return;

	for(uint i=0; i<tokenizer.tokens.size(); ++i)
	{
		Parser parser(tokenizer,i);

		switch(parser.key)
		{
			case(Parser::KEY_FLOAT3_POSITION):
			positions[positions.end()-1] = std::vector((const float*)&parser.data[0],(const float*)&parser.data[parser.data.size()-sizeof(float)]);
			break;
			case(Parser::KEY_FLOAT3_NORMAL):
			//normals.insert(std::vector((const float*)&parser.data[0],(const float*)&parser.data[parser.size()-1]);
			//case(Parser::KEY_FLOAT3_TEXCOORD):
			//texcoords.insert(std::vector((const float*)&parser.data[0],(const float*)&parser.data[parser.size()-1]);
		};
	}
};

int main(int argc,char* argv[])
{
	codec::obj::Tokenizer tokenizer("test.obj");

	for(uint i=0; i<tokenizer.tokens.size(); ++i)
	{
		codec::obj::Parser(tokenizer,i);
	}

	return 0;
	/*
	Scene scene("res/cornellbox.obj");

	for(uint i=0; i<scene.vertices.size(); ++i)
		std::cout << "v " << transpose(*(vec3*)scene.vertices[i].position) << std::endl;

	for(auto& m: scene.meshes)
		for(uint i=0; i<m.indices.size()/3; ++i)
			std::cout << "f " << transpose(((ivec3*)&m.indices[0])[i]+1) << std::endl;
	return 0;
	*/
}
