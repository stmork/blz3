/*
**
**	$Filename:	b3TxIMG.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading SGI-RLE images
**
**	(C) Copyright 2001  Steffen A. Mork
**    All Rights Reserved
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.12  2006/03/06 09:24:55  smork
**	- Small optimizations for precompiling.
**
**	Revision 1.11  2006/03/05 21:22:34  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.10  2005/01/24 18:32:34  sm
**	- Removed some static variables and functions.
**	
**	Revision 1.9  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.8  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.7  2002/01/01 13:50:21  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.6  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.5  2001/10/26 18:37:14  sm
**	- Creating search path support
**	- Splitting image pool support and image loading into
**	  their own area.
**	- Fixed JPEG to support b3Tx::b3AllocTx()
**	
**	Revision 1.4  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.3  2001/10/13 15:48:53  sm
**	- Minor image loading corrections
**	
**	Revision 1.2  2001/10/13 15:35:32  sm
**	- Adding further image file format support.
**	
**	Revision 1.1  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        SGI RLE image format                          **
**                                                                      **
*************************************************************************/

inline void b3Tx::b3UnpackSGI (
	b3_u08    *buffer,
	void      *inPtr,
	b3_count   count,
	b3_count   bytes,
	b3_offset  offset)
{
	b3_u16  *sBuffer;
	b3_u08  *bBuffer;
	b3_u16   pixel;

	switch (bytes)
	{
		case 1 :
			if (count != 0)
			{
				bBuffer  = (b3_u08 *)inPtr;
				bBuffer += (512 / sizeof(unsigned char) + offset);
				while (count-- > 0) *buffer++ = *bBuffer++;
			}
			else
			{
				bBuffer  = (b3_u08 *)inPtr;
				bBuffer += offset;
				do
				{
				    pixel = *bBuffer++;
				    count = (pixel & 0x7f);
				    if (count == 0) return;
			    	if (pixel & 0x80)
					{
						while(count--) *buffer++ = *bBuffer++;
				    }
					else
					{
						pixel = *bBuffer++;
						while(count--) *buffer++ = pixel;
				    }
				}
				while (true);
			}
			break;

		case 2 :
			if (count != 0)
			{
				bBuffer  = (b3_u08 *)inPtr;
				bBuffer += (512 / sizeof(unsigned short) + offset);
				while (count-- > 0)
				{
					*buffer++ = *bBuffer++;
				}
			}
			else
			{
				sBuffer  = (b3_u16 *)inPtr;
				sBuffer += offset;
				do
				{
				    pixel = *sBuffer++;
				    count = (pixel & 0x7f);
				    if (count == 0) return;
			    	if (pixel & 0x80)
					{
						while(count--) *buffer++ = *sBuffer++;
				    }
					else
					{
						pixel = *sBuffer++;
						while(count--) *buffer++ = pixel;
				    }
				}
				while (true);
			}
			break;
	}
}

inline void b3Tx::b3ConvertSGILine(
	b3_u16    *buffer,
	b3_offset  offset,
	b3_size    size,
	b3_count   bytes)
{
	if (bytes == 2)
	{
		buffer += offset;
		while (size-- > 0)
		{
			b3Endian::b3ChangeEndian16 (buffer++);
		}
	}
}

