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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                            Paintbrush PCX                            **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParsePCX8(b3_u08 * buffer)
{
	b3_u08    *   srcPtr;
	b3_u08    *   dstPtr, Col;
	b3_pkd_color  t;
	b3_res        xNewSize, yNewSize;
	b3_index      i = 0;

	b3PrintF(B3LOG_FULL, "IMG PCX  # b3ParsePCX8(%s)\n",
		(const char *)image_name);

	xNewSize = b3Endian::b3GetIntel16(&buffer[ 8]) + 1;
	yNewSize = b3Endian::b3GetIntel16(&buffer[10]) + 1;
	if (b3AllocTx(xNewSize, yNewSize, 8))
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
			t  = ((b3_pkd_color)srcPtr[0] << 16);
			t += ((b3_pkd_color)srcPtr[1] <<  8);
			t += (b3_pkd_color)srcPtr[2];
			srcPtr += 3;
			palette[i] = t;
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

b3_result b3Tx::b3ParsePCX4(b3_u08 * buffer)
{
	b3_u08    *   srcPtr;
	b3_u08    *   dstPtr, Col;
	b3_count      u, xSrcBytes, DataSize;
	b3_pkd_color  t;
	b3_index      i;

	b3PrintF(B3LOG_FULL, "IMG PCX  # b3ParsePCX4(%s)\n",
		(const char *)image_name);

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
		for (i = 0; i < pSize; i++)
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
