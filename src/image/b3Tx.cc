/*
**
**	$Filename:	b3Tx.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"
#include "blz3/system/b3Log.h"

#include <math.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.16  2002/01/01 13:50:21  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**
**	Revision 1.15  2001/12/23 10:58:38  sm
**	- Accelerated b3Display.
**	- Fixed YUV conversion.
**	- Accelerated ILBM access to image  pixel/row.
**	
**	Revision 1.14  2001/12/23 08:57:21  sm
**	- Fixed recursive calling bug in b3IsObscured(...)
**	- Minor intersection optimazations done.
**	
**	Revision 1.13  2001/12/16 11:07:45  sm
**	- Fixed b3Tx::b3Copy from ILBM images with color depth from 2 to 8.
**	  These images are converted into B3_TX_VGA now
**	- b3ScaleToGrey() fixed using correct palette
**	
**	Revision 1.12  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.11  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.10  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.9  2001/10/17 14:46:02  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**	
**	Revision 1.8  2001/10/16 15:21:24  sm
**	- Minor changes to compile with GCC 3.x
**	
**	Revision 1.7  2001/10/15 14:45:07  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
**	Revision 1.6  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.5  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	Revision 1.4  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	Revision 1.3  2001/07/03 18:14:08  sm
**	- Now having running threads. The system lib
**	  needed the appropriate project options
**	
**	Revision 1.2  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
**	Revision 1.8  2000/10/06 14:23:38  smork
**	- Palette handling for b3TxContrast() and b3TxColorFilter()
**	  added.
**	- Added Twain dialog button for Blizzard III Imaging
**	
**	Revision 1.7  2000/10/02 06:14:28  smork
**	- Introduced Blizzard III Imaging. This is a case study
**	  for creating new Blizzard III applications. Any new
**	  student has to learn exact this task. The
**	  applecation is missing following features:
**	  o saving main frame window format
**	  o size & position of toolbar
**	- New controls introduced:
**	  o CMauiShowGauss    for removing Gauss artefacts
**	                      from image
**	  o CMauiShowColor    for showing HSV color selection
**	  o CMauiHSVField     for slider legend of HSV color
**	                      selection
**	  Controls added to libIntGUI
**	- Added image anhancement for RGB8 images inside
**	  b3Tx:
**	  o b3TxGauss(...)       for removing Gauss arte-
**	                           facts
**	  O b3TxColorFilter(...) for image color filtering
**	
**	- PS:
**	  The CfM Twain32 5.20 driver works fine.
**	- PPS:
**	  It was nice to use MFC. Shall I change to server
**	  development? I don't think so...
**	
**	Revision 1.6  2000/09/29 13:04:02  smork
**	- Correct modifying HSP's when using Blizzard III Admin.
**	- Doing OnUpdate(MAUI_UPDATE_TX); inside CBlizzard IIIScrollView::OnInitialUpdate();
**	- Using "cfm twain" as CfM Twain driver key for *NOT* using
**	  simple TW_ONEVALUE.
**	
**	Revision 1.5  2000/09/29 11:14:57  smork
**	- ICAP_GAMMA changed from TW_RANGE to TW_ONEVALUE. It's working
**	  now with correct values. So the release version works now
**	  properly.
**	- The histogramme was computed wrong! The black usage is computed
**	  black pixel divided by all pixel (and not white pixel).
**	- When computing white page ratio for gray images count middle
**	  values as dark values.
**	
**	Revision 1.4  2000/08/14 12:26:34  smork
**	- Converting typo fixed
**	
**	Revision 1.3  2000/08/14 11:11:23  smork
**	- Inserted change log
**	- Inserted author tag
**	- Cleaned up file header
**	
*/

/*************************************************************************
**                                                                      **
**                        some constant tables                          **
**                                                                      **
*************************************************************************/

static const b3_u08 Bits[8] =
{
	128,64,32,16,8,4,2,1
};

