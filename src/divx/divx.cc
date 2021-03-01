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
#include "blz3/base/b3FileList.h"

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

static void b3Banner(const char * command)
{
	b3PrintF(B3LOG_NORMAL, "Blizzard III movie maker (DivX)\n");
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-2007\n");
	b3PrintF(B3LOG_NORMAL, "\n");
	if (command != nullptr)
	{
#ifdef BLZ3_USE_DIVX4LINUX
		b3PrintF(B3LOG_NORMAL, "USAGE:\n");
		b3PrintF(B3LOG_NORMAL, "%s Divx-File [-f][-d] {frame images}\n", command);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -d        debug level output\n");
		b3PrintF(B3LOG_NORMAL, "  -f        verbose level output\n");
		b3PrintF(B3LOG_NORMAL, "\n");
#else
		b3PrintF(B3LOG_NORMAL, "%s has no DivX support!\n", command);
#endif
	}
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
}


int main(int argc, char * argv[])
{
	b3FileList    list;
	b3FileEntry * entry;
#ifdef BLZ3_USE_DIVX4LINUX
	ENC_PARAM     encoding;
	ENC_FRAME     frame;
	ENC_RESULT    result;
	char     *    bitstream = nullptr;
	int           error;
#endif
	b3_u08    *   buffer = nullptr;
	b3_u08    *   dstPtr;
	b3_pkd_color * srcPtr, color;
	b3_size       size  = 0;
	b3_res        xSize = 0;
	b3_res        ySize = 0;
	avi_t    *    out;

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	for (int i = 2; i < argc; i++)
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

	out = AVI_open_output_file(argv[1]);
	if (out == NULL)
	{
		fprintf(stderr, "Cannot write %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

#ifdef BLZ3_USE_DIVX4LINUX
	memset(&encoding, 0, sizeof(encoding));
	memset(&frame,    0, sizeof(frame));
#endif

	list.b3Sort();
	for (entry = list.b3First(); entry != nullptr; entry = entry->Succ)
	{
		b3Tx    img;
		b3_bool isFirst = false;
		int     ino = 1;

		try
		{
			img.b3LoadImage(entry->b3Name());
			isFirst = true;
			if (size == 0)
			{
				xSize = (img.xSize + 15) & 0xfff0;
				ySize = (img.ySize +  7) & 0xfff8;
				size  = xSize * ySize;
			}
		}
		catch (b3TxException & t)
		{
			b3PrintF(B3LOG_NORMAL, "\n");
			b3PrintF(B3LOG_NORMAL, "Image error when processing image %s!\n", entry->b3Name());
			b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
			b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
		}
		catch (b3ExceptionBase & e)
		{
			b3PrintF(B3LOG_NORMAL, "\n");
			b3PrintF(B3LOG_NORMAL, "General Blizzard III error on image %s!\n", entry->b3Name());
			b3PrintF(B3LOG_NORMAL, "Error code: %d\n", e.b3GetError());
			b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", e.b3GetErrorMsg());
		}
		catch (...)
		{
			b3PrintF(B3LOG_NORMAL, "\n");
			b3PrintF(B3LOG_NORMAL, "Unknown error occured on image %s!\n", entry->b3Name());
		}

		if (isFirst)
		{
#ifdef BLZ3_USE_DIVX4LINUX
			if (encoding.handle == 0)
			{
				encoding.x_dim     = xSize;
				encoding.y_dim     = ySize;
				encoding.framerate =      25.0;
				encoding.quality   =       5;
				encoding.bitrate   = RATE_KBYTE(500);

				buffer             = new b3_u08[size * 3];
				bitstream          = new char[size * 6];

				error = encore(0, ENC_OPT_INIT, &encoding, 0);
				if ((error != ENC_OK) || (encoding.handle == 0))
				{
					fprintf(stderr, "ERROR CODE: %d (starting encoding)\nexiting...\n", error);
				}
				else
				{
					b3PrintF(B3LOG_DEBUG, "Start encoding...");
				}
				AVI_set_video(out, xSize, ySize, encoding.framerate, (char *)"DIVX");
			}
#endif

			srcPtr = img.b3GetTrueColorData();
			memset(buffer, 0, size * 3);
			if (srcPtr != nullptr)
			{
				// Recode image
				dstPtr  = &buffer[size * 3];
				for (b3_res y = 0; y < img.ySize; y++)
				{
					dstPtr -= (xSize * 3);
					b3_u08 * pixel  = dstPtr;
					for (b3_res x = 0; x < img.xSize; x++)
					{
						color    = srcPtr[x];
						*pixel++ =  color & 0x0000ff;
						*pixel++ = (color & 0x00ff00) >>  8;
						*pixel++ = (color & 0xff0000) >> 16;
					}
					srcPtr += img.xSize;
				}

#ifdef BLZ3_USE_DIVX4LINUX
				frame.image      = buffer;
				frame.bitstream  = bitstream;
				frame.length     =  0;
				frame.quant      =  0;
				frame.intra      = -1;
				frame.colorspace = ENC_CSP_RGB24;

				error = encore(encoding.handle, ENC_OPT_ENCODE, &frame, &result);
				if (error != ENC_OK)
				{
					fprintf(stderr, "\nERROR CODE: %d (encoding frame %d)\nexiting...\n", error, ino);
				}
				else
				{
					AVI_write_frame(out, bitstream, frame.length, 0);
					b3PrintF(B3LOG_DEBUG, "\n encoded frame %s (%d bytes)\n", entry->b3Name(), frame.length);
					b3PrintF(B3LOG_NORMAL, ".");
				}
#endif
			}
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Ignoring image %s\n", entry->b3Name());
		}
		ino++;
	}

#ifdef BLZ3_USE_DIVX4LINUX
	error = encore(encoding.handle, ENC_OPT_RELEASE, 0, 0);
	if (error != ENC_OK)
	{
		fprintf(stderr, "\nERROR CODE: %d (finishing encoding)\nexiting...\n", error);
	}
	else
	{
		b3PrintF(B3LOG_NORMAL, "\nDone.\n");
	}

	if (bitstream != nullptr)
	{
		delete [] bitstream;
	}
#endif
	if (buffer != nullptr)
	{
		delete [] buffer;
	}

	AVI_close(out);

	return EXIT_SUCCESS;
}
