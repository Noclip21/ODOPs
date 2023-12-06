#pragma once

#include <cgmath>

#include "../file/File.h"

#include <FreeImage.h>


struct Bitmap: public File
{
	enum Format
	{
		// field: bytes    channels   type = unsigned byte / float
		// mask:  0x0000ff 0x00ff00   0xff0000
		R8      = 1  | (1 << 8) | (1 << 16),
		RG8     = 2  | (2 << 8) | (1 << 16),
		RGB8    = 3  | (3 << 8) | (1 << 16),
		RGBA8   = 4  | (4 << 8) | (1 << 16),
		R32F    = 4  | (1 << 8) | (4 << 16),
		RG32F   = 8  | (2 << 8) | (4 << 16),
		RGB32F  = 12 | (3 << 8) | (4 << 16),
		RGBA32F = 16 | (4 << 8) | (4 << 16),
	};


	int width;
	int height;
	int format;


	inline static constexpr Format genFormat(const int bytesPerChannel,const int numChannels)
	{ 
		return (const Format)
			((bytesPerChannel*numChannels & 0x0000ff) | 
			((numChannels << 8) & 0x00ff00) | 
			((bytesPerChannel << 16) & 0xff0000));
	}

	inline static constexpr int bytesPerPixel(const int f)   { return (f & 0x0000ff);       }
	inline static constexpr int numChannels(const int f)     { return (f & 0x00ff00) >>  8; }
	inline static constexpr int bytesPerChannel(const int f) { return (f & 0xff0000) >> 16; }

	inline const int bytesPerPixel() const   { return bytesPerPixel(format);   }
	inline const int numChannels() const     { return numChannels(format);     }
	inline const int bytesPerChannel() const { return bytesPerChannel(format); }


	inline Bitmap();
	inline Bitmap(const Bitmap&);
	inline Bitmap(const int,const int,const int);
	inline Bitmap(const void*,const int,const int,const int);
	inline Bitmap(const char*,const int = 0);


	inline Bitmap& operator = (const Bitmap&);

	inline void* operator [](const uint);
	inline void* operator ()(const uint,const uint);

	inline const void* operator [](const uint) const;
	inline const void* operator ()(const uint,const uint) const;

	inline void operator ()(const void*,const uint);
	inline void operator ()(const void*,const uint,const uint);


	inline const Bitmap convert(const int) const;


	inline static bool write(const char*,const void*,const int,const int,const int,const int = 0);
	inline bool write(const char*,const int = 0) const;


	enum FileFormat
	{
		BMP    = FIF_BMP,
		ICO    = FIF_ICO,
		JPEG   = FIF_JPEG,
		JNG    = FIF_JNG,
		KOALA  = FIF_KOALA,
		LBM    = FIF_LBM,
		IFF    = FIF_IFF,
		MNG    = FIF_MNG,
		PBM    = FIF_PBM,
		PBMRAW = FIF_PBMRAW,
		PCD    = FIF_PCD,
		PCX    = FIF_PCX,
		PGM    = FIF_PGM,
		PGMRAW = FIF_PGMRAW,
		PNG    = FIF_PNG,
		PPM    = FIF_PPM,
		PPMRAW = FIF_PPMRAW,
		RAS    = FIF_RAS,
		TARGA  = FIF_TARGA,
		TIFF   = FIF_TIFF,
		WBMP   = FIF_WBMP,
		PSD    = FIF_PSD,
		CUT    = FIF_CUT,
		XBM    = FIF_XBM,
		XPM    = FIF_XPM,
		DDS    = FIF_DDS,
		GIF    = FIF_GIF,
		HDR    = FIF_HDR,
		FAXG3  = FIF_FAXG3,
		SGI    = FIF_SGI,
		EXR    = FIF_EXR,
		J2K    = FIF_J2K,
		JP2    = FIF_JP2,
		PFM    = FIF_PFM,
		PICT   = FIF_PICT,
		RAW    = FIF_RAW
	};
};
