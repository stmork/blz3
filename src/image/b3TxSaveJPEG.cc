/*
**
**	$Filename:	b3TxSaveJPEG.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving JPEG image
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

#include "blz3/image/b3Tx.h"
#include "blz3/system/b3File.h"

/*************************************************************************
**                                                                      **
**                        JPEG                                          **
**                                                                      **
*************************************************************************/

#ifdef GLOBAL
#undef GLOBAL /* ausser JPEG brauchen wir sowieso nur noch ANSI-C */
#endif

#ifdef FAR
#undef FAR
#endif

extern "C"
{
#	include "jinclude.h"
#	include "jpeglib.h"
#	include "jerror.h"
#	include <jpeglib.h>
#	include <setjmp.h>
}

#define JPEG_ROWS 8
#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */


	/* Expanded data destination object for stdio output */
typedef struct
{
	struct jpeg_destination_mgr pub; /* public fields */

	b3File *outfile; // target stream
	JOCTET *buffer;  // start of buffer
} my_destination_mgr;

typedef my_destination_mgr * my_dest_ptr;

class b3InfoJPEG : public b3Mem
{
	b3Tx                        *JPEGtx;
	b3File                       JPEGoutfile;
	b3_u08                      *JPEGrow;
	b3_pkd_color                *JPEGsrc;
	struct jpeg_compress_struct  JPEGcinfo;
	struct jpeg_error_mgr        JPEGjerr;
	JDIMENSION                   JPEGline;
	JDIMENSION                   JPEGwritten;
	JSAMPROW                     JPEGrow_pointer[JPEG_ROWS];
	int                          JPEGrow_stride;

public:
	      b3InfoJPEG(b3Tx *tx,const char *filename,b3_u32 quality = 75);
	     ~b3InfoJPEG();
	void  b3Write();
private:
	       void    b3JpegStdioDestPrivate (j_compress_ptr cinfo);
	static void    b3InitDestination      (j_compress_ptr cinfo);
	static boolean b3EmptyOutputBuffer    (j_compress_ptr cinfo);
	static void    b3TermDestination      (j_compress_ptr cinfo);
};

void b3InfoJPEG::b3InitDestination (j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

		/* Allocate the output buffer --- it will be released when done with image */
	dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
		OUTPUT_BUF_SIZE * SIZEOF(JOCTET));

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer   = OUTPUT_BUF_SIZE;
}

boolean b3InfoJPEG::b3EmptyOutputBuffer (j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

	if (dest->outfile->b3Write(dest->buffer, OUTPUT_BUF_SIZE) !=
		(size_t) OUTPUT_BUF_SIZE)
	{
		ERREXIT(cinfo, JERR_FILE_WRITE);
	}

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	return true;
}

void b3InfoJPEG::b3TermDestination (j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

		/* Write any data remaining in the buffer */
	if (datacount > 0)
	{
	    if (dest->outfile->b3Write(dest->buffer, datacount) != datacount)
	    {
			ERREXIT(cinfo, JERR_FILE_WRITE);
		}
	}
	dest->outfile->b3Flush();
}

void b3InfoJPEG::b3JpegStdioDestPrivate (j_compress_ptr  cinfo)
{
	my_dest_ptr dest;

	if (cinfo->dest == null)
	{
			/* first time for this JPEG object? */
		cinfo->dest = (struct jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			SIZEOF(my_destination_mgr));
	}

	dest = (my_dest_ptr) cinfo->dest;
	dest->pub.init_destination    = b3InitDestination;
	dest->pub.empty_output_buffer = b3EmptyOutputBuffer;
	dest->pub.term_destination    = b3TermDestination;
	dest->outfile                 = &JPEGoutfile;
}

