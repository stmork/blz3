/*
**
**	$Filename:	TransGIF.c       $  
**	$Release:	Dortmund 1997 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Make a GIF transparent
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

static long WriteGIF (
	struct BFile  *out,
	unsigned char *buffer,
	unsigned char *data,
	long           index)
{
	long size;

	size = data - &buffer[index];
	if (size <= 0) return index;
	BWrite (out,&buffer[index],size);
	index = data - buffer;
	return index;
}

static void InfoGIF(char *name)
{
	struct BFile      *in,*out;
	struct Extension  *transPtr,trans;
	struct Descriptor *descrPtr;
	unsigned char     *buffer,*data;
	long               size,diff,planes;
	bool               loop = true,visited = false,index = 0;

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

#ifdef DEBUG
	out = BOpen ("test.gif",B_WRITE);
#else
	out = BOpen (name,B_WRITE);
#endif
	if (out == null)
	{
		FreePartMem (&Mem,buffer);
		PrintF ("cannot open file for writing!\n");
		return;
	}

	trans.magic    = 0x21;
	trans.label    = 0xf9;
	trans.size     = 4;
	trans.flags    = 9;
	trans.delay[0] = 0;
	trans.delay[1] = 0;
	trans.index    = 0;
	trans.term     = 0;

	data[4]  = '9';
	data    += 6;
	planes   = ((long)data[4] & 0x07) + 1;
	data    += 7;
	data    += ((1 << planes) * 3);

	do
	{
		switch (data[0])
		{
			case 0x21 :
				switch (data[1])
				{
					case 0x01 :
						data += (data[2] + 3);
						while ((diff = data[0]) != 0) data += (diff + 1);
						break;

					case 0xf9 :
						transPtr         = (struct Extension *)data;
						transPtr->flags |= 1;
						data += (transPtr->size + 3);
						visited = true;
						break;

					case 0xfe :
						data += 2;
						while ((diff = data[0]) != 0)
						{
							data += (diff + 1);
						}
						break;

					case 0xff :
						data += (data[2] + 3);
						while ((diff = data[0]) != 0) data += (diff + 1);
						break;
				}
				while (data[0] != 0) data++;
				data++;
				break;

			case 0x2e :
				break;

			case 0x2c :
				if (!visited)
				{
					index = WriteGIF(out,buffer,data,index);
					BWrite (out,&trans,sizeof(trans));
				}
				descrPtr = (struct Descriptor *)data;
				data   += sizeof(struct Descriptor);
				planes  = (descrPtr->flags & 0x07) + 1;
				if (descrPtr->flags & 0x80) data += ((1 << planes) * 3);
				data++;

				while ((diff = data[0]) != 0) data += (diff + 1);
				data++;
				visited = false;
				break;

			case 0x3b :
				data++;
				loop = false;
				break;

			default :
				loop = false;
				break;
		}
	}
	while (loop);
	WriteGIF(out,buffer,data,index);
	BClose (out);

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
