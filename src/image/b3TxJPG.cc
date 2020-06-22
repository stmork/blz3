/*
**
**	$Filename:	b3TxJPG.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3TxInclude.h"

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

#ifdef HAVE_LIBJPEG
#include <jpeglib.h>
#include <setjmp.h>

/*************************************************************************
**                                                                      **
**                        b3JPEG class definition                       **
**                                                                      **
*************************************************************************/

struct b3_jpeg_error_mgr
{
	struct jpeg_error_mgr          m_ErrorMgr;
	jmp_buf                        m_SetjmpBuffer;
};

class b3JPEG
{
	struct jpeg_decompress_struct  m_Decompress;
	struct jpeg_source_mgr         m_SourceMgr;
	struct b3_jpeg_error_mgr       m_Error;
	JSAMPARRAY                     m_SampleArray;

public:
	b3JPEG();
	b3_bool b3Init(b3_u08 *buffer,b3_size buffer_size);
	b3_bool b3Decompress(b3Tx *tx);
	void    b3Deinit();

private:
	static void b3ErrorHandler (j_common_ptr cinfo);
	static void b3JpegInitSource(j_decompress_ptr cinfo);
	static boolean b3JpegFillInputBuffer (j_decompress_ptr cinfo);
	static void b3JpegSkipInputData (j_decompress_ptr cinfo,long num_bytes);
	static void b3JpegTermSource (j_decompress_ptr cinfo);
};

/*************************************************************************
**                                                                      **
**                        b3JPEG implementaion                          **
**                                                                      **
*************************************************************************/

b3JPEG::b3JPEG()
{
	memset(&m_Decompress, 0, sizeof(m_Decompress));
	memset(&m_SourceMgr,  0, sizeof(m_SourceMgr));
}

b3_bool b3JPEG::b3Init(b3_u08 *buffer,b3_size buffer_size)
{
	int row_stride;

	m_Decompress.err                     = jpeg_std_error(&m_Error.m_ErrorMgr);
	m_Error.m_ErrorMgr.error_exit = b3ErrorHandler;

	if (setjmp(m_Error.m_SetjmpBuffer) != 0)
	{
		return false;
	}
	jpeg_create_decompress(&m_Decompress);
	m_SourceMgr.init_source       = b3JpegInitSource;
	m_SourceMgr.fill_input_buffer = b3JpegFillInputBuffer;
	m_SourceMgr.skip_input_data   = b3JpegSkipInputData;
	m_SourceMgr.term_source       = b3JpegTermSource;
	m_SourceMgr.resync_to_restart = jpeg_resync_to_restart;
	m_SourceMgr.next_input_byte   = buffer;
	m_SourceMgr.bytes_in_buffer   = buffer_size;

	m_Decompress.src  = &m_SourceMgr;
	jpeg_read_header      (&m_Decompress, TRUE);
	jpeg_start_decompress (&m_Decompress);
	row_stride    = m_Decompress.output_width * m_Decompress.output_components;
	m_SampleArray = (*m_Decompress.mem->alloc_sarray)
					((j_common_ptr) &m_Decompress, JPOOL_IMAGE, row_stride, 1);

	return true;
}

b3_bool b3JPEG::b3Decompress(b3Tx *tx)
{
	b3_u08 *line;

	if (m_Decompress.out_color_space != JCS_GRAYSCALE)
	{
		b3_pkd_color *out;

		if (!tx->b3AllocTx(m_Decompress.output_width,m_Decompress.output_height,24))
		{
			return false;
		}
		out = tx->b3GetTrueColorData();

		while (m_Decompress.output_scanline < m_Decompress.output_height)
		{
			b3_coord x;

			jpeg_read_scanlines(&m_Decompress, m_SampleArray, 1);
			line = (b3_u08 *)m_SampleArray[0];
			for (x = 0;x < (b3_coord)m_Decompress.output_width;x++)
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
		if (!tx->b3AllocTx(m_Decompress.output_width,m_Decompress.output_height,8))
		{
			return false;
		}

		line = tx->b3GetIndexData();
		while (m_Decompress.output_scanline < m_Decompress.output_height)
		{
			jpeg_read_scanlines(&m_Decompress, m_SampleArray, 1);
			memcpy (line,m_SampleArray[0],m_Decompress.output_width);
			line += m_Decompress.output_width;
		}
	}
	return true;
}

void b3JPEG::b3Deinit()
{
	jpeg_finish_decompress (&m_Decompress);
	jpeg_destroy_decompress(&m_Decompress);
}

void b3JPEG::b3ErrorHandler (j_common_ptr cinfo)
{
	struct b3_jpeg_error_mgr *myerr = (struct b3_jpeg_error_mgr *) cinfo->err;

	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->m_SetjmpBuffer, 1);
}

void b3JPEG::b3JpegInitSource(j_decompress_ptr cinfo)
{
}

boolean b3JPEG::b3JpegFillInputBuffer (j_decompress_ptr cinfo)
{
	return TRUE;
}

void b3JPEG::b3JpegSkipInputData (
	j_decompress_ptr cinfo,
	long             num_bytes)
{
	struct jpeg_source_mgr *source = cinfo->src;

	source->next_input_byte += (size_t)num_bytes;
	source->bytes_in_buffer -= (size_t)num_bytes;
}

void b3JPEG::b3JpegTermSource (j_decompress_ptr cinfo)
{
}

/*************************************************************************
**                                                                      **
**                        b3Tx JPEG parsing                             **
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3ParseJPEG (b3_u08 *buffer,b3_size buffer_size)
{
	b3JPEG jpeg;

	b3PrintF(B3LOG_FULL,"IMG JPEG # b3ParseJPEG(%s)\n",
			 (const char *)image_name);

	// NOTE: this is only dummy because the data is completly loaded.
	if (!jpeg.b3Init(buffer,buffer_size))
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG JPEG # Error configuring JPEG decoder:\n");
		return B3_ERROR;
	}

	if (!jpeg.b3Decompress(this))
	{
		jpeg.b3Deinit();
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG JPEG # Error allocating memory:\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}
	jpeg.b3Deinit();

	FileType = FT_JPEG;
	return B3_OK;
}

#else

b3_result b3Tx::b3ParseJPEG (b3_u08 *buffer,b3_size buffer_size)
{
	b3FreeTx();
	b3PrintF(B3LOG_NORMAL,"IMG JPEG # Missing JPEG support:\n");
	B3_THROW(b3TxException, B3_TX_UNSUPP);
}

#endif
#endif

