#pragma once

#include "KDOP.h"


struct OBB: public Volume
{
	mat3x4 m;

	OBB(){}

	template<uint K>
	OBB(const KDOP<K>& kdop)
	{
		uint mi = 0;
		sah = std::numeric_limits<float>::infinity();
		for(uint i=0; i<kdop.numMatrices; ++i)
		{
			const float sah2 = kdop.SAH(i);
			if(sah2 >= sah) continue;
			sah = sah2;
			mi  = i;
		}

		const float* ds = kdop.ko+6*mi;

		vec3 s(ds[1]-ds[0],ds[3]-ds[2],ds[5]-ds[4]);
		     s = 1/vec3(
				(abs(s.x) > 5e-5)?s.x:5e-5,
				(abs(s.y) > 5e-5)?s.y:5e-5,
				(abs(s.z) > 5e-5)?s.z:5e-5);

		const vec3 t(-ds[0],-ds[2],-ds[4]);

		mat3 R = transpose(kdop.kn.m[mi]);

		m = mat3x4(R(0)*s,R(1)*s,R(2)*s,t*s);
	}

	// TODO
	virtual void init(std::initializer_list<const vec3> ps){}
	virtual void init(const Volume& _a,const Volume& _b){}
	virtual float SAH() const { return sah; }
	virtual const vec3 center() const { return vec3(); }


	virtual ~OBB(){}
};
