/*
**
**	$Filename:	InfoGIF.c        $  
**	$Release:	Dortmund 1997 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Printing GIF tags
**
**	(C) Copyright 1997  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard includes                             **
**                                                                      **
*************************************************************************/

#include <blizzard/sys/config.h>
#include <blizzard/sys/bio.h>

#include "gif.h"

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

static struct MemNode Mem;

static void InfoGIF(char *name)
{
	struct BFile      *in;
	struct Extension  *transPtr;
	struct Descriptor *descrPtr;
	unsigned char     *buffer,*data;
	long               size,i,diff,planes;
	bool               loop = true;

	PrintF ("\n");
	PrintF ("FILE: %s\n",name);
	in = BOpen (name,B_READ);
	if (in == null)
	{
		PrintF ("file not found!\n");
		return;
	}

	size = BSize (in);
	data = buffer = AllocKeyMem (&Mem,size);
	if (buffer == null)
	{
		BClose (in);
		PrintF ("buffer not allocated\n");
		return;
	}

	BRead  (in,buffer,size);
	BClose (in);
	if (strncmp(buffer,"GIF8",4) != 0)
	{
		FreePartMem (&Mem,buffer);
		PrintF ("not a GIF image!\n");
		return;
	}

	PrintF ("*** ");
	for (i=0;i<6;i++) PrintF ("%c",*data++);
	PrintF ("\n");
	PrintF ("width:  %4ld\n",VAL2(data));
	PrintF ("height: %4ld\n",VAL2(data+2));
	PrintF ("planes: %4ld\n",planes = ((long)data[4] & 0x07) + 1);
	data += 7;
	data += ((1 << planes) * 3);

	do
	{
		switch (data[0])
		{
			case 0x21 :
				switch (data[1])
				{
					case 0x01 :
						PrintF ("*** Plain text extension:\n");
						data += (data[2] + 3);
						while ((diff = data[0]) != 0) data += (diff + 1);
						break;

					case 0xf9 :
						transPtr         = (struct Extension *)data;
						PrintF ("*** Graphic Control Extension:\n");
						PrintF ("flags: $%02x\n",transPtr->flags);
						PrintF ("transparency: %s\n",
							transPtr->flags & 1 ? "yes" : "no");
						if (transPtr->flags & 1) PrintF ("transparent color index: %ld\n",
							transPtr->index);
						PrintF ("delay time:  %ld/100 s\n",VAL2(transPtr->delay));
						data += (transPtr->size + 3);
						break;

					case 0xfe :
						PrintF ("*** Comment Extension:\n");
						data += 2;
						while ((diff = data[0]) != 0)
						{
							for (i=1;i <= diff;i++) PrintF ("%c",data[i]);
							data += (diff + 1);
						}
						PrintF ("\n");
						break;

					case 0xff :
						PrintF ("*** Application Extension:\n");
						data += (data[2] + 3);
						while ((diff = data[0]) != 0) data += (diff + 1);
						break;
				}
				while (data[0] != 0) data++;
				data++;
				break;

			case 0x2e :
				PrintF ("*** $2E ???\n");
				break;

			case 0x2c :
				descrPtr = (struct Descriptor *)data;
				planes   = (descrPtr->flags & 0x07) + 1;
				PrintF ("*** Image descriptor:\n");
				PrintF ("xPos:   %4ld\n",VAL2(descrPtr->xPos));
				PrintF ("yPos:   %4ld\n",VAL2(descrPtr->yPos));
				PrintF ("xSize:  %4ld\n",VAL2(descrPtr->xSize));
				PrintF ("ySize:  %4ld\n",VAL2(descrPtr->ySize));
				PrintF ("planes: %4ld\n",planes);
				PrintF ("flags:   $%02x\n",descrPtr->flags);
				PrintF ("local color table: %s\n",
					descrPtr->flags & 0x80 ? "yes" : "no");
				PrintF ("interlace: %s\n",
					descrPtr->flags & 0x40 ? "yes" : "no");
				data   += sizeof(struct Descriptor);
				if (descrPtr->flags & 0x80) data += ((1 << planes) * 3);
				data++;

				while ((diff = data[0]) != 0) data += (diff + 1);
				data++;
				break;

			case 0x3b :
				PrintF ("*** Trailer:\n");
				data++;
				loop = false;
				break;

			default :
				PrintF ("*** $%02lx:%02lx\n",(long)data[0],(long)data[1]);
				loop = false;
				break;
		}
	}
	while (loop);

	FreePartMem (&Mem,buffer);
}

int main(int argc,char *argv[])
{
	long i;

	if (OpenRequirements())
	{
		for (i=1;i<argc;i++) InfoGIF (argv[i]);
		CloseRequirements();
	}
	return 0;
}
