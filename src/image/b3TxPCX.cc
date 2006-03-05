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

#include "b3TxInclude.h"
#include "blz3/base/b3Endian.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 21:22:34  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.7  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.6  2002/01/01 13:50:22  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.5  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.4  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
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

b3_result b3Tx::b3ParsePCX8 (b3_u08 *buffer)
{
	b3_u08       *srcPtr;
	b3_u08       *dstPtr,Col;
	b3_pkd_color  t;
	b3_res        xNewSize,yNewSize;
	b3_index      i = 0;

	b3PrintF(B3LOG_FULL,"IMG PCX  # b3ParsePCX8(%s)\n",
		(const char *)image_name);

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
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG PCX  # Error allocating memory:\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}
	return B3_OK;
}

b3_result b3Tx::b3ParsePCX4 (b3_u08 *buffer)
{
	b3_u08       *srcPtr;
	b3_u08       *dstPtr,Col;
	b3_count      u, xSrcBytes,DataSize;
	b3_pkd_color  t;
	b3_index      i;

	b3PrintF(B3LOG_FULL,"IMG PCX  # b3ParsePCX4(%s)\n",
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
		b3PrintF(B3LOG_NORMAL,"IMG PCX  # Error allocating memory:\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}
	return B3_OK;
}
