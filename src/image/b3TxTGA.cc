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

#include <blz3/image/b3Tx.h>

#include "b3TxTGA.h"

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
**                        TrueVision Targa-Format                       **
**                                                                      **
*************************************************************************/

long ParseTGA (
	struct Texture *Texture,
	unsigned char  *Data)
{
	register unsigned long *Copy;
	register long           DataSize,depth,depth2,t,dNext,xk,count,Inc = 1;
	         long           Color;

	Texture->xSize = (long)Data[12] + ((long)Data[13]<<8);
	Texture->ySize = (long)Data[14] + ((long)Data[15]<<8);

	DataSize = Texture->xSize*Texture->ySize;
	Copy = (unsigned long *)AllocTextureMem (Texture,DataSize*4);
	if (Copy==null)
	{
		Texture->FileType = FT_ERR_MEM;
		return 0;
	}
	Texture->Data     = (unsigned char *)Copy;
	Texture->Planes   = 24;
	Texture->FileType = FT_TGA;

	depth2 = (4 - (depth = Data[16]>>3)) << 3;  /* Farbtiefe */

	if (Data[17] & 0x20) dNext = 0;             /* bottom up */
	else                                        /* top down */
	{
		dNext  = (Texture->xSize * 8);
		Copy  += (Texture->xSize * (Texture->ySize-1));  /* letze Zeile */
	}

	if (Data[17] & 0x10)                          
	{                                           /* right left */
		dNext -= (Texture->xSize * 8);
		Copy  += (Texture->xSize - 1);
		Inc    = -1;
	}
       
	xk = Texture->xSize;                        /* Bildbreite */
	dNext  /= 4;

	switch (Data[2])                            /* Kompression */
	{
		case 2:                                 /* unkomprimiert */
			Data += ((long)Data[0] + ((long)Data[1]<<8) + 18L);
			while (DataSize > 0L)
			{
				Color = 0L;
				for (t=0; t<depth; t++)         /* Pixel übernehmen */
				{
					Color = (Color << 8) | (long)Data[0];
					Data++;
				}
				Color = Color << depth2;
				ChangeLong (&Color);
				Copy[0]  = Color;
				Copy    += Inc;
				DataSize--;

				if (--xk == 0)                  /* Zeilenende */
				{
					Copy -= dNext;              /* nächste Zeile */
					xk = Texture->xSize;
				}
			}
			break;
                                    
		case 10:                                /* komprimiert */
			Data += ((long)Data[0] + ((long)Data[1] << 8) + 18);
			while (DataSize > 0)
			{
				count = (Data[0] & 127) + 1;    /* Steuerbyte */
				if (Data[0] & 128)
				{                               /* nächste Farbe count mal */
					Data++;                
					Color = 0;
					for (t=0;t < depth;t++)     /* schreiben. */   
					{
						Color = (Color << 8) | (long)Data[0];
						Data++;
					}
					Color = Color << depth2;
					ChangeLong (&Color);

					while (count)
					{
						Copy[0]  = Color;
						Copy    += Inc;
						count--;
						DataSize--;

						if (--xk == 0)          /* Zeilenende */
						{
							Copy -= dNext;
							xk = Texture->xSize;
						}
					}
				}
				else
				{                               /* count Pixel übernehmen */
					Data++;
					while (count)
					{
						Color = 0;
						for (t=0;t < depth;t++)
						{
							Color = (Color << 8) | (long)Data[0];
							Data++;
						}
						Color = Color << depth2;
						ChangeLong (&Color);
						Copy[0]  = Color;
						Copy    += Inc;
						count--;
						DataSize--;

						xk--;                   /* Zeilenende */
						if (xk==0)
						{
							Copy -= dNext;
							xk = Texture->xSize;
						}
					}
				}
			}
			break;
	}
	return (RGB8);
}
