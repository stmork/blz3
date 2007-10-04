/*
**
**	$Filename:	b3TxLoadTIFF.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - TIFF loading
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

#include "b3TxInclude.h"
#include "blz3/system/b3File.h"

#include "b3TxTIFF.h"

/*************************************************************************
**                                                                      **
**                        memory routines for TIFF-library              **
**                                                                      **
*************************************************************************/

#include <tiff.h>
#include <tiffio.h>
#include <tiffiop.h>

/*************************************************************************
**                                                                      **
**                        memory access routines for TIFF-library       **
**                                                                      **
*************************************************************************/

class b3MemTiffInfo
{
public:
	b3_u08  *ptr;
	tsize_t  size;
	toff_t   pos;

public:
	b3MemTiffInfo(const b3_u08 *InitBuffer,tsize_t InitSize)
	{
		ptr  = (b3_u08 *)InitBuffer;
		size = InitSize;
		pos  = 0;
	}
};

tsize_t b3Tx::b3ReadProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	b3MemTiffInfo *value = (b3MemTiffInfo *)fd;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"IMG TIFF # b3ProcRead: ");
#endif

	memcpy (buf,&value->ptr[value->pos],size);
	value->pos += size;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"%p %9ld %9ld\n",value->ptr,value->pos,value->size);
#endif
	return size;
}

tsize_t b3Tx::b3WriteProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	b3MemTiffInfo *value = (b3MemTiffInfo *)fd;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"IMG TIFF # b3ProcWrte: ");
#endif

	memcpy (&value->ptr[value->pos],buf,size);
	value->pos += size;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"%p %9ld %9ld\n",value->ptr,value->pos,value->size);
#endif
	return size;
}

toff_t b3Tx::b3SeekProc(thandle_t fd, toff_t off, int whence)
{
	b3MemTiffInfo *value  = (b3MemTiffInfo *)fd;
	toff_t                  offset = value->pos;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"IMG TIFF # b3ProcSeek: ");
#endif

	switch (whence)
	{
	case SEEK_SET :
		value->pos  = off;
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL,"%p %9ld %9ld S\n",
				 value->ptr,value->pos,value->size);
#endif
		break;

	case SEEK_CUR :
		value->pos += off;
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL,"%p %9ld %9ld C\n",value->ptr,value->pos,value->size);
#endif
		break;

	case SEEK_END :
		value->pos  = value->size + off;
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL,"%p %9ld %9ld E\n",value->ptr,value->pos,value->size);
#endif
		break;
	}
	return offset;
}

int b3Tx::b3CloseProc(thandle_t fd)
{
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"IMG TIFF # b3ProcClos:\n");
#endif
	return 0;
}

toff_t b3Tx::b3SizeProc(thandle_t fd)
{
	b3MemTiffInfo *value = (b3MemTiffInfo *)fd;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"IMG TIFF # b3ProcSize: %ld\n",value->size);
#endif
	return value->size;
}

int b3Tx::b3MMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	b3MemTiffInfo *value = (b3MemTiffInfo *)fd;

	*pbase = value->ptr;
	*psize = value->size;

	return 1;
}

void b3Tx::b3UnmapProc(thandle_t fd, tdata_t base, toff_t size)
{
}

/*************************************************************************
**                                                                      **
**                        memory routines for TIFF-library              **
**                                                                      **
*************************************************************************/

// These methods load a TIFF image into this instance.
// Look at the TIFF programming manual for understanding this. Be happy
// with this piece of code.
long b3Tx::b3TIFFPalette(
	TIFF  *tiff,
	short  PaletteMode)
{
	if (depth == 1)
	{
		switch (PaletteMode)
		{
		case PHOTOMETRIC_MINISWHITE :
			palette[0] = 0xffffff;
			palette[1] = 0x000000;
			break;

		case PHOTOMETRIC_MINISBLACK :
			palette[0] = 0x000000;
			palette[1] = 0xffffff;
			break;

		default :
			type = B3_TX_UNDEFINED;
			break;
		}
	}

	if ((depth > 1) && (depth <= 8))
	{
		uint16 *r,*g,*b;
		long i,step,max,color;

		max  =          1 << depth;
		step = 0x01010100 >> depth;
		switch (PaletteMode)
		{
		case PHOTOMETRIC_MINISWHITE :
			color = 0xffffff;
			for (i = 0;i < max;i++)
			{
				palette[i]  = color;
				color      -= step;
			}
			break;

		case PHOTOMETRIC_MINISBLACK :
			color = 0x000000;
			for (i = 0;i < max;i++)
			{
				palette[i]  = color;
				color      += step;
			}
			break;

		case PHOTOMETRIC_PALETTE:
			TIFFGetField (tiff,TIFFTAG_COLORMAP,&r,&g,&b);
			for (i = 0;i < max;i++)
			{
				palette[i] =
					(((unsigned long)r[i] & 0xff00) << 8) |
					((unsigned long)g[i] & 0xff00) |
					(((unsigned long)b[i] & 0xff00) >> 8);
			}
			break;

		default :
			type = B3_TX_UNDEFINED;
			break;
		}
	}
	if (depth > 8)
	{
		if (PaletteMode != PHOTOMETRIC_RGB)
		{
			type = B3_TX_UNDEFINED;
		}
	}
	return type;
}

