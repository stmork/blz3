/*
**
**	$Filename:	b3TxLoadImage.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3TxInclude.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"

#include "b3TxIFF.h"
#include "b3TxTIFF.h"

#ifdef BLZ3_USE_OPENEXR
#include <ImfVersion.h>
#endif

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
	b3_index      pos;
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
			const char *jpg_start = (const char *)&buffer[i+6];
			      char *jfif      = strstr(jpg_start,"JFIF");
			      char *exif      = strstr(jpg_start,"Exif");

			if ((jfif != null) || (exif != null))
			{
				return b3ParseJPEG(&buffer[i],buffer_size - i);
			}
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
	pos      = 0;
	x        = 0;
	y        = 0;
	ppm_type = 0;
	i   = sscanf((const char *)buffer,"P%d %ld %ld %*d%ln",&ppm_type,&x,&y,&pos);
	b3PrintF (B3LOG_FULL,"PxM (%ld): (%ld,%ld - %ld) %d\n",ppm_type,x,y,i,pos);
	if (i >= 2)
	{
		switch (ppm_type)
		{
			case 4 :
				if ((b3_size)(((x + 7) >> 3) * y + pos) <= buffer_size)
				{
					pos = (b3_offset)buffer_size - ((x + 7) >> 3) * y;
					FileType = FT_PBM;
					return b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;

			case 5 :
				if ((b3_size)(x * y + pos) <= buffer_size)
				{
					pos = (b3_offset)buffer_size - x * y;
					FileType = FT_PGM;
					return b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;
				
			case 6 :
				if ((b3_size)(x * y * 3 + pos) <= buffer_size)
				{
					pos = (b3_offset)buffer_size - 3 * x * y;
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

#ifdef BLZ3_USE_OPENEXR
	// OpenEXR
	if (Imf::isImfMagic((const char *)buffer))
	{
		return b3ParseOpenEXR(buffer, buffer_size);
	}
#endif

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
		if (error_code != B3_OK)
		{
			B3_THROW(b3TxException,B3_TX_ERROR);
		}
		b3Name(name);
	}
	catch (b3FileException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Error loading %s (%s)\n",
			name,e.b3GetErrorMsg());
		if (throw_exception)
		{
			throw e;
		}
	}
	catch(b3TxException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Error parsing %s (%s)\n",
			name,e.b3GetErrorMsg());
		if (throw_exception)
		{
			throw e;
		}
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"Unknown error parsing %s\n",name);
		if (throw_exception)
		{
			throw;
		}
	}

	return error_code;
}

b3_tx_filetype b3Tx::b3GetFileType(const char *ext)
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
	if (stricmp(ext,"exr")  == 0) return FT_EXR;

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

	case FT_EXR:
		return "exr";

	default:
		return null;
	}
	return null;
}

b3_result b3Tx::b3SaveImage(const char *filename)
{
	b3Path         ext;
	b3_tx_filetype filetype;

	if (b3IsLoaded())
	{
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
		case FT_PS:
			return b3SavePS(filename);
		case FT_EXR:
			return b3SaveEXR(filename);

		case FT_UNKNOWN:
			B3_THROW(b3TxException, B3_TX_UNKNOWN_FILETYPE);

		default:
			return B3_ERROR;
		}
	}
	
	return B3_ERROR;
}
