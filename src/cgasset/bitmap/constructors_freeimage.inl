#pragma once

#include <algorithm>
#include <iostream>
#include <cstring>
#include <map>

#include "Bitmap.h"

#include "../file/constructors.inl"


////////////////////////////////////////////////////////////////////////////////
Bitmap::Bitmap():File(),width(0),height(0),format(0){}
////////////////////////////////////////////////////////////////////////////////
Bitmap::Bitmap(const Bitmap& obj):Bitmap(obj.data,obj.width,obj.height,obj.format){}
////////////////////////////////////////////////////////////////////////////////
Bitmap::Bitmap(const int w,const int h,const int f):
	File(w*h*(f & 0xff)),width(w),height(h),format(f){}
////////////////////////////////////////////////////////////////////////////////
Bitmap::Bitmap(const void* d,const int w,const int h,const int f):
	File(d,w*h*(f & 0xff)),width(w),height(h),format(f){}
////////////////////////////////////////////////////////////////////////////////
Bitmap::Bitmap(const char* filepath,const int convertFormat):Bitmap()
{
	static bool initlib = false;
	if(!initlib)
	{
		FreeImage_Initialise(true);
		initlib = true;
	}

	File file(filepath);

	if(!file) return;

	FIMEMORY* fiBuffer = FreeImage_OpenMemory((BYTE*)file.data,file.size);
	FIBITMAP* dib = FreeImage_LoadFromMemory(FreeImage_GetFileTypeFromMemory(fiBuffer,0),fiBuffer);
	FreeImage_CloseMemory(fiBuffer);

	if(!dib)
	{
		if(File::verbose) std::cout << "error parsing file" << std::endl;
		return;
	}

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	int fiFormat = FreeImage_GetColorType(dib);
	int fiType = FreeImage_GetImageType(dib);

	static const int fiFormatDefault = FIC_RGB;
	static const int fiTypeDefault   = FIT_BITMAP;
	static std::map<int,std::pair<int,int>> formatMap =
	{
		{Format::R8,      {FIC_MINISBLACK, FIT_BITMAP}},
		{Format::RGB8,    {FIC_RGB,        FIT_BITMAP}},
		{Format::RGBA8,   {FIC_RGBALPHA,   FIT_BITMAP}},
		{Format::R32F,    {FIC_MINISBLACK, FIT_FLOAT }},
		{Format::RGB32F,  {FIC_RGB,        FIT_RGBF  }},
		{Format::RGBA32F, {FIC_RGBALPHA,   FIT_RGBAF }}
	};
	static std::map<std::pair<int,int>,int> formatMapInv =
	{
		{{FIC_MINISWHITE, FIT_BITMAP}, Format::R8      },
		{{FIC_MINISBLACK, FIT_BITMAP}, Format::R8      },
		{{FIC_RGB,        FIT_BITMAP}, Format::RGB8    },
		{{FIC_RGBALPHA,   FIT_BITMAP}, Format::RGBA8   },
		{{FIC_MINISWHITE, FIT_FLOAT }, Format::R32F    },
		{{FIC_MINISBLACK, FIT_FLOAT }, Format::R32F    },
		{{FIC_RGB,        FIT_RGBF  }, Format::RGB32F  },
		{{FIC_RGBALPHA,   FIT_RGBAF }, Format::RGBA32F }
	};

	std::pair<int,int> p = formatMap[
		(convertFormat)?convertFormat:formatMapInv[{fiFormat,fiType}]];
	fiFormat = (p.first)?p.first:fiFormatDefault;
	fiType   = (p.second)?p.second:fiTypeDefault;
	format   = formatMapInv[{fiFormat,fiType}];

	size = width*height*bytesPerPixel();
	data = new char[size];
	if(convertFormat)
	{
		auto* tmp = dib;
		dib = FreeImage_ConvertToType(tmp,(FREE_IMAGE_TYPE)fiType);
		FreeImage_Unload(tmp);
	}
	FreeImage_ConvertToRawBits((BYTE*)data,dib,
		width*bytesPerPixel(),bytesPerPixel() << 3,
		FI_RGBA_RED_MASK,FI_RGBA_GREEN_MASK,FI_RGBA_BLUE_MASK,
		true);

	FreeImage_Unload(dib);

	// Convert BGR[A] to RGB[A]
	if(format == Format::RGB8 || format == Format::RGBA8)
		for(int i=0; i<width*height; ++i)
		{
			uint8_t* p = ((uint8_t*)data)+i*bytesPerPixel();
			std::swap(p[0],p[2]);
		}
}
////////////////////////////////////////////////////////////////////////////////
