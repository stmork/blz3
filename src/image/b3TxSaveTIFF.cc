/*
**
**	$Filename:	b3TxSaveTIFF.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - TIFF saving
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

#include "b3TxInclude.h"

#include <math.h>

#ifdef HAVE_LIBTIFF

/*************************************************************************
**                                                                      **
**                        b3Tx saving TIFFs                             **
**                                                                      **
*************************************************************************/

// These methods save the image associated with this instance as a TIFF.
// Look at the TIFF programming manual for understanding this. Be happy
// with this piece of code.
void b3Tx::b3GetSampleValues(
	long & bpp,
	long & spp)
{
	switch (type)
	{
	case B3_TX_ILBM :
		if (depth > 8)	// true color
		{
			bpp = 8;
			spp = 3;
		}
		else
		{
			bpp = depth;
			spp = 1;
		}
		break;

	case B3_TX_VGA :
		bpp = depth;
		spp = 1;
		break;

	case B3_TX_RGB4 :
	case B3_TX_RGB8 :
		bpp = 8;
		spp = 3;
		break;

	default :
		bpp = 8;
		spp = 1;
	}
}

b3_result b3Tx::b3SaveTIFFPalette(TIFF * tiff)
{
	b3_u08   *   cPtr;
	b3_u16       r[256], g[256], b[256];
	b3_coord     y;
	b3_res       max;
	char         software[128];
	b3_tx_error  result = B3_TX_OK;

	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx:  # saving TIFF paletted image (%dx%d)\n",
		xSize, ySize);
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE,  depth);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, (b3_f64)xDPI);
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, (b3_f64)yDPI);
	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, ySize);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	snprintf(software, sizeof(software), "Blizzard III V%s", B3_VERSION);
	TIFFSetField(tiff, TIFFTAG_SOFTWARE, software);

	// setting palette
	max = 1 << depth;
	for (y = 0; y < max; y++)
	{
		r[y] = (b3_u16)((palette[y] & 0xff0000) >> 8);
		g[y] = (b3_u16)(palette[y] & 0x00ff00);
		b[y] = (b3_u16)((palette[y] & 0x0000ff) << 8);
	}
	TIFFSetField(tiff, TIFFTAG_COLORMAP, &r, &g, &b);

	// OK, writing image data...
	cPtr = (b3_u08 *)data;
	for (y = 0; y < ySize; y++)
	{
		b3_u08 * pPtr = cPtr;	// to be changed for every bit depth...

		if (!TIFFWriteScanline(tiff, pPtr, y, 0))
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		cPtr += xSize;
	}

	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException, result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFFFax(TIFF * tiff)
{
	b3_coord     y;
	b3_count     xBytes;
	b3_u08   *   cPtr   = data;
	char         software[128];
	b3_tx_error  result = B3_TX_OK;

	// setting up tag directory
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx:  # saving TIFF fax G4 (%dx%d)\n",
		xSize, ySize);
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE,  1);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, (b3_f64)xDPI);
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, (b3_f64)yDPI);
	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, ySize);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	snprintf(software, sizeof(software), "Blizzard III V%s", B3_VERSION);
	TIFFSetField(tiff, TIFFTAG_SOFTWARE, software);

	xBytes = TX_BWA(xSize);
	for (y = 0; y < ySize; y++)
	{
		if (!TIFFWriteScanline(tiff, cPtr, y, 0))
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		cPtr += xBytes;
	}

	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException, result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFFTrueColor(TIFF * tiff)
{
	b3_pkd_color * lPtr;
	b3_coord      x, y;
	b3_u08    *   row, *rPtr, *gPtr, *bPtr;
	char          software[128];
	b3_tx_error   result = B3_TX_OK;

	// setting the TIFF tags
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx:  # saving TIFF true color (%dx%d)\n",
		xSize, ySize);
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE,  8);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, (b3_f64)xDPI);
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, (b3_f64)yDPI);
	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);

	snprintf(software, sizeof(software), "Blizzard III V%s", B3_VERSION);
	TIFFSetField(tiff, TIFFTAG_SOFTWARE, software);

	// alloc memory for three channels of one row
	row = b3TypedAlloc<b3_u08>(xSize * 3 * sizeof(b3_u08));
	if (row == nullptr)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3SaveTIFFTrueColor(): Not enough memory to allocate row\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	lPtr = b3GetTrueColorData();
	rPtr =  row;
	gPtr = &row[xSize];
	bPtr = &row[xSize + xSize];
	for (y = 0; y < ySize; y++)
	{
		// resample row
		for (x = 0; x < xSize; x++)
		{
			rPtr[x]  = (b3_u08)((lPtr[x] & 0xff0000) >> 16);
			gPtr[x]  = (b3_u08)((lPtr[x] & 0x00ff00) >>  8);
			bPtr[x]  = (b3_u08)(lPtr[x] & 0x0000ff);
		}

		// try to save red channel
		if (TIFFWriteScanline(tiff, rPtr, y, 0) != 1)
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		else
		{
			// try to save green channel
			if (TIFFWriteScanline(tiff, gPtr, y, 1) != 1)
			{
				y = ySize;
				result = B3_TX_NOT_SAVED;
			}
			else
			{
				// try to save blue channel
				if (TIFFWriteScanline(tiff, bPtr, y, 2) != 1)
				{
					y = ySize;
					result = B3_TX_NOT_SAVED;
				}
			}
		}

		// increase color source ptr by xSize
		lPtr += xSize;
	}
	b3Free(row);


	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException, result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFFRealColor(TIFF * tiff)
{
	b3_color   *  lPtr;
	b3_coord      x, y;
	b3_u16    *   row, *rPtr, *gPtr, *bPtr;
	char          software[128];
	b3_tx_error   result = B3_TX_OK;

	// setting the TIFF tags
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx:  # saving TIFF real color (%dx%d)\n",
		xSize, ySize);
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE,  16);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, (b3_f64)xDPI);
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, (b3_f64)yDPI);
	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

	TIFFSetField(tiff, TIFFTAG_COMPRESSION,  COMPRESSION_PACKBITS);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);

	snprintf(software, sizeof(software), "Blizzard III V%s", B3_VERSION);
	TIFFSetField(tiff, TIFFTAG_SOFTWARE, software);

	// alloc memory for three channels of one row
	row = b3TypedAlloc<b3_u16>(xSize * 3);
	if (row == nullptr)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3SaveTIFFTrueColor(): Not enough memory to allocate row\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	lPtr = b3GetHdrData();
	rPtr =  row;
	gPtr = &row[xSize];
	bPtr = &row[xSize + xSize];
	for (y = 0; y < ySize; y++)
	{
		// resample row
		for (x = 0; x < xSize; x++)
		{
			b3Color color = lPtr[x];

			color.b3Sat();
			rPtr[x]  = (b3_u16)(color[b3Color::R] * 65535.0);
			gPtr[x]  = (b3_u16)(color[b3Color::G] * 65535.0);
			bPtr[x]  = (b3_u16)(color[b3Color::B] * 65535.0);
		}

		// try to save red channel
		if (TIFFWriteScanline(tiff, rPtr, y, 0) != 1)
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		else
		{
			// try to save green channel
			if (TIFFWriteScanline(tiff, gPtr, y, 1) != 1)
			{
				y = ySize;
				result = B3_TX_NOT_SAVED;
			}
			else
			{
				// try to save blue channel
				if (TIFFWriteScanline(tiff, bPtr, y, 2) != 1)
				{
					y = ySize;
					result = B3_TX_NOT_SAVED;
				}
			}
		}

		// increase color source ptr by xSize
		lPtr += xSize;
	}
	b3Free(row);


	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException, result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFF(const char * nameTx)
{
	TIFF    *    tiff;
	b3_tx_error  result = B3_TX_OK;

	b3PrintF(B3LOG_FULL, "Saving TIFF: %s\n", nameTx);

	if ((xSize == 0) || (ySize == 0))
	{
		B3_THROW(b3TxException, B3_TX_NOT_SAVED);
	}

	if (nameTx == nullptr)
	{
		if (strlen(image_name) == 0)
		{
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}
	else
	{
		b3Name(nameTx);
	}

	tiff = TIFFOpen(image_name, "w");
	if (tiff)
	{
		b3PrintF(B3LOG_DEBUG, "### CLASS: b3Tx:  # saving TIFF (%s)\n",
			(char *)image_name);
		// Now select the saving version we need.
		if (depth == 1)
		{
			b3SaveTIFFFax(tiff);
		}
		else
		{
			switch (type)
			{
			case B3_TX_RGB4:
			case B3_TX_RGB8:
				b3SaveTIFFTrueColor(tiff);
				break;

			case B3_TX_FLOAT:
				b3SaveTIFFRealColor(tiff);
				break;

			default:
				b3SaveTIFFPalette(tiff);
				break;
			}
		}

		TIFFClose(tiff);
	}
	else
	{
		result = B3_TX_NOT_SAVED;
	}

	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException, result);
	}
	return B3_OK;
}
#endif

