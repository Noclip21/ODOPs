#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgbvh>

#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	Scene scene("input.obj");

	static const int layoutsPositions[] = {
		VertStream::COORDS,3
	};
	VertStream vs(scene,layoutsPositions);


	BVH<KDOP<8>,8> bvhKDOP((const vec3*)&vs.vertexArray[0],&vs.indices[0],vs.indices.size());
	BVH<OBB> bvh(bvhKDOP);

	for(uint i=0; i<bvh.nodes.size(); ++i)
	{
		Node<OBB> node = bvh.nodes[i];
		uint l = node.left  & bvh.leafMask, lx = node.left  & ~bvh.leafMask;
		uint r = node.right & bvh.leafMask, rx = node.right & ~bvh.leafMask;
		std::cout << "o node" << i
			<< "_l" << lx << ((l)?"(leaf)":"")
			<< "_r" << rx << ((r)?"(leaf)":"") << std::endl;
		std::cout << inverse(mat4(node.volume.m))*BOXWIRE << std::endl;
		std::cout << "o node" << i << " center" << std::endl;
		std::cout << Point(bvhKDOP.nodes[i].volume.center()) << std::endl;
		const vec3* verts = (const vec3*)&vs.vertexArray[0];
		if(l)
		{
			vec3 p1 = verts[vs.indices[3*lx]];
			vec3 p2 = verts[vs.indices[3*lx+1]];
			vec3 p3 = verts[vs.indices[3*lx+2]];
			std::cout << "o triangle" << lx << std::endl;
			std::cout << Triangle(p1,p2,p3) << std::endl;
		}
		if(r)
		{
			vec3 p1 = verts[vs.indices[3*rx]];
			vec3 p2 = verts[vs.indices[3*rx+1]];
			vec3 p3 = verts[vs.indices[3*rx+2]];
			std::cout << "o triangle" << rx << std::endl;
			std::cout << Triangle(p1,p2,p3) << std::endl;
		}
	}

	return 0;
}
