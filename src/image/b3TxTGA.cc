/*
**
**	$Filename:	b3TxTGA.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading TGA images
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
**	Revision 1.9  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**
**	Revision 1.8  2002/01/01 13:50:22  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.7  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.6  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.5  2001/10/23 15:50:31  sm
**	- Now parsing PCX4 correctly
**	- Found TGA parsing bug.
**	- Correcting path following behaviour.
**	
**	Revision 1.4  2001/10/15 14:45:08  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
**	Revision 1.3  2001/10/13 15:48:53  sm
**	- Minor image loading corrections
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
**                        TrueVision Targa-Format                       **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseTGA (b3_u08 *buffer)
{
	b3_pkd_color *srcPtr;
	b3_count      DataSize,depth1,depth2;
	b3_index      t,dNext,xk,count,Inc = 1;
	b3_pkd_color  Color;
	b3_res        xNewSize,yNewSize;

	xNewSize = b3Endian::b3GetIntel16(&buffer[12]);
	yNewSize = b3Endian::b3GetIntel16(&buffer[14]);

	b3PrintF(B3LOG_FULL,"IMG TGA  # b3ParseTGA(%s)\n",
		(const char *)image_name);
	if (b3AllocTx(xNewSize,yNewSize,24))
	{
		DataSize = xSize * ySize;
		srcPtr = (b3_pkd_color *)data;
		FileType = FT_TGA;

		depth2 = (4 - (depth1 = buffer[16] >> 3)) << 3;  /* Farbtiefe */

		if (buffer[17] & 0x20) dNext = 0;             /* bottom up */
		else                                        /* top down */
		{
			dNext  = (xSize * 8);
			srcPtr  += (xSize * (ySize - 1));  /* letze Zeile */
		}

		if (buffer[17] & 0x10)                          
		{                                           /* right left */
			dNext -= (xSize * 8);
			srcPtr  += (xSize - 1);
			Inc    = -1;
		}

		xk     = xSize;                        /* Bildbreite */
		dNext /= 4;

		switch (buffer[2])                            /* Kompression */
		{
		case 2:                                 /* unkomprimiert */
			buffer += (b3Endian::b3GetIntel16(buffer) + 18);
			while (DataSize > 0)
			{
				Color = 0;
				for (t = 0;t < depth1; t++)         /* Pixel übernehmen */
				{
					Color = (Color << 8) | *buffer++;
				}
				Color = Color << depth2;
				b3Endian::b3ChangeEndian32 (&Color);
				srcPtr[0]  = Color;
				srcPtr    += Inc;
				DataSize--;

				if (--xk == 0)                  /* Zeilenende */
				{
					srcPtr -= dNext;              /* nächste Zeile */
					xk = xSize;
				}
			}
			break;
                                    
		case 10:                                /* komprimiert */
			buffer += (b3Endian::b3GetIntel16(buffer) + 18);
			while (DataSize > 0)
			{
				count = (buffer[0] & 127) + 1;    /* Steuerbyte */
				if (buffer[0] & 128)
				{                               /* nächste Farbe count mal */
					buffer++;                
					Color = 0;
					for (t = 0;t < depth1;t++)     /* schreiben. */   
					{
						Color = (Color << 8) | *buffer++;
					}
					Color = Color << depth2;
					b3Endian::b3ChangeEndian32 (&Color);

					while (count)
					{
						srcPtr[0]  = Color;
						srcPtr    += Inc;
						count--;
						DataSize--;

						// End of line
						if (--xk == 0)
						{
							srcPtr -= dNext;
							xk = xSize;
						}
					}
				}
				else
				{                               /* count Pixel übernehmen */
					buffer++;
					while (count)
					{
						Color = 0;
						for (t = 0;t < depth1;t++)
						{
							Color = (Color << 8) | *buffer++;
						}
						Color = Color << depth2;
						b3Endian::b3ChangeEndian32 (&Color);
						srcPtr[0]  = Color;
						srcPtr    += Inc;
						count--;
						DataSize--;

						// End of line
						if (--xk == 0)
						{
							srcPtr -= dNext;
							xk    = xSize;
						}
					}
				}
			}
			break;
		}
	}
	else
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG TGA  # Error allocating memory:\n");
		b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
		b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
		throw b3TxException(B3_TX_MEMORY);
	}
	return B3_OK;
}
