/*
**
**	$Filename:	b3TxJPG.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading JPEG images
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**
**	Revision 1.4  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.3  2001/10/13 09:56:44  sm
**	- Minor corrections
**	
**	Revision 1.2  2001/10/13 09:54:32  sm
**	- Making include extern "C"
**	
**	Revision 1.1  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        handle JPEG's (JFIF)                          **
**                                                                      **
*************************************************************************/

#ifdef USE_JPEGLIB_LOAD

#ifdef GLOBAL
#undef GLOBAL /* ausser JPEG brauchen wir sowieso nur noch ANSI-C */
#endif

#ifdef FAR
#undef FAR
#endif

extern "C"
{
#	include "jpeglib.h"
#	include <setjmp.h>
}

struct my_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf               setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

static struct jpeg_source_mgr my_source_mgr;

static void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

static void JPEGinit_source(j_decompress_ptr cinfo)
{
}

static boolean JPEGfill_input_buffer (j_decompress_ptr cinfo)
{
	return TRUE;
}

static void JPEGskip_input_data (
	j_decompress_ptr cinfo,
	long             num_bytes)
{
	my_source_mgr.next_input_byte += (size_t)num_bytes;
	my_source_mgr.bytes_in_buffer -= (size_t)num_bytes;
}

static void JPEGterm_source (j_decompress_ptr cinfo)
{
}

b3_result b3Tx::b3ParseJPEG (b3_u08 *buffer,b3_size buffer_size)
{
	struct jpeg_decompress_struct  cinfo;
	struct my_error_mgr            jerr;
	JSAMPARRAY                     sample_array;
	int                            row_stride;
	b3_u08                        *line;
	b3_pkd_color                  *out,color;
	b3_tx_type                     type;
	b3_coord                       x;
	b3_size                        Max;

	cinfo.err           = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		b3FreeTx();
		throw new b3TxException(B3_TX_MEMORY);
	}
	jpeg_create_decompress(&cinfo);

		/* NOTE: this is only dummy because the data is completly loaded. */
	my_source_mgr.init_source       = JPEGinit_source;
	my_source_mgr.fill_input_buffer = JPEGfill_input_buffer;
	my_source_mgr.skip_input_data   = JPEGskip_input_data;
	my_source_mgr.term_source       = JPEGterm_source;
	my_source_mgr.resync_to_restart = jpeg_resync_to_restart;
	my_source_mgr.next_input_byte   = buffer;
	my_source_mgr.bytes_in_buffer   = buffer_size;
	cinfo.src = &my_source_mgr;

	jpeg_read_header      (&cinfo, TRUE);
	jpeg_start_decompress (&cinfo);
	row_stride   = cinfo.output_width * cinfo.output_components;
	sample_array = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	xSize = cinfo.output_width;
	ySize = cinfo.output_height;
	Max   = xSize * ySize;

	if (cinfo.out_color_space != JCS_GRAYSCALE)
	{
		out = (b3_pkd_color *)b3Alloc(Max * sizeof(b3_pkd_color));
		if (out == null)
		{
			jpeg_finish_decompress (&cinfo);
			jpeg_destroy_decompress(&cinfo);
			b3FreeTx();
			throw new b3TxException(B3_TX_MEMORY);
		}
		data = (b3_u08 *)out;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, sample_array, 1);
			line = (unsigned char *)sample_array[0];
			for (x = 0;x < (b3_coord)cinfo.output_width;x++)
			{
				color  = *line++;
				color  = (color << 8) | *line++;
				color  = (color << 8) | *line++;
				*out++ = color;
			}
		}
		depth   = 24;
		palette = null;
		type    = B3_TX_RGB8;
	}
	else
	{
		line = (b3_u08 *)b3Alloc(Max);
		if (line == null)
		{
			jpeg_finish_decompress (&cinfo);
			jpeg_destroy_decompress(&cinfo);
			b3FreeTx();
			throw new b3TxException(B3_TX_MEMORY);
		}

		out = (b3_pkd_color *)b3Alloc(Max * sizeof(b3_pkd_color));
		if (out == null)
		{
			jpeg_finish_decompress (&cinfo);
			jpeg_destroy_decompress(&cinfo);
			b3FreeTx();
			throw new b3TxException(B3_TX_MEMORY);
		}
		for (x = 0;x < 256;x++)
		{
			out[x] = 0x010101 * x;
		}
		depth   = 8;
		palette = out;
		data    = line;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, sample_array, 1);
			memcpy (line,sample_array[0],cinfo.output_width);
			line += cinfo.output_width;
		}
		type = B3_TX_VGA;
	}

	jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress(&cinfo);

	FileType = FT_JPEG;
	return B3_OK;
}

#else

b3_result b3Tx::b3ParseJPEG (b3_u08 *buffer,b3_size buffer_size)
{
	b3FreeTx();
	throw new b3TxException(B3_TX_UNSUPP);
}

#endif
