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

#include "encore2.h"

extern "C"
{
#include "avilib.h"
}

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	ENC_PARAM     encoding;
	ENC_FRAME     frame;
	ENC_RESULT    result;
	b3_u08       *buffer = null;
	b3_u08       *ptr;
	char         *bitstream = null;
	b3_pkd_color *data,color;
	b3_size       size = 0;
	int           error;
	avi_t        *out;

	out = AVI_open_output_file(argv[1]);
	if (out == NULL)
	{
		fprintf(stderr,"Cannot write %s\n",argv[1]);
		exit (10);
	}

	memset(&encoding,0,sizeof(encoding));
	memset(&frame,   0,sizeof(frame));
	for (int i = 2;i < argc;i++)
	{
		b3Tx img;

		img.b3LoadImage(argv[i]);
		img.b3MirrorVertical();
		if (encoding.handle == 0)
		{
			encoding.x_dim     = img.xSize;
			encoding.y_dim     = img.ySize;
			encoding.framerate = 25;
			encoding.quality   =  5;

			size             = img.xSize * img.ySize;
			buffer           = (b3_u08 *)malloc(size * 3);
			bitstream        = (char *)malloc(size * 6);

			error = encore(0,ENC_OPT_INIT,&encoding,0);
			if ((error != ENC_OK) || (encoding.handle == 0))
			{
				fprintf(stderr,"ERROR CODE: %d (starting encoding)\nexiting...\n",error);
			}
			else
			{
				printf("Start encoding...\n");
			}

			AVI_set_video(out, img.xSize, img.ySize, encoding.framerate, "DIVX");
		}

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

		frame.image      = buffer;
		frame.bitstream  = bitstream;
		frame.length     =  0;
		frame.quant      =  0;
		frame.intra      = -1;
		frame.colorspace = ENC_CSP_RGB24;

		error = encore(encoding.handle,ENC_OPT_ENCODE,&frame,&result);
		if (error != ENC_OK)
		{
			fprintf(stderr,"ERROR CODE: %d (encoding frame %d)\nexiting...\n",error,i-2);
		}
		else
		{
			AVI_write_frame(out,bitstream,frame.length,0);
			printf(" encoded frame %s (%d bytes)\n",argv[i],frame.length);
		}
	}

	error = encore(encoding.handle,ENC_OPT_RELEASE,0,0);
	if (error != ENC_OK)
	{
		fprintf(stderr,"ERROR CODE: %d (finishing encoding)\nexiting...\n",error);
	}
	else
	{
		printf("Done.\n");
	}

	AVI_close(out);

	return 0;
}
