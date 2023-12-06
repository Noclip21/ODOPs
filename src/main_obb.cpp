#include <iostream>

#include <cgasset>
#include <cgmath>
#include <cgbvh>

#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const Mesh mesh = {
		.verts = {
			{3,2,1},
			{2,3,4},
			{3,3,3},
			{4,4,2},
		},
		.indices = {{0},{1},{2},{3}},
	};

	// build KDOP
	KDOP<16> kdop;

	for(uint i=0; i<2*kdop.numNormals; i+=2)
	{
		kdop.ko[i  ] = +std::numeric_limits<float>::infinity();
		kdop.ko[i+1] = -std::numeric_limits<float>::infinity();
		for(auto& p: mesh.verts)
		{
			float d = dot(kdop.kn.v[i/2],p);
			kdop.ko[i  ] = min(kdop.ko[i  ],d);
			kdop.ko[i+1] = max(kdop.ko[i+1],d);
		}
	}


	// build OBB
	uint mi = 0;
	float sah = std::numeric_limits<float>::infinity();
	for(uint i=0; i<kdop.numMatrices; ++i)
	{
		const float sah2 = kdop.SAH(i);
		if(sah2 >= sah) continue;
		sah = sah2;
		mi  = i;
	}

	const float* ds = kdop.ko+6*mi;

	const vec3 s(ds[1]-ds[0],ds[3]-ds[2],ds[5]-ds[4]);
	const vec3 t(ds[0],ds[2],ds[4]);

	mat4 R = mat4(kdop.kn.m[mi]);
	mat4 S = mat4(scale(mat3(),s));
	mat4 T = translate(mat4(),t);

	mat4 m = R*T*S;

	std::cout << mesh << std::endl;
	std::cout << m*BOXWIRE << std::endl;

	return 0;
}
