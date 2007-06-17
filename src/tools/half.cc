/*
**
**      $Filename:	half.cc $
**      $Release:	Dortmund 2003 $
**      $Revision: 1454 $
**      $Date: 2006-07-02 10:10:09 +0200 (So, 02 Jul 2006) $
**      $Author: smork $
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
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3Tx.h"

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