static b3_bool errorHandlerInstalled = false;

/*************************************************************************
**                                                                      **
**                        TIFF error/warning handler                    **
**                                                                      **
*************************************************************************/

static void b3TIFFErrorHandler(
	const char *module,
	const char *fmt,
	va_list     args)
{
	char message[512];

	vsprintf (message,fmt,args);
	b3PrintF(B3LOG_NORMAL,"ERROR: %s %s\n",module,message);
}

static void b3TIFFWarnHandler(
	const char *module,
	const char *fmt,
	va_list     args)
{
	char    message[512];

	vsprintf (message,fmt,args);
	b3PrintF(B3LOG_NORMAL,"WARNING: %s %s\n",module,message);
}

b3Tx::b3Tx() : b3Link<b3Tx>(sizeof(b3Tx),USUAL_TEXTURE)
{
	data        = null;
	palette     = null;
	histogramme = null;
	grid        = null;
	type        = B3_TX_UNDEFINED;
	FileType    = FT_UNKNOWN;
	dSize       = 0;
	pSize       = 0;

	xSize       = 0;
	ySize       = 0;
	depth       = 0;
	ScanLines   = 0;

	image_name.b3Empty();
	whiteRatio  = 0.025;
	xDPI        = 1;
	yDPI        = 1;

	// set TIFF error und warning handler
	if (!errorHandlerInstalled)
	{
		TIFFSetErrorHandler   (b3TIFFErrorHandler);
		TIFFSetWarningHandler (b3TIFFWarnHandler);
		errorHandlerInstalled = true;
	}
	measure.b3Init(0,0,0);
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx instantiated (%p)\n",this);
#endif
}

b3Tx::b3Tx(b3Tx *orig) : b3Link<b3Tx>(sizeof(b3Tx),USUAL_TEXTURE)
{
	image_name.b3Empty();
	histogramme = null;
	grid        = null;
	data        = null;
	palette     = null;
	b3Copy(orig);
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx instantiated (%ldx%ld, %ld bits) (%p)\n",
		xSize,ySize,depth,this);
#endif
}

b3Tx::~b3Tx()
{
#ifdef VERBOSE
	b3Dump();
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx destroyed (%p)\n",this);
#endif
}

b3_bool b3Tx::b3AllocTx(
	b3_res x,
	b3_res y,
	b3_res d)
{
	b3FreeTx();
	data    = null;
	palette = null;

	dSize   = 0;
	pSize   = 0;
		   
	xSize   = 0;
	ySize   = 0;
	depth   = 0;

	if (d == 1)
	{
		dSize = TX_BWA(x) * y;
		pSize = 2;
		type  = B3_TX_ILBM;
	}
	if ((d > 1) && (d <= 8))
	{
		dSize =  x * y;
		pSize = 1 << d;
		type  = B3_TX_VGA;
	}
	if (d == 16)
	{
		dSize = (x * y) * sizeof(b3_u16);
		type  = B3_TX_RGB4;
	}
	if ((d == 24) || (d == 32))
	{
		dSize = (x * y) * sizeof(b3_pkd_color);
		type  = B3_TX_RGB8;
	}
	if (type == B3_TX_UNDEFINED)
	{
		return false;
	}

	if (dSize > 0)
	{
		data    = (b3_u08 *)b3Alloc(dSize);
		if (data == null)
		{
			type = B3_TX_UNDEFINED;
			return false;
		}
	}

	if (pSize > 0)
	{
		palette = (b3_pkd_color *)b3Alloc(pSize * sizeof(b3_pkd_color));
		if (palette == null)
		{
			b3Free();
			type = B3_TX_UNDEFINED;
			return false;
		}
	}

		// setting size values
	xSize  = x;
	ySize  = y;
	depth  = d;

		// initializing default palettes
	if (depth == 1)
	{
		palette[0] = 0x00ffffff;
		palette[1] = 0x00000000;
	}

	if (depth == 8)
	{
		long i;

		for (i = 0;i < 256;i++)
		{
			palette[i] = 0x010101 * i;
		}
	}
	measure.b3Init(xSize,ySize,depth);
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3AllocTx(%ldx%ld, %ld bits)\n",
		xSize,ySize,depth);
	return data != null;
}

