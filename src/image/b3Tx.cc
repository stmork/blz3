/*
**
**	$Filename:	b3Tx.cc $
**	$Release:	Dortmund 2001 - 2022 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines
**
**	(C) Copyright 2001 - 2024  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include <blz3/base/b3Color.h>
#include <blz3/base/b3Math.h>
#include "blz3/image/b3Tx.h"

#include "b3TxAlgorithms.h"

/*************************************************************************
**                                                                      **
**                        some constant tables                          **
**                                                                      **
*************************************************************************/

const b3_u08 b3Tx::m_Bits[8] =
{
	128, 64, 32, 16, 8, 4, 2, 1
};

const b3Color b3Tx::m_RgbEyeStimulus
{
	0.35, 0.51, 0.14, 0.0
};

bool b3Tx::m_ErrorHandlerInstalled = false;

/*************************************************************************
**                                                                      **
**                        TIFF error/warning handler                    **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBTIFF
void b3Tx::b3TIFFErrorHandler(
	const char * module,
	const char * fmt,
	va_list     args)
{
	char message[512];

	vsnprintf(message, sizeof(message), fmt, args);
	b3PrintF(B3LOG_NORMAL, "ERROR: %s %s\n", module, message);
}

void b3Tx::b3TIFFWarnHandler(
	const char * module,
	const char * fmt,
	va_list     args)
{
	char message[512];

	vsnprintf(message, sizeof(message), fmt, args);
	b3PrintF(B3LOG_NORMAL, "WARNING: %s %s\n", module, message);
}
#endif

/*************************************************************************
**                                                                      **
**                        b3Tx implementation                           **
**                                                                      **
*************************************************************************/

b3Tx::b3Tx() : b3Link<b3Tx>(sizeof(b3Tx), USUAL_TEXTURE)
{
	image_name.b3Empty();

	// set TIFF error und warning handler
	if (!m_ErrorHandlerInstalled)
	{
#ifdef HAVE_LIBTIFF
		TIFFSetErrorHandler(b3TIFFErrorHandler);
		TIFFSetWarningHandler(b3TIFFWarnHandler);
#endif
		m_ErrorHandlerInstalled = true;
	}
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx instantiated (%p)\n", this);
#endif
}

b3Tx::b3Tx(const b3Tx * orig) : b3Link<b3Tx>(sizeof(b3Tx), USUAL_TEXTURE)
{
	image_name.b3Empty();
	b3Copy(orig);
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx instantiated (%ldx%ld, %ld bits, type=%d) (%p)\n",
		xSize, ySize, depth, type, this);
#endif
}

b3Tx::~b3Tx()
{
#ifdef VERBOSE
	b3Dump();
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx destroyed (%p)\n", this);
#endif
}

bool b3Tx::b3AllocTx(
	b3_res x,
	b3_res y,
	b3_res d)
{
	void * new_ptr;

	if ((d <= 0) || (x <= 0) || (y <= 0))
	{
		// This meens, we need an empty image, so
		// free memory and nothing is OK!
		b3FreeTx();
		return false;
	}

	b3EndHist();

	if (d == 1)
	{
		dSize = TX_BWA(x) * y;
		pSize = 2;
		type  = B3_TX_ILBM;
	}
	if ((d > 1) && (d <= 8))
	{
		dSize =  x * y;
		pSize = 1 << d;
		type  = B3_TX_VGA;
	}
	if ((d == 12) || (d == 16))
	{
		dSize = (x * y) * sizeof(b3_u16);
		type  = B3_TX_RGB4;
	}
	if ((d == 24) || (d == 32))
	{
		dSize = (x * y) * sizeof(b3_pkd_color);
		type  = B3_TX_RGB8;
	}
	if ((d == 48) || (d == 64) || // half precision
		(d == 96) || (d == 128)) // single precision
	{
		dSize = (x * y) * sizeof(b3_color);
		type  = B3_TX_FLOAT;
	}

	if (type == B3_TX_UNDEFINED)
	{
		b3FreeTx();
		return false;
	}

	B3_ASSERT(dSize > 0);

	new_ptr = b3Realloc(data, dSize);
	if (new_ptr == nullptr)
	{
		b3FreeTx();
		return false;
	}
	data = new_ptr;

	if (pSize > 0)
	{
		palette = b3TypedRealloc<b3_pkd_color>(palette, pSize);
	}

	// setting size values
	xSize  = x;
	ySize  = y;
	depth  = d;

	// initializing default palettes
	if (depth == 1)
	{
		palette[0] = 0x00ffffff;
		palette[1] = 0x00000000;
	}

	// Initialize grey ramp
	if (depth == 8)
	{
		b3_loop i;

		for (i = 0; i < 256; i++)
		{
			palette[i] = 0x010101 * i;
		}
	}
	FileType  = FT_UNKNOWN;
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3AllocTx(%dx%d, %d bits) this=%p type=%d, %zu bytes)\n",
		xSize, ySize, depth, this, type, dSize);
	return true;
}

