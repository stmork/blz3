/*
**
**	$Filename:	gif_movie.cc $  
**	$Release:	Dortmund 1997, 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Insert delay for movie playing.
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/
  
/*
**	$Log$
**	Revision 1.3  2004/10/12 11:54:53  smork
**	- Some minor GCC compiler errors.
**
**	Revision 1.2  2004/10/07 10:33:08  sm
**	- Added some GIF tools and made them usable with Blizzard III.
**	
**
*/
  
/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

static void MovieGIF(char *name)
{
	b3File             in,out;
	b3_u08            *buffer,*data;
	b3_size            size;
	b3_count           diff,planes;
	b3_bool            loop = true,visited = false,index = 0;
	struct Extension  *transPtr;
	struct Descriptor *descrPtr;
	struct Repeator    repeat;

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
	in.b3Close();

#ifdef DEBUG
	if (!out.b3Open ("test.gif",B_WRITE))
#else
	if (!out.b3Open (name,B_WRITE))
#endif
	{
		b3PrintF (B3LOG_NORMAL, "cannot open file for writing!\n");
		return;
	}

	repeat.magic      = 0x21;
	repeat.label      = 0xff;
	repeat.size       = 11;
	repeat.content[0] = 3;
	repeat.content[1] = 1;
	repeat.content[2] = 0xe8;
	repeat.content[3] = 3;
	repeat.content[4] = 0;
	strncpy (repeat.text,"NETSCAPE2.0",11);

	data[4]  = '9';
	data    += 6;
	planes   = ((b3_count)data[4] & 0x07) + 1;
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
						if (!visited)
						{
							index = WriteGIF(out,buffer,data,index);
							out.b3Write (&repeat,sizeof(struct Repeator));
							visited = true;
						}
						transPtr = (struct Extension *)data;
						if (VAL2(transPtr->delay) == 0)
						{
							transPtr->delay[0] = 1;
						}
						transPtr->flags |= 8;
						data += (transPtr->size + 3);
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
						visited = true;
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
				descrPtr  = (struct Descriptor *)data;
				data     += sizeof(struct Descriptor);
				planes    = (descrPtr->flags & 0x07) + 1;
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

int main(int argc,char *argv[])
{
	int i;

	for (i = 1; i < argc; i++)
	{
		MovieGIF (argv[i]);
	}
	return 0;
}