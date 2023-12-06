#pragma once

#include "AABB.h"
#include "OBB.h"
#include "KDOP.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Bezier.h"

#include "constructors_AABB.inl"
#include "SAH_KDOP.inl"


////////////////////////////////////////////////////////////////////////////////
OBB::OBB():m(mat3(1),vec3(0)){}
////////////////////////////////////////////////////////////////////////////////
OBB::OBB(const mat3x4& m):m(m){}
////////////////////////////////////////////////////////////////////////////////
// Source https://www.shadertoy.com/view/sdsGDM
OBB::OBB(const vec3* p,const uint n)
{
	auto [mean,cov] = covariance(p,n);
	const mat3 u = eigs(cov);

	const mat3 ut = transpose(u);
	vec3 minv = +std::numeric_limits<float>::infinity(),
		 maxv = -std::numeric_limits<float>::infinity();
	for(uint i=0; i<n; ++i)
	{
		const vec3 v = ut*(p[i]-mean);
		minv = min(minv,v);
		maxv = max(maxv,v);
	}

	const vec3 s = max(maxv-minv,vec3(5e-5));
	const vec3 t = mean+u*minv;

	const mat4 S = scale(mat4(),vec4(s,1));
	const mat4 R = mat4(u);
	const mat4 T = translate(mat4(),t);

	m = mat3x4(inverse(T*R*S));
}
////////////////////////////////////////////////////////////////////////////////
OBB::OBB(const OBB& a,const OBB& b)
{
	const mat3x4 
		aInvM = mat3x4(inverse(mat4(a.m))),
		bInvM = mat3x4(inverse(mat4(b.m)));

	const vec3 v[16] = {
		aInvM*vec4(0,0,0,1), aInvM*vec4(0,0,1,1),
		aInvM*vec4(0,1,0,1), aInvM*vec4(0,1,1,1),
		aInvM*vec4(1,0,0,1), aInvM*vec4(1,0,1,1),
		aInvM*vec4(1,1,0,1), aInvM*vec4(1,1,1,1),
		bInvM*vec4(0,0,0,1), bInvM*vec4(0,0,1,1),
		bInvM*vec4(0,1,0,1), bInvM*vec4(0,1,1,1),
		bInvM*vec4(1,0,0,1), bInvM*vec4(1,0,1,1),
		bInvM*vec4(1,1,0,1), bInvM*vec4(1,1,1,1),
	};

	*this = OBB(v,16);
}
////////////////////////////////////////////////////////////////////////////////
template<>
OBB::OBB(const AABB& aabb):m(mat3(1),vec3(0))
{
	const vec3 s = 1.0/(aabb.pmax-aabb.pmin);
	const vec3 t = aabb.pmin*s;
	m(0,0) = s.x;
	m(1,1) = s.y;
	m(2,2) = s.z;
	m(3) = -t;
}
////////////////////////////////////////////////////////////////////////////////
template<>
OBB::OBB(const Sphere& sphere):OBB(AABB(sphere)){}
////////////////////////////////////////////////////////////////////////////////
template<>
OBB::OBB(const Triangle& triangle):OBB((const vec3*)&triangle,3){}
////////////////////////////////////////////////////////////////////////////////
template<>
OBB::OBB(const Bezier& bezier)
{
	// TODO
}
////////////////////////////////////////////////////////////////////////////////
template<uint K>
OBB::OBB(const KDOP<K>& kdop)
{
	uint mi = 0;
	float sah = std::numeric_limits<float>::infinity();
	for(uint i=0; i<kdop.numMatrices; ++i)
	{
		const float sah2 = kdop.SAH(i);
		if(sah2 >= sah) continue;
		sah = sah2;
		mi  = i;
	}
	m = kdop.m(mi);
}
////////////////////////////////////////////////////////////////////////////////