void b3Tx::b3FreeTx()
{
	if (data != nullptr)
	{
		b3Free(data);
		data  = nullptr;
		dSize = 0;
	}
	if (palette != nullptr)
	{
		try
		{
			b3Free(palette);
		}
		catch (...)
		{
			b3PrintF(B3LOG_DEBUG, "### CLASS: b3Tx   # b3Tx() Oops?\n");
			b3PrintF(B3LOG_DEBUG, "### CLASS: b3Tx   # b3Tx() %s %d\n", __FILE__, __LINE__);
		}
		palette = nullptr;
		pSize   = 0;
	}
	type      = B3_TX_UNDEFINED;
	FileType  = FT_UNKNOWN;
	xSize     = 0;
	ySize     = 0;
	depth     = 0;
	ScanLines = 0;
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3FreeTx()\n");
}

void b3Tx::b3TestPattern()
{
	b3Color       row[xSize];
	const b3_res  width = xSize >> 3;

	for (b3_res x = 0; x < xSize; x++)
	{
		b3_f32   r, g, b, value;
		b3_index idx = x / width;

		value = (b3_f32)(x % width) / width;
		b = (idx & 1) ? value : 0.0;
		r = (idx & 2) ? value : 0.0;
		g = (idx & 4) ? value : 0.0;
		row[x].b3Init(r, g, b);
	}

	if ((palette != nullptr) && (depth > 1))
	{
		for (b3_index i = 0; i < (1 << depth); i++)
		{
			palette[i] = b3Tx::b3IndexToColor(i);
		}
	}

	switch (type)
	{
	case B3_TX_FLOAT:
		b3TxAlgorithms::b3SetRow<b3_color>(row, this);
		break;

	case B3_TX_RGB8:
		b3TxAlgorithms::b3SetRow<b3_pkd_color>(row, this);
		break;

	case B3_TX_RGB4:
		b3TxAlgorithms::b3SetRow<b3_u16>(row, this);
		break;

	case B3_TX_VGA:
		b3TxAlgorithms::b3SetRow<b3_u08>(row, this, &b3Tx::b3ColorToIndex);
		break;

	case B3_TX_ILBM:
		if (depth == 1)
		{
			uint8_t * ptr = data;

			for (b3_res y = 0; y < ySize; y++)
			{
				for (b3_res x = 0; x < xSize; x += 16)
				{
					*ptr++ = (y & 2) ? 0xcc : 0x33;
					*ptr++ = (y & 2) ? 0x00 : 0xff;
				}
			}
		}
		else
		{
			B3_THROW(b3TxException, B3_TX_UNSUPP);
		}
		break;

	default:
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
}

bool b3Tx::b3IsGreyPalette() const noexcept
{
	b3_loop      i, max;
	b3_pkd_color mul, step;

	if (type != B3_TX_VGA)
	{
		return false;
	}

	if (depth > 8)
	{
		return false;
	}

	max  =   1 << depth;
	step = 256 >> depth;
	for (i = 0, mul = 0; i < max; i++, mul += step)
	{
		if ((0x010101 * mul) != palette[i])
		{
			return false;
		}
	}
	return true;
}

inline void b3Tx::b3CopyILBMtoVGA(
	b3_u08  * row,
	b3_coord  y) const
{
	b3_u08    dstBit = 1, Bit;
	b3_coord  x, d;
	b3_count  xBytes, xRest, index, BytesPerLine;

	BytesPerLine = TX_BWA(xSize);
	if ((palette != nullptr) && (depth < 8))
	{
		b3_u08 * Data   = &data[y * BytesPerLine * depth];

		xBytes = xSize >> 3;
		xRest  = xSize  & 7;
		for (d = 0; d < depth; d++)
		{
			index = 0;

			// Check complete source bytes
			for (x = 0; x < xBytes; x++)
			{
				if (Data[x] != 0)
				{
					if (Data[x] & 0x80)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x40)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x20)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x10)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x08)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x04)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x02)
					{
						row[index] |= dstBit;
					}
					index++;
					if (Data[x] & 0x01)
					{
						row[index] |= dstBit;
					}
					index++;
				}
				else
				{
					index += 8;
				}
			}

			// Compute rest bits of last byte
			for (x = 0; x < xRest; x++)
			{
				Bit = 128;
				if (Data[xBytes] & Bit)
				{
					row[index] |= dstBit;
				}
				index++;
			}

			// Do we have reached end of line correctly?
			B3_ASSERT(index <= xSize);

			// Bump indices
			dstBit <<= 1;
			Data    += BytesPerLine;
		}
	}
}

