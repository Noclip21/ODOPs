#pragma once

#include "AABB.h"
#include "OBB.h"
#include "KDOP.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Bezier.h"
#include "polynomial/polysolve.inl"


////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const vec3* p,const uint n)
{
	for(uint i=0; i<2*numNormals; i+=2)
	{
		ko[i  ] = +std::numeric_limits<float>::infinity();
		ko[i+1] = -std::numeric_limits<float>::infinity();
		for(uint j=0; j<n; ++j)
		{
			float d = dot(kn.v[i/2],p[j]);
			ko[i  ] = min(ko[i  ],d);
			ko[i+1] = max(ko[i+1],d);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const KDOP& a,const KDOP& b)
{
	for(uint i=0; i<2*numNormals;)
	{
		ko[i] = min(a.ko[i],b.ko[i]); i++;
		ko[i] = max(a.ko[i],b.ko[i]); i++;
	}
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const AABB& aabb)
{
	const vec3 s = aabb.pmax-aabb.pmin;

	const vec3 v[8] = {
		aabb.pmin+vec3(  0,  0,0), aabb.pmin+vec3(  0,  0,s.z),
		aabb.pmin+vec3(  0,s.y,0), aabb.pmin+vec3(  0,s.y,s.z),
		aabb.pmin+vec3(s.x,  0,0), aabb.pmin+vec3(s.x,  0,s.z),
		aabb.pmin+vec3(s.x,s.y,0), aabb.pmin+vec3(s.x,s.y,s.z),
	};

	*this = KDOP(v,8);
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const OBB& obb)
{
	const mat3x4 obbInvM = mat3x4(inverse(mat4(obb.m)));

	const vec3 v[8] = {
		obbInvM*vec4(0,0,0,1), obbInvM*vec4(0,0,1,1),
		obbInvM*vec4(0,1,0,1), obbInvM*vec4(0,1,1,1),
		obbInvM*vec4(1,0,0,1), obbInvM*vec4(1,0,1,1),
		obbInvM*vec4(1,1,0,1), obbInvM*vec4(1,1,1,1),
	};

	*this = KDOP(v,8);
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const Sphere& sphere)
{
	// TODO
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const Triangle& triangle):KDOP((const vec3*)&triangle,3){}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
KDOP<K>::KDOP(const Bezier& bezier):KDOP(bezier,Polynomial<float,1>(0)){}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
template<uint N>
KDOP<K>::KDOP(const Bezier& bezier,const Polynomial<float,N>& pr)
{
	for(uint i=0; i<numMatrices; ++i)
	{
		const mat3x4 v = transposeMulti(kn.m[i],mat3x4(bezier.a,bezier.b,bezier.c,bezier.d));
		AABB aabb(Bezier(v(0),v(1),v(2),v(3)),pr);
		ko[6*i+0] = aabb.pmin[0];
		ko[6*i+1] = aabb.pmax[0];
		ko[6*i+2] = aabb.pmin[1];
		ko[6*i+3] = aabb.pmax[1];
		ko[6*i+4] = aabb.pmin[2];
		ko[6*i+5] = aabb.pmax[2];
	}
}
////////////////////////////////////////////////////////////////////////////////
