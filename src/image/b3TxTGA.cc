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

b3_tx_type b3Tx::b3ParseTGA (b3_u08 *buffer)
{
	b3_pkd_color *srcPtr;
	b3_count      DataSize,depth1,depth2;
	b3_index      t,dNext,xk,count,Inc = 1;
	b3_pkd_color  Color;
	b3_res        xNewSize,yNewSize;

	xNewSize = b3Endian::b3GetIntel16(&buffer[12]);
	yNewSize = b3Endian::b3GetIntel16(&buffer[14]);

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
		FileType = FT_ERR_MEM;
	}
	return type;
}