inline void b3Tx::b3CopyILBMtoRGB8(
	b3_pkd_color * row,
	b3_coord       y) const
{
	b3_coord      x, d, BytesPerLine;
	b3_pkd_color  Color, Bit;

	BytesPerLine = TX_BWA(xSize);
	if (palette == nullptr)
	{
		for (x = 0; x < xSize; x++)
		{
			b3_u08 * Data   = data;

			Data  += ((y + 1) * BytesPerLine * depth + (x >> 3) - BytesPerLine);
			Color  = 0;
			Bit    = 128 >> (x & 7);
			for (d = 0; d < depth; d++)
			{
				Color *= 2;
				if (Data[0] & Bit)
				{
					Color |= 1;
				}
				Data -= BytesPerLine;
			}
			row[x] =
				((Color & 0x0000ff) << 16) |
				((Color & 0x00ff00)) |
				((Color & 0xff0000) >> 16);
		}
	}
}

inline void b3Tx::b3CopyILBMtoFloat(
	b3_color * row,
	b3_coord   y) const
{
	b3_coord      x, d, BytesPerLine;
	b3_pkd_color  Color, Bit;

	BytesPerLine = TX_BWA(xSize);
	if (palette == nullptr)
	{
		for (x = 0; x < xSize; x++)
		{
			b3_u08 * Data   = data;

			Data  += ((y + 1) * BytesPerLine * depth + (x >> 3) - BytesPerLine);
			Color  = 0;
			Bit    = 128 >> (x & 7);
			for (d = 0; d < depth; d++)
			{
				Color *= 2;
				if (Data[0] & Bit)
				{
					Color |= 1;
				}
				Data -= BytesPerLine;
			}
			row[x].r = (Color & 0x0000ff)        / 255.0;
			row[x].g = ((Color & 0x00ff00) >>  8) / 255.0;
			row[x].b = ((Color & 0xff0000) >> 16) / 255.0;
			row[x].a = 0;
		}
	}
}