void b3Tx::b3ParseSGI3(
	HeaderSGI *HeaderSGI,
	b3_u08    *buffer)
{
	b3_pkd_color *lineTable;
	b3_pkd_color *lineSizes;
	b3_pkd_color *lPtr,value;
	b3_u08       *line;
	b3_u08       *cPtr;
	b3_res        zSize = depth >> 3;
	long               x,y,z;
	long               rle,bytes,block;

	bytes =   HeaderSGI->type & 0x00ff; /* check bytes per pixel */
	rle   = ((HeaderSGI->type & 0xff00) == 0x0100) ? 0 : xSize; /* check RLE */

		/* line buffer */
	line = (b3_u08 *)b3Alloc(xSize * zSize * bytes);
	if (line == null)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG SGI  # Error allocating memory:\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}


		/* convert line offsets */
	lineTable = (b3_pkd_color *)&buffer[512];
	lineSizes = (b3_pkd_color *)&lineTable[zSize * ySize];
	if (HeaderSGI->imagic == IMAGIC2) /* check for converting endian */
	{
		if (rle <= 0) /* RLE data */
		{
			for (y=0;y<ySize;y++) for (z=0;z<zSize;z++)
			{
				b3Endian::b3ChangeEndian32 (&lineTable[y + z * ySize]);
				b3Endian::b3ChangeEndian32 (&lineSizes[y + z * ySize]);
				b3ConvertSGILine ((b3_u16 *)buffer,
					lineTable[y + z * ySize],
					lineSizes[y + z * ySize],bytes);
			}
		}
		else /* raw (VERBATIM) data */
		{
			for (y=0;y<ySize;y++) for (z=0;z<zSize;z++)
			{
				b3ConvertSGILine ((b3_u16 *)buffer,
					256 + y * xSize * zSize + z * xSize,xSize,bytes);
			}
		}
	}
	
	switch (type)
	{
		case B3_TX_RGB8 :
			lPtr  = (b3_pkd_color *)data;
			block = xSize * ySize;
			for (y = ySize - 1;y >= 0;y--)
			{
				if (rle > 0) /* read raw data */
				{
					b3UnpackSGI(&line[0],          buffer,rle,bytes,y * xSize);
					b3UnpackSGI(&line[xSize],      buffer,rle,bytes,y * xSize + block);
					b3UnpackSGI(&line[xSize+xSize],buffer,rle,bytes,y * xSize + block + block);
				}
				else /* read RLE packed data */
				{
					b3UnpackSGI(&line[0],          buffer,rle,bytes,lineTable[y]);
					b3UnpackSGI(&line[xSize],      buffer,rle,bytes,lineTable[y+ySize]);
					b3UnpackSGI(&line[xSize+xSize],buffer,rle,bytes,lineTable[y+ySize+ySize]);
				}
				for (x=0;x<xSize;x++)
				{
					value   = line[x];
					value   = (value << 8) | line[x + xSize];
					*lPtr++ = (value << 8) | line[x + xSize + xSize];
				}
			}
			break;

		case B3_TX_VGA :
			cPtr = data;
			for (y = ySize - 1;y >= 0;y--)
			{
				if (rle > 0) b3UnpackSGI (cPtr,buffer,rle,bytes,y * xSize);
				else         b3UnpackSGI (cPtr,buffer,rle,bytes,lineTable[y]);
				cPtr += xSize;
			}
			break;

		default :
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL,"IMG SGI  # Unsupported format:\n");
			B3_THROW(b3TxException,B3_TX_UNSUPP);
	}
	b3Free(line);
}

b3_result b3Tx::b3ParseSGI (b3_u08 *buffer)
{
	HeaderSGI *HeaderSGI;
	b3_bool    success = false;
	b3_res     xNewSize,yNewSize;
	b3_index   c;

	b3PrintF(B3LOG_FULL,"IMG SGI  # b3ParseSGI(%s)\n",
		(const char *)image_name);

	HeaderSGI = (struct HeaderSGI *)buffer;
	if (HeaderSGI->imagic == IMAGIC2)
	{
		b3Endian::b3ChangeEndian16 (&HeaderSGI->type);
		b3Endian::b3ChangeEndian16 (&HeaderSGI->dim);
		b3Endian::b3ChangeEndian16 (&HeaderSGI->xsize);
		b3Endian::b3ChangeEndian16 (&HeaderSGI->ysize);
		b3Endian::b3ChangeEndian16 (&HeaderSGI->zsize);
		b3Endian::b3ChangeEndian32 (&HeaderSGI->min);
		b3Endian::b3ChangeEndian32 (&HeaderSGI->max);
		b3Endian::b3ChangeEndian32 (&HeaderSGI->wastebytes);
		b3Endian::b3ChangeEndian32 (&HeaderSGI->colormap);
	}

	xNewSize = HeaderSGI->xsize;
	yNewSize = HeaderSGI->ysize;
	depth    = HeaderSGI->zsize << 3;


	// get texture type
	switch (HeaderSGI->colormap)
	{
		case 0 :
			switch (HeaderSGI->zsize)
			{
				case 1 :
					success = b3AllocTx(xNewSize,yNewSize,8);
					break;
				case 3 :
					success = b3AllocTx(xNewSize,yNewSize,24);
					break;

				default :
					b3FreeTx();
					b3PrintF(B3LOG_NORMAL,"IMG SGI  # Unsupported format:\n");
					B3_THROW( b3TxException,B3_TX_UNSUPP);
					break;
			}
			break;

		case 3 :
			success = b3AllocTx(xNewSize,yNewSize,8);
			if (success)
			{
				for (c=0;c<256;c++)
				{
					palette[c] =
						((c & 0xe0) << 16) |
						((c & 0x1c) << 11) |
						((c & 0x03) <<  6);
				}
			}
			break;

		default :
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL,"IMG SGI  # Unsupported format:\n");
			B3_THROW(b3TxException,B3_TX_UNSUPP);
			break;
	}
	if (success)
	{
		/* how are rows saved? */
		switch (HeaderSGI->dim)
		{
			case 3 :
				b3ParseSGI3(HeaderSGI,buffer);
				break;

			case 1 :
			case 2 :
			default :
				b3FreeTx();
				b3PrintF(B3LOG_NORMAL,"IMG SGI  # Wrong packing algorithms:\n");
				B3_THROW(b3TxException,B3_TX_ERR_PACKING);
		}

		// Success
		if (type != B3_TX_UNDEFINED)
		{
			FileType = FT_SGI_RLE;
		}
		else
		{
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL,"IMG SGI  # Error allocating memory:\n");
			B3_THROW(b3TxException,B3_TX_MEMORY);
		}
	}
	return B3_OK;
}
