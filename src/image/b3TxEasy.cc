/*
**
**	$Filename:	b3TxEasy.cc $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading simple uncompressed images
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

#include <blz3/image/b3Tx.h>

#include "b3TxEasy.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**
**	
*/

/*************************************************************************
**                                                                      **
**                       Rayshade MTV-Format                            **
**                                                                      **
*************************************************************************/

long ParseRAW (
	struct Texture *Texture,
	unsigned char  *Data,
	long            x,
	long            y,
	long            type)
{
	register unsigned char *newCData;
	register unsigned long *newLData,value;
	register long           i,Max;

	switch (type)
	{
		case 4 : /* bitmap */
			Texture->xSize  = x;
			Texture->ySize  = y;
			Texture->Planes = 1;
			Max             = ((x + 7) >> 3) * y;

			Texture->Palette = AllocTextureMem
				(Texture,2 * sizeof(unsigned long));
			if (Texture->Palette == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			Texture->Palette[0] = 0x00ffffff;
			Texture->Palette[1] = 0x00000000;

			newCData = AllocTextureMem (Texture,Max * sizeof(unsigned char));
			if (newCData == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			Texture->Data   = newCData;

			for (i = 0;i < Max;i++) *newCData++ = *Data++;
			return ILBM;

		case 5 : /* grey */
			Texture->xSize  = x;
			Texture->ySize  = y;
			Texture->Planes = 8;
			Max             = x * y;

			Texture->Palette = AllocTextureMem
				(Texture,256 * sizeof(unsigned long));
			if (Texture->Palette == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}

			newCData = AllocTextureMem (Texture,Max * sizeof(unsigned char));
			if (newCData == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			Texture->Data   = newCData;

			for (i = 0;i < 256;i++) Texture->Palette[i] = 0x00010101 * i;
			for (i = 0;i < Max;i++) *newCData++ = *Data++;

			return PCX8;

		case 6 : /* 24 Bit */
			Texture->xSize  = x;
			Texture->ySize  = y;
			Texture->Planes = 24;
			Max             = x * y;

			newLData = AllocTextureMem (Texture,Max * sizeof(unsigned long));
			if (newLData == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			Texture->Data = (unsigned char *)newLData;

			for (i = 0;i < Max;i++)
			{
				value       =                 *Data++;
				value       = (value << 8) | (*Data++);
				*newLData++ = (value << 8) | (*Data++);
			}
			return RGB8;

		default :
			return 0;
	}
}

/*************************************************************************
**                                                                      **
**                        BMP format                                    **
**                                                                      **
*************************************************************************/

long ParseBMP(
	struct Texture *Texture,
	unsigned char  *data)
{
	register long           xSize,ySize,colors,i,x,y,offset,planes,size,value;
	register unsigned long *palette,*Long;
	register unsigned char *new;

	if (GetIntelLong (&data[30]) != 0)
	{
		Texture->FileType = FT_ERR_PACKING;
		return 0;
	}
	xSize  = GetIntelShort (&data[18]);
	ySize  = GetIntelShort (&data[22]);
	colors = GetIntelLong  (&data[46]);
	planes = GetIntelLong  (&data[28]);
	if (colors == 0) colors = (GetIntelLong(&data[10]) - 54) >> 2;

	if (planes <= 8)
	{
		if (colors > 0)
		{
			palette = (unsigned long *)AllocTextureMem(Texture,
				colors * sizeof(unsigned long));
			if (palette == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			for (i=0;i<colors;i++) palette[i] = 
				((unsigned long)data[(i << 2) + 56] << 16) |
				((unsigned long)data[(i << 2) + 55] <<  8) |
				((unsigned long)data[(i << 2) + 54]);
		}
		else
		{
			colors = 1 << planes;
			value  = 8  - planes;
			palette = (unsigned long *)AllocTextureMem(Texture,
				colors * sizeof(unsigned long));
			if (palette == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			for (i=0;i<colors;i++) palette[i] = 0x00010101 * (i << value);
		}
	}

	switch (planes)
	{
		case  1 :
			size = ((xSize + 7) >> 3) * ySize * sizeof(unsigned char);
			break;

		case  4 :
			if (xSize & 1) xSize++;
			size = xSize * ySize * sizeof(unsigned char) >> 1;
			break;

		case  8 :
			size = xSize * ySize * sizeof(unsigned char);
			break;

		case 24 :
			size = xSize * ySize * sizeof(unsigned long);
			break;
	}

	new = (unsigned char *)AllocTextureMem (Texture,size);
	if (new == null)
	{
		Texture->FileType = FT_ERR_MEM;
		return 0;
	}

	Texture->xSize    = xSize;
	Texture->ySize    = ySize;
	Texture->Palette  = palette;
	Texture->Data     = new;
	Texture->FileType = FT_BMP;
	Texture->Planes   = planes;


		/* checking bits per pixel */
	data  += GetIntelLong (&data[10]);
	switch (planes)
	{
		case  1 :
			offset = ((xSize + 7) >> 3);
			new   += size;
			for (y=0;y<ySize;y++)
			{
				new -= offset;
				for (x=0;x<xSize;x+=8) *new++ = *data++;
				new -= offset;
			}
			return ILBM;

		case  4 :
			offset = (xSize + 7) >> 3;
			new   += size;
			for (y=0;y<ySize;y++)
			{
				new -= (offset << 2);
				for (x=0;x<xSize;x+=2)
				{
					value = 128 >> (x & 7);
					i     =  (x >> 3) + offset + offset + offset;
					if (data[0] & 0x80) new[i] |= value;
					i -= offset;
					if (data[0] & 0x40) new[i] |= value;
					i -= offset;
					if (data[0] & 0x20) new[i] |= value;
					i -= offset;
					if (data[0] & 0x10) new[i] |= value;

					value = value >> 1;
					i     =  (x >> 3) + offset + offset + offset;
					if (data[0] & 0x08) new[i] |= value;
					i -= offset;
					if (data[0] & 0x04) new[i] |= value;
					i -= offset;
					if (data[0] & 0x02) new[i] |= value;
					i -= offset;
					if (data[0] & 0x01) new[i] |= value;
					data++;
				}
			}
			return ILBM;

		case  8 :
			new += size;
			for (y=0;y<ySize;y++)
			{
				new -= xSize;
				for (x=0;x<xSize;x++) *new++ = *data++;
				new -= xSize;
			}
			return PCX8;

		case 24 :
			offset = xSize & 3;
			Long = (unsigned long *)new;
			Long += (xSize * ySize);
			for (y=0;y<ySize;y++)
			{
				Long -= xSize;
				for (x=0;x<xSize;x++)
				{
					value   =                data[2];
					value   = (value << 8) | data[1];
					*Long++ = (value << 8) | data[0];
					data += 3;
				}
				Long -= xSize;
				data += offset;
			}
			return RGB8;
	}

	if (DBUG(2)) PrintF ("that's it\n");
	return 0;
}

/*************************************************************************
**                                                                      **
**                        BMF files                                     **
**                                                                      **
*************************************************************************/

long ParseBMF (
	struct Texture *Texture,
	unsigned char  *Data,
	unsigned long   Size)
{
	long           x,y,xSize,ySize,Type,lSize;
	unsigned long *pixel;
	unsigned char *gray;

	Texture->FileType = FT_BMF;
	Texture->xSize    = xSize = GetIntelShort (&Data[2]);
	Texture->ySize    = ySize = GetIntelShort (&Data[4]);
	switch (Type = Data[6])
	{
		case 2 :
			Texture->Planes = 8;
			Texture->Data   = AllocTextureMem (Texture,
				xSize * ySize +
				sizeof(long) * 256);
			if (Texture->Data == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}

				/* creating gray ramp */
			Texture->Palette = (unsigned long *)Texture->Data;
			for (x=0;x<256;x++) Texture->Palette[x] = 0x010101 * x;

				/* copying Data */
			Texture->Data += (sizeof(unsigned long) * 256);
			Data += Size;
			gray  = Texture->Data;
			for (y = 0;y < ySize;y++)
			{
				Data -= xSize;
				for (x = 0;x < xSize;x++) *gray++ = Data[x];
			}
			Type = PCX8;
			break;

		case 4 :
			Texture->Planes   = 24;
			Texture->Data     = AllocTextureMem (Texture,
				xSize * ySize * sizeof(long));
			if (Texture->Data == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			pixel = (unsigned long *)Texture->Data;
			Data += Size;
			lSize = xSize + xSize + xSize;
			for (y=0;y < ySize;y++)
			{
				Data -= lSize;
				for (x = 0;x < xSize;x++)
				{
					*pixel++ =
						(Data[x]                 <<  0) |
						(Data[x + xSize]         <<  8) |
						(Data[x + xSize + xSize] << 16);
				}
			}
			Type = RGB8;
			break;
		default :
			Type = 0;
			break;
	}
	return Type;
}