long b3Tx::b3TIFFDecode(
	TIFF  *tiff,
	short  PlanarConfig)
{
	b3_coord x,y;

	// Reset number of loaded scanlines
	ScanLines = 0;

	// decode single plane TIFFs
	if ((depth == 1) && (type != B3_TX_UNDEFINED))
	{
		b3_u08 *cPtr;
		unsigned long  step;

		step = TX_BWA(xSize);
		cPtr = (b3_u08 *)data;
		for (y = 0;y < ySize;y++)
		{
			if (TIFFReadScanline (tiff,cPtr,y,0) != 1)
			{
				return type;
			}
			cPtr += step;
			ScanLines++;
		}
	}

	if ((depth > 1) && (depth <= 8) && (type != B3_TX_UNDEFINED))
	{
		b3_res    b;
		b3_size   max;
		b3_index  pos;
		b3_u32    bit;
		b3_u08   *lPtr,lVal;
		b3_u08   *cPtr,cVal;

		max = TIFFScanlineSize(tiff);
		switch (PlanarConfig)
		{
		case PLANARCONFIG_CONTIG :
			lPtr = (b3_u08 *)b3Alloc (max);
			if (lPtr)
			{
				cPtr = (b3_u08 *)data;
				for (y = 0;y < ySize;y++)
				{
					if (TIFFReadScanline (tiff,lPtr,y,0) != 1)
					{
						return type;
					}
					ScanLines++;

					bit  = 1 << 7;
					lVal = lPtr[pos = 0];
					for (x = 0;x < xSize;x++)
					{
						cVal = 0;
						for (b = 0;b < depth;b++)
						{
							cVal = (b3_u08)(cVal << 1);
							if (lVal & bit) cVal |= 1;
							bit = bit >> 1;
							if (bit == 0)
							{
								bit = 1 << 7;
								lVal = lPtr[++pos];
							}
						}
						*cPtr++ = cVal;
					}
				}
				b3Free (lPtr);
			}
			else
			{
				type = B3_TX_UNDEFINED;
			}
			break;

		default :
			type = B3_TX_UNDEFINED;
			break;
		}
	}

	if ((depth > 8) && (type != B3_TX_UNDEFINED))
	{
		uint32         *fPtr,*bPtr;
		b3_pkd_color    fSwp, bSwp;
		b3_count        max;

		max  = xSize * ySize;
		fPtr = (uint32 *)data;
		bPtr = fPtr + max;
		max  = ySize >> 1;
		if (TIFFReadRGBAImage (tiff,xSize,ySize,fPtr) == 0)
		{
			return type;
		}
		ScanLines = ySize;

		for (y = 0;y < max;y++)
		{
			bPtr -= xSize;
			for (x = 0;x < xSize;x++)
			{
				fSwp =
					((fPtr[x] & 0xff0000) >> 16) |
					(fPtr[x] & 0x00ff00) |
					((fPtr[x] & 0x0000ff) << 16);
				bSwp =
					((bPtr[x] & 0xff0000) >> 16) |
					(bPtr[x] & 0x00ff00) |
					((bPtr[x] & 0x0000ff) << 16);
				fPtr[x] = bSwp;
				bPtr[x] = fSwp;
			}
			fPtr += xSize;
		}
	}
	return type;
}

b3_result b3Tx::b3LoadTIFF (const char *tiff_name)
{
	b3File     tiff_file;
	b3_u08    *buffer;
	b3_size    size;
	b3_result  error_code = B3_OK;

	try
	{
		buffer     = tiff_file.b3ReadBuffer(tiff_name,size);
		error_code = b3LoadTIFF(tiff_name,buffer,size);
	}
	catch (b3FileException &e)
	{
		b3PrintF(B3LOG_NORMAL,"IMG TIFF # Error loading %s (%s)\n",
				 tiff_name,e.b3GetErrorMsg());
		error_code = B3_ERROR;
	}

	return error_code;
}

