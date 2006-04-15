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
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/04/15 20:34:55  sm
**	- Added support for ocean surface bump mapping.
**
**	Revision 1.3  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.2  2003/10/12 08:30:35  sm
**	- Added rotating of image before scaling and saving.
**	
**	Revision 1.1  2003/09/28 20:33:20  sm
**	- Ensure CPU count in image scaling methods.
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

enum op
{
	NOP = 0,
	RIGHT,
	TURN,
	LEFT
};

int main (int argc,char *argv[])
{
	int i;
	op  operation = NOP;

	for (i = 1;i < argc;i++)
	{
		b3Tx   image,half;
		b3_res xSize,ySize;

		     if (strcmp(argv[i],"-r") == 0) operation = RIGHT;
		else if (strcmp(argv[i],"-t") == 0) operation = TURN;
		else if (strcmp(argv[i],"-l") == 0) operation = LEFT;
		else
		{
			try
			{
				if (image.b3LoadImage(argv[i]) == B3_OK)
				{
					switch (operation)
					{
					case RIGHT:
						image.b3TurnRight();
						break;
					case LEFT:
						image.b3TurnLeft();
						break;
					case TURN:
						image.b3Turn();
						break;
					case NOP:
						break;
					}

					xSize = (image.xSize >> 1);
					ySize = (image.ySize >> 1);
					if (half.b3AllocTx(xSize, ySize, 24))
					{
						half.b3ScaleToGrey(&image);
						half.b3SaveJPEG(argv[i]);
					}
				}
			}
			catch(b3TxException &t)
			{
				b3PrintF(B3LOG_NORMAL,"Error code: %d\n",t.b3GetError());
				b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n",t.b3GetErrorMsg());
			}
			operation = NOP;
		}
	}
	return EXIT_SUCCESS;
}
