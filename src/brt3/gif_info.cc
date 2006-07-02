/*
**
**	$Filename:	gif_info.cc $  
**	$Release:	Dortmund 1997, 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Printing GIF tags
**
**	(C) Copyright 1997, 2004  Steffen A. Mork
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

#include "blz3/b3Config.h"
#include "gif.h"

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

static void InfoGIF(char *name)
{
	b3File             in;
	b3_u08            *buffer,*data;
	b3_size            size;
	b3_count           i,diff,planes;
	b3_bool            loop = true;
	struct Extension  *transPtr;
	struct Descriptor *descrPtr;

	b3PrintF(B3LOG_NORMAL, "\n");
	b3PrintF(B3LOG_NORMAL, "FILE: %s\n",name);

	buffer = in.b3ReadBuffer(name,size);
	if (buffer == null)
	{
		b3PrintF(B3LOG_NORMAL, "no memory available.\n");
		return;
	}
	data = buffer;

	if (strncmp((const char *)buffer,"GIF8",4) != 0)
	{
		b3PrintF(B3LOG_NORMAL, "not a GIF image!\n");
		return;
	}

	b3PrintF(B3LOG_NORMAL, "*** ");
	for (i = 0; i < 6; i++)
	{
		b3PrintF(B3LOG_NORMAL, "%c",*data++);
	}
	b3PrintF(B3LOG_NORMAL, "\n");
	b3PrintF(B3LOG_NORMAL, "width:  %4ld\n",VAL2(data));
	b3PrintF(B3LOG_NORMAL, "height: %4ld\n",VAL2(data+2));
	b3PrintF(B3LOG_NORMAL, "planes: %4ld\n",planes = ((long)data[4] & 0x07) + 1);
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
						b3PrintF(B3LOG_NORMAL, "*** Plain text extension:\n");
						data += (data[2] + 3);
						while ((diff = data[0]) != 0) data += (diff + 1);
						break;

					case 0xf9 :
						transPtr         = (struct Extension *)data;
						b3PrintF(B3LOG_NORMAL, "*** Graphic Control Extension:\n");
						b3PrintF(B3LOG_NORMAL, "flags: $%02x\n",transPtr->flags);
						b3PrintF(B3LOG_NORMAL, "transparency: %s\n",
							transPtr->flags & 1 ? "yes" : "no");
						if (transPtr->flags & 1)
						{
							b3PrintF(B3LOG_NORMAL, "transparent color index: %ld\n",
								transPtr->index);
						}
						b3PrintF(B3LOG_NORMAL, "delay time:  %ld/100 s\n",VAL2(transPtr->delay));
						data += (transPtr->size + 3);
						break;

					case 0xfe :
						b3PrintF(B3LOG_NORMAL, "*** Comment Extension:\n");
						data += 2;
						while ((diff = data[0]) != 0)
						{
							for (i=1;i <= diff;i++) b3PrintF(B3LOG_NORMAL, "%c",data[i]);
							data += (diff + 1);
						}
						b3PrintF(B3LOG_NORMAL, "\n");
						break;

					case 0xff :
						b3PrintF(B3LOG_NORMAL, "*** Application Extension:\n");
						data += (data[2] + 3);
						while ((diff = data[0]) != 0) data += (diff + 1);
						break;
				}
				while (data[0] != 0) data++;
				data++;
				break;

			case 0x2e :
				b3PrintF(B3LOG_NORMAL, "*** $2E ???\n");
				break;

			case 0x2c :
				descrPtr = (struct Descriptor *)data;
				planes   = (descrPtr->flags & 0x07) + 1;
				b3PrintF(B3LOG_NORMAL, "*** Image descriptor:\n");
				b3PrintF(B3LOG_NORMAL, "xPos:   %4ld\n",VAL2(descrPtr->xPos));
				b3PrintF(B3LOG_NORMAL, "yPos:   %4ld\n",VAL2(descrPtr->yPos));
				b3PrintF(B3LOG_NORMAL, "xSize:  %4ld\n",VAL2(descrPtr->xSize));
				b3PrintF(B3LOG_NORMAL, "ySize:  %4ld\n",VAL2(descrPtr->ySize));
				b3PrintF(B3LOG_NORMAL, "planes: %4ld\n",planes);
				b3PrintF(B3LOG_NORMAL, "flags:   $%02x\n",descrPtr->flags);
				b3PrintF(B3LOG_NORMAL, "local color table: %s\n",
					descrPtr->flags & 0x80 ? "yes" : "no");
				b3PrintF(B3LOG_NORMAL, "interlace: %s\n",
					descrPtr->flags & 0x40 ? "yes" : "no");
				data   += sizeof(struct Descriptor);
				if (descrPtr->flags & 0x80) data += ((1 << planes) * 3);
				data++;

				while ((diff = data[0]) != 0) data += (diff + 1);
				data++;
				break;

			case 0x3b :
				b3PrintF(B3LOG_NORMAL, "*** Trailer:\n");
				data++;
				loop = false;
				break;

			default :
				b3PrintF(B3LOG_NORMAL, "*** $%02lx:%02lx\n",(long)data[0],(long)data[1]);
				loop = false;
				break;
		}
	}
	while (loop);
}

static void b3Banner(const char *command)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III GIF info viewer\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001, 2002, 2003, 2004, 2005\n");
	b3PrintF(B3LOG_NORMAL,"\n");
	if (command != null)
	{
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s {GIF-Files}\n",command);
		b3PrintF(B3LOG_NORMAL,"\n");
	}
	b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
}

int main(int argc,char *argv[])
{
	if (argc > 1)
	{
		int i;

		for (i = 1; i < argc; i++)
		{
			InfoGIF (argv[i]);
		}
	}
	else
	{
		b3Banner(argv[0]);
	}
	return EXIT_SUCCESS;
}
