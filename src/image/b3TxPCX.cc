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
**	Revision 1.3  2001/10/23 15:50:31  sm
**	- Now parsing PCX4 correctly
**	- Found TGA parsing bug.
**	- Correcting path following behaviour.
**
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

b3_tx_type b3Tx::b3ParsePCX4 (b3_u08 *buffer)
{
	b3_u08       *srcPtr;
	b3_u08       *dstPtr,Col;
	b3_count      u, xSrcBytes,DataSize;
	b3_pkd_color  t;
	b3_index      i;

	xSize = b3Endian::b3GetIntel16(&buffer[ 8]) + 1;
	ySize = b3Endian::b3GetIntel16(&buffer[10]) + 1;
	depth = buffer[65];
	dSize = TX_BWA(xSize) * ySize * depth;
	pSize = 1 << depth;

	data    = (b3_u08 *)b3Alloc(dSize);
	palette = (b3_pkd_color *)b3Alloc(pSize * sizeof(b3_pkd_color));
	if ((data != null) && (palette != null))
	{
		type      = B3_TX_ILBM;
		FileType  = FT_PCX4;
		xSrcBytes = buffer[66];

		srcPtr   = (b3_u08 *)&buffer[128];
		DataSize = ySize * xSrcBytes * depth;

		u = 16;
		for (i = 0;i < pSize; i++)
		{
			t  = (b3_pkd_color)buffer[u++] << 16;
			t += (b3_pkd_color)buffer[u++] <<  8;
			t += (b3_pkd_color)buffer[u++];
			palette[i] = t;
		}

		dstPtr = (b3_u08 *)data;

		i = 0;
		while (i < DataSize)
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
	}
	else
	{
		b3FreeTx();
		FileType = FT_ERR_MEM;
	}
	return type;
}