b3_bool b3Tx::b3IsGreyPalette()
{
	b3_index     i;
	b3_count     max;
	b3_pkd_color mul,step;

	if (type != B3_TX_VGA)
	{
		return false;
	}

	if (depth > 8)
	{
		return false;
	}

	max  =   1 << depth;
	step = 256 >> depth;
	for (i = 0,mul = 0;i < max;i++,mul += step)
	{
		if ((0x010101 * mul) != palette[i])
		{
			return false;
		}
	}
	return true;
}

inline void b3Tx::b3CopyILBMtoVGA (
	b3_u08   *row,
	b3_coord  y)
{
	b3_u08   *Data;
	b3_u08    dstBit = 1,Bit;
	b3_coord  x,d;
	b3_count  xBytes,xRest,index,BytesPerLine;

	BytesPerLine = TX_BWA(xSize);
	if ((palette != null) && (depth < 8))
	{
		xBytes = xSize >> 3;
		xRest  = xSize  & 7;
		Data = &data[y * BytesPerLine * depth];
		for (d = 0;d < depth;d++)
		{
			index = 0;

			// Check complete source bytes
			for (x = 0;x < xBytes;x++)
			{
				if (Data[x] != 0)
				{
					if (Data[x] & 0x80) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x40) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x20) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x10) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x08) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x04) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x02) row[index] |= dstBit;
					index++;
					if (Data[x] & 0x01) row[index] |= dstBit;
					index++;
				}
				else
				{
					index += 8;
				}
			}

			// Compute rest bits of last byte
			for (x = 0;x < xRest;x++)
			{
				Bit = 128;
				if (Data[xBytes] & Bit) row[index] |= dstBit;
				index++;
			}

			// Do we have reached end of line correctly?
			B3_ASSERT(index <= xSize);

			// Bump indices
			dstBit <<= 1;
			Data    += BytesPerLine;
		}
	}
}

inline void b3Tx::b3CopyILBMtoRGB8 (
	b3_pkd_color *row,
	b3_coord      y)
{
	b3_u08       *Data;
	b3_coord      x,d,BytesPerLine;
	b3_pkd_color  Color,Bit;

	BytesPerLine = TX_BWA(xSize);
	if (palette == null)
	{
		for (x = 0;x < xSize;x++)
		{
			Data   = data;
			Data  += ((y+1) * BytesPerLine * depth + (x >> 3) - BytesPerLine);
			Color  = 0;
			Bit    = 128 >> (x & 7);
			for (d = 0;d < depth;d++)
			{
				Color *= 2;
				if (Data[0] & Bit) Color |= 1;
				Data -= BytesPerLine;
			}
			row[x] =
				((Color & 0x0000ff) << 16) |
				((Color & 0x00ff00)) |
				((Color & 0xff0000) >> 16);
		}
	}
}

