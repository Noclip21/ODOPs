#pragma once

#include "ShapeBase.h"
#include "OBB.h"


////////////////////////////////////////////////////////////////////////////////
template<uint K>
struct KDOP: ShapeBase,VolumeBase
{ 
	struct StaticData;
	
	static constexpr uint numMatrices = K*K*K;
	static constexpr uint numNormals  = 3*numMatrices;

	static const StaticData kn;

	float ko[2*numNormals];

	KDOP() = default;
	KDOP(const KDOP&) = default;
	KDOP(const vec3*,const uint);
	KDOP(const KDOP&,const KDOP&);
	KDOP(const AABB&);
	KDOP(const OBB&);
	KDOP(const Sphere&);
	KDOP(const Triangle&);
	KDOP(const Bezier&);
	template<uint N>
	KDOP(const Bezier&,const Polynomial<float,N>&);

	inline const mat3x4 m(const uint) const;

	inline const vec3 center() const;
	inline const float SAH(const uint) const;
	inline const float SAH() const;
};
////////////////////////////////////////////////////////////////////////////////
template<uint K>
struct KDOP<K>::StaticData
{
	const union{
		mat3 m[numMatrices];
		vec3 v[numNormals];
	};

	// https://en.wikipedia.org/wiki/Rotation_matrix#Skew_parameters_via_Cayley's_formula
	constexpr StaticData()
	{
		uint id = 0;
		for(float i=0; i<K; ++i)
		for(float j=0; j<K; ++j)
		for(float k=0; k<K; ++k)
		{
			float x = i/K, y = j/K, z = k/K;
			float xx = x*x, yy = y*y, zz = z*z,xy = x*y, xz = x*z, yz = y*z;

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
template<typename T>
constexpr bool is_KDOP(typename T::StaticData*){ return true; };
template<typename T>
constexpr bool is_KDOP(...){ return false; };
////////////////////////////////////////////////////////////////////////////////
