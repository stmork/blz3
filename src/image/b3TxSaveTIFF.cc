/*
**
**	$Filename:	b3TxSaveTIFF.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - TIFF saving
**
**	(C) Copyright 2001  Steffen A. Mork
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
**	Revision 1.7  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**
**	Revision 1.6  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.5  2002/03/13 19:01:58  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.4  2002/01/01 13:50:22  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.3  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.2  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
**	Revision 1.4  2000/09/25 06:42:48  smork
**	- Excessive usage of enum b3_result to prevent type conflicts.
**	
**	Revision 1.3  2000/09/21 10:22:27  smork
**	- Setting Blizzard III projects to warning: Found
**	  some uninitialized variables.
**	- changed b3Mutex from CMutex to CRITICAL_SECTION (thread
**	  synchronization only)
**	- introduced b3IPCMutex for process synchronization
**	
**	Revision 1.2  2000/08/14 11:11:23  smork
**	- Inserted change log
**	- Inserted author tag
**	- Cleaned up file header
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Tx saving TIFFs                             **
**                                                                      **
*************************************************************************/

// These methods save the image associated with this instance as a TIFF.
// Look at the TIFF programming manual for understanding this. Be happy
// with this piece of code.
void b3Tx::b3GetSampleValues (
	long &bpp,
	long &spp)
{
	switch (type)
	{
	case B3_TX_ILBM :
		if (depth > 8)	// true color
		{
			bpp = 8;
			spp = 3;
		}
		else
		{
			bpp = depth;
			spp = 1;
		}
		break;
	case B3_TX_VGA :
		bpp = depth;
		spp = 1;
		break;
	case B3_TX_RGB4 :
	case B3_TX_RGB8 :
		bpp = 8;
		spp = 3;
	default :
		bpp = 8;
		spp = 1;
	}
}

