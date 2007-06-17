/*
**
**      $Filename:	pw.cc $
**      $Release:	Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:	Steffen A. Mork $
**
**      Blizzard III - Halfs an image
**
**      (C) Copyright 2003  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

int main (int argc,char *argv[])
{
	int           xMax,yMax;
	b3Tx          img;
	b3_index      index = 4;

	if (argc < 4)
	{
		return EXIT_FAILURE;
	}

	xMax = atoi(argv[2]);
	yMax = atoi(argv[3]);
	if (argc < (xMax * yMax + 4))
	{
		return EXIT_FAILURE;
	}
	img.b3AllocTx(256 * xMax,256 * yMax,24);
	for (int y = 0;y < yMax;y++)
	{
		for (int x = 0;x < xMax;x++)
		{
			b3_res xSize,ySize;
			b3Tx patch;

			b3PrintF(B3LOG_NORMAL,"Loading %s...\n",argv[index]);
			patch.b3LoadImage(argv[index]);
			xSize = patch.xSize;
			ySize = patch.ySize;

			img.b3Blit(&patch,x * xSize,y * ySize,xSize,ySize);
			index++;
		}
	}
	img.b3SaveTGA(argv[1]);

	return EXIT_SUCCESS;
}
