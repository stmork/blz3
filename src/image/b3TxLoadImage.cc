/*
**
**	$Filename:	b3TxLoadImage.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading different types of image formats
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
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Endian.h"

#include "b3TxIFF.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.14  2003/09/26 07:23:16  sm
**	- New JPEG library
**
**	Revision 1.13  2002/08/25 13:03:02  sm
**	- Added a tool to restore correct file extensions.
**	- b3Tx can determine the image types' file extension.
**	
**	Revision 1.12  2002/08/17 17:31:22  sm
**	- Introduced animation support (Puh!)
**	
**	Revision 1.11  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.10  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.9  2002/08/07 12:38:43  sm
**	- Modified exception definition. Exceptions are identified with
**	  a three character code to unify error codes. This is necessary
**	  to convert error codes into error messages inside applications.
**	- Added some additional b3Hash methods.
**	- Added -Wall compiler option to all C++ files.
**	- Removed some compiler warnings.
**	
**	Revision 1.8  2002/03/13 19:01:58  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.7  2002/02/20 20:23:57  sm
**	- Some type cleanups done.
**	
**	Revision 1.6  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.5  2002/01/17 19:17:03  sm
**	- Fixed ILBM to other unfiltered scaling
**	
**	Revision 1.4  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
**	Revision 1.3  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.2  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.1  2001/10/26 18:37:14  sm
**	- Creating search path support
**	- Splitting image pool support and image loading into
**	  their own area.
**	- Fixed JPEG to support b3Tx::b3AllocTx()
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Image loading             
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3LoadImage (b3_u08 *buffer,b3_size buffer_size)
{
	b3_pkd_color *LongData;
	HeaderTIFF   *TIFF;
	HeaderSGI    *HeaderSGI;
	int           pos;
	b3_coord      x,y;
	b3_s32        ppm_type;
	b3_index      i;

	b3FreeTx();
	LongData = (b3_pkd_color *)buffer;
	if (buffer_size < 4)
	{
		b3FreeTx();
		B3_THROW(b3TxException,B3_TX_ERR_HEADER);
	}

	// schon mal irgend ein IFF
	if (b3Endian::b3GetMot32(LongData) == IFF_FORM)
	{
		switch (b3Endian::b3GetMot32(&LongData[2]))
		{
			case IFF_ILBM : return b3ParseIFF_ILBM(buffer,buffer_size);
			case IFF_RGB8 :	return b3ParseIFF_RGB8(buffer,buffer_size);
			case IFF_RGBN : return b3ParseIFF_RGB4(buffer,buffer_size);
			case IFF_YUVN : return b3ParseIFF_YUVN(buffer,buffer_size);
			default :
				b3FreeTx();
				B3_THROW(b3TxException,B3_TX_ERR_HEADER);
		}
	}


	// JPEG
	for (i = 0;i < (b3_index)B3_MIN(256,buffer_size - 2);i++)
	{
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xd8) && (buffer[i+2] == 0xff))
		{
			return b3ParseJPEG(&buffer[i],buffer_size - i);
		}
	}


	// GIF
	if (strncmp((const char *)buffer,"GIF",3) == 0)
	{
		return b3ParseGIF (buffer);
	}


	// TIFF
	TIFF = (HeaderTIFF *)buffer;
	if ((TIFF->TypeCPU == B3_BIG_ENDIAN) || (TIFF->TypeCPU == B3_LITTLE_ENDIAN))
	{
#ifndef USE_TIFFLIB_LOAD
		if (TIFF->VersionTIFF == 0x2a00)
		{
			ChangeTIFF (TIFF);
		}
		if (TIFF->VersionTIFF == 0x002a)
		{
			return b3ParseTIFF(TIFF,buffer_size);
		}
#else
		return b3LoadTIFF(b3Name(),buffer,buffer_size);
#endif
	}


	// PPM6
	pos = 0;
	x   = 0;
	y   = 0;
	ppm_type = 0;
	i   = sscanf((const char *)buffer,"P%d %ld %ld %*d%n",&ppm_type,&x,&y,&pos);
	b3PrintF (B3LOG_FULL,"PxM (%ld): (%ld,%ld - %ld) %d\n",ppm_type,x,y,i,pos);
	if (i >= 2)
	{
		switch (ppm_type)
		{
			case 4 :
				if ((b3_size)(((x + 7) >> 3) * y + pos) <= buffer_size)
				{
					pos = buffer_size - ((x + 7) >> 3) * y;
					FileType = FT_PBM;
					return b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;

			case 5 :
				if ((b3_size)(x * y + pos) <= buffer_size)
				{
					pos = buffer_size - x * y;
					FileType = FT_PGM;
					return b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;
				
			case 6 :
				if ((b3_size)(x * y * 3 + pos) <= buffer_size)
				{
					pos = buffer_size - 3 * x * y;
					FileType = FT_PPM;
					return b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;

			case 1 : /* ASCII pendants */
			case 2 :
			case 3 :
			default :
				b3FreeTx();
				B3_THROW(b3TxException,B3_TX_UNSUPP);
		}
	}


	// BMP
	if ((buffer[0] == 'B') &&
	    (buffer[1] == 'M') &&
	    (b3Endian::b3GetIntel32(&buffer[2]) == buffer_size))
	{
		return b3ParseBMP(buffer);
	}


	// MTV
	if (sscanf((const char *)buffer,"%ld %ld",&x,&y) == 2)
	{
		for (pos = 0;(pos < 100) && (buffer[pos] != 10);pos++);
		if (buffer[pos++] == 10)
		{
			if ((b3_size)(x * y * 3 + pos) == buffer_size)
			{
				FileType = FT_MTV;
				return b3ParseRAW (&buffer[pos],x,y,6);
			}
		}
	}


	// BMF
	x = b3Endian::b3GetIntel16 (&buffer[2]);
	y = b3Endian::b3GetIntel16 (&buffer[4]);
	switch (buffer[6])
	{
		case 2 :
			i = 1;
			break;
		case 4 :
			i = 3;
			break;
		default :
			i = 0;
			break;
	}
	if ((b3_size)(x * y * i + 16) == buffer_size)
	{
		return b3ParseBMF(buffer,buffer_size);
	}
	
	// SGI RLE
	HeaderSGI = (struct HeaderSGI *)buffer;
	if ((HeaderSGI->imagic == IMAGIC1) || (HeaderSGI->imagic == IMAGIC2))
	{
		return b3ParseSGI(buffer);
	}


	// Targa
	if ((buffer[2] == 2) || (buffer[2] == 10))
	{
		if ((b3Endian::b3Get32(&buffer[4]) == 0) &&
		    (b3Endian::b3Get32(&buffer[8]) == 0))
		{
			if ((buffer[16]==32) || (buffer[16]==24))
			{
				return b3ParseTGA(buffer);
			}
		}
	}


	// PCX
	if ((buffer[2] == 1) && (buffer[0] == 0x0a))
	{
		switch (buffer[3])			/* Bits pro Pixel */
		{
			case 8  : return b3ParsePCX8(buffer);
			case 1 	: return b3ParsePCX4(buffer);
			default :
				b3FreeTx();
				B3_THROW(b3TxException,B3_TX_UNSUPP);
		}
	}


	// really unknown
	b3FreeTx();
	B3_THROW(b3TxException,B3_TX_UNSUPP);
}

