/*
**
**	$Filename:	b3TxSaveJPEG.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving JPEG image
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
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
#include "blz3/image/b3TxExif.h"

/*************************************************************************
**                                                                      **
**                        JPEG                                          **
**                                                                      **
*************************************************************************/

#ifdef HAVE_JPEGLIB_H

#ifdef GLOBAL
#undef GLOBAL /* ausser JPEG brauchen wir sowieso nur noch ANSI-C */
#endif

#ifdef FAR
#undef FAR
#endif

#include <jpeglib.h>
#include <jerror.h>

#define JPEG_ROWS           8
#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */


/* Expanded data destination object for stdio output */
struct b3_jpeg_dest_mgr : jpeg_destination_mgr
{
	b3File * outfile; // target stream
	JOCTET * buffer; // start of buffer
};

class b3InfoJPEG : protected b3TxSaveInfo
{
	struct jpeg_compress_struct  JPEGcinfo;
	struct jpeg_error_mgr        JPEGjerr;
	JSAMPROW                     JPEGrow_pointer[JPEG_ROWS];
	b3_u08           *           JPEGrow        = nullptr;
	JDIMENSION                   JPEGline       = 0;
	JDIMENSION                   JPEGwritten    = 0;
	int                          JPEGrow_stride = 0;

public:
	b3InfoJPEG(b3Tx * tx, const char * filename, const b3_u32 quality = 85);
	virtual ~b3InfoJPEG();

	void  b3Write();

private:
	void           b3JpegStdioDestPrivate(j_compress_ptr cinfo);
	static void    b3InitDestination(j_compress_ptr cinfo);
	static boolean b3EmptyOutputBuffer(j_compress_ptr cinfo);
	static void    b3TermDestination(j_compress_ptr cinfo);
};

void b3InfoJPEG::b3InitDestination(j_compress_ptr cinfo)
{
	b3_jpeg_dest_mgr * dest = static_cast<b3_jpeg_dest_mgr *>(cinfo->dest);

	/* Allocate the output buffer --- it will be released when done with image */
	dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_IMAGE,
			OUTPUT_BUF_SIZE * sizeof(JOCTET));

	dest->next_output_byte = dest->buffer;
	dest->free_in_buffer   = OUTPUT_BUF_SIZE;
}

boolean b3InfoJPEG::b3EmptyOutputBuffer(j_compress_ptr cinfo)
{
	b3_jpeg_dest_mgr * dest = static_cast<b3_jpeg_dest_mgr *>(cinfo->dest);

	if (dest->outfile->b3Write(dest->buffer, OUTPUT_BUF_SIZE) !=
		(size_t) OUTPUT_BUF_SIZE)
	{
		ERREXIT(cinfo, JERR_FILE_WRITE);
	}

	dest->next_output_byte = dest->buffer;
	dest->free_in_buffer = OUTPUT_BUF_SIZE;

	return true;
}

void b3InfoJPEG::b3TermDestination(j_compress_ptr cinfo)
{
	b3_jpeg_dest_mgr * dest      = static_cast<b3_jpeg_dest_mgr *>(cinfo->dest);
	size_t             datacount = OUTPUT_BUF_SIZE - dest->free_in_buffer;

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

void b3InfoJPEG::b3JpegStdioDestPrivate(j_compress_ptr  cinfo)
{
	b3_jpeg_dest_mgr * dest;

	if (cinfo->dest == nullptr)
	{
		/* first time for this JPEG object? */
		cinfo->dest = (struct jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT,
				sizeof(b3_jpeg_dest_mgr));
	}

	dest = static_cast<b3_jpeg_dest_mgr *>(cinfo->dest);
	dest->init_destination    = b3InitDestination;
	dest->empty_output_buffer = b3EmptyOutputBuffer;
	dest->term_destination    = b3TermDestination;
	dest->outfile             = &m_File;
}

b3InfoJPEG::b3InfoJPEG(
	b3Tx    *    tx,
	const char * filename,
	const b3_u32 quality) :
	b3TxSaveInfo(tx, filename)
{
	b3_coord i;

	JPEGrow_stride = tx->xSize * 3;
	JPEGrow        = b3TypedAlloc<b3_u08>(JPEGrow_stride * JPEG_ROWS);
	if (JPEGrow == nullptr)
	{
		m_File.b3Close();
		b3Free();
		b3PrintF(B3LOG_NORMAL, "Save JPEG: no row mem!\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	for (i = 0; i < JPEG_ROWS; i++)
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

	jpeg_set_defaults(&JPEGcinfo);
	jpeg_set_quality(&JPEGcinfo, quality, TRUE);
	jpeg_start_compress(&JPEGcinfo, TRUE);
}

void b3InfoJPEG::b3Write()
{
	for (b3_coord y = 0; y < m_Tx->ySize; y++)
	{
		b3_u08 * line = (b3_u08 *)JPEGrow_pointer[JPEGline++];

		if (JPEGline >= JPEG_ROWS)
		{
			JPEGline = 0;
		}

		const b3_coord xMax = m_Tx->xSize;

		m_Tx->b3GetRow(m_ThisRow, y);
		for (b3_coord x = 0; x < xMax; x++)
		{
			*line++ = (m_ThisRow[x] & 0xff0000) >> 16;
			*line++ = (m_ThisRow[x] & 0x00ff00) >>  8;
			*line++ = (m_ThisRow[x] & 0x0000ff);
		}
		if (JPEGline == 0)
		{
			jpeg_write_scanlines(&JPEGcinfo, JPEGrow_pointer, JPEG_ROWS);
			JPEGwritten += JPEG_ROWS;
		}
	}
}

b3InfoJPEG::~b3InfoJPEG()
{
	if (JPEGline != 0)
	{
		jpeg_write_scanlines(&JPEGcinfo, JPEGrow_pointer, JPEGline);
		JPEGwritten += JPEGline;
	}

	if (JPEGwritten < (JDIMENSION)m_Tx->ySize)
	{
		b3_u08 * line = (b3_u08 *)JPEGrow_pointer[0];

		for (b3_coord x = 0; x < m_Tx->xSize; x++)
		{
			*line++ = 0;
			*line++ = 0;
			*line++ = 0;
		}
		for (b3_coord y = JPEGwritten; y < m_Tx->ySize; y++)
		{
			jpeg_write_scanlines(&JPEGcinfo, JPEGrow_pointer, 1);
		}
	}

	jpeg_finish_compress(&JPEGcinfo);
	jpeg_destroy_compress(&JPEGcinfo);
}

b3_result b3Tx::b3SaveJPEG(
	const char   *   filename,
	const b3_u32     quality,
	b3TxExif    *    exif)
{
	b3PrintF(B3LOG_FULL, "Saving JPEG: %s, quality %u\n", filename, quality);

	{
		b3InfoJPEG info(this, filename, quality);

		info.b3Write();
	}
	if (exif != nullptr)
	{
		exif->b3SetResolution(xDPI, yDPI);
		exif->b3SetQuality(quality);
		exif->b3Write(filename);
	}
	else
	{
		b3TxExif default_exif;

		default_exif.b3SetResolution(xDPI, yDPI);
		default_exif.b3SetQuality(quality);
		default_exif.b3Write(filename);
	}
	return B3_OK;
}
#endif

