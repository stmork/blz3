/*
**
**	$Filename:	h264.cc $
**	$Release:	Dortmund 2004, 2005, 2006, 2007, 2012 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Encodes single frames into a H.264 movie
**
**	(C) Copyright 2004, 2005, 2006, 2007, 2012  Steffen A. Mork
**	    All Rights Reserved
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
#include "blz3/image/b3Tx.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3FileList.h"

#ifdef BLZ3_USE_X264
#	include <stdint.h>

extern "C"
{
#	include "x264.h"
}

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
	b3PrintF(B3LOG_NORMAL,"Blizzard III movie maker (h.264)\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001-20012\n");
	b3PrintF(B3LOG_NORMAL,"\n");
	if (command != null)
	{
#ifdef BLZ3_USE_X264
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s h.264-File [-f][-d] {frame images}\n", command);
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"  -d        debug level output\n");
		b3PrintF(B3LOG_NORMAL,"  -f        verbose level output\n");
		b3PrintF(B3LOG_NORMAL,"\n");
#else
		b3PrintF(B3LOG_NORMAL,"%s has no h.264 support!\n",command);
#endif
	}
	b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
}

#ifdef BLZ3_USE_X264
static void b3RecodeRGB2YUV(b3Tx &img, x264_picture_t &pic_in, b3_res ySize)
{
	b3_u08       *yPtr = (b3_u08 *)pic_in.img.plane[0];
	b3_u08       *uPtr = (b3_u08 *)pic_in.img.plane[1];
	b3_u08       *vPtr = (b3_u08 *)pic_in.img.plane[2];
	b3_pkd_color *line = img.b3GetTrueColorData();
	b3_pkd_color  pixel, r, g, b;

	memset(yPtr,   0, ySize * pic_in.img.i_stride[0]);
	memset(uPtr, 128, ySize * pic_in.img.i_stride[1] >> 1);
	memset(vPtr, 128, ySize * pic_in.img.i_stride[2] >> 1);

	for (b3_res iy = 0; iy < img.ySize; iy++)  
	{
		for (b3_res ix = 0; ix < img.xSize; ix++)
		{
			pixel = line[ix];
			r = (pixel & 0xff0000) >> 16;
			g = (pixel & 0x00ff00) >>  8;
			b = (pixel & 0x0000ff);

			yPtr[ix] =  0.257 * r + 0.504 * g + 0.098 * b;

			if ((ix & 1) && (iy & 1))
			{
				uPtr[ix >> 1] = -0.148 * r - 0.291 * g + 0.439 * b + 128;
					vPtr[ix >> 1] =  0.439 * r - 0.368 * g - 0.071 * b + 128;
			}
		}
		line += img.xSize;
		yPtr += pic_in.img.i_stride[0];
		if (iy & 1)
		{
			uPtr += pic_in.img.i_stride[1];
			vPtr += pic_in.img.i_stride[2];
		}
	}
}

static void b3Encode(x264_t *x264, x264_picture_t *pic_in, b3File &file)
{
	x264_picture_t  pic_out;
	x264_nal_t     *nals; 
	int             i_nals;
	int             i_size;

	x264_encoder_encode(x264, &nals, &i_nals, pic_in, &pic_out);
	for (int i = 0; i < i_nals; i++)
	{
		file.b3Write(nals[i].p_payload, nals[i].i_payload);
	}
	b3PrintF(B3LOG_NORMAL,".");
}
#endif

int main(int argc,char *argv[])
{
	b3FileList      list;
	b3FileEntry    *entry;
#ifdef BLZ3_USE_X264
	x264_t         *x264;
	x264_param_t    param;
	x264_picture_t  pic_in;
	x264_nal_t     *nals; 
	int             i_nals = 0;
#endif
	b3_size         size  = 0;
	b3_res          xSize = 0;
	b3_res          ySize = 0;
	b3_bool         isFirst = true;

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	for (int i = 2;i < argc;i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'd' :
				b3Log::b3SetLevel(B3LOG_DEBUG);
				break;

			case 'f' :
				b3Log::b3SetLevel(B3LOG_FULL);
				break;
			}
		}
		else
		{
			switch (b3Dir::b3Exists(argv[i]))
			{
			case B3_TYPE_DIR:
				list.b3RecCreateList(argv[i]);
				break;

			case B3_TYPE_FILE:
				list.b3Add(argv[i]);
				break;

			case B3_NOT_EXISTANT:
				break;

			default:
				break;
			}
		}
	}

#ifdef BLZ3_USE_X264
	b3File file(argv[1], B_WRITE);

	x264_param_default(&param);
#ifdef HAVE_X264_PARAMETER_DEFINITION
	x264_param_default_preset( &param, "medium", "stillimage" ) ;
	param.b_vfr_input      = 0;
	param.i_timebase_num   = param.i_fps_den;
	param.i_timebase_den   = param.i_fps_num;
#else
	param.rc.i_rc_method   = X264_RC_CRF;
#endif
	param.i_frame_total    = list.b3GetCount();
	param.i_fps_num        =  50;
	param.i_fps_den        =   1;
	param.b_interlaced     =   0;
//	param.b_vfr_input      =   0;
//	param.rc.i_bitrate     = 512;
	param.i_keyint_max     = param.i_fps_num;
//	param.b_intra_refresh  = 1;
	param.b_repeat_headers = 0;
//	param.b_annexb         = 1;
    param.i_csp            = X264_CSP_I420;
//	param.i_log_level      = X264_LOG_ERROR;
#endif


	list.b3Sort();
	for (entry = list.b3First();entry != null;entry = entry->Succ)
	{
		b3Tx    img;
		int     ino = 1;

		try
		{
			img.b3LoadImage(entry->b3Name());
			if (isFirst)
			{
				xSize          = (img.xSize + 15) & 0xfff0;
				ySize          = (img.ySize +  7) & 0xfff8;
				size           = xSize * ySize;
#ifdef BLZ3_USE_X264
				param.i_width          = xSize;
				param.i_height         = ySize;
//				param.vui.i_sar_width  = xSize;
//				param.vui.i_sar_height = ySize;
#ifdef HAVE_X264_PARAMETER_DEFINITION
				x264_param_apply_profile(&param, "baseline");
#endif
				x264 = x264_encoder_open(&param);
//				x264_encoder_parameters(x264, &param);

				if (!param.b_repeat_headers)
				{
					x264_nal_t *headers;
					int         i_nals = 0;

					b3PrintF(B3LOG_DEBUG, "Writing header...\n");
					x264_encoder_headers(x264, &headers, &i_nals);
					for (int i = 0; i < i_nals; i++)
					{
						file.b3Write(headers[i].p_payload, headers[i].i_payload);
					}
				}

				x264_picture_alloc(&pic_in, param.i_csp, param.i_width, param.i_height);
				pic_in.i_type       = X264_TYPE_AUTO;
#endif
				isFirst = false;
				b3PrintF(B3LOG_DEBUG,"Start encoding of %d frames...\n", param.i_frame_total);
			}
		}
		catch (b3TxException &t)
		{
			b3PrintF(B3LOG_NORMAL,"\n");
			b3PrintF(B3LOG_NORMAL,"Image error when processing image %s!\n", entry->b3Name());
			b3PrintF(B3LOG_NORMAL,"Error code: %d\n", t.b3GetError());
			b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n", t.b3GetErrorMsg());
		}
		catch (b3ExceptionBase &e)
		{
			b3PrintF(B3LOG_NORMAL,"\n");
			b3PrintF(B3LOG_NORMAL,"General Blizzard III error on image %s!\n", entry->b3Name());
			b3PrintF(B3LOG_NORMAL,"Error code: %d\n", e.b3GetError());
			b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n", e.b3GetErrorMsg());
		}
		catch (...)
		{
			b3PrintF(B3LOG_NORMAL,"\n");
			b3PrintF(B3LOG_NORMAL,"Unknown error occured on image %s!\n", entry->b3Name());
		}

#ifdef BLZ3_USE_X264
		pic_in.i_pts = ino;
		b3RecodeRGB2YUV(img, pic_in, ySize);
		b3Encode(x264, &pic_in, file);
#endif
		ino++;
	}

#ifdef BLZ3_USE_X264
#ifdef HAVE_X264_DELAYED_FRAMES
	for(int delayed = x264_encoder_delayed_frames(x264); delayed > 0; delayed--)
	{
		b3Encode(x264, &pic_in, file);
	}
#endif
	b3PrintF(B3LOG_NORMAL, "\n");
	if (!isFirst)
	{
		x264_picture_clean(&pic_in);
	}
	x264_encoder_close(x264);
#endif

	return EXIT_SUCCESS;
}