b3InfoJPEG::b3InfoJPEG(b3Tx *tx,const char *filename,b3_u32 quality)
{
	b3_coord i;

	JPEGtx         = tx;
	JPEGrow_stride = tx->xSize * 3;
	JPEGline       = 0;
	JPEGwritten    = 0;
	JPEGsrc        = (b3_pkd_color *)b3Alloc(tx->xSize * sizeof(b3_pkd_color));
	if (JPEGsrc == null)
	{
		b3PrintF (B3LOG_NORMAL,"Save JPEG: no row mem!\n");
		throw new b3TxException(B3_TX_MEMORY);
	}

	JPEGrow        = (b3_u08 *)b3Alloc(JPEGrow_stride * JPEG_ROWS);
	if (JPEGrow == null)
	{
		b3Free();
		b3PrintF (B3LOG_NORMAL,"Save JPEG: no row mem!\n");
		throw new b3TxException(B3_TX_MEMORY);
	}
	for (i = 0;i < JPEG_ROWS;i++)
	{
		JPEGrow_pointer[i] = (JSAMPROW)&JPEGrow[i * JPEGrow_stride];
	}

	JPEGcinfo.err = jpeg_std_error(&JPEGjerr);
	jpeg_create_compress(&JPEGcinfo);
	if(!JPEGoutfile.b3Open(filename, B_WRITE))
	{
		b3Free();
		b3PrintF (B3LOG_NORMAL,"Save JPEG: file (%s) not created!\n",JPEGtx->b3Name());
		throw new b3TxException(B3_TX_NOT_SAVED);
	}

	b3JpegStdioDestPrivate(&JPEGcinfo);
	JPEGcinfo.image_width      = tx->xSize;
	JPEGcinfo.image_height     = tx->ySize;
	JPEGcinfo.input_components = 3;
	JPEGcinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults   (&JPEGcinfo);
	jpeg_set_quality    (&JPEGcinfo, quality, TRUE );
	jpeg_start_compress (&JPEGcinfo, TRUE);
	
	tx->b3Name(filename);
}

void b3InfoJPEG::b3Write()
{
	b3_u08       *line;
	b3_coord      x,y,xMax;

	for (y = 0;y < JPEGtx->ySize;y++)
	{
		line = (b3_u08 *)JPEGrow_pointer[JPEGline++];
		if (JPEGline >= JPEG_ROWS)
		{
			JPEGline = 0;
		}

		xMax = JPEGtx->xSize;
		JPEGtx->b3GetRow(JPEGsrc,y);
		for (x = 0;x < xMax;x++)
		{
			*line++ = (JPEGsrc[x] & 0xff0000) >> 16;
			*line++ = (JPEGsrc[x] & 0x00ff00) >>  8;
			*line++ = (JPEGsrc[x] & 0x0000ff);
		}
		if (JPEGline == 0)
		{
			jpeg_write_scanlines (&JPEGcinfo, JPEGrow_pointer, JPEG_ROWS);
			JPEGwritten += JPEG_ROWS;
		}
	}
}

b3InfoJPEG::~b3InfoJPEG()
{
	b3_u08   *line;
	b3_coord  x,y;

	if (JPEGline != 0)
	{
		jpeg_write_scanlines (&JPEGcinfo, JPEGrow_pointer, JPEGline);
		JPEGwritten += JPEGline;
	}

	if (JPEGwritten < (JDIMENSION)JPEGtx->ySize)
	{
		line = (b3_u08 *)JPEGrow_pointer[0];
		for (x = 0;x < JPEGtx->xSize;x++)
		{
			*line++ = 0;
			*line++ = 0;
			*line++ = 0;
		}
		for (y = JPEGwritten;y < JPEGtx->ySize;y++)
		{
			jpeg_write_scanlines(&JPEGcinfo, JPEGrow_pointer, 1);
		}
	}

	jpeg_finish_compress (&JPEGcinfo);
	jpeg_destroy_compress(&JPEGcinfo);
	JPEGoutfile.b3Close();
}

b3_result b3Tx::b3SaveJPEG(const char *filename,b3_u32 quality)
{
	b3InfoJPEG info(this,filename,quality);
	info.b3Write();

	return B3_OK;
}
