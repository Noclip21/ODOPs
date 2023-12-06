#pragma once

#include "../texture/operators.inl"


// https://download.nvidia.com/developer/Papers/2005/NP2_Mipmapping/NP2_Mipmap_Creation.pdf
template<typename T>
const Texture<T> downscale(const Texture<T>& tex0,const int w1,const int h1)
{
	Texture<T> tex1(w1,h1);

	const int w0 = tex0.width, h0 = tex0.height;

	const float wRatio    = float(w0)/w1,
			    hRatio    = float(h0)/h1,
	            wRatioInv = float(w1)/w0,
			    hRatioInv = float(h1)/h0;

	const int wRatioUp = (w0+w1-1)/w1,
	          hRatioUp = (h0+h1-1)/h1;

	for(int i1=0; i1<w1; ++i1)
	for(int j1=0; j1<h1; ++j1)
	{
		const int i0Offset = i1*wRatio,
		          j0Offset = j1*hRatio;
		T& p = tex1(i1,j1) = 0;
		for(int i0=0; i0<wRatioUp; ++i0)
		for(int j0=0; j0<hRatioUp; ++j0)
		{
			const float w = 
				 (std::fminf(i0+1,wRatio)-i0)*wRatioInv
				*(std::fminf(j0+1,hRatio)-j0)*hRatioInv;
			p = p + w*tex0(i0Offset+i0,j0Offset+j0);
		}
	}

	return tex1;

	/*
	// Map coordinates u,v = map from (w,h) to unit interval
	// Precompute step
	float su1 = 1.0f/w1, sv1 = 1.0f/h1;
	// Iterate on output
	for(float u1=0; u1<1; u1 += su1)
	for(float v1=0; v1<1; v1 += sv1)
	{
		// Compute interval scaled to original image size
		const float i0Start =  u1     *w0, j0Start =  v1     *h0,
			        i0End   = (u1+su1)*w0, j0End   = (v1+sv1)*h0;

		// Precompute interval length
		const float i0Len = 1.0f/std::fmaxf(1,i0End-i0Start),
			        j0Len = 1.0f/std::fmaxf(1,j0End-j0Start);

		// Iterate on samples
		T p(0);
		for(float i0f=i0Start; i0f<std::ceil(i0End); ++i0f)
		for(float j0f=j0Start; j0f<std::ceil(j0End); ++j0f)
		{
			const int i0 = std::floor(i0f), j0 = std::floor(j0f);
			// Compute pixel weight
			const float w = 
				 (std::fminf(i0+1,i0End)-std::fmaxf(i0Start,i0))*i0Len
				*(std::fminf(j0+1,j0End)-std::fmaxf(j0Start,j0))*j0Len;
			p = p + w*data0[w0*j0+i0];
		}

		// Map back to (w,h)
		const int i1 = u1*w1, j1 = v1*h1;
		data1[w1*j1+i1] = p;
	}
	*/
}
