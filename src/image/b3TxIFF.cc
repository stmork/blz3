/*
**
**	$Filename:	b3TxIFF.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading IFF images
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

#include "b3TxIFF.h"
#include "blz3/base/b3Math.h"

/*************************************************************************
**                                                                      **
**                        local structures                              **
**                                                                      **
*************************************************************************/

class b3_tx_yuv_table
{
	static b3_tx_yuv_table  m_MultYuvTable;
	static b3_u08           m_ConvertBits[8];

	b3_s16 MultRV[256];
	b3_s16 MultGU[256];
	b3_s16 MultGV[256];
	b3_s16 MultBU[256];
	b3_u32 MultR[768];
	b3_u32 MultG[768];
	b3_u32 MultB[768];

	inline b3_tx_yuv_table()
	{
		long				 i;
		double				 Rad;

		for (i = 0; i < 256; i++)
		{
			Rad = (i - 128.0) * 0.8588;
			MultRV[i] = 256 + (long)(Rad *  1.587);  /*  = 156 / 112 *  1.14  */
			MultGU[i] = 128 + (long)(Rad * -0.394);  /*  = 112 / 112 * -0.394 */
			MultGV[i] = 128 + (long)(Rad * -0.8092); /*  = 156 / 112 * -0.581 */
			MultBU[i] = 256 + (long)(Rad *  2.028);  /*  = 112 / 112 *  2.028 */

			Rad = i -  16.0;
			MultR[i + 256] = (b3_u32)(Rad * 1.1644) << 16; /*  = 255 / (235 - 16) */
			MultG[i + 256] = (b3_u32)(Rad * 1.1644) <<  8; /*  = 255 / (235 - 16) */
			MultB[i + 256] = (b3_u32)(Rad * 1.1644);     /*  = 255 / (235 - 16) */

			if (i <  16)
			{
				MultR[i + 256] =   0;
				MultG[i + 256] =   0;
				MultB[i + 256] =   0;
			}
			if (i > 235)
			{
				MultR[i + 256] = 0xff0000;
				MultG[i + 256] = 0x00ff00;
				MultB[i + 256] = 0x0000ff;
			}

			MultR[i]     = 0;
			MultG[i]     = 0;
			MultB[i]     = 0;
			MultR[i + 512] = 0xff0000;
			MultG[i + 512] = 0x00ff00;
			MultB[i + 512] = 0x0000ff;
		}
	}

	friend class b3Tx;
};

b3_tx_yuv_table b3_tx_yuv_table::m_MultYuvTable;

b3_u08 b3_tx_yuv_table::m_ConvertBits[8]
{
	128, 64, 32, 16, 8, 4, 2, 1
};

