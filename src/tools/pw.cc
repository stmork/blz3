/*
**
**      $Filename:      half.cc $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**
**	Revision 1.1  2004/03/03 10:35:03  sm
**	- Added tool for patchworking a lot of small images
**	
**
*/

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
