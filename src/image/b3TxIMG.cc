/*
**
**	$Filename:	b3TxIMG.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading SGI-RLE images
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include <blz3/image/b3Tx.h>

#include "b3TxIMG.h"

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
**                        SGI RLE image format                          **
**                                                                      **
*************************************************************************/

static void UnpackSGI (
	register unsigned char  *buffer,
	register void           *inPtr,
	register long            count,
	register unsigned long   bytes,
	register long            offset)
{
	register unsigned short *sBuffer;
	register unsigned char  *bBuffer;
	register unsigned short  pixel;

	switch (bytes)
	{
		case 1 :
			if (count != 0)
			{
				bBuffer  = inPtr;
				bBuffer += (512 / sizeof(unsigned char) + offset);
				while (count-- > 0) *buffer++ = *bBuffer++;
			}
			else
			{
				bBuffer  = inPtr;
				bBuffer += offset;
				do
				{
				    pixel = *bBuffer++;
				    count = (pixel & 0x7f);
				    if (count == 0) return;
			    	if (pixel & 0x80)
					{
						while(count--) *buffer++ = *bBuffer++;
				    }
					else
					{
						pixel = *bBuffer++;
						while(count--) *buffer++ = pixel;
				    }
				}
				while (true);
			}
			break;

		case 2 :
			if (count != 0)
			{
				bBuffer  = inPtr;
				bBuffer += (512 / sizeof(unsigned short) + offset);
				while (count-- > 0) *buffer++ = *bBuffer++;
			}
			else
			{
				sBuffer  = inPtr;
				sBuffer += offset;
				do
				{
				    pixel = *sBuffer++;
				    count = (pixel & 0x7f);
				    if (count == 0) return;
			    	if (pixel & 0x80)
					{
						while(count--) *buffer++ = *sBuffer++;
				    }
					else
					{
						pixel = *sBuffer++;
						while(count--) *buffer++ = pixel;
				    }
				}
				while (true);
			}
			break;
	}
}

static void ConvertSGILine(
	register unsigned short *Data,
	register long            offset,
	register long            size,
	register long            bytes)
{
	if (bytes != 2) return;
	Data += offset;
	while (size-- > 0) ChangeWord (Data++);
}

static long ParseSGI3(
	struct HeaderSGI *HeaderSGI,
	unsigned char    *Data,
	unsigned char    *new,
	long              type,
	long              xSize,
	long              ySize,
	long              zSize)
{
	register unsigned long     *lineTable;
	register unsigned long     *lineSizes;
	register unsigned long     *lPtr,value;
	register unsigned char     *line;
	register long               x,y,z;
	register long               rle,bytes,block;

	bytes =   HeaderSGI->type & 0x00ff; /* check bytes per pixel */
	rle   = ((HeaderSGI->type & 0xff00) == 0x0100) ? 0 : xSize; /* check RLE */

		/* line buffer */
	line = (unsigned char *)AllocKeyMem(&TextureMem,xSize * zSize * bytes);
	if (line == null) return 0;


		/* convert line offsets */
	lineTable = (unsigned long *)&Data[512];
	lineSizes = (unsigned long *)&lineTable[zSize * ySize];
	if (HeaderSGI->imagic == IMAGIC2) /* check for converting endian */
	{
		if (rle <= 0) /* RLE data */
			for (y=0;y<ySize;y++) for (z=0;z<zSize;z++)
		{
			ChangeLong (&lineTable[y + z * ySize]);
			ChangeLong (&lineSizes[y + z * ySize]);
			ConvertSGILine ((unsigned short *)Data,
				lineTable[y + z * ySize],
				lineSizes[y + z * ySize],bytes);
		}
		else /* raw (VERBATIM) data */
			for (y=0;y<ySize;y++) for (z=0;z<zSize;z++)
		{
			ConvertSGILine ((unsigned short *)Data,
				256 + y * xSize * zSize + z * xSize,xSize,bytes);
		}
	}
	
	switch (type)
	{
		case RGB8 :
			lPtr  = (unsigned long *)new;
			block = xSize * ySize;
			for (y = ySize - 1;y >= 0;y--)
			{
				if (rle > 0) /* read raw data */
				{
					UnpackSGI(&line[0],          Data,rle,bytes,y * xSize);
					UnpackSGI(&line[xSize],      Data,rle,bytes,y * xSize + block);
					UnpackSGI(&line[xSize+xSize],Data,rle,bytes,y * xSize + block + block);
				}
				else /* read RLE packed data */
				{
					UnpackSGI(&line[0],          Data,rle,bytes,lineTable[y]);
					UnpackSGI(&line[xSize],      Data,rle,bytes,lineTable[y+ySize]);
					UnpackSGI(&line[xSize+xSize],Data,rle,bytes,lineTable[y+ySize+ySize]);
				}
				for (x=0;x<xSize;x++)
				{
					value   = line[x];
					value   = (value << 8) | line[x + xSize];
					*lPtr++ = (value << 8) | line[x + xSize + xSize];
				}
			}
			break;

		case PCX8 :
			for (y = ySize - 1;y >= 0;y--)
			{
				if (rle > 0) UnpackSGI (new,Data,rle,bytes,y * xSize);
				else         UnpackSGI (new,Data,rle,bytes,lineTable[y]);
				new += xSize;
			}
			break;

		default :
			type = 0;
			break;
	}
	FreePartMem (&TextureMem,line);
	return type;
}

