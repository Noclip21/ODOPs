#pragma once

#include <cgmath>

#include "Bitmap.h"
#include "constructors.inl"
#include "operators.inl"


////////////////////////////////////////////////////////////////////////////////
static const void convert(void* dest,const int fmt2,const void* src,const int fmt1)
{
	const int T1 = Bitmap::bytesPerChannel(fmt1);
	const int C1 = Bitmap::numChannels(fmt1);
	const int T2 = Bitmap::bytesPerChannel(fmt2);
	const int C2 = Bitmap::numChannels(fmt2);

	if(T1 == 1){
		if(C1 == 1){
			if(T2 == 1){
				     if(C2 == 2) *(rg8*    )dest = r8_rg8        (*(const r8*)src);
				else if(C2 == 3) *(rgb8*   )dest = r8_rgb8       (*(const r8*)src);
				else if(C2 == 4) *(rgba8*  )dest = r8_rgba8      (*(const r8*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = r8_r32f       (*(const r8*)src);
				else if(C2 == 2) *(rg32f*  )dest = r8_rg32f      (*(const r8*)src);
				else if(C2 == 3) *(rgb32f* )dest = r8_rgb32f     (*(const r8*)src);
				else if(C2 == 4) *(rgba32f*)dest = r8_rgba32f    (*(const r8*)src);
			}
		}else if(C1 == 2){
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = rg8_r8        (*(const rg8*)src);
				else if(C2 == 3) *(rgb8*   )dest = rg8_rgb8      (*(const rg8*)src);
				else if(C2 == 4) *(rgba8*  )dest = rg8_rgba8     (*(const rg8*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = rg8_r32f      (*(const rg8*)src);
				else if(C2 == 2) *(rg32f*  )dest = rg8_rg32f     (*(const rg8*)src);
				else if(C2 == 3) *(rgb32f* )dest = rg8_rgb32f    (*(const rg8*)src);
				else if(C2 == 4) *(rgba32f*)dest = rg8_rgba32f   (*(const rg8*)src);
			}
		}else if(C1 == 3){
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = rgb8_r8       (*(const rgb8*)src);
				else if(C2 == 2) *(rg8*    )dest = rgb8_rg8      (*(const rgb8*)src);
				else if(C2 == 4) *(rgba8*  )dest = rgb8_rgba8    (*(const rgb8*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = rgb8_r32f     (*(const rgb8*)src);
				else if(C2 == 2) *(rg32f*  )dest = rgb8_rg32f    (*(const rgb8*)src);
				else if(C2 == 3) *(rgb32f* )dest = rgb8_rgb32f   (*(const rgb8*)src);
				else if(C2 == 4) *(rgba32f*)dest = rgb8_rgba32f  (*(const rgb8*)src);
			}
		}else{
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = rgba8_r8      (*(const rgba8*)src);
				else if(C2 == 2) *(rg8*    )dest = rgba8_rg8     (*(const rgba8*)src);
				else if(C2 == 3) *(rgb8*   )dest = rgba8_rgb8    (*(const rgba8*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = rgba8_r32f    (*(const rgba8*)src);
				else if(C2 == 2) *(rg32f*  )dest = rgba8_rg32f   (*(const rgba8*)src);
				else if(C2 == 3) *(rgb32f* )dest = rgba8_rgb32f  (*(const rgba8*)src);
				else if(C2 == 4) *(rgba32f*)dest = rgba8_rgba32f (*(const rgba8*)src);
			}
		}
	}else{
		if(C1 == 1){
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = r32f_r8       (*(const r32f*)src);
				else if(C2 == 2) *(rg8*    )dest = r32f_rg8      (*(const r32f*)src);
				else if(C2 == 3) *(rgb8*   )dest = r32f_rgb8     (*(const r32f*)src);
				else if(C2 == 4) *(rgba8*  )dest = r32f_rgba8    (*(const r32f*)src);
			}else{
				     if(C2 == 2) *(rg32f*  )dest = r32f_rg32f    (*(const r32f*)src);
				else if(C2 == 3) *(rgb32f* )dest = r32f_rgb32f   (*(const r32f*)src);
				else if(C2 == 4) *(rgba32f*)dest = r32f_rgba32f  (*(const r32f*)src);
			}
		}else if(C1 == 2){
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = rg32f_r8      (*(const rg32f*)src);
				else if(C2 == 2) *(rg8*    )dest = rg32f_rg8     (*(const rg32f*)src);
				else if(C2 == 3) *(rgb8*   )dest = rg32f_rgb8    (*(const rg32f*)src);
				else if(C2 == 4) *(rgba8*  )dest = rg32f_rgba8   (*(const rg32f*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = rg32f_r32f    (*(const rg32f*)src);
				else if(C2 == 3) *(rgb32f* )dest = rg32f_rgb32f  (*(const rg32f*)src);
				else if(C2 == 4) *(rgba32f*)dest = rg32f_rgba32f (*(const rg32f*)src);
			}
		}else if(C1 == 3){
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = rgb32f_r8     (*(const rgb32f*)src);
				else if(C2 == 2) *(rg8*    )dest = rgb32f_rg8    (*(const rgb32f*)src);
				else if(C2 == 3) *(rgb8*   )dest = rgb32f_rgb8   (*(const rgb32f*)src);
				else if(C2 == 4) *(rgba8*  )dest = rgb32f_rgba8  (*(const rgb32f*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = rgb32f_r32f   (*(const rgb32f*)src);
				else if(C2 == 2) *(rg32f*  )dest = rgb32f_rg32f  (*(const rgb32f*)src);
				else if(C2 == 4) *(rgba32f*)dest = rgb32f_rgba32f(*(const rgb32f*)src);
			}
		}else{
			if(T2 == 1){
				     if(C2 == 1) *(r8*     )dest = rgba32f_r8    (*(const rgba32f*)src);
				else if(C2 == 2) *(rg8*    )dest = rgba32f_rg8   (*(const rgba32f*)src);
				else if(C2 == 3) *(rgb8*   )dest = rgba32f_rgb8  (*(const rgba32f*)src);
				else if(C2 == 4) *(rgba8*  )dest = rgba32f_rgba8 (*(const rgba32f*)src);
			}else{
				     if(C2 == 1) *(r32f*   )dest = rgba32f_r32f  (*(const rgba32f*)src);
				else if(C2 == 2) *(rg32f*  )dest = rgba32f_rg32f (*(const rgba32f*)src);
				else if(C2 == 3) *(rgb32f* )dest = rgba32f_rgb32f(*(const rgba32f*)src);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
const Bitmap Bitmap::convert(const int newFormat) const
{
	if(!newFormat || newFormat == format) return *this;

	Bitmap bitmap(width,height,newFormat);

	const uint n = width*height;
	for(uint i=0; i<n; ++i)
		::convert(bitmap[i],newFormat,(*this)[i],format);

	return bitmap;
}
