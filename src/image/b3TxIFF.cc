/*
**
**	$Filename:	b3TxIFF.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading IFF images
**
**	(C) Copyright 2001  Steffen A. Mork
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
#include "blz3/base/b3Endian.h"

#include "b3TxIFF.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**
**	Revision 1.2  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.1  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        local structures                              **
**                                                                      **
*************************************************************************/

b3_yuv_table   mult_yuv_table;
static b3_bool mult_yuv_table_loaded = false;

static b3_u08 ConvertBits[8] =
{
	128,64,32,16,8,4,2,1
};

/*************************************************************************
**                                                                      **
**                       Impulse IFF-RGBx-Format                        **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseIFF_RGB8 (b3_u08 *buffer,b3_size buffer_size)
{
	b3_u08 *CharData;
	b3_u32 *Set;
	b3_u32 *LongData;
	b3_u32  Amount,k,Colour,Pos = 12,Max,i=0;

	palette	 = null;
	while (Pos < buffer_size)
	{
		LongData = (b3_u32 *)&buffer[Pos];
		CharData = (b3_u08 *)LongData;
		switch (b3Endian::b3GetMot32(LongData))
		{
			case IFF_BMHD :
				xSize	= b3Endian::b3GetMot16 (&CharData[ 8]);
				ySize	= b3Endian::b3GetMot16 (&CharData[10]);
				depth	= CharData[16];
				break;

			case IFF_BODY :
				Max  = xSize * ySize;
				data = (b3_u08 *)b3Alloc(Max * 4);
				if (data==null)
				{
					b3FreeTx();
					throw new b3TxException(B3_TX_MEMORY);
				}
				Set = (b3_u32 *)data;
                
				CharData += 8;
				while (i < Max)
				{
					Colour  = (long)CharData[0] * 65536;	CharData++;
					Colour += (long)CharData[0] * 256;		CharData++;
					Colour += (long)CharData[0];
					CharData++;
					Amount  = CharData[0] & 0x7f;
					if ((CharData[0] & 0x80) == 0)
					{
						Colour |= 0xff000000;
					}
					CharData++;

					if (Amount == 0)
					{
						Amount = CharData[0];
						CharData++;
						if (Amount == 0)
						{
							Amount = b3Endian::b3GetMot16(CharData);
							CharData += 2;
						}
					}
					for (k = 0;k < Amount;k++)
					{
						Set[0] = Colour;
						Set++;
					}
					i += Amount;
				}
				break;
		}
		Pos += (b3Endian::b3GetMot32 (&LongData[1])+8);
		if (Pos & 1) Pos++;
	}
	type     = B3_TX_RGB8;
	FileType = FT_RGB8;
	return B3_OK;
}

b3_result b3Tx::b3ParseIFF_RGB4 (b3_u08 *buffer,b3_size buffer_size)
{
	b3_u08  *CharData;
	b3_u32  *LongData;
	b3_u16 *Set;
	b3_u32  Amount,k,Pos = 12,Max,i=0;
	b3_u16  Colour;

	palette  = null;
	FileType = FT_RGB4;
	while (Pos < buffer_size)
	{
		LongData = (b3_u32 *)&buffer[Pos];
		CharData = (b3_u08 *)LongData;
		switch (b3Endian::b3GetMot32(LongData))
		{
			case IFF_BMHD :
				xSize	= b3Endian::b3GetMot16 (&CharData[ 8]);
				ySize	= b3Endian::b3GetMot16 (&CharData[10]);
				depth	= CharData[16];
				break;
			case IFF_BODY :
				Max = xSize * ySize;
				data = (b3_u08 *)b3Alloc(Max * 2);
				if (data==null)
				{
					b3FreeTx();
					throw new b3TxException(B3_TX_MEMORY);
				}
				Set = (b3_u16 *)data;
                
				CharData += 8;
				while (i < Max)
				{
					Colour  = (b3_u16)CharData[0] << 8;	CharData++;
					Amount  = (b3_u32) CharData[0];		CharData++;
					Colour |= (b3_u16)Amount;
					Colour  = Colour >> 4;
					if ((Amount & 0x8)==0) Colour |= 0xf000;
					Amount &= 7;
					if (Amount == 0)
					{
						Amount = CharData[0];
						CharData++;
						if (Amount == 0)
						{
							Amount = b3Endian::b3GetMot16 (CharData);
							CharData += 2;
						}
					}
					for (k = 0;k < Amount;k++)
					{
						Set[0] = Colour;
						Set++;
					}
					i += Amount;
				}
				break;
		}
		Pos += (b3Endian::b3GetMot32 (&LongData[1])+8);
		if (Pos & 1) Pos++;
	}
	type = B3_TX_RGB4;
	return B3_OK;
}

/*************************************************************************
**                                                                      **
**                       IFF-ILBM-Format (Amiga)                        **
**                                                                      **
*************************************************************************/

