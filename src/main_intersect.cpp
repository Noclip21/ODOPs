#include <iostream>

#include <cgmath>
#include <cgbvh>

#include "intersect.h"

const vec3 aabbs[][2] = {
	// Cube
	{{0,1,0},{2,2,2}},
	// Plane
	{{0,1,0},{2,1,2}},
	// Line
	{{0,1,2},{2,1,2}},
	// Point
	{{2,1,2},{2,1,2}},

};
const char* aabbstr[] = {
	"Cube ",
	"Plane",
	"Line ",
	"Point",
};
const Ray rays[] = {
	// Does not intersect
	{{1, -1,  1},{ 2,  2,  0}},
	//Internal to volume
	{{1,1.2,  1},{0,   1,  0}},
	//Intersects planes
	{{3,  2,  2},{-4, -3, -1}},
	//Internal to plane
	{{0,  1,  0},{2,   0,  2}},
	//Intersects at edge
	{{0,  1,  2},{ 2,  0,  0}},
	//Intersects at vertex
	{{3,  3,  3},{-1, -2, -1}},
};
const char* raystr[] = {
	"Does not intersect  ", // 0
	"Internal to volume  ", // 1
	"Intersects planes   ", // 2
	"Internal to plane   ", // 3
	"Intersects at edge  ", // 4
	"Intersects at vertex", // 5
};

const int numAABBs = sizeof(aabbs)/(2*sizeof(vec3));
const int numRays  = sizeof(rays)/sizeof(Ray);
const bool expectedResults[numAABBs][numRays] = {
	//       0 1 2 3 4 5 j
	/* 0 */ {0,1,1,1,1,1},
	/* 1 */ {0,0,1,1,1,1},
	/* 2 */ {0,0,0,1,1,1},
	/* 3 */ {0,0,0,1,1,1},
	/* i */
};
////////////////////////////////////////////////////////////////////////////////
void printVert(const vec3& v)
{ std::cout << "v " << transpose(v) << std::endl; }
////////////////////////////////////////////////////////////////////////////////
static int vertexCounter = 1;
void printRay(int i)
{
	const Ray& ray = rays[i];
	const vec3 p0(ray.o),p1(ray.o+ray.d);
	std::cout << "o " << raystr[i] << std::endl;
	printVert(p0); printVert(p1);
	std::cout << "f";
	std::cout << " " << vertexCounter++;
	std::cout << " " << vertexCounter++;
	std::cout << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
void printAABB(int i)
{
	const vec3& pmin = aabbs[i][0];
	const vec3& pmax = aabbs[i][1];
	std::cout << "o " << aabbstr[i] << std::endl;
	printVert(vec3(pmax.x,pmax.y,pmin.z));
	printVert(vec3(pmax.x,pmin.y,pmin.z));
	printVert(vec3(pmax.x,pmax.y,pmax.z));
	printVert(vec3(pmax.x,pmin.y,pmax.z));
	printVert(vec3(pmin.x,pmax.y,pmin.z));
	printVert(vec3(pmin.x,pmin.y,pmin.z));
	printVert(vec3(pmin.x,pmax.y,pmax.z));
	printVert(vec3(pmin.x,pmin.y,pmax.z));
	int v0 = vertexCounter,   v1 = vertexCounter+1,
		v2 = vertexCounter+2, v3 = vertexCounter+3,
		v4 = vertexCounter+4, v5 = vertexCounter+5,
		v6 = vertexCounter+6, v7 = vertexCounter+7;
	std::cout
		<< "f " << v0 << " " << v4 << " " << v6 << " " << v2 << std::endl
		<< "f " << v3 << " " << v2 << " " << v6 << " " << v7 << std::endl
		<< "f " << v7 << " " << v6 << " " << v4 << " " << v5 << std::endl
		<< "f " << v5 << " " << v1 << " " << v3 << " " << v7 << std::endl
		<< "f " << v1 << " " << v0 << " " << v2 << " " << v3 << std::endl
		<< "f " << v5 << " " << v4 << " " << v0 << " " << v1 << std::endl;
	vertexCounter += 8;
}
////////////////////////////////////////////////////////////////////////////////
void printTestAABB(int i,int j)
{
	AABB aabb;
	aabb.pmin = aabbs[i][0];
	aabb.pmax = aabbs[i][1];
	const Ray& ray = rays[j];
	bool hit = intersect<bool,Ray>(ray,aabb);
	bool expected = expectedResults[i][j];
	std::cout
		<< "Object: " << aabbstr[i] << "\t"
		<< "Ray: " << raystr[j] << "\t"
		<< "Hit = " << hit << "\t"
		<< expected << " expected" << "\t"
		<< ((hit == expected)?"OK":"ERROR") << std::endl;
		
}
////////////////////////////////////////////////////////////////////////////////
void printTestOBB(int i,int j)
{
	OBB obb;
	vec3 pmin = aabbs[i][0];
	vec3 pmax = aabbs[i][1];

	const float inf = 1.0f/0.0f;
	vec3 s = pmax-pmin;
		 s = 1/vec3(
		 	(abs(s.x) > 5e-5)?s.x:5e-5,
			(abs(s.y) > 5e-5)?s.y:5e-5,
			(abs(s.z) > 5e-5)?s.z:5e-5);

	vec3 t = -pmin;

	mat3 R = transpose(mat3());
	     R = mat3(R(0)*s,R(1)*s,R(2)*s);

	obb.m = mat3x4(R(0),R(1),R(2),R*t);

	const Ray& ray = rays[j];

	bool hit = intersect<bool,Ray>(ray,obb);
	bool expected = expectedResults[i][j];

	std::cout
		<< "Object: " << aabbstr[i] << "\t"
		<< "Ray: " << raystr[j] << "\t"
		<< "Hit = " << hit << "\t"
		<< expected << " expected" << "\t"
		<< ((hit == expected)?"OK":"ERROR") << std::endl;
		
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
#if 0
	for(int i=0; i<numRays; ++i)
		printRay(i);

	for(int i=0; i<numAABBs; ++i)
		printAABB(i);
#endif
#if 0
	for(int i=0; i<numAABBs; ++i)
		for(int j=0; j<numRays; ++j)
			printTestAABB(i,j);
#endif
#if 1
	for(int i=0; i<numAABBs; ++i)
		for(int j=0; j<numRays; ++j)
			printTestOBB(i,j);
#endif
	return 0;
}
