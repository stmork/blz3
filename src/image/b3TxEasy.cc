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
#include <blz3/base/b3Endian.h>

/**
 * BMP format header.
 *
 * Source: https://de.wikipedia.org/wiki/Windows_Bitmap
 */
#pragma pack(2)
struct b3HeaderBMP
{
	uint16_t bfType;     //!< ASCII-Zeichenkette "BM" (Hex: 0x42 0x4D, Dezimal: 66 77).
	uint32_t bfSize;     //!< Größe der BMP-Datei in Byte. (unzuverlässig)
	uint32_t bfReserved; //!< Reserviert, von der Software abhängig, standardmäßig 0

	/**
	 * Offset der Bilddaten in Byte vom Beginn der Datei an.
	 *
	 *  ieser hat nicht immer den Wert 54 (er ergibt sich aus 14 Byte Header +
	 *  40 Byte Infoblock) und muss daher dynamisch ausgelesen werden, weil es
	 *  sonst ggf. zu Fehldarstellungen im Bild kommt.
	 */
	uint32_t bfOffBits;

	/** Größe der BITMAPINFOHEADER-Struktur in Byte. */
	uint32_t biSize;

	/**
	 * Breite der Bitmap in Pixel. Dabei ist das erste Byte niederwertig und
	 * das letzte Byte höchstwertig.
	 */
	int32_t biWidth;

	/**
	 * Der Betrag gibt die Höhe der Bitmap in Pixel an. Dabei ist das erste
	 * Byte niederwertig und das letzte Byte höchstwertig.
	 *
	 * <ul>
	 * <li>Ist der Wert positiv, so ist die Bitmap eine sogenannte
	 * "bottom-up"-Bitmap (die Bilddaten beginnen mit der untersten und enden
	 * mit der obersten Bildzeile). Dies ist die gebräuchlichste Variante.</li>
	 *
	 * <li>Ist der Wert negativ, so ist die Bitmap eine “top-down”-Bitmap (die
	 * Bilddaten beginnen mit der obersten und enden mit der untersten Bildzeile).</li>
	 * </ul>
	 */
	int32_t biHeight;

	/**
	 * 1 (Stand in einigen älteren Formaten wie PCX für die Anzahl der
	 * Farbebenen, wird aber für BMP nicht verwendet)
	 */
	uint16_t biPlanes;

	/**
	 * Gibt die Farbtiefe der Bitmap in bpp an; muss einer der folgenden
	 * Werte sein: 1, 4, 8, 16, 24 oder 32. Bei 1, 4 und 8 bpp sind die Farben
	 * indiziert.
	 */
	uint16_t biBitCount;

	/**
	 * Einer der folgenden Werte:
	 *
	 * <ul>
	 * <li>0 (BI_RGB): Bilddaten sind unkomprimiert.</li>
	 * <li>1 (BI_RLE8): Bilddaten sind lauflängenkodiert für 8 bpp. Nur erlaubt
	 * wenn biBitCount=8 und biHeight positiv.</li>
	 * <li>2 (BI_RLE4): Bilddaten sind lauflängenkodiert für 4 bpp. Nur
	 * erlaubt wenn biBitCount=4 und biHeight positiv.</li>
	 * <li>3 (BI_BITFIELDS): Bilddaten sind unkomprimiert und benutzerdefiniert
	 * (mittels Farbmasken) kodiert. Nur erlaubt wenn biBitCount=16 oder 32.</li>
	 * </ul>
	 */
	uint32_t biCompression;

	/**
	 * Wenn biCompression=BI_RGB: Entweder 0 oder die Größe der Bilddaten in
	 * Byte. Ansonsten: Größe der Bilddaten in Byte.
	 */
	uint32_t biSizeImage;

	/**
	 * Horizontale Auflösung des Zielausgabegerätes in Pixel pro Meter; wird
	 * aber für BMP-Dateien meistens auf 0 gesetzt.
	 */
	int32_t biXPelsPerMeter;

	/**
	 * Vertikale Auflösung des Zielausgabegerätes in Pixel pro Meter; wird
	 * aber für BMP-Dateien meistens auf 0 gesetzt.
	 */
	int32_t biYPelsPerMeter;

	/**
	 * Wenn biBitCount=1: 0. Wenn biBitCount=4 oder 8: die Anzahl der Einträge
	 * der Farbtabelle; 0 bedeutet die maximale Anzahl (2, 16 oder 256).
	 *
	 * Ansonsten: Die Anzahl der Einträge der Farbtabelle (0=keine Farbtabelle).
	 * Auch wenn sie in diesem Fall nicht notwendig ist, kann dennoch eine für
	 * die Farbquantisierung empfohlene Farbtabelle angegeben werden.
	 */
	uint32_t biClrUsed;

	/**
	 * Wenn biBitCount=1, 4 oder 8: Die Anzahl sämtlicher im Bild verwendeten
	 * Farben; 0 bedeutet alle Farben der Farbtabelle.
	 *
	 * Ansonsten:
	 * Wenn eine Farbtabelle vorhanden ist und diese sämtliche im Bild
	 * verwendeten Farben enthält: deren Anzahl.
	 *
	 * Ansonsten: 0.@brief biClrImportant
	 */
	uint32_t biClrImportant;
};
#pragma pack()

/*************************************************************************
**                                                                      **
**                       Rayshade MTV-Format                            **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseRAW(
	const b3_u08 * buffer,
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

b3_result b3Tx::b3ParseBMP(const b3_u08 * buffer)
{
	b3HeaderBMP  * bmp = (b3HeaderBMP *)buffer;
	b3_pkd_color * lPtr;
	b3_u08    *    cPtr;
	b3_res         xNewSize, yNewSize;
	b3_coord       x, y;
	b3_count       numPlanes, numColors;
	b3_count       i, offset, value;

	B3_ASSERT(offsetof(b3HeaderBMP, biSize) == 14);
	B3_ASSERT(sizeof(b3HeaderBMP) == 54);

	xNewSize  = b3Endian::b3GetIntel32(&bmp->biWidth);
	yNewSize  = b3Endian::b3GetIntel32(&bmp->biHeight);
	numPlanes = b3Endian::b3GetIntel16(&bmp->biBitCount);
	numColors = b3Endian::b3GetIntel32(&bmp->biClrUsed);

	b3PrintF(B3LOG_FULL, "IMG BMP  # b3ParseBMP(%s)\n",
		(const char *)image_name);

	if (bmp->biCompression != 0)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG BMP  # Unsupported packing:\n");
		B3_THROW(b3TxException, B3_TX_ERR_PACKING);
	}

	if (numColors == 0)
	{
		numColors = (bmp->bfOffBits - sizeof(b3HeaderBMP)) >> 2;
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
			const b3_u08 * color_ptr = &buffer[bmp->bfOffBits - numColors * 4];

			for (i = 0; i < numColors; i++)
			{
				const b3_u08 b = *color_ptr++;
				const b3_u08 g = *color_ptr++;
				const b3_u08 r = *color_ptr++;
				color_ptr++;

				palette[i] = b3Color::b3MakePkdColor(r, g, b);
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
	buffer += bmp->bfOffBits;
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
		cPtr  = data;
		cPtr += dSize;
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

b3_result b3Tx::b3ParseBMF(const b3_u08 * buffer, b3_size buffer_size)
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