long ParseSGI (
	struct Texture *Texture,
	unsigned char  *Data)
{
	register struct HeaderSGI  *HeaderSGI;
	register unsigned char     *new;
	register long               type = 0,need = 0,xSize,ySize,zSize,c;

	HeaderSGI = (struct HeaderSGI *)Data;
	if (HeaderSGI->imagic == IMAGIC2)
	{
		ChangeWord (&HeaderSGI->type);
		ChangeWord (&HeaderSGI->dim);
		ChangeWord (&HeaderSGI->xsize);
		ChangeWord (&HeaderSGI->ysize);
		ChangeWord (&HeaderSGI->zsize);
		ChangeLong (&HeaderSGI->min);
		ChangeLong (&HeaderSGI->max);
		ChangeLong (&HeaderSGI->wastebytes);
		ChangeLong (&HeaderSGI->colormap);
	}

	Texture->xSize  =  xSize = HeaderSGI->xsize;
	Texture->ySize  =  ySize = HeaderSGI->ysize;
	Texture->Planes = (zSize = HeaderSGI->zsize) << 3;


		/* get texture type */
	switch (HeaderSGI->colormap)
	{
		case 0 :
			switch (HeaderSGI->zsize)
			{
				case 1 :
					Texture->Palette = (unsigned long *)AllocTextureMem
						(Texture,256 * sizeof(unsigned long));
					if (Texture->Palette == null)
					{
						Texture->FileType = FT_ERR_MEM;
						return 0;
					}
					for (c=0;c<256;c++) Texture->Palette[c] = 0x010101 * c;
					type = PCX8;
					need = xSize * ySize * sizeof(unsigned char *);
					break;
				case 3 :
					type = RGB8;
					need = xSize * ySize * sizeof(unsigned long *);
					break;
			}
			break;

		case 3 :
			Texture->Palette = (unsigned long *)AllocTextureMem
				(Texture,256 * sizeof(unsigned long));
			if (Texture->Palette == null)
			{
				Texture->FileType = FT_ERR_MEM;
				return 0;
			}
			for (c=0;c<256;c++) Texture->Palette[c] =
				((c & 0xe0) << 16) |
				((c & 0x1c) << 11) |
				((c & 0x03) <<  6);
			type = PCX8;
			need = xSize * ySize * sizeof(unsigned char *);
			break;

		default :
			Texture->FileType = FT_ERR_UNSUPP;
			break;
	}
	if (type == 0) return type;

	Texture->Data = new = (unsigned char *)AllocTextureMem(Texture,need);
	if (new  == null)
	{
		Texture->FileType = FT_ERR_MEM;
		return 0;
	}


		/* how are rows saved? */
	switch (HeaderSGI->dim)
	{
		case 3 :
			Texture->FileType = FT_ERR_MEM; /* default */
			type = ParseSGI3(HeaderSGI,Data,new,type,xSize,ySize,zSize);
			break;

		case 1 :
		case 2 :
		default :
			Texture->FileType = FT_ERR_PACKING;
			type = 0;
			break;
	}
	if (type != 0) Texture->FileType = FT_SGI_RLE;
	return type;
}
