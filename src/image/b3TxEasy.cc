/*
**
**	$Filename:	b3TxEasy.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading simple uncompressed images
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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                       Rayshade MTV-Format                            **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseRAW(
	b3_u08 * buffer,
	b3_res   x,
	b3_res   y,
	b3_s32   ppm_type)
{
	b3_u08    *   newCData;
	b3_count      i, Max;

	b3PrintF(B3LOG_FULL, "IMG RAW  # b3ParseRAW(%s)\n",
		(const char *)image_name);

	switch (ppm_type)
	{
	case 4 : /* bitmap */
		if (b3AllocTx(x, y, 1))
		{
			b3_count xSrcBytes = TX_BBA(x);
			b3_count xDstBytes = TX_BWA(x);

			newCData = data;
			for (y = 0; y < ySize; y++)
			{
				memcpy(newCData, buffer, xSrcBytes);
				buffer   += xSrcBytes;
				newCData += xDstBytes;
			}
			palette[0] = 0x00ffffff;
			palette[1] = 0x00000000;
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG RAW  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		break;

	case 5 : /* grey */
		if (b3AllocTx(x, y, 8))
		{
			memcpy(data, buffer, x * y);
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG RAW  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		break;

	case 6 : /* 24 Bit */
		if (b3AllocTx(x, y, 24))
		{
			b3_pkd_color * newLData = (b3_u32 *)data;

			Max      = x * y;
			for (i = 0; i < Max; i++)
			{
				const b3_u08 r = *buffer++;
				const b3_u08 g = *buffer++;
				const b3_u08 b = *buffer++;

				*newLData++ = b3Color::b3MakePkdColor(r, g, b);
			}
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG RAW  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		break;

	default :
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG RAW  # Unknown format:\n");
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
	return B3_OK;
}

/*************************************************************************
**                                                                      **
**                        BMP format                                    **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseBMP(b3_u08 * buffer)
{
	b3_pkd_color * lPtr;
	b3_u08    *    cPtr;
	b3_res         xNewSize, yNewSize;
	b3_coord       x, y;
	b3_count       numPlanes, numColors;
	b3_count       i, offset, value;

	b3PrintF(B3LOG_FULL, "IMG BMP  # b3ParseBMP(%s)\n",
		(const char *)image_name);

	if (b3Endian::b3GetIntel32(&buffer[30]) != 0)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG BMP  # Unsupported packing:\n");
		B3_THROW(b3TxException, B3_TX_ERR_PACKING);
	}
	xNewSize  = b3Endian::b3GetIntel16(&buffer[18]);
	yNewSize  = b3Endian::b3GetIntel16(&buffer[22]);
	numColors = b3Endian::b3GetIntel32(&buffer[46]);
	numPlanes = b3Endian::b3GetIntel32(&buffer[28]);
	if (numColors == 0)
	{
		numColors = (b3Endian::b3GetIntel32(&buffer[10]) - 54) >> 2;
	}

	if (!b3AllocTx(xNewSize, yNewSize, numPlanes))
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG BMP  # Unsupported color format:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	FileType = FT_BMP;

	if (numPlanes <= 8)
	{
		if (numColors > 0)
		{
			for (i = 0; i < numColors; i++)
			{
				palette[i] =
					((b3_pkd_color)buffer[(i << 2) + 56] << 16) |
					((b3_pkd_color)buffer[(i << 2) + 55] <<  8) |
					((b3_pkd_color)buffer[(i << 2) + 54]);
			}
		}
		else
		{
			numColors = 1 << numPlanes;
			value     = 8  - numPlanes;
			for (i = 0; i < numColors; i++)
			{
				palette[i] = 0x00010101 * (i << value);
			}
		}
	}



	/* checking bits per pixel */
	buffer += b3Endian::b3GetIntel32(&buffer[10]);
	switch (numPlanes)
	{
	case  1 :
		offset = TX_BBA(xSize);
		cPtr   = data + dSize;
		for (y = 0; y < ySize; y++)
		{
			cPtr -= offset;
			for (x = 0; x < xSize; x += 8)
			{
				*cPtr++ = *buffer++;
			}
			cPtr -= offset;
		}
		break;

	case  4 :
		offset = (xSize + 7) >> 3;
		cPtr   = data;
		cPtr  += dSize;
		for (y = 0; y < ySize; y++)
		{
			cPtr -= (offset << 2);
			for (x = 0; x < xSize; x += 2)
			{
				value = 128 >> (x & 7);
				i     = (x >> 3) + offset + offset + offset;
				if (buffer[0] & 0x80)
				{
					cPtr[i] |= value;
				}
				i -= offset;
				if (buffer[0] & 0x40)
				{
					cPtr[i] |= value;
				}
				i -= offset;
				if (buffer[0] & 0x20)
				{
					cPtr[i] |= value;
				}
				i -= offset;
				if (buffer[0] & 0x10)
				{
					cPtr[i] |= value;
				}

				value = value >> 1;
				i     = (x >> 3) + offset + offset + offset;
				if (buffer[0] & 0x08)
				{
					cPtr[i] |= value;
				}
				i -= offset;
				if (buffer[0] & 0x04)
				{
					cPtr[i] |= value;
				}
				i -= offset;
				if (buffer[0] & 0x02)
				{
					cPtr[i] |= value;
				}
				i -= offset;
				if (buffer[0] & 0x01)
				{
					cPtr[i] |= value;
				}
				buffer++;
			}
		}
		break;

	case  8 :
		cPtr = data + dSize;
		for (y = 0; y < ySize; y++)
		{
			cPtr   -= xSize;
			memcpy(cPtr, buffer, xSize);
			buffer += xSize;
		}
		break;

	case 24 :
		offset = xSize & 3;
		lPtr   = data;
		lPtr  += (xSize * ySize);
		for (y = 0; y < ySize; y++)
		{
			lPtr -= xSize;
			for (x = 0; x < xSize; x++)
			{
				// reversed blue, green, red
				const b3_u08 b = *buffer++;
				const b3_u08 g = *buffer++;
				const b3_u08 r = *buffer++;

				*lPtr++ = b3Color::b3MakePkdColor(r, g, b);
			}
			lPtr   -= xSize;
			buffer += offset;
		}
		break;

	default:
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG BMP  # Unsupported color format:\n");
		B3_THROW(b3TxException, B3_TX_ERR_HEADER);
	}

	return B3_OK;
}

/*************************************************************************
**                                                                      **
**                        BMF files                                     **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseBMF(b3_u08 * buffer, b3_size buffer_size)
{
	b3_u08    *   gray;
	b3_coord      x, y;
	b3_res        xNewSize, yNewSize;
	b3_count      lSize;

	b3PrintF(B3LOG_FULL, "IMG BMF  # b3ParseBMF(%s)\n",
		(const char *)image_name);

	xNewSize = b3Endian::b3GetIntel16(&buffer[2]);
	yNewSize = b3Endian::b3GetIntel16(&buffer[4]);

	switch (buffer[6])
	{
	case 2 :
		if (b3AllocTx(xNewSize, yNewSize, 8))
		{
			buffer += buffer_size;
			gray    = data;
			for (y = 0; y < ySize; y++)
			{
				buffer -= xSize;
				memcpy(gray, buffer, xSize);
				gray += xSize;
			}
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG BMF  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		break;

	case 4 :
		if (b3AllocTx(xNewSize, yNewSize, 24))
		{
			b3_pkd_color * pixel;

			pixel   = (b3_pkd_color *)data;
			buffer += buffer_size;
			lSize   = xSize + xSize + xSize;
			for (y = 0; y < xSize; y++)
			{
				buffer -= lSize;
				for (x = 0; x < xSize; x++)
				{
					const b3_u08 r = buffer[x + xSize + xSize];
					const b3_u08 g = buffer[x + xSize];
					const b3_u08 b = buffer[x];

					*pixel++ = b3Color::b3MakePkdColor(r, g, b);
				}
			}
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG BMF  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		break;
	}
	FileType = FT_BMF;
	return B3_OK;
}
