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
**	Revision 1.8  2001/11/09 18:58:53  sm
**	- Fixed JPEG handling
**
**	Revision 1.7  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.6  2001/10/26 18:37:14  sm
**	- Creating search path support
**	- Splitting image pool support and image loading into
**	  their own area.
**	- Fixed JPEG to support b3Tx::b3AllocTx()
**	
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
	b3_pkd_color                  *out;
	b3_coord                       x;

	b3PrintF(B3LOG_FULL,"IMG JPEG # b3ParseJPEG(%s)\n",
		(const char *)name);

	cinfo.err           = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG JPEG # Error configuring JPEG decoder:\n");
		b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
		b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
		throw new b3TxException(B3_TX_ERROR);
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

	if (cinfo.out_color_space != JCS_GRAYSCALE)
	{
		if (!b3AllocTx(cinfo.output_width,cinfo.output_height,24))
		{
			jpeg_finish_decompress (&cinfo);
			jpeg_destroy_decompress(&cinfo);
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL,"IMG JPEG # Error allocating memory:\n");
			b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
			b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
			throw new b3TxException(B3_TX_MEMORY);
		}

		out = (b3_pkd_color *)data;
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, sample_array, 1);
			line = (b3_u08 *)sample_array[0];
			for (x = 0;x < (b3_coord)cinfo.output_width;x++)
			{
				*out++ =
					((b3_pkd_color)line[0] << 16) |
					((b3_pkd_color)line[1] <<  8) |
					 (b3_pkd_color)line[2];
				line += 3;
			}
		}
	}
	else
	{
		if (!b3AllocTx(cinfo.output_width,cinfo.output_height,8))
		{
			jpeg_finish_decompress (&cinfo);
			jpeg_destroy_decompress(&cinfo);
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL,"IMG JPEG # Error allocating memory:\n");
			b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
			b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
			throw new b3TxException(B3_TX_MEMORY);
		}

		line = (b3_u08 *)data;
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, sample_array, 1);
			memcpy (line,sample_array[0],cinfo.output_width);
			line += cinfo.output_width;
		}
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
	b3PrintF(B3LOG_NORMAL,"IMG JPEG # Missing JPEG support:\n");
	b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
	b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
	throw new b3TxException(B3_TX_UNSUPP);
}

#endif
