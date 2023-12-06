#pragma once

#include <algorithm>
#include <cstring>
#include <map>

#include "Bitmap.h"
#include "convert.inl"


////////////////////////////////////////////////////////////////////////////////
bool Bitmap::write(
	const char* filepath,
	const void* data,
	const int   width,
	const int   height,
	const int   format,
	const int   convertFormat)
{
	return Bitmap(data,width,height,format).write(filepath,convertFormat);
}
////////////////////////////////////////////////////////////////////////////////
bool Bitmap::write(const char* filepath,const int convertFormat) const
{ 
	static std::map<int,std::pair<int,int>> formatMap =
	{
		{Format::R8,      {FIC_MINISBLACK, FIT_BITMAP}},
		{Format::RGB8,    {FIC_RGB,        FIT_BITMAP}},
		{Format::RGBA8,   {FIC_RGBALPHA,   FIT_BITMAP}},
		{Format::R32F,    {FIC_MINISBLACK, FIT_FLOAT }},
		{Format::RGB32F,  {FIC_RGB,        FIT_RGBF  }},
		{Format::RGBA32F, {FIC_RGBALPHA,   FIT_RGBAF }}
	};

	Bitmap img = *this;
	if(convertFormat)
		img = img.convert(convertFormat);
	
	// Convert RGB[A] to BGR[A]
	if(img.format == Format::RGB8 || img.format == Format::RGBA8)
		for(int i=0; i<img.width*img.height; ++i)
		{
			uint8_t* p = ((uint8_t*)img.data)+i*img.bytesPerPixel();
			std::swap(p[0],p[2]);
		}

	FIBITMAP* dib = FreeImage_ConvertFromRawBitsEx(true,
		(BYTE*)img.data,
		(FREE_IMAGE_TYPE)formatMap[img.format].second,
		img.width,img.height,
		width*img.bytesPerPixel(),img.bytesPerPixel() << 3,
		FI_RGBA_RED_MASK,FI_RGBA_GREEN_MASK,FI_RGBA_BLUE_MASK,
		true);

	if(!dib) return false;

	FIMEMORY* fiBuffer = FreeImage_OpenMemory();
	bool ret = FreeImage_SaveToMemory(FreeImage_GetFIFFromFilename(filepath),dib,fiBuffer,0);

	BYTE* buffer;
	uint32_t fileSize;
	FreeImage_AcquireMemory(fiBuffer,&buffer,&fileSize);

	ret = ret && File::write(filepath,buffer,fileSize);

	FreeImage_CloseMemory(fiBuffer);

	FreeImage_Unload(dib);

	return ret;
}
////////////////////////////////////////////////////////////////////////////////