b3_result b3Tx::b3SaveTIFFPalette(TIFF *tiff)
{
	b3_u08      *cPtr,*pPtr;
	b3_u16       r[256],g[256],b[256];
	b3_coord     y;
	b3_res       max;
	char         software[128];
	b3_tx_error  result = B3_TX_OK;

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx:  # saving TIFF paletted image (%ldx%ld)\n",
		xSize,ySize);
	TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,  depth);
	TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,1);
	TIFFSetField(tiff,TIFFTAG_XRESOLUTION,(b3_f64)xDPI);
	TIFFSetField(tiff,TIFFTAG_YRESOLUTION,(b3_f64)yDPI);
	TIFFSetField(tiff,TIFFTAG_RESOLUTIONUNIT,RESUNIT_INCH);

	TIFFSetField(tiff,TIFFTAG_COMPRESSION,COMPRESSION_LZW);
	TIFFSetField(tiff,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_PALETTE);
	TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,ySize);
	TIFFSetField(tiff,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG);

	sprintf (software,"Blizzard III V%d.%02d",B3_VERSION,B3_REVISION);
	TIFFSetField( tiff, TIFFTAG_SOFTWARE, software );

	// setting palette
	max = 1 << depth;
	for (y = 0;y < max;y++)
	{
		r[y] = (b3_u16)((palette[y] & 0xff0000) >> 8);
		g[y] = (b3_u16) (palette[y] & 0x00ff00);  
		b[y] = (b3_u16)((palette[y] & 0x0000ff) << 8);
	}
	TIFFSetField (tiff,TIFFTAG_COLORMAP,&r,&g,&b);

	// OK, writing image data...
	cPtr = (b3_u08 *)data;
	for (y = 0;y < ySize;y++)
	{
		pPtr = cPtr;	// to be changed for every bit depth...
		if (!TIFFWriteScanline(tiff, pPtr, y, 0))
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		cPtr += xSize;
	}

	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException,result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFFFax(TIFF *tiff)
{
	b3_coord     y;
	b3_count     xBytes;
	b3_u08      *cPtr   = data;
	char         software[128];
	b3_tx_error  result = B3_TX_OK;

	// setting up tag directory
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx:  # saving TIFF fax G4 (%ldx%ld)\n",
		xSize,ySize);
	TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,  1);
	TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,1);
	TIFFSetField(tiff,TIFFTAG_XRESOLUTION,(b3_f64)xDPI);
	TIFFSetField(tiff,TIFFTAG_YRESOLUTION,(b3_f64)yDPI);
	TIFFSetField(tiff,TIFFTAG_RESOLUTIONUNIT,RESUNIT_INCH);

	TIFFSetField(tiff,TIFFTAG_COMPRESSION,COMPRESSION_CCITTFAX4);
	TIFFSetField(tiff,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_MINISWHITE);
	TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,ySize);
	TIFFSetField(tiff,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG);

	sprintf (software,"Blizzard III V%d.%02d",B3_VERSION,B3_REVISION);
	TIFFSetField( tiff, TIFFTAG_SOFTWARE, software );

	xBytes = TX_BWA(xSize);
	for (y = 0;y < ySize;y++)
	{
		if (!TIFFWriteScanline(tiff, cPtr, y, 0))
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		cPtr += xBytes;
	}

	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException,result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFFTrueColor(TIFF *tiff)
{
	b3_pkd_color *lPtr;
	b3_coord      x,y;
	b3_u08       *row,*rPtr,*gPtr,*bPtr;
	char          software[128];
	b3_tx_error   result = B3_TX_OK;

	// setting the TIFF tags
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx:  # saving TIFF true color (%ldx%ld)\n",
		xSize,ySize);
	TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,     xSize);
	TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,    ySize);
	TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,  8);
	TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,3);
	TIFFSetField(tiff,TIFFTAG_XRESOLUTION,(b3_f64)xDPI);
	TIFFSetField(tiff,TIFFTAG_YRESOLUTION,(b3_f64)yDPI);
	TIFFSetField(tiff,TIFFTAG_RESOLUTIONUNIT,RESUNIT_INCH);

	TIFFSetField(tiff,TIFFTAG_COMPRESSION,COMPRESSION_PACKBITS);
	TIFFSetField(tiff,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_RGB);
	TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,1);
	TIFFSetField(tiff,TIFFTAG_PLANARCONFIG,PLANARCONFIG_SEPARATE);

	sprintf (software,"Blizzard III V%d.%02d",B3_VERSION,B3_REVISION);
	TIFFSetField( tiff, TIFFTAG_SOFTWARE, software );

	// alloc memory for three channels of one row
	row = (b3_u08 *)b3Alloc (xSize * 3 * sizeof(b3_u08));
	if (row == null)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3SaveTIFFTrueColor(): Not enough memory to allocate row\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	lPtr = (b3_pkd_color *)data;
	rPtr =  row;
	gPtr = &row[xSize];
	bPtr = &row[xSize + xSize];
	for (y = 0; y < ySize;y++)
	{
		// resample row
		for (x = 0;x < xSize;x++)
		{
			rPtr[x]  = (b3_u08)((lPtr[x] & 0xff0000) >> 16);
			gPtr[x]  = (b3_u08)((lPtr[x] & 0x00ff00) >>  8);
			bPtr[x]  = (b3_u08) (lPtr[x] & 0x0000ff);
		}

		// try to save red channel
		if (TIFFWriteScanline(tiff,rPtr,y,0) != 1)
		{
			y      = ySize;
			result = B3_TX_NOT_SAVED;
		}
		else
		{
			// try to save green channel
			if (TIFFWriteScanline(tiff,gPtr,y,1) != 1)
			{
				y = ySize;
				result = B3_TX_NOT_SAVED;
			}
			else
			{
				// try to save blue channel
				if (TIFFWriteScanline(tiff,bPtr,y,2) != 1)
				{
					y = ySize;
					result = B3_TX_NOT_SAVED;
				}
			}
		}

		// increase color source ptr by xSize
		lPtr += xSize;
	}
	b3Free (row);


	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException,result);
	}
	return B3_OK;
}

b3_result b3Tx::b3SaveTIFF(const char *nameTx)
{
	TIFF        *tiff;
	b3_tx_error  result = B3_TX_OK;

	if ((xSize == 0) || (ySize == 0))
	{
		B3_THROW(b3TxException,B3_TX_NOT_SAVED);
	}

	if (nameTx == null)
	{
		if (strlen(image_name) == 0)
		{
			B3_THROW(b3TxException,B3_TX_NOT_SAVED);
		}
	}
	else
	{
		b3Name(nameTx);
	}

	tiff = TIFFOpen(image_name,"w");
	if (tiff)
	{
		b3PrintF(B3LOG_DEBUG,"### CLASS: b3Tx:  # saving TIFF (%s)\n",
			(char *)image_name);
		// Now select the saving version we need.
		if (depth == 1)
		{
			b3SaveTIFFFax(tiff);
		}
		else
		{
			if ((type == B3_TX_RGB4) || (type == B3_TX_RGB8))
			{
				b3SaveTIFFTrueColor (tiff);
			}
			else
			{
				b3SaveTIFFPalette (tiff);
			}
		}

		TIFFClose(tiff);
	}
	else 
	{
		result = B3_TX_NOT_SAVED;
	}

	if (result != B3_TX_OK)
	{
		B3_THROW(b3TxException,result);
	}
	return B3_OK;
}
