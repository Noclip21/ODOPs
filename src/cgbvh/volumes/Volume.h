#pragma once

#include <cgmath>


struct Volume
{
	float sah;

	Volume():sah(std::numeric_limits<float>::infinity()){}
	
	virtual void init(std::initializer_list<const vec3>) = 0;
	virtual void init(const Volume&,const Volume&) = 0;
	virtual float SAH() const = 0;
	virtual const vec3 center() const = 0;

	friend inline bool operator > (const Volume& a,const Volume& b)
	{ return (a.sah > b.sah); }
};
