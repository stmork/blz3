/*
**
**	$Filename:	b3TxPCX.cc $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading PCX images
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

#include "b3TxPCX.h"

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
**                            Paintbrush PCX                            **
**                                                                      **
*************************************************************************/

long ParsePCX8 (
	struct Texture	*Texture,
	unsigned char	*Data)
{
	register unsigned char *CharData;
	register unsigned char *Set,Col;
	register unsigned long *LongData;
	register long           DataSize,i=0,t;			 

	Texture->xSize		= GetIntelShort(&Data[ 8]) + 1;
	Texture->ySize		= GetIntelShort(&Data[10]) + 1;
	Texture->Planes		= 8;
	Texture->FileType	= FT_PCX8;

	CharData = (unsigned char*)&Data[128];
	DataSize = Texture->xSize * Texture->ySize;

	Texture->Palette = (unsigned long *)AllocTextureMem (Texture,DataSize + 1024);
	if (Texture->Palette==null)
	{
		Texture->FileType = FT_ERR_MEM;
		return (0L);
	}
	Texture->Data = (unsigned char *)(Texture->Palette + 256);

	Set = (unsigned char *)Texture->Data;

	while (i < DataSize)
	{
		Col = CharData[0];
		CharData++;

		if ((Col & 192)==192)
		{
			t   = (Col & 63);
			Col = CharData[0];
			CharData++;
		}
		else t = 1;

		for (i += t;t > 0;t--) *Set++ = Col;
	}             

	CharData++;      /* Zeiger auf Palette */
	LongData = (unsigned long *)Texture->Palette;

	for (i=0; i<256; i++)
	{
		t  = ((unsigned long)CharData[0] << 16);	CharData++;
		t += ((unsigned long)CharData[0] <<  8);	CharData++;
		t +=  (unsigned long)CharData[0];			CharData++;
		LongData[0] = t;
		LongData++;
	}
	return PCX8;
}

long ParsePCX4 (
	struct Texture *Texture,
	unsigned char  *Data)
{
	register unsigned char *CharData;
	register unsigned char *Set,Col;
	register long           DataSize,u,i,t, BytesProZeile;			 

	Texture->xSize    = GetIntelShort(&Data[ 8]) + 1;
	Texture->ySize    = GetIntelShort(&Data[10]) + 1;
	Texture->Planes   = Data[65];
	Texture->FileType = FT_PCX4;
	BytesProZeile     = Data[66];

	CharData = (unsigned char *)&Data[128];
	DataSize = Texture->ySize * BytesProZeile * Texture->Planes;

	Texture->Palette = (unsigned long *)AllocTextureMem (Texture,DataSize + 64);
	if (Texture->Palette == null) return (0L);
	Texture->Data = (unsigned char *)(Texture->Palette + 16);

	u = 16;
	for (i=0; i<16; i++)
	{
		t  = (unsigned long)Data[u++] * 65536;
		t += (unsigned long)Data[u++] * 256;
		t += (unsigned long)Data[u++];
		Texture->Palette[i] = t;
	}

	Set = (unsigned char *)Texture->Data;

	i = 0;
	while (i < DataSize)
	{
		Col = CharData[0];
		CharData++;

		if ((Col & 192)==192)
		{
			t = (Col & 63);
			Col = CharData[0];
			CharData++;
		}
		else t = 1;

		for (i += t;t > 0;t--) *Set++ = Col;
	}             

	return ILBM;
}
