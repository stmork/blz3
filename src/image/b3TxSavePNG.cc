/*
**
**	$Filename:	b3TxSavePNG.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving PNG image
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3TxSaveInfo.h"

#ifdef HAVE_PNG_H

#include <png.h>

class b3InfoPNG : protected b3TxSaveInfo
{
	png_structp    png_ptr  = nullptr;
	png_infop      info_ptr = nullptr;

public:
	b3InfoPNG(b3Tx * tx, const char * filename);
	virtual ~b3InfoPNG();

	inline operator png_structp () const
	{
		return png_ptr;
	}

	inline operator png_infop() const
	{
		return info_ptr;
	}

	static inline void b3Convert(const b3_pkd_color & in, png_bytep out)
	{
		*out++ =  (in & 0x00ff0000) >> 16;
		*out++ =  (in & 0x0000ff00) >>  8;
		*out++ =  (in & 0x000000ff);
		*out++ = ((in & 0xff000000) >> 24) ^ 0xff;
	}

private:
	static void b3Write(
		png_structp png_ptr,
		png_bytep   buffer,
		png_size_t  size);

	static void b3Flush(png_structp png_ptr);
};

b3InfoPNG::b3InfoPNG(b3Tx * tx, const char * filename) :
	b3TxSaveInfo(tx, filename)
{
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (png_ptr != nullptr)
	{
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr != nullptr)
		{
			// Everything is fine.
			png_set_write_fn(png_ptr, this, b3InfoPNG::b3Write, b3InfoPNG::b3Flush);
			return;
		}
	}

	png_destroy_write_struct(&png_ptr, &info_ptr);
	b3PrintF(B3LOG_NORMAL, "IMG PNG  # Initialization failed!");
	B3_THROW(b3TxException, B3_TX_MEMORY);
}

b3InfoPNG::~b3InfoPNG()
{
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

void b3InfoPNG::b3Write(
	png_structp png_ptr,
	png_bytep   buffer,
	png_size_t  size)
{
	b3InfoPNG * png = (b3InfoPNG *)png_get_io_ptr(png_ptr);

	png->m_File.b3Write(buffer, size);
}

void b3InfoPNG::b3Flush(png_structp png_ptr)
{
	b3InfoPNG * png = (b3InfoPNG *)png_get_io_ptr(png_ptr);

	png->m_File.b3Flush();
}

b3_result b3Tx::b3SavePNG(const char * filename)
{
	b3InfoPNG png(this, filename);

	/* write header */
	if (setjmp(png_jmpbuf(png)))
	{
		b3PrintF(B3LOG_NORMAL, "IMG PNG  # Error during write image header!");
		B3_THROW(b3TxException, B3_TX_ERROR);
	}

	png_set_IHDR(png, png, xSize, ySize, 8,
		PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png, png);
	const size_t channels = png_get_channels(png, png);

	/* write bytes */
	if (setjmp(png_jmpbuf(png)))
	{
		b3PrintF(B3LOG_NORMAL, "IMG PNG  # Error during write image data!");
		B3_THROW(b3TxException, B3_TX_ERROR);
	}

	for (b3_res y = 0; y < ySize; y++)
	{
		b3_pkd_color row[xSize];
		png_byte     bytes[xSize * channels];
		png_bytep    ptr = bytes;

		b3GetRow(row, y);
		for (b3_res x = 0; x < xSize; x++)
		{
			png.b3Convert(row[x], ptr);
			ptr += channels;
		}
		png_write_row(png, bytes);
	}

	/* end write */
	if (setjmp(png_jmpbuf(png)))
	{
		b3PrintF(B3LOG_NORMAL, "IMG PNG  # Error during finalizing image data!");
		B3_THROW(b3TxException, B3_TX_ERROR);
	}

	png_write_end(png, png);
	return B3_OK;
}

#endif