void b3Tx::b3EHBPalette ()
{
	b3_u32 *OldPalette;
	b3_u32 *NewPalette,i,Color;

	FileType = FT_ILBM_EHB;
	pSize    = 64;
	NewPalette = (b3_u32 *)b3Alloc(pSize * sizeof(b3_u32));
	if (NewPalette==null)
	{
		b3FreeTx();
		throw new b3TxException(B3_TX_MEMORY);
	}
	OldPalette = palette;
	for (i = 0;i < 32;i++)
	{
		NewPalette[i]		= (Color = OldPalette[i]);
		if (Color & 0x00100000) Color |= 0x00010000;
		if (Color & 0x00001000) Color |= 0x00000100;
		NewPalette[i+32]	= (Color >> 1) & 0x00707070;
	}
	b3Free (OldPalette);
	palette = NewPalette;
	type    = B3_TX_ILBM;
}

void b3Tx::b3ConvertILBMLine (
	b3_u08   *Line,
	b3_u08   *Interleave,
	b3_res    xMax,
	b3_count  Planes)
{
	long			 p,offset,x,Width;
	b3_u08	 Color;

	Width  = ((xMax + 15) & 0x7ffffff0) >> 3;
	offset = Planes * Width;
	for (x = 0;x < xMax;x++)
	{
		Color = 0;
		Interleave += offset;
		for (p=0;p<Planes;p++)
		{
			Interleave -= Width;
			Color      += Color;
			if (ConvertBits[x & 7] & Interleave[0]) Color |= 1;
		}
		if ((x & 7)==7) Interleave++;
		Line[0] = Color;
		Line++;
	}
}