void b3Tx::b3Copy(b3Tx *srcTx)
{
	type = B3_TX_UNDEFINED;
	if (!srcTx->b3IsLoaded())
	{
		b3FreeTx();
		return;
	}

	if (b3AllocTx(srcTx->xSize,srcTx->ySize,srcTx->depth))
	{
		b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3Copy(): ");
		if (data != null)
		{
			if (srcTx->type == B3_TX_ILBM)
			{
				b3_u08       *cPtr;
				b3_pkd_color *lPtr;
				b3_coord      y;

				switch (type)
				{
				case B3_TX_VGA:
					cPtr = (b3_u08 *)data;
					for (y = 0;y < ySize;y++)
					{
						srcTx->b3CopyILBMtoVGA(cPtr,y);
						cPtr += xSize;
					}
					break;

				case B3_TX_RGB8:
					lPtr = (b3_pkd_color *)data;;
					for (y = 0;y < ySize;y++)
					{
						srcTx->b3CopyILBMtoRGB8(lPtr,y);
						lPtr += xSize;
					}
					break;

				case B3_TX_ILBM:
					// Never created
					memcpy (data,   srcTx->b3GetData(),   dSize);
					break;

				default:
				case B3_TX_RGB4:
					b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx   # b3Copy(): unsupported destination format!\n");
				}
			}
			else
			{
				memcpy (data,   srcTx->b3GetData(),   dSize);
			}
			b3PrintF(B3LOG_FULL," [data - %ld]",dSize);
		}
		if (palette != null)
		{
			memcpy (palette,srcTx->b3GetPalette(),pSize * sizeof(b3_pkd_color));
			b3PrintF(B3LOG_FULL," [palette - %ld]",pSize);
		}

	
		b3PrintF(B3LOG_FULL,"\n");
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx # b3Copy(): NOT ENOUGH MEMORY!\n");
		throw new b3TxException(B3_TX_MEMORY);
	}

	xDPI        = srcTx->xDPI;
	yDPI        = srcTx->yDPI;
	whiteRatio  = srcTx->whiteRatio;
	ScanLines   = srcTx->ScanLines;
	measure     = srcTx->measure;
}

void b3Tx::b3FreeTx()
{
	if (data != null)
	{
		b3Free (data);
		data = null;
	}
	if (palette != null)
	{
		try
		{
			b3Free ((void *)palette);
		}
		catch(...)
		{
			b3PrintF(B3LOG_DEBUG,"### CLASS: b3Tx   # b3Tx() Oops?\n");
			b3PrintF(B3LOG_DEBUG,"### CLASS: b3Tx   # b3Tx() %s %d\n",__FILE__,__LINE__);
		}
		palette = null;
	}
	b3EndHist();
	type      = B3_TX_UNDEFINED;
	FileType  = FT_UNKNOWN;
	xSize     = 0;
	ySize     = 0;
	depth     = 0;
	ScanLines = 0;
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3FreeTx()\n");
}

void *b3Tx::b3GetData()
{
	return data;
}

b3_bool b3Tx::b3IsLoaded()
{
	return ((xSize != 0) && (ySize != 0) && (data != null));
}

char *b3Tx::b3Name()
{
	return (char *)image_name;
}

void b3Tx::b3Name(const char *ImageName)
{
	strcpy (image_name,ImageName != null ? ImageName : "");
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3Name(%s)\n",
		(const char *)image_name);
}

b3Measure *b3Tx::b3GetMeasure()
{
	return &measure;
}

b3_pkd_color *b3Tx::b3GetPalette()
{
	return palette;
}

void b3Tx::b3SetPalette(
	b3_pkd_color *newPalette,
	b3_count      NumColors)
{
	// compute bit depth
	for (depth = 0;NumColors > 1;depth++)
	{
		NumColors = NumColors >> 1;
	}
	type = (depth == 1 ? B3_TX_ILBM : B3_TX_VGA);

	// exchange pointer
	b3Free (palette);
	palette = newPalette;
}

void b3Tx::b3SetData(void *newData,b3_res newXSize,b3_res newYSize)
{
	b3EndHist();
	b3Free(data);
	data  = (b3_u08 *)newData;

	xSize = newXSize;
	ySize = newYSize;
}

void b3Tx::b3GetResolution(b3_res &xd,b3_res &yd)
{
	xd = xDPI;
	yd = yDPI;
}

b3_bool b3Tx::b3IsBW()
{
	return (depth == 1) && (type == B3_TX_ILBM);
}

b3_bool b3Tx::b3IsTrueColor()
{
	return (depth >= 24) && (type == B3_TX_RGB8);
}

