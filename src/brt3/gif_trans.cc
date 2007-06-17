/*
**
**	$Filename:	gif_trans.cc $  
**	$Release:	Dortmund 1997, 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Make a GIF transparent
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

#include "gif.h"

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

static void TransGIF(char *name)
{
	b3File             in,out;
	b3_u08            *buffer,*data;
	b3_size            size;
	b3_count           diff,planes;
	b3_bool            loop = true,visited = false,index = 0;
	struct Extension  *transPtr,trans;
	struct Descriptor *descrPtr;

	b3PrintF (B3LOG_NORMAL, "\n");
	b3PrintF (B3LOG_NORMAL, "FILE: %s\n",name);

	buffer = in.b3ReadBuffer(name,size);
	if (buffer == null)
	{
		b3PrintF (B3LOG_NORMAL, "buffer not allocated\n");
		return;
	}
	data = buffer;

	if (strncmp((const char *)buffer,"GIF8",4) != 0)
	{
		b3PrintF (B3LOG_NORMAL, "not a GIF image!\n");
		return;
	}

#ifdef _DEBUG
	if (!out.b3Open("test.gif",B_WRITE))
#else
	if (!out.b3Open(name,B_WRITE))
#endif
	{
		b3PrintF (B3LOG_NORMAL, "cannot open file for writing!\n");
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
						while ((diff = data[0]) != 0)
						{
							data += (diff + 1);
						}
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
						while ((diff = data[0]) != 0)
						{
							data += (diff + 1);
						}
						break;
				}
				while (data[0] != 0)
				{
					data++;
				}
				data++;
				break;

			case 0x2e :
				break;

			case 0x2c :
				if (!visited)
				{
					index = WriteGIF(out,buffer,data,index);
					out.b3Write (&trans,sizeof(trans));
				}
				descrPtr = (struct Descriptor *)data;
				data   += sizeof(struct Descriptor);
				planes  = (descrPtr->flags & 0x07) + 1;
				if (descrPtr->flags & 0x80)
				{
					data += ((1 << planes) * 3);
				}
				data++;

				while ((diff = data[0]) != 0)
				{
					data += (diff + 1);
				}
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
	out.b3Close();
}

static void b3Banner(const char *command)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III GIF transparentizer\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001-2007\n");
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
			TransGIF (argv[i]);
		}
	}
	else
	{
		b3Banner(argv[0]);
	}
	return EXIT_SUCCESS;
}