b3_result b3Tx::b3LoadTIFF(
	const char    *tiff_name,
	const b3_u08  *tiff_buffer,
	const b3_size  tiff_size)
{
	TIFF    *tiff;
	b3_u16   bps = 1,spp = 1,pm = PHOTOMETRIC_MINISWHITE,pc = 0;
	b3_u16   compression = 0,fillorder = 0;
	char    *my_hostname = null;
	char    *my_username = null;
	char    *my_software = null;
	b3_f32   xDoubleDPI  = 200;
	b3_f32   yDoubleDPI  = 200;

#if 1
	b3MemTiffInfo  tiff_info(tiff_buffer,(tsize_t)tiff_size);

	tiff = TIFFClientOpen (
			   tiff_name,"r",(thandle_t)&tiff_info,
			   b3ReadProc,
			   b3WriteProc,
			   b3SeekProc,
			   b3CloseProc,
			   b3SizeProc,
			   b3MMapProc,
			   b3UnmapProc);
#else
	tiff = TIFFOpen (tiff_name,"r");
#endif
	if (tiff == null)
	{
		B3_THROW(b3TxException,B3_TX_NOT_FOUND);
	}
	b3Name(tiff_name);
	b3PrintF(B3LOG_FULL,"IMG TIFF # b3LoadTIFF(%s)\n",(const char *)image_name);

	TIFFGetField (tiff,TIFFTAG_IMAGEWIDTH,      &xSize);
	TIFFGetField (tiff,TIFFTAG_IMAGELENGTH,     &ySize);
	TIFFGetField (tiff,TIFFTAG_SAMPLESPERPIXEL, &spp);
	TIFFGetField (tiff,TIFFTAG_BITSPERSAMPLE,   &bps);
	TIFFGetField (tiff,TIFFTAG_PHOTOMETRIC,     &pm);
	TIFFGetField (tiff,TIFFTAG_PLANARCONFIG,    &pc);
	TIFFGetField (tiff,TIFFTAG_HOSTCOMPUTER,    &my_hostname);
	TIFFGetField (tiff,TIFFTAG_ARTIST,          &my_username);
	TIFFGetField (tiff,TIFFTAG_SOFTWARE,        &my_software);
	TIFFGetField (tiff,TIFFTAG_XRESOLUTION,     &xDoubleDPI);
	TIFFGetField (tiff,TIFFTAG_YRESOLUTION,     &yDoubleDPI);
	TIFFGetField (tiff,TIFFTAG_COMPRESSION,     &compression);
	TIFFGetField (tiff,TIFFTAG_FILLORDER,       &fillorder);
	xDPI = (long)xDoubleDPI;
	yDPI = (long)yDoubleDPI;

	depth = spp * bps;
	b3PrintF(B3LOG_FULL,"IMG TIFF # xmax:              %4lu\n",xSize);
	b3PrintF(B3LOG_FULL,"IMG TIFF # ymax:              %4lu\n",ySize);
	b3PrintF(B3LOG_FULL,"IMG TIFF # samples per pixel: %4hu\n",spp);
	b3PrintF(B3LOG_FULL,"IMG TIFF # bits per sample:   %4hu\n",bps);
	b3PrintF(B3LOG_FULL,"IMG TIFF # photometric:       %4hu\n",pm);
	b3PrintF(B3LOG_FULL,"IMG TIFF # planar config:     %4hu\n",pc);
	b3PrintF(B3LOG_FULL,"IMG TIFF # compression:       %4hu\n",compression);
	if (my_username) b3PrintF(B3LOG_FULL,"IMG TIFF # creator:           %s\n",my_username);
	if (my_software) b3PrintF(B3LOG_FULL,"IMG TIFF # creator software:  %s\n",my_software);
	if (my_hostname) b3PrintF(B3LOG_FULL,"IMG TIFF # creator host:      %s\n",my_hostname);

	if (b3AllocTx (xSize,ySize,depth))
	{
		if (b3TIFFPalette(tiff,pm) != B3_TX_UNDEFINED)
		{
			b3TIFFDecode(tiff,pc);
		}
	}
	TIFFClose(tiff);

	if (type == B3_TX_UNDEFINED)
	{
		b3FreeTx();
		B3_THROW(b3TxException,B3_TX_NOT_FOUND);
	}

	return B3_OK;
}
