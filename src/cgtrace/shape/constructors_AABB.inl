#pragma once

#include <limits>

#include "AABB.h"
#include "OBB.h"
#include "KDOP.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Bezier.h"

#include "interpolate_Bezier.inl"


////////////////////////////////////////////////////////////////////////////////
AABB::AABB():
	pmin(+std::numeric_limits<float>::infinity()),
	pmax(-std::numeric_limits<float>::infinity()){}
////////////////////////////////////////////////////////////////////////////////
AABB::AABB(const vec3& pmin,const vec3& pmax):pmin(pmin),pmax(pmax){}
////////////////////////////////////////////////////////////////////////////////
AABB::AABB(const vec3* p,const uint n):AABB()
{
	for(uint i=0; i<n; ++i)
	{
		pmin = min(pmin,p[i]);	
		pmax = max(pmax,p[i]);	
	}
}
////////////////////////////////////////////////////////////////////////////////
AABB::AABB(const AABB& a,const AABB& b):
	pmin(min(a.pmin,b.pmin)),pmax(max(a.pmax,b.pmax)){}
////////////////////////////////////////////////////////////////////////////////
template<>
AABB::AABB(const OBB& obb):AABB()
{
	const mat3x4 invM = mat3x4(inverse(mat4(obb.m)));
	const vec3 v[8] = {
		invM*vec4(0,0,0,1), invM*vec4(0,0,1,1),
		invM*vec4(0,1,0,1), invM*vec4(0,1,1,1),
		invM*vec4(1,0,0,1), invM*vec4(1,0,1,1),
		invM*vec4(1,1,0,1), invM*vec4(1,1,1,1),
	};
	for(uint i=0; i<8; ++i)
	{
		pmin = min(pmin,v[i]);
		pmax = max(pmax,v[i]);
	}
}
////////////////////////////////////////////////////////////////////////////////
template<>
AABB::AABB(const Sphere& sphere):
	pmin(sphere.o-sphere.r),pmax(sphere.o+sphere.r){}
////////////////////////////////////////////////////////////////////////////////
template<>
AABB::AABB(const Triangle& triangle):AABB((const vec3*)&triangle.p1,3){}
////////////////////////////////////////////////////////////////////////////////
template<>
AABB::AABB(const Bezier& bezier):AABB(bezier,Polynomial<float,1>(0)){}
////////////////////////////////////////////////////////////////////////////////
template<uint N>
AABB::AABB(const Bezier& bezier,const Polynomial<float,N>& pr)
{
	// Convert from bezier coefficients to polynomial coefficients
	const vec3& a = bezier.a, &b = bezier.b, &c = bezier.c, &d = bezier.d;
	const vec3 
		A =   -a +3*b -3*c + d,
		B = +3*a -6*b +3*c,
		C = -3*a +3*b,
		D =   +a;

	// Split multivariate coefficients to single variate axis-aligned
	// polynomials
	Polynomial<float,4> s[3] = {
		Polynomial<float,4>(D.x,C.x,B.x,A.x),
		Polynomial<float,4>(D.y,C.y,B.y,A.y),
		Polynomial<float,4>(D.z,C.z,B.z,A.z)
	};

	// Compute pmin / pmax for each axis
	for(uint i=0; i<3; ++i)
	{
		// pmin / pmax default to t = 0, t = 1
		pmin[i] = min(polyeval(0.0f,s[i])-polyeval(0.0f,pr),polyeval(1.0f,s[i])-polyeval(1.0f,pr));
		pmax[i] = max(polyeval(0.0f,s[i])+polyeval(0.0f,pr),polyeval(1.0f,s[i])+polyeval(1.0f,pr));

		// Compute the roots of s'(t) - r'(t) and s'(t) + r'(t)
		auto dmin = derivative(s[i]-pr);
		auto dmax = derivative(s[i]+pr);
		polysolveInterval(0.0f,1.0f,dmin);
		polysolveInterval(0.0f,1.0f,dmax);

		for(uint j=0; j<dmin.nRoots; ++j)
		{
			const float tmin = dmin.roots[j];
			const float tmax = dmax.roots[j];
			// Evaluate t's within the [0,1] interval
			if(tmin > 0 && tmin < 1) pmin[i] = min(pmin[i],polyeval(tmin,s[i])-polyeval(tmin,pr));
			if(tmax > 0 && tmax < 1) pmax[i] = max(pmax[i],polyeval(tmax,s[i])+polyeval(tmax,pr));
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
AABB::AABB(const KDOP<K>& kdop):AABB(OBB(kdop)){}
////////////////////////////////////////////////////////////////////////////////
