/*
**
**	$Filename:	b3TxIMG.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading SGI-RLE images
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**    All Rights Reserved
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

#include "blz3/base/b3Endian.h"
#include "blz3/image/b3Tx.h"

#include "b3TxSGI.h"

/*************************************************************************
**                                                                      **
**                        SGI RLE image format                          **
**                                                                      **
*************************************************************************/

inline void b3Tx::b3UnpackSGI(
	b3_u08   *   buffer,
	const void * inPtr,
	b3_count     count,
	b3_count     bytes,
	b3_offset    offset)
{
	const b3_u08 * bBuffer;
	b3_u16         pixel;

	switch (bytes)
	{
	case 1 :
		if (count != 0)
		{
			bBuffer  = static_cast<const b3_u08 *>(inPtr);
			bBuffer += (512 / sizeof(unsigned char) + offset);
			while (count-- > 0)
			{
				*buffer++ = *bBuffer++;
			}
		}
		else
		{
			bBuffer  = static_cast<const b3_u08 *>(inPtr);
			bBuffer += offset;
			do
			{
				pixel = *bBuffer++;

				count = (pixel & 0x7f);
				if (count == 0)
				{
					return;
				}
				if (pixel & 0x80)
				{
					while (count--)
					{
						*buffer++ = *bBuffer++;
					}
				}
				else
				{
					pixel = *bBuffer++;
					while (count--)
					{
						*buffer++ = pixel;
					}
				}
			}
			while (true);
		}
		break;

	case 2 :
		if (count != 0)
		{
			bBuffer  = static_cast<const b3_u08 *>(inPtr);
			bBuffer += (512 / sizeof(unsigned short) + offset);
			while (count-- > 0)
			{
				*buffer++ = *bBuffer++;
			}
		}
		else
		{
			const b3_u16 * sBuffer  = static_cast<const b3_u16 *>(inPtr);;

			sBuffer += offset;
			do
			{
				pixel = *sBuffer++;
				count = (pixel & 0x7f);

				// cppcheck-suppress knownConditionTrueFalse
				if (count == 0)
				{
					return;
				}
				if (pixel & 0x80)
				{
					while (count--)
					{
						*buffer++ = *sBuffer++;
					}
				}
				else
				{
					pixel = *sBuffer++;
					while (count--)
					{
						*buffer++ = pixel;
					}
				}
			}
			while (true);
		}
		break;
	}
}

inline void b3Tx::b3ConvertSGILine(
	const b3_u16  *  buffer,
	const b3_offset  offset,
	b3_size          size,
	const b3_count   bytes)
{
	if (bytes == 2)
	{
		buffer += offset;
		while (size-- > 0)
		{
			b3Endian::b3ChangeEndian16(const_cast<b3_u16 *>(buffer++));
		}
	}
}

void b3Tx::b3ParseSGI3(
	const b3HeaderSGI * HeaderSGI,
	const b3_u08    *   buffer)
{
	const b3_pkd_color * lineTable;
	const b3_pkd_color * lineSizes;
	b3_pkd_color * lPtr, value;
	b3_u08    *    line;
	b3_u08    *    cPtr;
	b3_res         zSize = depth >> 3;
	b3_count       rle, bytes, block;

	bytes =   HeaderSGI->type & 0x00ff; /* check bytes per pixel */
	rle   = ((HeaderSGI->type & 0xff00) == 0x0100) ? 0 : xSize; /* check RLE */

	/* line buffer */
	line = b3TypedAlloc<b3_u08>(xSize * zSize * bytes);
	if (line == nullptr)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG SGI  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}


	/* convert line offsets */
	lineTable = reinterpret_cast<const b3_pkd_color *>(&buffer[512]);
	lineSizes = reinterpret_cast<const b3_pkd_color *>(&lineTable[zSize * ySize]);
	if (HeaderSGI->imagic == IMAGIC2) /* check for converting endian */
	{
		if (rle <= 0) /* RLE data */
		{
			for (b3_count y = 0; y < ySize; y++)
			{
				for (b3_count z = 0; z < zSize; z++)
				{
					b3Endian::b3ChangeEndian32(
						const_cast<b3_u32 *>(&lineTable[y + z * ySize]));
					b3Endian::b3ChangeEndian32(
						const_cast<b3_u32 *>(&lineSizes[y + z * ySize]));
					b3ConvertSGILine(reinterpret_cast<const b3_u16 *>(buffer),
						lineTable[y + z * ySize],
						lineSizes[y + z * ySize], bytes);
				}
			}
		}
		else /* raw (VERBATIM) data */
		{
			for (b3_count y = 0; y < ySize; y++)
			{
				for (b3_count z = 0; z < zSize; z++)
				{
					b3ConvertSGILine(reinterpret_cast<const b3_u16 *>(buffer),
						256 + y * xSize * zSize + z * xSize, xSize, bytes);
				}
			}
		}
	}

	switch (type)
	{
	case B3_TX_RGB8 :
		lPtr  = data;
		block = xSize * ySize;
		for (b3_count y = ySize - 1; y >= 0; y--)
		{
			if (rle > 0) /* read raw data */
			{
				b3UnpackSGI(&line[0],             buffer, rle, bytes, y * xSize);
				b3UnpackSGI(&line[xSize],         buffer, rle, bytes, y * xSize + block);
				b3UnpackSGI(&line[xSize + xSize], buffer, rle, bytes, y * xSize + block + block);
			}
			else /* read RLE packed data */
			{
				b3UnpackSGI(&line[0],             buffer, rle, bytes, lineTable[y]);
				b3UnpackSGI(&line[xSize],         buffer, rle, bytes, lineTable[y + ySize]);
				b3UnpackSGI(&line[xSize + xSize], buffer, rle, bytes, lineTable[y + ySize + ySize]);
			}
			for (b3_count x = 0; x < xSize; x++)
			{
				value   = line[x];
				value   = (value << 8) | line[x + xSize];
				*lPtr++ = (value << 8) | line[x + xSize + xSize];
			}
		}
		break;

	case B3_TX_VGA :
		cPtr = data;
		for (b3_count y = ySize - 1; y >= 0; y--)
		{
			if (rle > 0)
			{
				b3UnpackSGI(cPtr, buffer, rle, bytes, y * xSize);
			}
			else
			{
				b3UnpackSGI(cPtr, buffer, rle, bytes, lineTable[y]);
			}
			cPtr += xSize;
		}
		break;

	default :
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG SGI  # Unsupported format:\n");
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
	b3Free(line);
}

