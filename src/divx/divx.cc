/*
**
**	$ilename:	divx.cc $
**	$Release:	Dortmund 2004, 2005, 2006, 2007 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**      Blizzard III - Encodes single frames into a DivX
**
**      (C) Copyright 2004, 2005, 2006, 2007  Steffen A. Mork
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
#include "blz3/image/b3Tx.h"
#include "blz3/base/b3Color.h"

#ifdef BLZ3_USE_DIVX4LINUX
#include "encore2.h"

#define RATE_KBIT(kbit)   ((kbit) * 1024)
#define RATE_KBYTE(kbyte) (RATE_KBIT((kbyte) * 8))

#endif

extern "C"
{
#include "avilib.h"
}

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static void b3Banner(const char *command)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III movie maker (DivX)\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001-2007\n");
	b3PrintF(B3LOG_NORMAL,"\n");
	if (command != null)
	{
#ifdef BLZ3_USE_DIVX4LINUX
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s Divx-File {frame images}\n",command);
		b3PrintF(B3LOG_NORMAL,"\n");
#else
		b3PrintF(B3LOG_NORMAL,"%s has no DivX support!\n",command);
#endif
	}
	b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
}

int main(int argc,char *argv[])
{
#ifdef BLZ3_USE_DIVX4LINUX
	ENC_PARAM     encoding;
	ENC_FRAME     frame;
	ENC_RESULT    result;
	char         *bitstream = null;
	int           error;
#endif
	b3_u08       *buffer = null;
	b3_u08       *ptr;
	b3_pkd_color *data,color;
	b3_size       size = 0;
	avi_t        *out;

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	out = AVI_open_output_file(argv[1]);
	if (out == NULL)
	{
		fprintf(stderr,"Cannot write %s\n",argv[1]);
		exit (EXIT_FAILURE);
	}

#ifdef BLZ3_USE_DIVX4LINUX
	memset(&encoding,0,sizeof(encoding));
	memset(&frame,   0,sizeof(frame));
#endif
	for (int i = 2;i < argc;i++)
	{
		b3Tx img;

		img.b3LoadImage(argv[i]);
		img.b3MirrorVertical();
#ifdef BLZ3_USE_DIVX4LINUX
		if (encoding.handle == 0)
		{
			encoding.x_dim     = img.xSize;
			encoding.y_dim     = img.ySize;
			encoding.framerate =      25.0;
			encoding.quality   =       5;
			encoding.bitrate   = RATE_KBYTE(500);

			size               = img.xSize * img.ySize;
			buffer             = new b3_u08[size * 3];
			bitstream          = new char[size * 6];

			error = encore(0,ENC_OPT_INIT,&encoding,0);
			if ((error != ENC_OK) || (encoding.handle == 0))
			{
				fprintf(stderr,"ERROR CODE: %d (starting encoding)\nexiting...\n",error);
			}
			else
			{
				b3PrintF(B3LOG_DEBUG,"Start encoding...");
			}
			AVI_set_video(out, img.xSize, img.ySize, encoding.framerate, "DIVX");
		}
#endif

		// Recode image
		ptr  = buffer;
		data = img.b3GetTrueColorData();
		for (b3_size k = 0;k < size;k++)
		{
			color  = *data++;
			*ptr++ =  color & 0x0000ff;
			*ptr++ = (color & 0x00ff00) >>  8;
			*ptr++ = (color & 0xff0000) >> 16;
		}

#ifdef BLZ3_USE_DIVX4LINUX
		frame.image      = buffer;
		frame.bitstream  = bitstream;
		frame.length     =  0;
		frame.quant      =  0;
		frame.intra      = -1;
		frame.colorspace = ENC_CSP_RGB24;

		error = encore(encoding.handle,ENC_OPT_ENCODE,&frame,&result);
		if (error != ENC_OK)
		{
			fprintf(stderr,"\nERROR CODE: %d (encoding frame %d)\nexiting...\n",error,i-2);
		}
		else
		{
			AVI_write_frame(out,bitstream,frame.length,0);
			b3PrintF(B3LOG_DEBUG,"\n encoded frame %s (%d bytes)\n",argv[i],frame.length);
			b3PrintF(B3LOG_NORMAL,".");
		}
#endif
	}

#ifdef BLZ3_USE_DIVX4LINUX
	error = encore(encoding.handle, ENC_OPT_RELEASE, 0, 0);
	if (error != ENC_OK)
	{
		fprintf(stderr,"\nERROR CODE: %d (finishing encoding)\nexiting...\n",error);
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"\nDone.\n");
	}

	if (bitstream != null)
	{
		delete [] bitstream;
	}
#endif
	if (buffer != null)
	{
		delete [] buffer;
	}

	AVI_close(out);

	return EXIT_SUCCESS;
}
