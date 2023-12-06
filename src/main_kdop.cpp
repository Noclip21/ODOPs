#include <iostream>
#include <vector>

#include <cgmath>
#include <cgasset>
#include <cgtrace>

#include "Mesh.h"


int main(int argc,char* argv[])
{
	static constexpr uint N = 2;
	KDOP<N> kdop;

	for(uint i=0; i<kdop.numMatrices; ++i)
	{
		std::cout << "o directions_" << i << std::endl;
		std::cout << mat4(kdop.kn.m[i])*scale(mat4(),vec4(0.01,0.01,1,1))*BOX << std::endl;
		std::cout << mat4(kdop.kn.m[i])*scale(mat4(),vec4(0.01,1,0.01,1))*BOX << std::endl;
		std::cout << mat4(kdop.kn.m[i])*scale(mat4(),vec4(1,0.01,0.01,1))*BOX << std::endl;
		std::cout << mat4(kdop.kn.m[i])*scale(mat4(),vec4(0.01,0.01,-1,1))*BOX << std::endl;
		std::cout << mat4(kdop.kn.m[i])*scale(mat4(),vec4(0.01,-1,0.01,1))*BOX << std::endl;
		std::cout << mat4(kdop.kn.m[i])*scale(mat4(),vec4(-1,0.01,0.01,1))*BOX << std::endl;
	}

	for(uint i=0; i<kdop.numMatrices; ++i)
	{
		std::cout << "o boxes_" << i << std::endl;
		std::cout << mat4(kdop.kn.m[i])*translate(mat4(),vec3(-0.5))*BOX << std::endl;
	}

	return 0;
}
