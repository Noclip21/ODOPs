#pragma once

#include <limits>

#include "Volume.h"


template<uint K>
struct KDOP: public Volume
{
	struct StaticData;

	static constexpr uint numMatrices = K*K*K;
	static constexpr uint numNormals  = 3*numMatrices;

	static const StaticData kn;

	float ko[2*numNormals];

	virtual void init(std::initializer_list<const vec3> ps)
	{
		for(uint i=0; i<2*numNormals; i+=2)
		{
			ko[i  ] = +std::numeric_limits<float>::infinity();
			ko[i+1] = -std::numeric_limits<float>::infinity();
			for(auto& p: ps)
			{
				float d = dot(kn.v[i/2],p);
				ko[i  ] = min(ko[i  ],d);
				ko[i+1] = max(ko[i+1],d);
			}
		}
		sah = SAH();
	}
	virtual void init(const Volume& _a,const Volume& _b)
	{
		auto a = *static_cast<const KDOP*>(&_a);
		auto b = *static_cast<const KDOP*>(&_b);
		for(uint i=0; i<2*numNormals;)
		{
			ko[i++] = min(a.ko[i],b.ko[i]);
			ko[i++] = max(a.ko[i],b.ko[i]);
		}
		sah = SAH();
	};
	float SAH(const int i) const
	{
		const float* ds = ko+6*i;
		const vec3 v(ds[1]-ds[0],ds[3]-ds[2],ds[5]-ds[4]);
		return v.x*v.y + v.x*v.z + v.y*v.z;
	}
	virtual float SAH() const
	{
		float s = std::numeric_limits<float>::infinity();
		for(uint i=0; i<numMatrices; ++i)
			s = min(s,SAH(i));
		return s;
	}
	virtual const vec3 center() const
	{
		uint mi = 0;
		for(uint i=0; i<numMatrices; ++i)
		{
			const float sah2 = SAH(i);
			if(sah2 > sah) continue;
			mi = i;
		}
		return
			 ((ko[6*mi  ]+ko[6*mi+1])*0.5)*kn.m[mi](0)
			+((ko[6*mi+2]+ko[6*mi+3])*0.5)*kn.m[mi](1)
			+((ko[6*mi+4]+ko[6*mi+5])*0.5)*kn.m[mi](2);
	}

	virtual ~KDOP(){}
};
////////////////////////////////////////////////////////////////////////////////
template<uint K>
struct KDOP<K>::StaticData
{
	const union{
		mat3 m[numMatrices];
		vec3 v[numNormals];
	};

	constexpr StaticData()
	{
		uint id = 0;
		for(float i=0; i<K; ++i)
			for(float j=0; j<K; ++j)
				for(float k=0; k<K; ++k)
				{
					const float x  = i/K, y  = j/K, z  = k/K,
					            xx = x*x, yy = y*y, zz = z*z,
						        xy = x*y, xz = x*z, yz = y*z;
					m[id++] = mat3(
						1+xx-yy-zz,   2*xy+2*z,   2*xz-2*y,
  						2*xy-2*z,   1-xx+yy-zz,   2*x+2*yz,
  						2*y+2*xz,     2*yz-2*x, 1-xx-yy+zz)/(1+xx+yy+zz);
				}
	}
};
////////////////////////////////////////////////////////////////////////////////
template<uint K> const typename KDOP<K>::StaticData KDOP<K>::kn = KDOP<K>::StaticData();
////////////////////////////////////////////////////////////////////////////////