/*************************************************************************
**                                                                      **
**                       Impulse IFF-RGBx-Format                        **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseIFF_RGB8(const b3_u08 * buffer, b3_size buffer_size)
{
	b3_u08    *   cPtr;
	b3_pkd_color * dstPtr, Color;
	b3_u32    *   srcPtr;
	b3_count      Amount, Max;
	b3_index      Pos = 12, i = 0, k;

	b3PrintF(B3LOG_FULL, "IMG IFF  # b3ParseIFF_RGB8(%s)\n",
		(const char *)image_name);

	palette	 = nullptr;
	while (Pos < (b3_index)buffer_size)
	{
		srcPtr = (b3_u32 *)&buffer[Pos];
		cPtr   = (b3_u08 *)srcPtr;
		switch (b3Endian::b3GetMot32(srcPtr))
		{
		case IFF_BMHD :
			xSize	= b3Endian::b3GetMot16(&cPtr[ 8]);
			ySize	= b3Endian::b3GetMot16(&cPtr[10]);
			depth	= cPtr[16];
			break;

		case IFF_BODY :
			if (!b3AllocTx(xSize, ySize, 24))
			{
				b3FreeTx();
				b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
				B3_THROW(b3TxException, B3_TX_MEMORY);
			}
			Max = xSize * ySize;
			dstPtr = (b3_pkd_color *)data;

			cPtr += 8;
			while (i < Max)
			{
				Color =
					((b3_pkd_color)cPtr[0] << 16) |
					((b3_pkd_color)cPtr[1] <<  8) |
					(b3_pkd_color)cPtr[2];
				cPtr += 3;
				Amount  = cPtr[0] & 0x7f;
				if ((cPtr[0] & 0x80) == 0)
				{
					Color |= 0xff000000;
				}
				cPtr++;

				if (Amount == 0)
				{
					Amount = *cPtr++;
					if (Amount == 0)
					{
						Amount = b3Endian::b3GetMot16(cPtr);
						cPtr += 2;
					}
				}
				for (k = 0; k < Amount; k++)
				{
					*dstPtr++ = Color;
				}
				i += Amount;
			}
			break;
		}
		Pos += (b3Endian::b3GetMot32(&srcPtr[1]) + 8);
		if (Pos & 1)
		{
			Pos++;
		}
	}
	type     = B3_TX_RGB8;
	FileType = FT_RGB8;
	return B3_OK;
}

b3_result b3Tx::b3ParseIFF_RGB4(const b3_u08 * buffer, b3_size buffer_size)
{
	b3_u08 * CharData;
	b3_u32 * LongData;
	b3_u16 * dstPtr;
	b3_u32   Pos = 12, Max, i = 0;

	b3PrintF(B3LOG_FULL, "IMG IFF  # b3ParseIFF_RGB4(%s)\n",
		(const char *)image_name);

	palette  = nullptr;
	FileType = FT_RGB4;
	while (Pos < buffer_size)
	{
		LongData = (b3_u32 *)&buffer[Pos];
		CharData = (b3_u08 *)LongData;
		switch (b3Endian::b3GetMot32(LongData))
		{
		case IFF_BMHD :
			xSize	= b3Endian::b3GetMot16(&CharData[ 8]);
			ySize	= b3Endian::b3GetMot16(&CharData[10]);
			depth	= CharData[16] & 0xc;
			break;

		case IFF_BODY :
			Max    = xSize * ySize;
			dstPtr = b3TypedAlloc<b3_u16>(Max);
			data   = dstPtr;
			dSize  = Max * sizeof(b3_u16);
			if (data == nullptr)
			{
				b3FreeTx();
				b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
				B3_THROW(b3TxException, B3_TX_MEMORY);
			}

			CharData += 8;
			while (i < Max)
			{
				b3_u16 Color   = b3_u16(*CharData++) << 8;
				b3_u32 Amount  = *CharData++;

				Color  |= Amount;
				Color >>= 4;

				if ((Amount & 0x8) == 0)
				{
					Color |= 0xf000;
				}
				Amount &= 7;
				if (Amount == 0)
				{
					Amount = *CharData++;
					if (Amount == 0)
					{
						Amount = b3Endian::b3GetMot16(CharData);
						CharData += 2;
					}
				}
				for (b3_u32 k = 0; k < Amount; k++)
				{
					*dstPtr++ = Color;
				}
				i += Amount;
			}
			break;
		}
		Pos += (b3Endian::b3GetMot32(&LongData[1]) + 8);
		if (Pos & 1)
		{
			Pos++;
		}
	}
	type = B3_TX_RGB4;
	return B3_OK;
}

/*************************************************************************
**                                                                      **
**                       IFF-ILBM-Format (Amiga)                        **
**                                                                      **
*************************************************************************/

void b3Tx::b3EHBPalette()
{
	b3_pkd_color * OldPalette;
	b3_pkd_color * NewPalette, i, Color;

	FileType = FT_ILBM_EHB;
	pSize    = 64;
	NewPalette = b3TypedAlloc<b3_pkd_color>(pSize);
	if (NewPalette == nullptr)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	OldPalette = palette;
	for (i = 0; i < 32; i++)
	{
		NewPalette[i]		= (Color = OldPalette[i]);
		if (Color & 0x00100000)
		{
			Color |= 0x00010000;
		}
		if (Color & 0x00001000)
		{
			Color |= 0x00000100;
		}
		NewPalette[i + 32]	= (Color >> 1) & 0x00707070;
	}
	b3Free(OldPalette);
	palette = NewPalette;
	type    = B3_TX_ILBM;
}

void b3Tx::b3ConvertILBMLine(
	b3_u08  * Line,
	b3_u08  * Interleave,
	b3_res    xMax,
	b3_count  Planes)
{
	long			 p, offset, x, Width;
	b3_u08	 Color;

	Width  = ((xMax + 15) & 0x7ffffff0) >> 3;
	offset = Planes * Width;
	for (x = 0; x < xMax; x++)
	{
		Color = 0;
		Interleave += offset;
		for (p = 0; p < Planes; p++)
		{
			Interleave -= Width;
			Color      += Color;
			if (b3_tx_yuv_table::m_ConvertBits[x & 7] & Interleave[0])
			{
				Color |= 1;
			}
		}
		if ((x & 7) == 7)
		{
			Interleave++;
		}
		Line[0] = Color;
		Line++;
	}
}

