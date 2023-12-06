#ifndef PIXEL_CONSTRUCTORS_H
#define PIXEL_CONSTRUCTORS_H

#include "Pixel.h"


////////////////////////////////////////////////////////////////////////////////
pixel::Pixel(const Bitmap::Format format):format(format){}
////////////////////////////////////////////////////////////////////////////////
pixel::Pixel(const void* data,const uint format):Pixel((const Bitmap::Format)format)
{
	switch(Bitmap::bytesPerChannel(format))
	{
		case(sizeof(uint8_t)):
			for(uint i=0; i<Bitmap::numChannels(format); ++i)
				data8[i] = ((const uint8_t*)data)[i];
			break;
		case(sizeof(float)):
			for(uint i=0; i<Bitmap::numChannels(format); ++i)
				data32f[i] = ((const float*)data)[i];
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
pixel::Pixel(const Pixel& p):Pixel(&p,p.format){}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint c>
pixel::Pixel(const Pixel<T,c>& p):Pixel(&p,Bitmap::genFormat(sizeof(T),c)){}
////////////////////////////////////////////////////////////////////////////////

#endif // PIXEL_CONSTRUCTORS_H