b3_bool b3Tx::b3IsPalette()
{
	return palette != null;
}

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

b3_pkd_color b3Tx::b3GetValue (
	b3_coord x,
	b3_coord y)
{
	switch (type)
	{
		case B3_TX_ILBM : return b3ILBMValue (x,y);
		case B3_TX_RGB4 : return b3RGB4Value (x,y);
		case B3_TX_RGB8 : return b3RGB8Value (x,y);
		case B3_TX_VGA  : return b3VGAValue  (x,y);
	}
	return 0;
}

inline b3_pkd_color b3Tx::b3ILBMValue (
	b3_coord x,
	b3_coord y)
{
	b3_u08       *Address;
	b3_pkd_color  Bit,PlaneValue;
	b3_index      BytesPerLine;
	b3_res        i;

	BytesPerLine = TX_BWA(xSize);
	PlaneValue   = 0;
	Address      = (b3_u08 *)data;
	Address     += ((y + 1) * BytesPerLine * depth + (x >> 3));
	Bit          = Bits[x & 7];
	for (i = 0;i < depth;i++)
	{
		Address     -= BytesPerLine;
		PlaneValue <<= 1;
		if (Address[0] & Bit)
		{
			PlaneValue |= 1;
		}
	}
	if (palette != null)
	{
		return palette[PlaneValue];
	}

	if (depth >= 24)
	{
		i = PlaneValue;
		PlaneValue  = (i & 0xff0000) >> 16;
		PlaneValue |=  i & 0x00ff00;
		PlaneValue |= (i & 0x0000ff) << 16;
	}
	return PlaneValue;
}

inline b3_pkd_color b3Tx::b3RGB4Value (
	b3_coord x,
	b3_coord y)
{
	register b3_u16 *Address;
	register b3_pkd_color   Colour,Result;

	Address  = (b3_u16 *)data;
	Address += (y * xSize + x);
	Colour = (b3_pkd_color)Address[0];
	Result  = (Colour & 0x0f00) << 12;
	Result |= (Colour & 0x00f0) <<  8;
	Result |= (Colour & 0x000f) <<  4;
	return Result;
}

inline b3_pkd_color b3Tx::b3RGB8Value (
	b3_coord x,
	b3_coord y)
{
	register b3_pkd_color *Address;

	Address  = (b3_pkd_color *)data;
	return Address[y * xSize + x];
}

inline b3_pkd_color b3Tx::b3VGAValue (
	b3_coord x,
	b3_coord y)
{
	return palette == null ? 0 : palette[data[y * xSize + x]];
}

/*************************************************************************
**                                                                      **
**                        for stenceling                                **
**                                                                      **
*************************************************************************/

b3_bool b3Tx::b3IsBackground(b3_coord x,b3_coord y)
{
	b3_u08       *cPtr,bit;
	b3_u16       *sPtr;
	b3_pkd_color *lPtr,result;
	b3_count      i,xBytes;

	switch (type)
	{
		case B3_TX_ILBM :
			xBytes = TX_BWA(xSize);
			cPtr   = (b3_u08 *)data;
			cPtr  += ((y + 1) * xBytes * depth + (x >> 3));
			result = 0;
			bit    = Bits[x & 7];
			for (i = 0;i < depth;i++)
			{
				cPtr    -= xBytes;
				result <<= 1;
				if (*cPtr & bit)
				{
					result |= 1;
				}
			}

			return result != 0;

		case B3_TX_RGB8	:
			lPtr = (b3_pkd_color *)data;
			return (lPtr[x + y * xSize] & 0x00ffffff) != 0;

		case B3_TX_RGB4	:
			sPtr = (b3_u16 *)data;
			return (sPtr[x + y * xSize] & 0x0fff) != 0;

		case B3_TX_VGA	:
			cPtr = (b3_u08 *)data;
			return cPtr[x + y * xSize] != 0;
	}
	return false;
}

