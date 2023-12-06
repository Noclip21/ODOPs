#pragma once

#include <cmath>

#include "trilinear.inl"


// https://registry.khronos.org/OpenGL/extensions/EXT/EXT_texture_filter_anisotropic.txt
// alternative: https://www.shadertoy.com/view/4lXfzn
template<Wrap W,typename T>
inline const T anisotropic(
	const TextureLod<T>& tex,
	const float u,const float v,
	float udx,float vdx,
	float udy,float vdy)
{
#if 1

	const float pu = std::sqrt(udx*udx + vdx*vdx),
	            pv = std::sqrt(udy*udy + vdy*vdy);
	const float pMax = std::max(pu,pv),
	            pMin = std::min(pu,pv);
	const float n = std::min(std::ceil(pMax/pMin),tex.maxAniso);
	const float s = std::log2(pMax/n);
	
	const float dt = 1.0f/n;

	udx *= dt; vdx *= dt;
	udy *= dt; vdy *= dt;

	T p(0);
	if(pu > pv)
    	for(int i=0; i<n; ++i)
			p = p + trilinear<W,T>(tex,u+i*udx,v+i*vdx,s);
	else
    	for(int i=0; i<n; ++i)
			p = p + trilinear<W,T>(tex,u+i*udy,v+i*vdy,s);
	return p*dt;

#else

		// Alternatice
	mat2 J = inverse(mat2(vec2(idx,jdx),vec2(idy,jdy)));
    J = outerProduct(J,J);
    
    float det = determinant(J),
          trace = J(0,0)+J(1,1);
    det = sqrt(abs(trace*trace-4*det));
    float e1 = (trace-det)*0.5,
          e2 = (trace+det)*0.5;
    float k = log2(5e-3f+max(width,height)/sqrt(e2));

    if(e1 >= 1) return (*this)(i,j,k);
	vec2 v = normalize(vec2(-J(0,1),J(0,0)-e1))/sqrt(e1);

	pixel p = 0;
	float w = 1.0f/max(sqrt(e1),maxAniso);
    for(float t=-0.5; t<0.5; t += w)
		p = p + w*(*this)(i+t*v.x,j+t*v.y,k);

	return p;

#endif
}
