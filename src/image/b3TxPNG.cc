/*
**
**	$Filename:	b3TxPNG.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading PNG images
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        handle PNG's                                  **
**                                                                      **
*************************************************************************/

#ifdef HAVE_PNG_H

#include <png.h>
#include <setjmp.h>

/*************************************************************************
**                                                                      **
**                        b3PNG class definition                        **
**                                                                      **
*************************************************************************/

class b3PNG
{
	png_structp    png_ptr  = nullptr;
	png_infop      info_ptr = nullptr;

	const b3_u08 * m_Buffer;
	const b3_size  m_Size;
	b3_size        m_Index = 0;

public:
	b3PNG(const b3_u08 * buffer, const b3_size size);
	~b3PNG();

	inline operator png_structp () const
	{
		return png_ptr;
	}

	inline operator png_infop() const
	{
		return info_ptr;
	}

private:
	static void b3Read(
		png_structp png_ptr,
		png_bytep   outBytes,
		png_size_t  byteCountToRead);
};

b3PNG::b3PNG(const b3_u08 * buffer, const b3_size size) :
	m_Buffer(buffer), m_Size(size)
{
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (png_ptr != nullptr)
	{
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr != nullptr)
		{
			// Everything is fine.
			png_set_read_fn(png_ptr, this, b3PNG::b3Read);
			return;
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	b3PrintF(B3LOG_NORMAL, "IMG PNG  # Initialization failed!");
	B3_THROW(b3TxException, B3_TX_MEMORY);
}

b3PNG::~b3PNG()
{
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

void b3PNG::b3Read(
	png_structp png_ptr,
	png_bytep   outBytes,
	png_size_t  byteCountToRead)
{
	b3PNG * png = (b3PNG *)png_get_io_ptr(png_ptr);

	const b3_size size = std::min(byteCountToRead, png->m_Size - png->m_Index);
	memcpy(outBytes, &png->m_Buffer[png->m_Index], size);
	png->m_Index += size;
}

/*************************************************************************
**                                                                      **
**                        PNG parsing                                   **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParsePNG(const b3_u08 * buffer, b3_size buffer_size)
{
	b3PNG png(buffer, buffer_size);

	b3PrintF(B3LOG_FULL, "IMG PNG  # b3ParsePNG(%s)\n",
		(const char *)image_name);

	if (setjmp(png_jmpbuf(png)))
	{
		b3PrintF(B3LOG_NORMAL, "IMG PNG  # Error during init_io!");
		B3_THROW(b3TxException, B3_TX_ERR_HEADER);
	}

	png_read_info(png, png);
	png_set_strip_16(png);
	png_set_interlace_handling(png);

	const png_byte    color_type  = png_get_color_type(png, png);
	const png_uint_32 bytesPerRow = png_get_rowbytes(png, png);

	xSize = png_get_image_width(png, png);
	ySize = png_get_image_height(png, png);
	depth = png_get_bit_depth(png, png);
	png_read_update_info(png, png);

	/* read file */
	if (setjmp(png_jmpbuf(png)))
	{
		b3PrintF(B3LOG_NORMAL, "IMG PNG  # Error during read image!");
		B3_THROW(b3TxException, B3_TX_ERROR);
	}

	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		if (b3AllocTx(xSize, ySize, 8))
		{
			b3_u08 * row = data;

			for (b3_res y = 0; y < ySize; y++)
			{
				png_read_row(png, row, nullptr);
				row += xSize;
			}
		}
		break;

	case PNG_COLOR_TYPE_PALETTE:
		if (b3AllocTx(xSize, ySize, 8))
		{
			png_colorp png_palette = nullptr;
			int        count       = 0;
			b3_u08  *  row         = data;

			png_get_PLTE(png, png, &png_palette, &count);
			for (int p = 0; p < count; p++)
			{
				palette[p] = b3Color::b3MakePkdColor(
						png_palette[p].red,
						png_palette[p].green,
						png_palette[p].blue);
			}

			for (b3_res y = 0; y < ySize; y++)
			{
				png_read_row(png, row, nullptr);
				row += xSize;
			}
		}
		break;

	case PNG_COLOR_TYPE_RGB:
		if (b3AllocTx(xSize, ySize, 24))
		{
			b3_pkd_color * ptr = data;

			for (b3_res y = 0; y < ySize; y++)
			{
				b3_u08   row[bytesPerRow];
				b3_u08 * cPtr = row;

				png_read_row(png, row, nullptr);
				for (b3_res x = 0; x < xSize ; x++)
				{
					*ptr++ = b3Color::b3MakePkdColor(
							cPtr[0], cPtr[1], cPtr[2]);
					cPtr += 3;
				}
			}
		}
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		if (b3AllocTx(xSize, ySize, 32))
		{
			b3_pkd_color * ptr = data;

			for (b3_res y = 0; y < ySize; y++)
			{
				b3_u08   row[bytesPerRow];
				b3_u08 * cPtr = row;

				png_read_row(png, row, nullptr);
				for (b3_res x = 0; x < xSize ; x++)
				{
					*ptr++ = b3Color::b3MakePkdColor(
							cPtr[0], cPtr[1], cPtr[2], cPtr[3] ^ 0xff);
					cPtr += 4;
				}
			}
		}
		break;

	default:
		b3PrintF(B3LOG_NORMAL, "IMG PNG  # Unsupported image format!");
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
	return B3_OK;
}

#else

b3_result b3Tx::b3ParsePNG(const b3_u08 * buffer, b3_size buffer_size)
{
	b3FreeTx();
	b3PrintF(B3LOG_NORMAL, "IMG PNG  # Missing PNG support:\n");
	B3_THROW(b3TxException, B3_TX_UNSUPP);
}

#endif
