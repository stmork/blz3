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

#include "blz3/image/b3Tx.h"
#include "blz3/base/b3Endian.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/10/13 15:35:32  sm
**	- Adding further image file format support.
**
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

b3_tx_type b3Tx::b3ParsePCX8 (b3_u08 *buffer)
{
	b3_u08       *srcPtr;
	b3_u08       *dstPtr,Col;
	b3_pkd_color  t;
	b3_res        xNewSize,yNewSize;
	b3_index      i = 0;

	xNewSize = b3Endian::b3GetIntel16(&buffer[ 8]) + 1;
	yNewSize = b3Endian::b3GetIntel16(&buffer[10]) + 1;
	if (b3AllocTx(xNewSize,yNewSize,8))
	{
		FileType = FT_PCX8;
		srcPtr   = &buffer[128];
		dstPtr   = data;
		while (i < dSize)
		{
			Col = *srcPtr++;

			if ((Col & 192) == 192)
			{
				t   = (Col & 63);
				Col = *srcPtr++;
				for (i += t;t > 0;t--)
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

		for (i = 0;i < 256;i++)
		{
			t  = ((b3_pkd_color)srcPtr[0] << 16);
			t += ((b3_pkd_color)srcPtr[1] <<  8);
			t +=  (b3_pkd_color)srcPtr[2];
			srcPtr += 3;
			palette[i] = t;
		}
	}
	else
	{
		FileType = FT_ERR_MEM;
	}
	return type;
}

b3_tx_type b3Tx::b3ParsePCX4 (b3_u08 *Data)
{
	b3_u08       *CharData;
	b3_u08       *Set,Col;
	b3_size       DataSize,u,i, BytesProZeile;			 
	b3_res        xNewSize,yNewSize;
	b3_pkd_color  t;

	xNewSize    = b3Endian::b3GetIntel16(&Data[ 8]) + 1;
	yNewSize    = b3Endian::b3GetIntel16(&Data[10]) + 1;

	if (b3AllocTx(xNewSize,yNewSize,4))
	{
#if 0
	Planes   = Data[65];
	FileType = FT_PCX4;
	BytesProZeile     = Data[66];

	CharData = (unsigned char *)&Data[128];
	DataSize = Texture->ySize * BytesProZeile * Texture->Planes;

	Texture->Palette = (unsigned long *)AllocTextureMem (Texture,DataSize + 64);
	if (Texture->Palette == null) return (0L);
	Texture->Data = (unsigned char *)(Texture->Palette + 16);

	u = 16;
	for (i=0; i<16; i++)
	{
		t  = (b3_pkd_color)Data[u++] << 16;
		t += (b3_pkd_color)Data[u++] <<  8;
		t += (b3_pkd_color)Data[u++];
		palette[i] = t;
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
#endif
	}
	else
	{
		FileType = FT_ERR_MEM;
	}
	return type;
}