void b3Tx::b3Copy(const b3Tx * srcTx)
{
	type = B3_TX_UNDEFINED;
	if (!srcTx->b3IsLoaded())
	{
		b3FreeTx();
		return;
	}

	if (b3AllocTx(srcTx->xSize, srcTx->ySize, srcTx->depth))
	{
		b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Copy(): ");
		if (data != nullptr)
		{
			if (srcTx->type == B3_TX_ILBM)
			{
				b3_u08    *   bPtr;
				b3_pkd_color * lPtr;
				b3_color   *  cPtr;
				b3_coord      y;

				switch (type)
				{
				case B3_TX_VGA:
					bPtr = data;
					for (y = 0; y < ySize; y++)
					{
						srcTx->b3CopyILBMtoVGA(bPtr, y);
						bPtr += xSize;
					}
					break;

				case B3_TX_RGB8:
					lPtr = data;;
					for (y = 0; y < ySize; y++)
					{
						srcTx->b3CopyILBMtoRGB8(lPtr, y);
						lPtr += xSize;
					}
					break;

				case B3_TX_FLOAT:
					cPtr = data;;
					for (y = 0; y < ySize; y++)
					{
						srcTx->b3CopyILBMtoFloat(cPtr, y);
						cPtr += xSize;
					}
					break;

				case B3_TX_ILBM:
					// Never created
					memcpy(data,   srcTx->b3GetVoidData(),   dSize);
					break;

				default:
				case B3_TX_RGB4:
					b3PrintF(B3LOG_NORMAL, "### CLASS: b3Tx   # b3Copy(): unsupported destination format!\n");
				}
			}
			else
			{
				memcpy(data,   srcTx->b3GetVoidData(),   dSize);
			}
			b3PrintF(B3LOG_FULL, " [data - %zd]", dSize);
		}
		if (palette != nullptr)
		{
			memcpy(palette, srcTx->b3GetPalette(), std::min(pSize, srcTx->pSize) * sizeof(b3_pkd_color));
			b3PrintF(B3LOG_FULL, " [palette - %zd]", pSize);
		}


		b3PrintF(B3LOG_FULL, "\n");
	}
	else
	{
		b3PrintF(B3LOG_NORMAL, "### CLASS: b3Tx # b3Copy(): NOT ENOUGH MEMORY!\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	xDPI        = srcTx->xDPI;
	yDPI        = srcTx->yDPI;
	white_ratio = srcTx->white_ratio;
	ScanLines   = srcTx->ScanLines;
}

bool b3Tx::b3IsLoaded() const noexcept
{
	return ((xSize != 0) && (ySize != 0) && (data != nullptr) && (type != B3_TX_UNDEFINED));
}

void b3Tx::b3Name(const char * ImageName)
{
	image_name = ImageName != nullptr ? ImageName : "";
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Name(%s)\n",
		static_cast<const char *>(image_name));
}

void b3Tx::b3SetPalette(
	const b3_pkd_color * newPalette,
	const b3_count       NumColors)
{
	// compute bit depth
	depth = b3Math::b3Log2(NumColors);
	pSize = NumColors;
	type = (depth == 1 ? B3_TX_ILBM : B3_TX_VGA);

	// exchange pointer
	b3Free(palette);
	palette = const_cast<b3_pkd_color *>(newPalette);
}

void b3Tx::b3SetData(const b3_tx_data newData, const b3_size size)
{
	if (size != dSize)
	{
		B3_THROW(b3TxException, B3_TX_WRONG_SIZE);
	}
	b3EndHist();
	b3Free(data);

	data  = newData;
}

void b3Tx::b3GetResolution(b3_res & xd, b3_res & yd) const
{
	xd = xDPI;
	yd = yDPI;
}

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

b3_f32 b3Tx::b3GetBlue(const b3_coord x, const b3_coord y) const
{
	switch (type)
	{
	case B3_TX_ILBM:
	case B3_TX_RGB4:
		return (b3GetValue(x, y) & 0xff) * 0.0039215686;

	case B3_TX_VGA:
		return palette == nullptr ? 0 : (palette[b3Get<b3_u08>(x, y)] & 0xff) * 0.0039215686;

	case B3_TX_RGB8:
		return (b3Get<b3_pkd_color>(x, y) & 0xff) * 0.0039215686;

	case B3_TX_FLOAT:
		return b3Get<b3_color>(x, y).b;

	case B3_TX_UNDEFINED :
		return 0;

	default :
		B3_THROW(b3TxException, B3_TX_UNKNOWN_DATATYPE);
	}
}

const b3Color b3Tx::b3GetHdrValue(const b3_coord x, const b3_coord y) const
{
	switch (type)
	{
	case B3_TX_VGA:
		return b3Color(palette == nullptr ? b3Color::B3_BLACK : palette[b3Get<b3_u08>(x, y)]);

	case B3_TX_RGB4:
		return b3Color(b3Get<b3_u16>(x, y));

	case B3_TX_RGB8:
		return b3Color(b3Get<b3_pkd_color>(x, y));

	case B3_TX_FLOAT:
		return b3Color(b3Get<b3_color>(x, y));

	case B3_TX_ILBM:
	case B3_TX_UNDEFINED:
		return b3Color(b3GetValue(x, y));

	default :
		B3_THROW(b3TxException, B3_TX_UNKNOWN_DATATYPE);
	}
}

b3_pkd_color b3Tx::b3GetValue(
	const b3_coord x,
	const b3_coord y) const
{
	switch (type)
	{
	case B3_TX_ILBM:
		return b3ILBMValue(x, y);

	case B3_TX_RGB4:
		return b3Convert(b3Get<b3_u16>(x, y));

	case B3_TX_RGB8:
		return b3Get<b3_pkd_color>(x, y);

	case B3_TX_VGA:
		return palette == nullptr ? b3Color::B3_BLACK : palette[b3Get<b3_u08>(x, y)];

	case B3_TX_FLOAT:
		return b3Color(b3Get<b3_color>(x, y));

	case B3_TX_UNDEFINED :
		return b3Color::B3_BLACK;

	default :
		B3_THROW(b3TxException, B3_TX_UNKNOWN_DATATYPE);
	}
}

inline b3_pkd_color b3Tx::b3ILBMValue(
	const b3_coord x,
	const b3_coord y) const
{
	b3_u08    *   Address;
	b3_pkd_color  Bit;
	b3_u32        PlaneValue = 0, pattern = 1;
	b3_index      BytesPerLine;
	b3_res        i;

	BytesPerLine = TX_BWA(xSize);
	PlaneValue   = 0;
	Address      = data;
	Address     += (y * BytesPerLine * depth + (x >> 3));
	//	Bit          = m_Bits[x & 7];
	Bit          = 128 >> (x & 7);
	for (i = 0; i < depth; i++)
	{
		if (Address[0] & Bit)
		{
			PlaneValue |= pattern;
		}
		Address += BytesPerLine;
		pattern += pattern;
	}
	if (palette != nullptr)
	{
		return palette[PlaneValue];
	}

	if (depth >= 24)
	{
		i = PlaneValue;
		PlaneValue  = (i & 0xff0000) >> 16;
		PlaneValue |=  i & 0x00ff00;
		PlaneValue |= (i & 0x0000ff) << 16;
	}
	return PlaneValue;
}

/*************************************************************************
**                                                                      **
**                        for stenceling                                **
**                                                                      **
*************************************************************************/

bool b3Tx::b3IsBackground(const b3_coord x, const b3_coord y) const
{
	b3_u08  *  bPtr, bit;
	b3_count   xBytes;

	switch (type)
	{
	case B3_TX_ILBM :
		xBytes = TX_BWA(xSize);
		bPtr   = data;
		bPtr  += ((y + 1) * xBytes * depth + (x >> 3));
		bit    = m_Bits[x & 7];
		for (b3_res i = 0; i < depth; i++)
		{
			bPtr   -= xBytes;
			if (*bPtr & bit)
			{
				return true;
			}
		}

		// Check for first index.
		return false;

	case B3_TX_RGB8	:
		return (b3Get<b3_pkd_color>(x, y) & 0xff000000) != 0;

	case B3_TX_RGB4	:
		return (b3Get<b3_u16>(x, y) & 0xf000) != 0;

	case B3_TX_VGA	:
		return (palette[b3Get<b3_u08>(x, y)] & 0xff000000) != 0;

	case B3_TX_FLOAT :
		return b3Get<b3_color>(x, y).a > 0;

	default:
		// No image -> no hit -> always transparent...
		return true;
	}
	return true;
}

/*************************************************************************
**                                                                      **
**                        get a complete texture line                   **
**                                                                      **
*************************************************************************/

inline void b3Tx::b3GetILBM(
	b3_pkd_color  * ColorLine,
	const b3_coord  y) const
{
	b3_u08    *   Data;
	b3_coord      x, d, BytesPerLine;
	b3_pkd_color  Color, Bit;

	BytesPerLine = TX_BWA(xSize);
	if (palette != nullptr)
	{
		for (x = 0; x < xSize; x++)
		{
			Data   = data;
			Data  += ((y + 1) * BytesPerLine * depth + (x >> 3));
			Color  = 0;
			Bit    = m_Bits[x & 7];
			for (d = 0; d < depth; d++)
			{
				Data   -= BytesPerLine;
				Color <<= 1;
				if (Data[0] & Bit)
				{
					Color |= 1;
				}
			}
			ColorLine[0] = palette[Color];
			ColorLine++;
		}
	}
	else
	{
		for (x = 0; x < xSize; x++)
		{
			Data   = data;
			Data  += ((y + 1) * BytesPerLine * depth + (x >> 3));
			Color  = 0;
			Bit    = m_Bits[x & 7];
			for (d = 0; d < depth; d++)
			{
				Data   -= BytesPerLine;
				Color <<= 1;
				if (Data[0] & Bit)
				{
					Color |= 1;
				}
			}
			ColorLine[0] =
				((Color & 0x0000ff) << 16) |
				((Color & 0x00ff00)) |
				((Color & 0xff0000) >> 16);
			ColorLine++;
		}
	}
}

void b3Tx::b3GetRow(
	b3_color    *   Line,
	const b3_coord  y) const
{
	switch (type)
	{
	case B3_TX_ILBM:
		for (b3_coord x = 0; x < xSize; x++)
		{
			*Line++ = b3Color(b3ILBMValue(x, y));
		}
		break;

	case B3_TX_VGA:
		if (palette != nullptr)
		{
			b3TxAlgorithms::b3GetRow<b3_u08, b3_color>(Line, this, y,
				[this] (const b3_u08 index)
			{
				return b3Color(palette[index]);
			});
		}
		else
		{
			const b3_color black = b3Color(b3Color::B3_BLACK);

			for (b3_coord x = 0; x < xSize; x++)
			{
				*Line++ = black;
			}
		}
		break;

	case B3_TX_RGB4:
		b3TxAlgorithms::b3GetRow<b3_u16, b3_color>(Line, this, y);
		break;

	case B3_TX_RGB8:
		b3TxAlgorithms::b3GetRow<b3_pkd_color, b3_color>(Line, this, y);
		break;

	case B3_TX_FLOAT:
		b3TxAlgorithms::b3GetRow<b3_color, b3_color>(Line, this, y);
		break;

	default:
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
	}
}

void b3Tx::b3GetRow(
	b3_pkd_color * Line,
	const b3_coord y) const
{
	switch (type)
	{
	case B3_TX_ILBM :
		b3GetILBM(Line, y);
		break;

	case B3_TX_RGB8 :
		{
			const b3_pkd_color * src = b3GetTrueColorData();

			src += y * xSize;
			std::copy(src, src + xSize, Line);
		}
		break;

	case B3_TX_RGB4 :
		b3TxAlgorithms::b3GetRow<b3_u16, b3_pkd_color>(Line, this, y,
			&b3Tx::b3Convert);
		break;

	case B3_TX_VGA :
		b3TxAlgorithms::b3GetRow<b3_u08, b3_pkd_color>(Line, this, y,
			[this] (const b3_u08 index)
		{
			return palette[index];
		});
		break;

	case B3_TX_FLOAT:
		b3TxAlgorithms::b3GetRow<b3_color, b3_pkd_color>(Line, this, y);
		break;

	default:
		B3_THROW(b3TxException, B3_TX_UNKNOWN_DATATYPE);
	}
}