/*************************************************************************
**                                                                      **
**                        get a complete texture line                   **
**                                                                      **
*************************************************************************/

inline void b3Tx::b3GetILBM (
	b3_pkd_color *ColorLine,
	b3_coord      y)
{
	b3_u08       *Data;
	b3_coord      x,d,BytesPerLine;
	b3_pkd_color  Color,Bit;

	BytesPerLine = TX_BWA(xSize);
	if (palette != null)
	{
		for (x = 0;x < xSize;x++)
		{
			Data   = (b3_u08 *)data;
			Data  += ((y+1) * BytesPerLine * depth + (x >> 3));
			Color  = 0;
			Bit    = Bits[x & 7];
			for (d = 0;d < depth;d++)
			{
				Data   -= BytesPerLine;
				Color <<= 1;
				if (Data[0] & Bit)
				{
					Color |= 1;
				}
			}
			ColorLine[0] = palette[Color];
			ColorLine++;
		}
	}
	else
	{
		for (x = 0;x < xSize;x++)
		{
			Data   = data;
			Data  += ((y + 1) * BytesPerLine * depth + (x >> 3));
			Color  = 0;
			Bit    = Bits[x & 7];
			for (d = 0;d < depth;d++)
			{
				Data   -= BytesPerLine;
				Color <<= 1;
				if (Data[0] & Bit)
				{
					Color |= 1;
				}
			}
			ColorLine[0] =
				((Color & 0x0000ff) << 16) |
				((Color & 0x00ff00)) |
				((Color & 0xff0000) >> 16);
			ColorLine++;
		}
	}
}

inline void b3Tx::b3GetRGB8 (
	b3_pkd_color *dst,
	b3_coord      y)
{
	b3_pkd_color *src;

	src = (b3_pkd_color *)data;
#if 0
	memcpy (dst,&src[y * xSize],xSize * sizeof(b3_pkd_color));
#else
	b3_coord x;
	b3_res   xInnerMax,xOuterMax;

	src += (y * xSize);

	xOuterMax = xSize >> 3;
	xInnerMax = xSize & 0x7;
	for (x = 0;x < xOuterMax;x++)
	{
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
	}
	for (x = 0;x < xInnerMax;x++)
	{
		*dst++ = *src++;
	}
#endif
}

inline void b3Tx::b3GetRGB4 (
	b3_pkd_color *ColorLine,
	b3_coord      y)
{
	b3_u16       *Data;
	b3_coord      x;
	b3_pkd_color  Color;

	Data  = (b3_u16 *)data;
	Data += (xSize * y);
	for (x = 0;x < xSize;x++)
	{
		Color  = ((long)Data[0] & 0x0f00) << 12;
		Color |= ((long)Data[0] & 0x00f0) <<  8;
		Color |= ((long)Data[0] & 0x000f) <<  4;
		ColorLine[0] = Color;
		ColorLine++;
		Data++;
	}
}

inline void b3Tx::b3GetVGA (
	b3_pkd_color *ColorLine,
	b3_coord      y)
{
	register b3_u08   *Data;
	register b3_coord  x;

	Data    = (b3_u08 *)data;
	Data   += (xSize * y);
	for (x = 0;x < xSize;x++)
	{
		ColorLine[0] = palette[Data[0]];
		ColorLine++;
		Data++;
	}
}

void b3Tx::b3GetRow (
	b3_pkd_color *Line,
	b3_coord      y)
{
	switch (type)
	{
		case B3_TX_ILBM :
			b3GetILBM (Line,y);
			break;
		case B3_TX_RGB8 :
			b3GetRGB8 (Line,y);
			break;
		case B3_TX_RGB4 :
			b3GetRGB4 (Line,y);
			break;
		case B3_TX_VGA :
			b3GetVGA  (Line,y);
			break;
	}
}
