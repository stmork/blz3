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

#include "b3TxSaveInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2001/11/09 18:58:53  sm
**	- Fixed JPEG handling
**
**	Revision 1.3  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
**	Revision 1.2  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	
*/

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
#	include <setjmp.h>
}

#define JPEG_ROWS           8
#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */


	/* Expanded data destination object for stdio output */
typedef struct
{
	struct jpeg_destination_mgr pub; /* public fields */

	b3File *outfile; // target stream
	JOCTET *buffer;  // start of buffer
} my_destination_mgr;

typedef my_destination_mgr * my_dest_ptr;

class b3InfoJPEG : protected b3TxSaveInfo
{
	b3_u08                      *JPEGrow;
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
	dest->outfile                 = &m_File;
}

b3InfoJPEG::b3InfoJPEG(b3Tx *tx,const char *filename,b3_u32 quality) :
	b3TxSaveInfo(tx,filename)
{
	b3_coord i;

	JPEGrow_stride = tx->xSize * 3;
	JPEGline       = 0;
	JPEGwritten    = 0;

	JPEGrow        = (b3_u08 *)b3Alloc(JPEGrow_stride * JPEG_ROWS);
	if (JPEGrow == null)
	{
		m_File.b3Close();
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

	b3JpegStdioDestPrivate(&JPEGcinfo);
	JPEGcinfo.image_width      = tx->xSize;
	JPEGcinfo.image_height     = tx->ySize;
	JPEGcinfo.input_components = 3;
	JPEGcinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults   (&JPEGcinfo);
	jpeg_set_quality    (&JPEGcinfo, quality, TRUE );
	jpeg_start_compress (&JPEGcinfo, TRUE);
}

void b3InfoJPEG::b3Write()
{
	b3_u08       *line;
	b3_coord      x,y,xMax;

	for (y = 0;y < m_Tx->ySize;y++)
	{
		line = (b3_u08 *)JPEGrow_pointer[JPEGline++];
		if (JPEGline >= JPEG_ROWS)
		{
			JPEGline = 0;
		}

		xMax = m_Tx->xSize;
		m_Tx->b3GetRow(m_ThisRow,y);
		for (x = 0;x < xMax;x++)
		{
			*line++ = (m_ThisRow[x] & 0xff0000) >> 16;
			*line++ = (m_ThisRow[x] & 0x00ff00) >>  8;
			*line++ = (m_ThisRow[x] & 0x0000ff);
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

	if (JPEGwritten < (JDIMENSION)m_Tx->ySize)
	{
		line = (b3_u08 *)JPEGrow_pointer[0];
		for (x = 0;x < m_Tx->xSize;x++)
		{
			*line++ = 0;
			*line++ = 0;
			*line++ = 0;
		}
		for (y = JPEGwritten;y < m_Tx->ySize;y++)
		{
			jpeg_write_scanlines(&JPEGcinfo, JPEGrow_pointer, 1);
		}
	}

	jpeg_finish_compress (&JPEGcinfo);
	jpeg_destroy_compress(&JPEGcinfo);
}

b3_result b3Tx::b3SaveJPEG(const char *filename,b3_u32 quality)
{
	b3InfoJPEG info(this,filename,quality);
	info.b3Write();
	return B3_OK;
}