b3_result b3Tx::b3LoadImage(const char *name,b3_bool throw_exception)
{
	b3File     file;
	b3_u08    *buffer;
	b3_size    size;
	b3_result  error_code = B3_ERROR;

	try
	{
		b3Name(name);
		buffer     = file.b3ReadBuffer(name,size);
		error_code = b3LoadImage(buffer,size);
		b3Name(name);
		error_code = B3_OK;
	}
	catch (b3FileException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Error loading %s (%s)\n",
			name,e.b3GetErrorMsg());
		if (throw_exception)
		{
			throw;
		}
	}
	catch(b3TxException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Error parsing %s (%s)\n",
			name,e.b3GetErrorMsg());
		if (throw_exception)
		{
			throw;
		}
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"Unknown error parsing %s\n",
			name);
		if (throw_exception)
		{
			throw;
		}
	}
	return error_code;
}

const b3_tx_filetype b3Tx::b3GetFileType(const char *ext)
{
	if (stricmp(ext,"tif")  == 0) return FT_TIFF;
	if (stricmp(ext,"tiff") == 0) return FT_TIFF;
	if (stricmp(ext,"tga")  == 0) return FT_TGA;
	if (stricmp(ext,"jpg")  == 0) return FT_JPEG;
	if (stricmp(ext,"jpeg") == 0) return FT_JPEG;
	if (stricmp(ext,"gif")  == 0) return FT_GIF;
	if (stricmp(ext,"rgb8") == 0) return FT_RGB8;
	if (stricmp(ext,"rgb4") == 0) return FT_RGB4;
	if (stricmp(ext,"rgbn") == 0) return FT_RGB4;
	if (stricmp(ext,"pcx")  == 0) return FT_PCX8;
	if (stricmp(ext,"lbm")  == 0) return FT_ILBM;
	if (stricmp(ext,"iff")  == 0) return FT_ILBM;
	if (stricmp(ext,"ilbm") == 0) return FT_ILBM;
	if (stricmp(ext,"yuv")  == 0) return FT_YUV;
	if (stricmp(ext,"img")  == 0) return FT_SGI_RLE;
	if (stricmp(ext,"ps")   == 0) return FT_PS;
	if (stricmp(ext,"bmp")  == 0) return FT_BMP;

	return FT_UNKNOWN;
}

const char *b3Tx::b3GetExt()
{
	return b3GetExt(FileType);
}

const char *b3Tx::b3GetExt(b3_tx_filetype type)
{
	switch(type)
	{
	case FT_PCX4:
	case FT_PCX8:
		return "pcx";

	case FT_ILBM:
	case FT_ILBM_HAM:
	case FT_ILBM_HAM8:
	case FT_ILBM_EHB:
	case FT_ILBM_24:
		return "iff";

	case FT_RGB8:
		return "rgb8";
	case FT_RGB4:
		return "rgbn";

	case FT_MTV:
		return "mtv";

	case FT_YUV:
		return "yuv";

	case FT_TIFF:
		return "tif";

	case FT_TGA:
		return "tga";

	case FT_GIF:
		return "gif";

	case FT_PPM6:
		return "ppm";

	case FT_BMP:
		return "bmp";

	case FT_SGI_RLE:
		return "img";

	case FT_PPM:
	case FT_PGM:
	case FT_PBM:
		return "ppm";

	case FT_JPEG:
		return "jpg";

	case FT_BMF:
		return "bmf";

	case FT_PS:
		return "ps";

	default:
		return null;
	}
	return null;
}

b3_result b3Tx::b3SaveImage(const char *filename)
{
	b3Path         ext;
	b3_tx_filetype filetype;

	ext.b3ExtractExt(filename);
	filetype = b3GetFileType(ext);
	switch(filetype)
	{
	case FT_JPEG:
		return b3SaveJPEG(filename);
	case FT_TIFF:
		return b3SaveTIFF(filename);
	case FT_TGA:
		return b3SaveTGA(filename);
	case FT_RGB8:
		return b3SaveRGB8(filename);

	default:
		return B3_ERROR;
	}
	
	// Never reached
	return B3_ERROR;
}
