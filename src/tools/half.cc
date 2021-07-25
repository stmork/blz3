/*
**
**	$Filename:	half.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Halfs an image
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Date.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/system/b3Runtime.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3Tx.h"
#include "blz3/image/b3TxExif.h"

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

static void b3Banner(const char * command)
{
	const b3Date now;

	b3PrintF(B3LOG_NORMAL, "%s Image half scaler\n", b3Runtime::b3GetProduct());
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-%d\n", now.year);
	b3PrintF(B3LOG_NORMAL, "\n");

	if (command != nullptr)
	{
		b3PrintF(B3LOG_NORMAL, "USAGE:\n");
		b3PrintF(B3LOG_NORMAL, "%s [-r][-l][-t][-g|-G] {frame images}\n", command);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -r        turn image right.\n");
		b3PrintF(B3LOG_NORMAL, "  -l        turn image left.\n");
		b3PrintF(B3LOG_NORMAL, "  -t        turn image upside down.\n");
		b3PrintF(B3LOG_NORMAL, "  -g        leave geo tagging.\n");
		b3PrintF(B3LOG_NORMAL, "  -G        remove geo tagging.\n");
		b3PrintF(B3LOG_NORMAL, "\n");
	}
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
}

int main(int argc, char * argv[])
{
	op   operation  = NOP;
	bool remove_geo = false;

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	for (int i = 1; i < argc; i++)
	{
		b3Tx   image, half;
		b3_res xSize, ySize;

		if (strcmp(argv[i], "-r") == 0)
		{
			operation = RIGHT;
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			operation = TURN;
		}
		else if (strcmp(argv[i], "-l") == 0)
		{
			operation = LEFT;
		}
		else if (strcmp(argv[i], "-g") == 0)
		{
			remove_geo = false;
		}
		else if (strcmp(argv[i], "-G") == 0)
		{
			remove_geo = true;
		}
		else
		{
			try
			{
				if (image.b3LoadImage(argv[i]) == B3_OK)
				{
					b3TxExif exif(argv[i]);

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
						if (remove_geo)
						{
							exif.b3RemoveGpsData();
						}
						exif.b3Update();
						half.b3ScaleToGrey(&image);
#ifdef HAVE_JPEGLIB_H
						half.b3SaveJPEG(argv[i], B3_JPG_QUALITY, &exif);
#else
						// TODO: Save as TGA image instead!
#endif
					}
				}
			}
			catch (b3TxException & t)
			{
				b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
				b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
			}
			operation = NOP;
		}
	}
	return EXIT_SUCCESS;
}
