/*
**
**	$Filename:	divx.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**      Blizzard III - Encodes single frames into a DivX
**
**      (C) Copyright 2004  Steffen A. Mork
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

#ifdef BLZ3_USE_AVILIB
extern "C"
{
#include "avilib.h"
}
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**
**	Revision 1.5  2004/08/26 07:08:10  sm
**	- Higher bitrate in DivX encoder.
**	- Disbled unnecessary AUTO_NORMAL option in OpenGL renderer.
**	
**	Revision 1.4  2004/08/17 08:35:37  sm
**	- Correct make depend command
**	- Correct install
**	
**	Revision 1.3  2004/08/16 06:05:43  sm
**	- Added divx define rules.
**	
**	Revision 1.2  2004/08/04 13:56:24  sm
**	- More quiet divx encoder
**	
**	Revision 1.1  2004/08/03 10:46:26  sm
**	- Added simgle frame to DivX/AVI conversion tool
**	- Added image mirror (not completely implemented yet)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

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
#ifdef BLZ3_USE_AVILIB
	avi_t        *out;
#endif

#ifdef BLZ3_USE_AVILIB
	out = AVI_open_output_file(argv[1]);
	if (out == NULL)
	{
		fprintf(stderr,"Cannot write %s\n",argv[1]);
		exit (EXIT_FAILURE);
	}
#endif

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
			buffer             = (b3_u08 *)malloc(size * 3);
			bitstream          = (char *)malloc(size * 6);

			error = encore(0,ENC_OPT_INIT,&encoding,0);
			if ((error != ENC_OK) || (encoding.handle == 0))
			{
				fprintf(stderr,"ERROR CODE: %d (starting encoding)\nexiting...\n",error);
			}
			else
			{
				b3PrintF(B3LOG_DEBUG,"Start encoding...");
			}

#ifdef BLZ3_USE_AVILIB
			AVI_set_video(out, img.xSize, img.ySize, encoding.framerate, "DIVX");
#endif
		}
#endif

		// Recode image
		ptr  = buffer;
		data = (b3_pkd_color *)img.b3GetData();
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
#ifdef BLZ3_USE_AVILIB
			AVI_write_frame(out,bitstream,frame.length,0);
#endif
			b3PrintF(B3LOG_DEBUG,"\n encoded frame %s (%d bytes)\n",argv[i],frame.length);
			b3PrintF(B3LOG_NORMAL,".");
		}
#endif
	}

#ifdef BLZ3_USE_DIVX4LINUX
	error = encore(encoding.handle,ENC_OPT_RELEASE,0,0);
	if (error != ENC_OK)
	{
		fprintf(stderr,"\nERROR CODE: %d (finishing encoding)\nexiting...\n",error);
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"\nDone.\n");
	}
#endif

#ifdef BLZ3_USE_AVILIB
	AVI_close(out);
#endif

	return EXIT_SUCCESS;
}
