#include <iostream>

#include <cgmath>

#include "sample.h"
#include "Mesh.h"

int main(int argc,char* argv[])
{
	static const int n = 1024;
	vec3 samples[n];
	genUniformSphericalSamples(samples,n);
	for(uint i=0; i<n; ++i)
	{
		std::cout << "v " << transpose(samples[i]) << std::endl;
	}
	return 0;
}
