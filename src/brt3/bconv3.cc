/*
**
**	$Filename:	bconv3.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting image formats
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Dir.h"
#include "blz3/image/b3Tx.h"

static void convert_tiff(
	b3Tx    *   tx,
	const char * out,
	b3_bool     filtered,
	b3_res      xNewSize,
	b3_res      yNewSize,
	b3_f64      scale)
{
	b3_res newDepth;

	if(tx->b3IsLoaded() && (strlen(out) > 0))
	{
		b3Tx scaled;

		if((xNewSize == tx->xSize) && (yNewSize == tx->ySize))
		{
			// Resolution not defined -> use scale
			if((scale > 0) && (scale != 1))
			{
				// Scale is "usable"
				xNewSize = (b3_res)floor(tx->xSize * scale + 0.5);
				yNewSize = (b3_res)floor(tx->ySize * scale + 0.5);
				if(tx->depth == 1)
				{
					newDepth = 8;
				}
				else
				{
					newDepth = tx->depth;
				}
				scaled.b3AllocTx(xNewSize, yNewSize, newDepth);
			}
			else
			{
				// Prevent from scaling
				scaled.b3AllocTx(xNewSize, yNewSize, tx->depth);
			}
		}
		else
		{
			scaled.b3AllocTx(xNewSize, yNewSize, tx->depth);
		}

		if(filtered)
		{
			scaled.b3ScaleToGrey(tx);
		}
		else
		{
			scaled.b3Scale(tx);
		}
#ifdef HAVE_LIBTIFF
		scaled.b3SaveTIFF(out);
#endif
		tx->b3FreeTx();
	}
}

static void convert_jpeg(
	b3Tx    *   tx,
	const char * out,
	b3_u32      quality,
	b3_bool     filtered,
	b3_res      xNewSize,
	b3_res      yNewSize,
	b3_f64      scale)
{
	if(tx->b3IsLoaded() && (strlen(out) > 0))
	{
		b3Tx scaled;

		if((xNewSize == tx->xSize) && (yNewSize == tx->ySize))
		{
			// Resolution not defined -> use scale
			if((scale > 0) && (scale != 1))
			{
				// Scale is "usable"
				xNewSize = (b3_res)floor(tx->xSize * scale + 0.5);
				yNewSize = (b3_res)floor(tx->ySize * scale + 0.5);
				scaled.b3AllocTx(xNewSize, yNewSize, 24);
			}
			else
			{
				// Prevent from scaling
				scaled.b3AllocTx(xNewSize, yNewSize, 24);
			}
		}
		else
		{
			scaled.b3AllocTx(xNewSize, yNewSize, 24);
		}

		// As opposed to TIFF new image is true color annyway
		if(filtered)
		{
			scaled.b3ScaleToGrey(tx);
		}
		else
		{
			scaled.b3Scale(tx);
		}
#ifdef HAVE_LIBJPEG
		scaled.b3SaveJPEG(out, quality);
#endif
		tx->b3FreeTx();
	}
}

static void b3Banner(const char * command)
{
	b3PrintF(B3LOG_NORMAL, "Blizzard III Image converter\n");
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-2007\n");
	b3PrintF(B3LOG_NORMAL, "\n");
	if(command != null)
	{
		b3PrintF(B3LOG_NORMAL,
			"%s -i input-img [-f][-u][g][-r thresh][-s scale]\n"
			"   [-x width][-y height][-q quality] -j output-jpg|-t output-tiff\n", command);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -f             filtered scaling\n");
		b3PrintF(B3LOG_NORMAL, "  -u             unfiltered scaling\n");
		b3PrintF(B3LOG_NORMAL, "  -g             convert to grey\n");
		b3PrintF(B3LOG_NORMAL, "  -r threshold   convert to B/W image with threshold\n");
		b3PrintF(B3LOG_NORMAL, "  -s scale       scaling image\n");
		b3PrintF(B3LOG_NORMAL, "  -x width       new image width\n");
		b3PrintF(B3LOG_NORMAL, "  -y height      new image height\n");
		b3PrintF(B3LOG_NORMAL, "  -q quality     JPEG quality\n");
		b3PrintF(B3LOG_NORMAL, "  -j filename    save as JPEG\n");
		b3PrintF(B3LOG_NORMAL, "  -t filename    save as TIFF\n");
		b3PrintF(B3LOG_NORMAL, "\n");
	}
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
}

int main(int argc, char * argv[])
{
	b3Tx    tx;
	int     i;
	b3Path  in_filename;
	b3Path  out_filename;
	char    number[1024]; // Oops! Possible stack overrun! Should be fixed!
	b3_bool filtered = false;
	b3_f64  scale    =   1.0;
	b3_f64  grey;
	b3_f64  ratio;
	b3_u32  quality  =  75;
	b3_res  xNewSize = 640;
	b3_res  yNewSize = 480;

	if(argc <= 1)
	{
		b3Banner(argv[0]);
		return EXIT_SUCCESS;
	}

	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case 'i':
				i = b3Runtime::b3ParseOption(argc, argv, i, in_filename, B3_FILESTRINGLEN);

				// Could be any fileformat
				tx.b3LoadImage(in_filename);

				// Predefine scaling
				xNewSize = tx.xSize;
				yNewSize = tx.ySize;
				break;

			case 'j':
				i = b3Runtime::b3ParseOption(argc, argv, i, out_filename, B3_FILESTRINGLEN);
				convert_jpeg(&tx, out_filename, quality, filtered, xNewSize, yNewSize, scale);
				break;

			case 't':
				i = b3Runtime::b3ParseOption(argc, argv, i, out_filename, B3_FILESTRINGLEN);
				convert_tiff(&tx, out_filename, filtered, xNewSize, yNewSize, scale);
				break;

			case 'f':
				filtered = true;
				break;
			case 'u':
				filtered = false;
				break;

			case 'g':
				i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
				grey = atof(number);
				if(tx.b3IsLoaded())
				{
					tx.b3TransToBW(grey, B3_THRESHOLD_WHITE_RATIO);
				}
				break;
			case 'r':
				i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
				ratio = atof(number);
				if(tx.b3IsLoaded())
				{
					tx.b3TransToBW(ratio, B3_THRESHOLD_USE);
				}
				break;
			case 'q':
				i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
				quality = atoi(number);
				break;

			case 's':
				i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
				scale = atof(number);
				break;
			case 'x':
				i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
				xNewSize = atoi(number);
				break;
			case 'y':
				i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
				yNewSize = atoi(number);
				break;
			}
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Argument error!\n");
		}
	}

	return EXIT_SUCCESS;
}
