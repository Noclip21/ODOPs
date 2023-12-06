#include <cmath>
#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtrace>

#include "Mesh.h"
#include "polynomial/derivative.inl"
#include "polynomial/polysolve.inl"


////////////////////////////////////////////////////////////////////////////////
struct BevelSegment: Mesh
{
	BevelSegment(
		const uint nVerts,
		const vec3& p0,const vec3& p1,
		const vec3& d0,const vec3& d1,
		const float r0,const float r1)
	{
		verts.resize(2*nVerts);
		const mat3 m0 = lookRotation(d0);
		const mat3 m1 = lookRotation(d1);
		for(uint i=0; i<nVerts; ++i)
		{
			const float a = 2*M_PI*float(i)/nVerts;
			verts[i].x = r0*cos(a); verts[nVerts+i].x = r1*cos(a);
			verts[i].y = r0*sin(a); verts[nVerts+i].y = r1*sin(a);
			verts[i].z = verts[nVerts+i].z = 0;
			verts[i]   = p0+m0*verts[i];
			verts[nVerts+i] = p1+m1*verts[nVerts+i];
		}
		indices.resize(2*nVerts);
		for(uint i=0; i<nVerts; ++i)
		{
			const uint j = (i+1)%nVerts;
			indices[i]        = {i,i+nVerts,j};
			indices[nVerts+i] = {j,i+nVerts,j+nVerts};
		}
	}
};
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{

	Bezier bezier;
	bezier.a = vec3(-1, 0, 1);
	bezier.b = vec3( 2,-1, 2);
	bezier.c = vec3(-1, 2, 2);
	bezier.d = vec3( 0,-1, 3);

	const vec3& a = bezier.a, &b = bezier.b, &c = bezier.c, &d = bezier.d;
	const vec3 
		A =   -a +3*b -3*c + d,
		B = +3*a -6*b +3*c,
		C = -3*a +3*b,
		D =   +a;

	Polynomial<float,3> r(+0.02,+0.14,-0.14);

	const uint nVerts = 64;

	std::cout << "o f" << std::endl;
	for(float i=1; i<=nVerts; ++i)
	{
		float t0 = (i-1)/nVerts;
		float t1 = i/nVerts;
		vec3 p0 = bezier.interpolate(t0);
		vec3 p1 = bezier.interpolate(t1);
		vec3 d0 = bezier.interpolatedt(t0);
		vec3 d1 = bezier.interpolatedt(t1);
		BevelSegment s(16,p0,p1,d0,d1,0.002,0.002);
		std::cout << s << std::endl;
	}

	std::cout << "o r" << std::endl;
	for(float i=1; i<=nVerts; ++i)
	{
		float t0 = (i-1)/nVerts;
		float t1 = i/nVerts;
		vec3 p0 = bezier.interpolate(t0);
		vec3 p1 = bezier.interpolate(t1);
		vec3 d0 = bezier.interpolatedt(t0);
		vec3 d1 = bezier.interpolatedt(t1);
		BevelSegment s(16,p0,p1,d0,d1,polyeval(t0,r),polyeval(t1,r));
		std::cout << s << std::endl;
	}

	Polynomial<vec3,4> s(D,C,B,A);
	auto sMin = s - r;
	auto sMax = s + r;
	auto sdMin = derivative(sMin);
	auto sdMax = derivative(sMax);
	std::cout << "o s_min" << std::endl;
	for(float i=1; i<=nVerts; ++i)
	{
		float t0 = (i-1)/nVerts;
		float t1 = i/nVerts;
		vec3 p0Min = polyeval(vec3(t0),sMin);
		vec3 p1Min = polyeval(vec3(t1),sMin);
		vec3 d0Min = polyeval(vec3(t0),sdMin);
		vec3 d1Min = polyeval(vec3(t1),sdMin);
		std::cout << BevelSegment(16,p0Min,p1Min,d0Min,d1Min,0.005,0.005) << std::endl;
	}
	std::cout << "o s_max" << std::endl;
	for(float i=1; i<=nVerts; ++i)
	{
		float t0 = (i-1)/nVerts;
		float t1 = i/nVerts;
		vec3 p0Max = polyeval(vec3(t0),sMax);
		vec3 p1Max = polyeval(vec3(t1),sMax);
		vec3 d0Max = polyeval(vec3(t0),sdMax);
		vec3 d1Max = polyeval(vec3(t1),sdMax);
		std::cout << BevelSegment(16,p0Max,p1Max,d0Max,d1Max,0.005,0.005) << std::endl;
	}

	AABB aabb(bezier,r);
	mat4 m = mat4(scale(mat3(),aabb.pmax-aabb.pmin));
	m = translate(m,aabb.pmin);
	std::cout << "o box" <<  std::endl;
	std::cout << m*BOX << std::endl;


	/*
	uint nBeziers = 1;
	for(uint i=0; i<nBeziers; ++i)
	for(uint j=0; j<nBeziers; ++j)
	{
		float x = 1.1*i;
		float z = 1.1*j;
		Bezier bezier(
			vec3(float(rand()%1024)/1024+x,float(rand()%1024)/1024,float(rand()%1024)/1024+z),
			vec3(float(rand()%1024)/1024+x,float(rand()%1024)/1024,float(rand()%1024)/1024+z),
			vec3(float(rand()%1024)/1024+x,float(rand()%1024)/1024,float(rand()%1024)/1024+z),
			vec3(float(rand()%1024)/1024+x,float(rand()%1024)/1024,float(rand()%1024)/1024+z));

		Polynomial<float,3> r(+0.02,+0.14,-0.14);

		uint nVerts = 32;
		static uint bezierId = 0;
		std::cout << "o bezier_" << bezierId++ << std::endl;
		for(float i=1; i<=nVerts; ++i)
		{
			float t0 = (i-1)/nVerts;
			float t1 = i/nVerts;
			vec3 p0 = bezier.interpolate(t0);
			vec3 p1 = bezier.interpolate(t1);
			vec3 d0 = bezier.interpolatedt(t0);
			vec3 d1 = bezier.interpolatedt(t1);
			BevelSegment s(16,p0,p1,d0,d1,polyeval(t0,r),polyeval(t1,r));
			std::cout << s << std::endl;
		}

		OBB obb(KDOP<4>(bezier,r));

		mat4 m = inverse(mat4(obb.m));
		static uint boxId = 0;
		std::cout << "o box_" << boxId++ <<  std::endl;
		std::cout << m*BOXWIRE << std::endl;
	}
	*/

	return 0;
}
