/*
**
**	$Filename:	b3TxPCX.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading PCX images
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

#include "blz3/base/b3Endian.h"
#include "blz3/image/b3Tx.h"

#pragma pack(1)
/**
 * The PCX file header.
 *Sources:
 * <ul>
 * <li>https://www.fileformat.info/format/pcx/egff.htm</li>
 * <li>https://de.wikipedia.org/wiki/Picture_exchange</li>
 * </ul>
 */
struct b3HeaderPCX
{
	b3_u08 Identifier;        //!< PCX Id Number (Always 0x0A)
	b3_u08 Version;           //!< Version Number
	b3_u08 Encoding;          //!< Encoding Format
	b3_u08 BitsPerPixel;      //!< Bits per Pixel
	b3_u16 XStart;            //!< Left of image
	b3_u16 YStart;            //!< Top of Image
	b3_u16 XEnd;              //!< Right of Image
	b3_u16 YEnd;              //!< Bottom of image
	b3_u16 HorzRes;           //!< Horizontal Resolution
	b3_u16 VertRes;           //!< Vertical Resolution
	b3_u08 Palette[48];       //!< 16-Color EGA Palette
	b3_u08 Reserved1;         //!< Reserved (Always 0)
	b3_u08 NumBitPlanes;      //!< Number of Bit Planes
	b3_u16 BytesPerLine;      //!< Bytes per Scan-line
	b3_u16 PaletteType;       //!< Palette Type
	b3_u16 HorzScreenSize;    //!< Horizontal Screen Size
	b3_u16 VertScreenSize;    //!< Vertical Screen Size
	b3_u08 Reserved2[54];     //!< Reserved (Always 0)
};
#pragma pack()

/*************************************************************************
**                                                                      **
**                            Paintbrush PCX                            **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParsePCX8(const b3_u08 * buffer)
{
	b3_pkd_color   t;
	b3_res         xNewSize, yNewSize;
	b3_size        i = 0;

	b3PrintF(B3LOG_FULL, "IMG PCX  # b3ParsePCX8(%s)\n",
		(const char *)image_name);

	xNewSize = b3Endian::b3GetIntel16(&buffer[ 8]) + 1;
	yNewSize = b3Endian::b3GetIntel16(&buffer[10]) + 1;
	if (b3AllocTx(xNewSize, yNewSize, 8))
	{
		const b3_u08 * srcPtr = &buffer[128];
		b3_u08    *    dstPtr = data;

		FileType = FT_PCX8;
		while (i < dSize)
		{
			b3_u08 Col = *srcPtr++;

			if ((Col & 192) == 192)
			{
				t   = (Col & 63);
				Col = *srcPtr++;
				for (i += t; t > 0; t--)
				{
					*dstPtr++ = Col;
				}
			}
			else
			{
				*dstPtr++ = Col;
				i++;
			}

		}

		srcPtr++;      /* Zeiger auf Palette */
		for (i = 0; i < 256; i++)
		{
			palette[i] = b3Color::b3MakePkdColor(srcPtr[0], srcPtr[1], srcPtr[2]);
			srcPtr += 3;
		}
	}
	else
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG PCX  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	return B3_OK;
}

b3_result b3Tx::b3ParsePCX4(const b3_u08 * buffer)
{
	b3_u08        Col;
	b3_count      u, xSrcBytes;
	b3_pkd_color  t;
	b3_size       i, DataSize;

	b3PrintF(B3LOG_FULL, "IMG PCX  # b3ParsePCX4(%s)\n",
		(const char *)image_name);

	xSize = b3Endian::b3GetIntel16(&buffer[ 8]) + 1;
	ySize = b3Endian::b3GetIntel16(&buffer[10]) + 1;
	depth = buffer[65];
	dSize = TX_BWA(xSize) * ySize * depth;
	pSize = 1 << depth;

	data    = b3TypedAlloc<b3_u08>(dSize);
	palette = b3TypedAlloc<b3_pkd_color>(pSize);
	if ((data != nullptr) && (palette != nullptr))
	{
		type      = B3_TX_ILBM;
		FileType  = FT_PCX4;
		xSrcBytes = buffer[66];

		const b3_u08 * srcPtr = &buffer[128];
		DataSize = ySize * xSrcBytes * depth;

		u = 16;
		for (i = 0; i < pSize; i++)
		{
			t  = (b3_pkd_color)buffer[u++] << 16;
			t += (b3_pkd_color)buffer[u++] <<  8;
			t += (b3_pkd_color)buffer[u++];
			palette[i] = t;
		}

		b3_u08 * dstPtr = data;

		i = 0;
		while (i < DataSize)
		{
			Col = *srcPtr++;
			if ((Col & 192) == 192)
			{
				t   = (Col & 63);
				Col = *srcPtr++;
				for (i += t; t > 0; t--)
				{
					*dstPtr++ = Col;
				}
			}
			else
			{
				*dstPtr++ = Col;
				i++;
			}
		}
	}
	else
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG PCX  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	return B3_OK;
}