void b3Tx::b3HamPalette (b3_bool HAM8)
{
	b3_u32    *LData;
	b3_u08    *OldData;
	b3_u16    *NewData;
	b3_u16    *sData;
	b3_u08    *Line;
	b3_u32     Color;
	b3_offset  offset;
	b3_coord   x,y,Nibble;

	if (HAM8 )
	{
		FileType = FT_ILBM_HAM8;
		NewData = (b3_u16 *)b3Alloc(xSize * ySize * sizeof(b3_u32));
	}
	else
	{
		FileType = FT_ILBM_HAM;
		NewData = (b3_u16 *)b3Alloc(xSize * ySize * sizeof(b3_u16));
	}
	if (NewData == null)
	{
		b3FreeTx();
		throw new b3TxException(B3_TX_MEMORY);
	}
	Line    = (b3_u08 *)b3Alloc(xSize);
	if (Line==null)
	{
		b3Free (NewData);
		b3FreeTx();
		throw new b3TxException(B3_TX_MEMORY);
	}
	OldData = data;
	sData   = NewData;
	offset  = (((xSize + 15) & 0x7fffff0) >> 3) * depth;
	if (HAM8)
	{
		LData  = (b3_u32 *)NewData;
		for (y = 0;y<ySize;y++)
		{
			b3ConvertILBMLine (Line,OldData,xSize,depth);
			Color  = palette[0];
			for (x=0;x<xSize;x++)
			{
				Nibble = Line[x] & 0x3f;
				switch  (Line[x] & 0xc0)
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
						Color = (Color & 0xff00ff) | (Nibble << 10);						break;
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
		for (y = 0;y < ySize;y++)
		{
			b3ConvertILBMLine (Line,OldData,xSize,depth);
			Color  = (palette[0] & 0xf00000) >> 12;
			Color |= (palette[0] & 0x00f000) >>  8;
			Color |= (palette[0] & 0x0000f0) >>  4;
			for (x = 0;x < xSize;x++)
			{
				Nibble = (long)Line[x] & 0x0f;
				switch  (Line[x] & 0x30)
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
	palette = null;

	type = (HAM8 ? B3_TX_RGB8 : B3_TX_RGB4);
}

b3_result b3Tx::b3ParseIFF_ILBM (b3_u08 *buffer,b3_size buffer_size)
{
	b3_u08  *Copy;
	b3_u08  *CharData;
	b3_u32   Code,i;
	b3_u32   Max,k,Colour,Pos = 12;
	b3_u32  *Set;
	b3_u32  *LongData;
	b3_bool  Compressed=false,Ham=false,EHB=false,Ham8=false;

	palette	 = null;
	FileType = FT_ILBM;
	while (Pos < buffer_size)
	{
		LongData  = (b3_u32 *)&buffer[Pos];
		CharData  = (b3_u08 *)LongData;
		switch (b3Endian::b3GetMot32(LongData))
		{
			case IFF_BMHD :
				xSize	= b3Endian::b3GetMot16 (&CharData[ 8]);
				ySize	= b3Endian::b3GetMot16 (&CharData[10]);
				depth	= CharData[16];
				if (depth >= 24)
				{
					FileType = FT_ILBM_24;
				}
				switch (CharData[18])
				{
					case 0 : Compressed = false; break;
					case 1 : Compressed = true;  break;
					default :
						b3FreeTx();
						throw new b3TxException(B3_TX_ERR_PACKING);
				}
				break;
			case IFF_CMAP :
				Max = b3Endian::b3GetMot32(&LongData[1]) / 3;
				palette = (b3_u32 *)b3Alloc(Max * sizeof(b3_u32));
				if (palette==null)
				{
					b3FreeTx();
					throw new b3TxException(B3_TX_MEMORY);
				}
				Set = (b3_u32 *)palette;
				CharData += 8;
				for (k = 0;k < Max;k++)
				{
					Colour  = ((b3_u32)CharData[0] << 16); CharData++;
					Colour |= ((b3_u32)CharData[0] <<  8); CharData++;
					Colour |=  (b3_u32)CharData[0];        CharData++;
					Set[0] = Colour;
					Set++;
				}
				break;
			case IFF_CAMG :
				if (b3Endian::b3GetMot32(&LongData[2]) & HAM)
				{
					Ham = true;
					if  (b3Endian::b3GetMot32(&LongData[2]) & HIRES)
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
					Max *=  (ySize * depth);
					data = (b3_u08 *)b3Alloc(Max + ySize + Max);
					if (data==null)
					{
						b3FreeTx();
						throw new b3TxException(B3_TX_MEMORY);
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
							for (i = 0;i<=Code;i++)
							{
								Copy[0] = CharData[0];
								Copy+=1;
							}
							CharData+=1;
						}
						else
						{
							for (i=0;i<=Code;i++)
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
					Max = b3Endian::b3GetMot32 (&LongData[1]);
					data = (b3_u08 *)b3Alloc(Max);
					if (data==null)
					{
						b3FreeTx();
						throw new b3TxException(B3_TX_MEMORY);
					}
					Copy = (b3_u08 *)data;
					CharData += 8;
					memcpy (data,CharData,Max);
				}
				break;
		}
		LongData++;
		Pos += (b3Endian::b3GetMot32(LongData) + 8);
		if (Pos & 1) Pos++;
	}
	if (EHB)
	{
		b3EHBPalette ();
	}
	if (Ham)
	{
		b3HamPalette (Ham8);
	}
	type = B3_TX_ILBM;
	return B3_OK;
}

/*************************************************************************
**                                                                      **
**                       IFF-YUVN-Format (Amiga)                        **
**                                                                      **
*************************************************************************/

b3_bool b3Tx::b3CalcYUVTable ()
{
	long				 i;
	double				 Rad;

	if (!mult_yuv_table_loaded)
	{
		for (i = 0;i < 256;i++)
		{
			Rad = (i - 128.0) * 0.8588;
			mult_yuv_table.MultRV[i] = 256 + (long)(Rad *  1.587);  /*  = 156 / 112 *  1.14  */
			mult_yuv_table.MultGU[i] = 128 + (long)(Rad * -0.394);  /*  = 112 / 112 * -0.394 */
			mult_yuv_table.MultGV[i] = 128 + (long)(Rad * -0.8092); /*  = 156 / 112 * -0.581 */
			mult_yuv_table.MultBU[i] = 256 + (long)(Rad *  2.028);  /*  = 112 / 112 *  2.028 */

			Rad = i -  16.0;
			mult_yuv_table.MultR[i+256] = (b3_u32)(Rad * 1.1644) << 16; /*  = 255 / (235 - 16) */
			mult_yuv_table.MultG[i+256] = (b3_u32)(Rad * 1.1644) <<  8; /*  = 255 / (235 - 16) */
			mult_yuv_table.MultB[i+256] = (b3_u32)(Rad * 1.1644);       /*  = 255 / (235 - 16) */
			if (i <  16)
			{
				mult_yuv_table.MultR[i+256] =   0;
				mult_yuv_table.MultG[i+256] =   0;
				mult_yuv_table.MultB[i+256] =   0;
			}
			if (i > 235)
			{
				mult_yuv_table.MultR[i+256] = 0xff0000;
				mult_yuv_table.MultG[i+256] = 0x00ff00;
				mult_yuv_table.MultB[i+256] = 0x0000ff;
			}

			mult_yuv_table.MultR[i]     = 0;
			mult_yuv_table.MultG[i]     = 0;
			mult_yuv_table.MultB[i]     = 0;
			mult_yuv_table.MultR[i+512] = 0xff0000; 
			mult_yuv_table.MultG[i+512] = 0x00ff00; 
			mult_yuv_table.MultB[i+512] = 0x0000ff; 
		}
		mult_yuv_table_loaded = true;
	}
	return true;
}

static inline b3_u32 b3ShiftCount(b3_count Count)
{
	b3_u32 Shift = 0;

	while (Count > 1)
	{
		Shift++;
		Count = Count >> 1;
	}
	return Shift;
}

b3_result b3Tx::b3ParseIFF_YUVN (b3_u08 *buffer,b3_size buffer_size)
{
	b3_u08 *Y = null;
	b3_u08 *U = null;
	b3_u08 *V = null;
	b3_u08 *CharData;
	b3_u32 *LongData;
	b3_u08  y,u,v,Uprev,Vprev;
	b3_u32  i,k,Max,Pos = 12,Count = 0,Shift;

	palette  = null;
	FileType = FT_YUV;
	LongData = (b3_u32 *)&buffer[Pos];
	CharData = (b3_u08 *)LongData;

	while (Pos < buffer_size)
	{
		LongData = (b3_u32 *)CharData;
		switch (b3Endian::b3GetMot32(LongData))
		{
			case IFF_YCHD :
				xSize	= b3Endian::b3GetMot16 (&CharData[ 8]);
				ySize	= b3Endian::b3GetMot16 (&CharData[10]);
				depth	= 24;
				switch (CharData[22])
				{
					case 0 :
						break;

					default :
						b3FreeTx();
						throw new b3TxException(B3_TX_ERR_PACKING);
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
							throw new b3TxException(B3_TX_ERR_HEADER);
						}
						break;

					case YCHD_MODE_411 :
						Count = 4;
						if (xSize & 3)
						{
							b3FreeTx();
							throw new b3TxException(B3_TX_ERR_HEADER);
						}
						break;

					default :
						Count = 1;
						break;
				}
				break;

			case IFF_DATY :
				Y = (b3_u08 *)(CharData+8);
				break;
			case IFF_DATU :
				U = (b3_u08 *)(CharData+8);
				break;
			case IFF_DATV :
				V = (b3_u08 *)(CharData+8);
				break;

		}
		Pos			+= ((b3Endian::b3GetMot32(&LongData[1]) + 9) & 0xfffffffe);
		CharData	+= ((b3Endian::b3GetMot32(&LongData[1]) + 9) & 0xfffffffe);
	}
	Max = xSize * ySize;

	if (Count == 0)		/* schwarz weiss  */
	{
		if (Y == null)
		{
			b3FreeTx();
			throw new b3TxException(B3_TX_ERR_HEADER);
		}
		LongData = (b3_u32 *)b3Alloc(Max * sizeof(b3_u32));
		if (LongData == null)
		{
			b3FreeTx();
			throw new b3TxException(B3_TX_MEMORY);
		}
		data = (b3_u08 *)LongData;

		for (k = 0;k < Max;k++)
		{
			LongData++;
			Y++;
		}
	}
	else					/* bunt */
	{
		if ((Y==null) || (U==null) || (V==null))
		{
			b3FreeTx();
			throw new b3TxException(B3_TX_ERR_HEADER);
		}
		if (!b3CalcYUVTable())
		{
			b3FreeTx();
			throw new b3TxException(B3_TX_MEMORY);
		}
		LongData = (b3_u32 *)b3Alloc(Max * sizeof(b3_u32));
		if (LongData==null)
		{
			b3FreeTx();
			throw new b3TxException(B3_TX_MEMORY);
		}
		data = (b3_u08 *)LongData;

		Uprev = U[0];
		Vprev = V[0];
		Shift = b3ShiftCount(Count);
		for (k = 0;k < Max;k += Count)
		{
			b3_u16 Uprop,Vprop;

			Uprop = (b3_u16)Uprev << Shift;
			Vprop = (b3_u16)Vprev << Shift;
			for (i = 0;i < Count;i++)
			{
				y = Y[0];
				u = Uprop >> Shift;
				v = Vprop >> Shift;
				LongData[0] =
					mult_yuv_table.MultR[y + mult_yuv_table.MultRV[v]] |
					mult_yuv_table.MultG[y + mult_yuv_table.MultGU[u] + mult_yuv_table.MultGV[v]] |
					mult_yuv_table.MultB[y + mult_yuv_table.MultBU[u]];

				LongData++;
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