b3_result b3Tx::b3ParseSGI(const b3_u08 * buffer)
{
	const b3HeaderSGI * HeaderSGI = reinterpret_cast<const struct b3HeaderSGI *>(buffer);
	b3_bool             success = false;
	b3_res              xNewSize, yNewSize;
	b3_index            c;

	b3PrintF(B3LOG_FULL, "IMG SGI  # b3ParseSGI(%s)\n",
		static_cast<const char *>(image_name));

	if (HeaderSGI->imagic == IMAGIC2)
	{
		b3HeaderSGI * header = const_cast<b3HeaderSGI *>(HeaderSGI);

		b3Endian::b3ChangeEndian16(&header->type);
		b3Endian::b3ChangeEndian16(&header->dim);
		b3Endian::b3ChangeEndian16(&header->xsize);
		b3Endian::b3ChangeEndian16(&header->ysize);
		b3Endian::b3ChangeEndian16(&header->zsize);
		b3Endian::b3ChangeEndian32(&header->min);
		b3Endian::b3ChangeEndian32(&header->max);
		b3Endian::b3ChangeEndian32(&header->wastebytes);
		b3Endian::b3ChangeEndian32(&header->colormap);
	}

	xNewSize = HeaderSGI->xsize;
	yNewSize = HeaderSGI->ysize;
	depth    = HeaderSGI->zsize << 3;


	// get texture type
	switch (HeaderSGI->colormap)
	{
	case 0 :
		switch (HeaderSGI->zsize)
		{
		case 1 :
			success = b3AllocTx(xNewSize, yNewSize, 8);
			break;
		case 3 :
			success = b3AllocTx(xNewSize, yNewSize, 24);
			break;

		default :
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG SGI  # Unsupported format:\n");
			B3_THROW(b3TxException, B3_TX_UNSUPP);
			break;
		}
		break;

	case 3 :
		success = b3AllocTx(xNewSize, yNewSize, 8);
		if (success)
		{
			for (c = 0; c < 256; c++)
			{
				palette[c] =
					((c & 0xe0) << 16) |
					((c & 0x1c) << 11) |
					((c & 0x03) <<  6);
			}
		}
		break;

	default :
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG SGI  # Unsupported format:\n");
		B3_THROW(b3TxException, B3_TX_UNSUPP);
		break;
	}
	if (success)
	{
		/* how are rows saved? */
		switch (HeaderSGI->dim)
		{
		case 3 :
			b3ParseSGI3(HeaderSGI, buffer);
			break;

		case 1 :
		case 2 :
		default :
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG SGI  # Wrong packing algorithms:\n");
			B3_THROW(b3TxException, B3_TX_ERR_PACKING);
		}

		// Success
		if (type != B3_TX_UNDEFINED)
		{
			FileType = FT_SGI_RLE;
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG SGI  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
	}
	return B3_OK;
}
