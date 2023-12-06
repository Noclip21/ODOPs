#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>

#include "Mesh.h"
#include "vertstream/VertStream.h"

////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	Scene model1("res/dummyL.obj"),model2("res/dummyR.obj");

	const int layout[] = {VertStream::COORDS,3,0};
	VertStream vs1(model1,layout),vs2(model2,layout);

	KDOP<2> kdop1((const vec3*)&vs1.vertexArray[0],vs1.vertexArray.size()/3);
	KDOP<2> kdop2((const vec3*)&vs2.vertexArray[0],vs2.vertexArray.size()/3);
	KDOP<2> kdop3(kdop1,kdop2);


	for(uint i=0; i<kdop1.numMatrices; ++i)
	{
		std::cout << "o kdop_1" << "_" << i << std::endl;
		std::cout << inverse(mat4(kdop1.m(i)))*BOX << std::endl;
	}
	for(uint i=0; i<kdop2.numMatrices; ++i)
	{
		std::cout << "o kdop_2" << "_" << i << std::endl;
		std::cout << inverse(mat4(kdop2.m(i)))*BOX << std::endl;
	}
	for(uint i=0; i<kdop3.numMatrices; ++i)
	{
		std::cout << "o kdop_3" << "_" << i << std::endl;
		std::cout << inverse(mat4(kdop3.m(i)))*BOX << std::endl;
	}


	std::cout << "o obb1" << std::endl;
	std::cout << inverse(mat4(OBB(kdop1).m))*BOX << std::endl;

	std::cout << "o obb2" << std::endl;
	std::cout << inverse(mat4(OBB(kdop2).m))*BOX << std::endl;

	std::cout << "o obb3" << std::endl;
	std::cout << inverse(mat4(OBB(kdop3).m))*BOX << std::endl;

	return 0;
}
