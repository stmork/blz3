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
**	Revision 1.1  2003/09/28 20:33:20  sm
**	- Ensure CPU count in image scaling methods.
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

int main (int argc,char *argv[])
{
	int i;

	for (i = 1;i < argc;i++)
	{
		b3Tx   image,half;
		b3_res xSize,ySize;

		try
		{
			if (image.b3LoadImage(argv[i]) == B3_OK)
			{
				xSize = (image.xSize >> 1);
				ySize = (image.ySize >> 1);
				if (half.b3AllocTx(xSize,ySize,24))
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
	}
	return 0;
}