void b3Tx::b3HamPalette(bool HAM8)
{
	b3_u32  *  LData;
	b3_u08  *  OldData;
	b3_u16  *  NewData;
	b3_u16  *  sData;
	b3_u08  *  Line;
	b3_u32     Color;
	b3_offset  offset;
	b3_coord   x, y, Nibble;

	if (HAM8)
	{
		FileType = FT_ILBM_HAM8;
		NewData = b3TypedAlloc<b3_u16>(xSize * ySize);
	}
	else
	{
		FileType = FT_ILBM_HAM;
		NewData = b3TypedAlloc<b3_u16>(xSize * ySize);
	}
	if (NewData == nullptr)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	Line    = b3TypedAlloc<b3_u08>(xSize);
	if (Line == nullptr)
	{
		b3Free(NewData);
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	OldData = data;
	sData   = NewData;
	offset  = (((xSize + 15) & 0x7fffff0) >> 3) * depth;
	if (HAM8)
	{
		LData  = (b3_u32 *)NewData;
		for (y = 0; y < ySize; y++)
		{
			b3ConvertILBMLine(Line, OldData, xSize, depth);
			Color  = palette[0];
			for (x = 0; x < xSize; x++)
			{
				Nibble = Line[x] & 0x3f;
				switch (Line[x] & 0xc0)
				{
				case 0x00 : /* mode */
					Color  = palette[Nibble];
					break;
				case 0x40 : /* hold & modify blue */
					Color = (Color & 0xffff00) | (Nibble <<  2);
					break;
				case 0x80 : /* hold & modify red */
					Color = (Color & 0x00ffff) | (Nibble << 18);
					break;
				case 0xc0 : /* hold & modify green */
					Color = (Color & 0xff00ff) | (Nibble << 10);
					break;
				}
				LData[0] = Color;
				LData++;
			}
			OldData += offset;
		}
		depth = 24;
	}
	else
	{
		for (y = 0; y < ySize; y++)
		{
			b3ConvertILBMLine(Line, OldData, xSize, depth);
			Color  = (palette[0] & 0xf00000) >> 12;
			Color |= (palette[0] & 0x00f000) >>  8;
			Color |= (palette[0] & 0x0000f0) >>  4;
			for (x = 0; x < xSize; x++)
			{
				Nibble = (long)Line[x] & 0x0f;
				switch (Line[x] & 0x30)
				{
				case 0x00 : /* mode */
					Color  = (palette[Nibble] & 0xf00000) >> 12;
					Color |= (palette[Nibble] & 0x00f000) >>  8;
					Color |= (palette[Nibble] & 0x0000f0) >>  4;
					break;
				case 0x10 : /* hold & modify blue */
					Color = (Color & 0x0ff0) |  Nibble;
					break;
				case 0x20 : /* hold & modify red */
					Color = (Color & 0x00ff) | (Nibble << 8);
					break;
				case 0x30 : /* hold & modify green */
					Color = (Color & 0x0f0f) | (Nibble << 4);
					break;
				}
				NewData[0] = Color;
				NewData++;
			}
			OldData += offset;
		}
		depth  = 12;
	}
	b3Free(Line);
	b3Free(palette);
	b3Free(data);
	data    = (b3_u08 *)sData;
	palette = nullptr;

	type = (HAM8 ? B3_TX_RGB8 : B3_TX_RGB4);
}

b3_result b3Tx::b3ParseIFF_ILBM(const b3_u08 * buffer, b3_size buffer_size)
{
	b3_u08 * Copy;
	b3_u08 * CharData;
	b3_u32   Code, i;
	b3_u32   Max, k, Pos = 12;
	b3_u32 * Set;
	b3_u32 * LongData;
	bool     Compressed = false, Ham = false, EHB = false, Ham8 = false;

	b3PrintF(B3LOG_FULL, "IMG IFF  # b3ParseIFF_ILBM(%s)\n",
		(const char *)image_name);

	palette	 = nullptr;
	FileType = FT_ILBM;
	while (Pos < buffer_size)
	{
		LongData  = (b3_u32 *)&buffer[Pos];
		CharData  = (b3_u08 *)LongData;
		switch (b3Endian::b3GetMot32(LongData))
		{
		case IFF_BMHD :
			xSize	= b3Endian::b3GetMot16(&CharData[ 8]);
			ySize	= b3Endian::b3GetMot16(&CharData[10]);
			depth	= CharData[16];
			if (depth >= 24)
			{
				FileType = FT_ILBM_24;
			}
			switch (CharData[18])
			{
			case 0 :
				Compressed = false;
				break;
			case 1 :
				Compressed = true;
				break;
			default :
				b3FreeTx();
				b3PrintF(B3LOG_NORMAL, "IMG IFF  # Wrong packing algorithm:\n");
				B3_THROW(b3TxException, B3_TX_ERR_PACKING);
			}
			break;
		case IFF_CMAP :
			Max = b3Endian::b3GetMot32(&LongData[1]) / 3;
			palette = b3TypedAlloc<b3_pkd_color>(Max);
			pSize = Max;
			if (palette == nullptr)
			{
				b3FreeTx();
				b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");

				B3_THROW(b3TxException, B3_TX_MEMORY);
			}
			Set = (b3_u32 *)palette;
			CharData += 8;
			for (k = 0; k < Max; k++)
			{
				const b3_u08 r = *CharData++;
				const b3_u08 g = *CharData++;
				const b3_u08 b = *CharData++;

				*Set++ = b3Color::b3MakePkdColor(r, g, b);
			}
			break;
		case IFF_CAMG :
			if (b3Endian::b3GetMot32(&LongData[2]) & HAM)
			{
				Ham = true;
				if (b3Endian::b3GetMot32(&LongData[2]) & HIRES)
				{
					Ham8 = true;
				}
			}
			else
			{
				if (b3Endian::b3GetMot32(&LongData[2]) & EXTRA_HALFBRITE)
				{
					EHB = true;
				}
			}
			break;

		case IFF_BODY :
			if (Compressed)
			{
				Max  = ((xSize + 15) & 0x7ffffff0) >> 3;
				Max *= (ySize * depth);
				data = b3TypedAlloc<b3_u08>(Max + ySize + Max);
				if (data == nullptr)
				{
					b3FreeTx();
					b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
					B3_THROW(b3TxException, B3_TX_MEMORY);
				}

				Copy = (b3_u08 *)data;
				CharData += 8;
				k = 0;
				while (k < Max)
				{
					Code = (b3_u32)CharData[0];
					CharData++;
					if (Code & 0x80)
					{
						Code = (256 - Code) & 0xff;
						for (i = 0; i <= Code; i++)
						{
							Copy[0] = CharData[0];
							Copy += 1;
						}
						CharData += 1;
					}
					else
					{
						for (i = 0; i <= Code; i++)
						{
							Copy[0] = CharData[0];
							CharData++;
							Copy++;
						}
					}
					k += (Code + 1);
				}
			}
			else
			{
				Max = b3Endian::b3GetMot32(&LongData[1]);
				data = b3TypedAlloc<b3_u08>(Max);
				if (data == nullptr)
				{
					b3FreeTx();
					b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
					B3_THROW(b3TxException, B3_TX_MEMORY);
				}
				Copy = (b3_u08 *)data;
				CharData += 8;
				memcpy(data, CharData, Max);
			}
			break;
		}
		LongData++;
		Pos += (b3Endian::b3GetMot32(LongData) + 8);
		if (Pos & 1)
		{
			Pos++;
		}
	}
	if (EHB)
	{
		b3EHBPalette();
	}
	if (Ham)
	{
		b3HamPalette(Ham8);
	}
	type = B3_TX_ILBM;
	return B3_OK;
}

/*************************************************************************
**                                                                      **
**                       IFF-YUVN-Format (Amiga)                        **
**                                                                      **
*************************************************************************/

inline b3_u32 b3Tx::b3ShiftCount(b3_count Count)
{
	b3_u32 Shift = 0;

	while (Count > 1)
	{
		Shift++;
		Count = Count >> 1;
	}
	return Shift;
}

b3_result b3Tx::b3ParseIFF_YUVN(const b3_u08 * buffer, b3_size buffer_size)
{
	b3_u08 * Y = nullptr;
	b3_u08 * U = nullptr;
	b3_u08 * V = nullptr;
	b3_u08 * CharData;
	b3_u32 * LongData;
	b3_u08  y, u, v, Uprev, Vprev;
	b3_u32  i, k, Max, Pos = 12, Count = 0, Shift;

	b3PrintF(B3LOG_FULL, "IMG IFF  # b3ParseIFF_YUVN(%s)\n",
		(const char *)image_name);

	palette  = nullptr;
	FileType = FT_YUV;
	LongData = (b3_u32 *)&buffer[Pos];
	CharData = (b3_u08 *)LongData;

	while (Pos < buffer_size)
	{
		LongData = (b3_u32 *)CharData;
		switch (b3Endian::b3GetMot32(LongData))
		{
		case IFF_YCHD :
			xSize	= b3Endian::b3GetMot16(&CharData[ 8]);
			ySize	= b3Endian::b3GetMot16(&CharData[10]);
			depth	= 24;
			switch (CharData[22])
			{
			case 0 :
				break;

			default :
				b3FreeTx();
				b3PrintF(B3LOG_NORMAL, "IMG IFF  # Wrong packing algorithm:\n");

				B3_THROW(b3TxException, B3_TX_ERR_PACKING);
			}
			switch (CharData[24])
			{
			case YCHD_MODE_200 :
			case YCHD_MODE_400 :
				Count = 0;
				break;

			case YCHD_MODE_222 :
			case YCHD_MODE_444 :
				Count = 1;
				break;

			case YCHD_MODE_422 :
			case YCHD_MODE_211 :
				Count = 2;
				if (xSize & 1)
				{
					b3FreeTx();
					b3PrintF(B3LOG_NORMAL, "IMG IFF  # Wrong resolution:\n");
					B3_THROW(b3TxException, B3_TX_ERR_HEADER);
				}
				break;

			case YCHD_MODE_411 :
				Count = 4;
				if (xSize & 3)
				{
					b3FreeTx();
					b3PrintF(B3LOG_NORMAL, "IMG IFF  # Wrong resolution:\n");
					B3_THROW(b3TxException, B3_TX_ERR_HEADER);
				}
				break;

			default :
				Count = 1;
				break;
			}
			break;

		case IFF_DATY :
			Y = (b3_u08 *)(CharData + 8);
			break;
		case IFF_DATU :
			U = (b3_u08 *)(CharData + 8);
			break;
		case IFF_DATV :
			V = (b3_u08 *)(CharData + 8);
			break;

		}
		Pos			+= ((b3Endian::b3GetMot32(&LongData[1]) + 9) & 0xfffffffe);
		CharData	+= ((b3Endian::b3GetMot32(&LongData[1]) + 9) & 0xfffffffe);
	}
	Max = xSize * ySize;

	if (Count == 0)		/* schwarz weiss  */
	{
		if (Y == nullptr)
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		if (!b3AllocTx(xSize, ySize, 8))
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		memcpy(data, Y, Max);
	}
	else					/* bunt */
	{
		if ((Y == nullptr) || (U == nullptr) || (V == nullptr))
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_ERR_HEADER);
		}
		if (!b3AllocTx(xSize, ySize, 24))
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG IFF  # Error allocating memory:\n");
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}
		LongData = (b3_pkd_color *)data;

		Uprev = U[0];
		Vprev = V[0];
		Shift = b3ShiftCount(Count);
		for (k = 0; k < Max; k += Count)
		{
			b3_u16 Uprop, Vprop;

			Uprop = (b3_u16)Uprev << Shift;
			Vprop = (b3_u16)Vprev << Shift;
			for (i = 0; i < Count; i++)
			{
				y = Y[0];
				u = Uprop >> Shift;
				v = Vprop >> Shift;
				*LongData++ =
					b3_tx_yuv_table::m_MultYuvTable.MultR[y + b3_tx_yuv_table::m_MultYuvTable.MultRV[v]] |
					b3_tx_yuv_table::m_MultYuvTable.MultG[y + b3_tx_yuv_table::m_MultYuvTable.MultGU[u] + b3_tx_yuv_table::m_MultYuvTable.MultGV[v]] |
					b3_tx_yuv_table::m_MultYuvTable.MultB[y + b3_tx_yuv_table::m_MultYuvTable.MultBU[u]];

				Uprop -= Uprev;
				Vprop -= Vprev;
				Uprop += U[0];
				Vprop += V[0];
				Y++;
			}
			Uprev  = *U++;
			Vprev  = *V++;
		}
	}

	return B3_OK;
}
