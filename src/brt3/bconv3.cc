/*
**
**	$Filename:	bconv3.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2005/06/09 09:24:00  smork
**	- Added image conversion tool to installation.
**
**	Revision 1.2  2004/11/29 09:58:00  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.1  2001/11/06 17:14:02  sm
**	- Introducing JPEG saving
**	- Made some library fine tunings on TIFF and JPEG
**	- Conversion tool any image in/ JPEG or TIFF image out. Includes:
**	  o scaling
**	  o filtering
**	  o B/W conversion
**	
**
*/

static int parse_option(int argc,int i,char *argv[],char *option)
{
	if (strlen(argv[i]) > 2)
	{
		strcpy(option,&argv[i][2]);
	}
	else
	{
		i++;
		if (i < argc)
		{
			strcpy (option,argv[i]);
		}
		else
		{
			option[0] = 0;
		}
	}
	return i;
}

static void convert_tiff(
	b3Tx       *tx,
	const char *out,
	b3_bool     filtered,
	b3_res      xNewSize,
	b3_res      yNewSize,
	b3_f64      scale)
{
	b3_res newDepth;

	if (tx->b3IsLoaded() && (strlen(out) > 0))
	{
		b3Tx scaled;

		if ((xNewSize == tx->xSize) && (yNewSize == tx->ySize))
		{
			// Resolution not defined -> use scale
			if ((scale > 0) && (scale != 1))
			{
				// Scale is "usable"
				xNewSize = (b3_res)floor(tx->xSize * scale + 0.5);
				yNewSize = (b3_res)floor(tx->ySize * scale + 0.5);
				if (tx->depth == 1)
				{
					newDepth = 8;
				}
				else
				{
					newDepth = tx->depth;
				}
				scaled.b3AllocTx(xNewSize,yNewSize,newDepth);
			}
			else
			{
				// Prevent from scaling
				scaled.b3AllocTx(xNewSize,yNewSize,tx->depth);
			}
		}
		else
		{
			scaled.b3AllocTx(xNewSize,yNewSize,tx->depth);
		}
		
		if (filtered)
		{
			scaled.b3ScaleToGrey(tx);
		}
		else
		{
			scaled.b3Scale(tx);
		}
		scaled.b3SaveTIFF(out);
		tx->b3FreeTx();
	}
}

static void convert_jpeg(
	b3Tx       *tx,
	const char *out,
	b3_u32      quality,
	b3_bool     filtered,
	b3_res      xNewSize,
	b3_res      yNewSize,
	b3_f64      scale)
{
	if (tx->b3IsLoaded() && (strlen(out) > 0))
	{
		b3Tx scaled;

		if ((xNewSize == tx->xSize) && (yNewSize == tx->ySize))
		{
			// Resolution not defined -> use scale
			if ((scale > 0) && (scale != 1))
			{
				// Scale is "usable"
				xNewSize = (b3_res)floor(tx->xSize * scale + 0.5);
				yNewSize = (b3_res)floor(tx->ySize * scale + 0.5);
				scaled.b3AllocTx(xNewSize,yNewSize,24);
			}
			else
			{
				// Prevent from scaling
				scaled.b3AllocTx(xNewSize,yNewSize,24);
			}
		}
		else
		{
			scaled.b3AllocTx(xNewSize,yNewSize,24);
		}
		
		// As opposed to TIFF new image is true color annyway
		if (filtered)
		{
			scaled.b3ScaleToGrey(tx);
		}
		else
		{
			scaled.b3Scale(tx);
		}
		scaled.b3SaveJPEG(out,quality);
		tx->b3FreeTx();
	}
}

static void b3Banner(const char *command)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III Image converter\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001, 2002, 2003, 2004, 2005\n");
	b3PrintF(B3LOG_NORMAL,"\n");
	if (command != null)
	{
		b3PrintF(B3LOG_NORMAL,
			"%s -i input-img [-f][-u][g][-r thresh][-s scale]\n"
			"   [-x width][-y height][-q quality] -j output-jpg|-t output-tiff\n",command);
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"  -f             filtered scaling\n");
		b3PrintF(B3LOG_NORMAL,"  -u             unfiltered scaling\n");
		b3PrintF(B3LOG_NORMAL,"  -g             convert to grey\n");
		b3PrintF(B3LOG_NORMAL,"  -r threshold   convert to B/W image with threshold\n");
		b3PrintF(B3LOG_NORMAL,"  -s scale       scaling image\n");
		b3PrintF(B3LOG_NORMAL,"  -x width       new image width\n");
		b3PrintF(B3LOG_NORMAL,"  -y height      new image height\n");
		b3PrintF(B3LOG_NORMAL,"  -q quality     JPEG quality\n");
		b3PrintF(B3LOG_NORMAL,"  -j filename    save as JPEG\n");
		b3PrintF(B3LOG_NORMAL,"  -t filename    save as TIFF\n");
		b3PrintF(B3LOG_NORMAL,"\n");
	}
	b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
}

int main(int argc,char *argv[])
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

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		return EXIT_SUCCESS;
	}

	for (i = 1;i < argc;i++)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case 'i':
				i = parse_option(argc,i,argv,in_filename);

				// Could be any fileformat
				tx.b3LoadImage(in_filename);

				// Predefine scaling
				xNewSize = tx.xSize;
				yNewSize = tx.ySize;
				break;

			case 'j':
				i = parse_option(argc,i,argv,out_filename);
				convert_jpeg(&tx,out_filename,quality,filtered,xNewSize,yNewSize,scale);
				break;

			case 't':
				i = parse_option(argc,i,argv,out_filename);
				convert_tiff(&tx,out_filename,filtered,xNewSize,yNewSize,scale);
				break;

			case 'f':
				filtered = true;
				break;
			case 'u':
				filtered = false;
				break;

			case 'g':
				i = parse_option(argc,i,argv,number);
				grey = atof(number);
				if (tx.b3IsLoaded())
				{
					tx.b3TransToBW(grey,B3_THRESHOLD_WHITE_RATIO);
				}
				break;
			case 'r':
				i = parse_option(argc,i,argv,number);
				ratio = atof(number);
				if (tx.b3IsLoaded())
				{
					tx.b3TransToBW(ratio,B3_THRESHOLD_USE);
				}
				break;
			case 'q':
				i = parse_option(argc,i,argv,number);
				quality = atoi(number);
				break;

			case 's':
				i = parse_option(argc,i,argv,number);
				scale = atof(number);
				break;
			case 'x':
				i = parse_option(argc,i,argv,number);
				xNewSize = atoi(number);
				break;
			case 'y':
				i = parse_option(argc,i,argv,number);
				yNewSize = atoi(number);
				break;
			}
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"Argument error!\n");
		}
	}

	return EXIT_SUCCESS;
}
