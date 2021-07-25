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
**	(C) Copyright 2001 - 2021  Steffen A. Mork
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

#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Endian.h"
#include "blz3/image/b3Tx.h"
#include "blz3/image/b3TxExif.h"

#include "b3TxIFF.h"
#include "b3TxSGI.h"
#include "b3TxTIFF.h"

#include <regex>

#ifdef BLZ3_USE_OPENEXR
#include <ImfVersion.h>
#endif

#ifdef HAVE_PNG_H
#include <png.h>
#endif

/*************************************************************************
**                                                                      **
**                        Image loading
**                                                                      **
*************************************************************************/

b3_result b3Tx::b3LoadImage(const b3_u08 * buffer, b3_size buffer_size)
{
	const b3_u32    *    LongData;
	const b3HeaderTIFF * header_tiff;
	const b3HeaderSGI  * header_sgi;
	b3_size              pos;
	b3_coord             x, y;
	b3_size              i;

	b3FreeTx();

	// Check for small buffer
	LongData = reinterpret_cast<const b3_u32 *>(buffer);
	if (buffer_size < 4)
	{
		B3_THROW(b3TxException, B3_TX_ERR_HEADER);
	}

	// Check for IFF
	if (b3Endian::b3GetMot32(LongData) == IFF_FORM)
	{
		const b3_u32 iff_type = b3Endian::b3GetMot32(&LongData[2]);

		switch (iff_type)
		{
		case IFF_ILBM :
			return b3ParseIFF_ILBM(buffer, buffer_size);
		case IFF_RGB8 :
			return b3ParseIFF_RGB8(buffer, buffer_size);
		case IFF_RGBN :
			return b3ParseIFF_RGB4(buffer, buffer_size);
		case IFF_YUVN :
			return b3ParseIFF_YUVN(buffer, buffer_size);
		default :
			B3_THROW(b3TxException, B3_TX_ERR_HEADER);
		}
	}


#ifdef HAVE_JPEGLIB_H
	// JPEG
	for (i = 0; i < std::min<b3_size>(256, buffer_size - 2); i++)
	{
		if ((buffer[i] == 0xff) && (buffer[i + 1] == 0xd8) && (buffer[i + 2] == 0xff))
		{
			return b3ParseJPEG(&buffer[i], buffer_size - i);
		}
	}
#endif


	// GIF
	if (strncmp((const char *)buffer, "GIF", 3) == 0)
	{
		return b3ParseGIF(buffer);
	}


#ifdef HAVE_PNG_H
	// PNG
	if (png_sig_cmp(buffer, 0, 8) == 0)
	{
		return b3ParsePNG(buffer, buffer_size);
	}
#endif


#ifdef HAVE_LIBTIFF
	// TIFF
	header_tiff = reinterpret_cast<const b3HeaderTIFF *>(buffer);
	if ((header_tiff->TypeCPU == B3_BIG_ENDIAN) || (header_tiff->TypeCPU == B3_LITTLE_ENDIAN))
	{
		return b3LoadTIFF(b3Name(), buffer, buffer_size);
	}
#endif


	// PPM types 4, 5 and 6 binary non ASCII.
	// Use https://regex101.com/
	if (buffer[0] == 'P')
	{
		static const std::regex  regex(R"(P([1-6])\n(\d+)\s+(\d+)\n.*)");
		std::smatch              matcher;

		// Init.
		x        = 0;
		y        = 0;

		b3_size space = 0;
		for (pos = 0; (pos < buffer_size) && (space < 3); pos++)
		{
			if (isspace(buffer[pos]))
			{
				space++;
			}
		}
		const std::string        parse((const char *)buffer, pos);
		if (std::regex_match(parse, matcher, regex))
		{
			unsigned                 ppm_type;

			ppm_type = std::stoul(matcher[1]);
			x        = std::stol(matcher[2]);
			y        = std::stol(matcher[3]);

			switch (ppm_type)
			{
			case 4 :
				if ((b3_size)(((x + 7) >> 3) * y + pos) <= buffer_size)
				{
					pos = (b3_offset)buffer_size - ((x + 7) >> 3) * y;
					FileType = FT_PBM;
					return b3ParseRAW(&buffer[pos], x, y, ppm_type);
				}
				break;

			case 5 :
				if ((b3_size)(x * y + pos) <= buffer_size)
				{
					pos = (b3_offset)buffer_size - x * y;
					FileType = FT_PGM;
					return b3ParseRAW(&buffer[pos], x, y, ppm_type);
				}
				break;

			case 6 :
				if ((b3_size)(x * y * 3 + pos) <= buffer_size)
				{
					pos = (b3_offset)buffer_size - 3 * x * y;
					FileType = FT_PPM;
					return b3ParseRAW(&buffer[pos], x, y, ppm_type);
				}
				break;

			case 1 : /* ASCII pendants */
			case 2 :
			case 3 :
			default :
				B3_THROW(b3TxException, B3_TX_UNSUPP);
			}
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
	for (pos = 0; (pos < 100) && (buffer[pos] != 10); pos++);
	if (pos > 3)
	{
		static const std::regex  regex(R"((\d+)\s+(\d+).*)");
		std::smatch              matcher;
		const std::string        parse((const char *)buffer, pos++);

		if (std::regex_match(parse, matcher, regex))
		{
			x = std::stol(matcher[1]);
			y = std::stol(matcher[2]);

			if ((b3_size)(x * y * 3 + pos) == buffer_size)
			{
				FileType = FT_MTV;
				return b3ParseRAW(&buffer[pos], x, y, 6);
			}
		}
	}


	// BMF
	x = b3Endian::b3GetIntel16(&buffer[2]);
	y = b3Endian::b3GetIntel16(&buffer[4]);
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
		return b3ParseBMF(buffer, buffer_size);
	}

	// SGI RLE
	header_sgi = reinterpret_cast<const b3HeaderSGI *>(buffer);
	if ((header_sgi->imagic == IMAGIC1) || (header_sgi->imagic == IMAGIC2))
	{
		return b3ParseSGI(buffer);
	}

	// Targa
	if ((buffer[2] == 2) || (buffer[2] == 10))
	{
		if ((b3Endian::b3Get32(&buffer[4]) == 0) &&
			(b3Endian::b3Get32(&buffer[8]) == 0))
		{
			if ((buffer[16] == 32) || (buffer[16] == 24))
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
		case 8  :
			return b3ParsePCX8(buffer);
		case 1 	:
			return b3ParsePCX4(buffer);
		default :
			B3_THROW(b3TxException, B3_TX_UNSUPP);
		}
	}


	// really unknown
	B3_THROW(b3TxException, B3_TX_UNSUPP);
}

b3_result b3Tx::b3LoadImage(const char * name, bool throw_exception)
{
	b3File     file;
	b3_size    size;
	b3_result  error_code = B3_ERROR;

	try
	{
		b3Name(name);
		b3_u08 * buffer = file.b3ReadBuffer(name, size);
		error_code      = b3LoadImage(buffer, size);
		if (error_code != B3_OK)
		{
			B3_THROW(b3TxException, B3_TX_ERROR);
		}
		b3Name(name);
	}
	catch (b3FileException & e)
	{
		b3PrintF(B3LOG_NORMAL, "Error loading %s (%s)\n",
			name, e.b3GetErrorMsg());
		if (throw_exception)
		{
			throw;
		}
	}
	catch (b3TxException & e)
	{
		b3PrintF(B3LOG_NORMAL, "Error parsing %s (%s)\n",
			name, e.b3GetErrorMsg());
		if (throw_exception)
		{
			throw;
		}
	}
	catch (...)
	{
		b3PrintF(B3LOG_NORMAL, "Unknown error parsing %s\n", name);
		if (throw_exception)
		{
			throw;
		}
	}

	return error_code;
}

b3_tx_filetype b3Tx::b3GetFileType(const char * ext)
{
	if (b3StringTool::b3CaseCompare(ext, "tif")  == 0)
	{
		return FT_TIFF;
	}
	if (b3StringTool::b3CaseCompare(ext, "tiff") == 0)
	{
		return FT_TIFF;
	}
	if (b3StringTool::b3CaseCompare(ext, "tga")  == 0)
	{
		return FT_TGA;
	}
	if (b3StringTool::b3CaseCompare(ext, "jpg")  == 0)
	{
		return FT_JPEG;
	}
	if (b3StringTool::b3CaseCompare(ext, "jpeg") == 0)
	{
		return FT_JPEG;
	}
	if (b3StringTool::b3CaseCompare(ext, "gif")  == 0)
	{
		return FT_GIF;
	}
	if (b3StringTool::b3CaseCompare(ext, "rgb8") == 0)
	{
		return FT_RGB8;
	}
	if (b3StringTool::b3CaseCompare(ext, "rgb4") == 0)
	{
		return FT_RGB4;
	}
	if (b3StringTool::b3CaseCompare(ext, "rgbn") == 0)
	{
		return FT_RGB4;
	}
	if (b3StringTool::b3CaseCompare(ext, "pcx")  == 0)
	{
		return FT_PCX8;
	}
	if (b3StringTool::b3CaseCompare(ext, "lbm")  == 0)
	{
		return FT_ILBM;
	}
	if (b3StringTool::b3CaseCompare(ext, "iff")  == 0)
	{
		return FT_ILBM;
	}
	if (b3StringTool::b3CaseCompare(ext, "ilbm") == 0)
	{
		return FT_ILBM;
	}
	if (b3StringTool::b3CaseCompare(ext, "yuv")  == 0)
	{
		return FT_YUV;
	}
	if (b3StringTool::b3CaseCompare(ext, "img")  == 0)
	{
		return FT_SGI_RLE;
	}
	if (b3StringTool::b3CaseCompare(ext, "ps")   == 0)
	{
		return FT_PS;
	}
	if (b3StringTool::b3CaseCompare(ext, "bmp")  == 0)
	{
		return FT_BMP;
	}
	if (b3StringTool::b3CaseCompare(ext, "exr")  == 0)
	{
		return FT_EXR;
	}

	return FT_UNKNOWN;
}

const char * b3Tx::b3GetExt() const
{
	return b3GetExt(FileType);
}

const char * b3Tx::b3GetExt(b3_tx_filetype type)
{
	switch (type)
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
		return nullptr;
	}
	return nullptr;
}

b3_result b3Tx::b3SaveImage(const char * filename, b3TxExif * exif)
{
	b3Path         ext;
	b3_tx_filetype filetype;

	if (exif != nullptr)
	{
		exif->b3GetResolution(xDPI, yDPI);
	}

	if (b3IsLoaded())
	{
		ext.b3ExtractExt(filename);
		filetype = b3GetFileType(ext);

		switch (filetype)
		{
#ifdef HAVE_LIBJPEG
		case FT_JPEG:
			return b3SaveJPEG(filename, B3_JPG_QUALITY, exif);
#endif
#ifdef HAVE_LIBTIFF
		case FT_TIFF:
			return b3SaveTIFF(filename, exif);
#endif
		case FT_TGA:
			return b3SaveTGA(filename);
		case FT_RGB8:
			return b3SaveRGB8(filename);
		case FT_PS:
			return b3SavePS(filename);
#ifdef BLZ3_USE_OPENEXR
		case FT_EXR:
			return b3SaveEXR(filename);
#endif

		case FT_UNKNOWN:
			B3_THROW(b3TxException, B3_TX_UNKNOWN_FILETYPE);

		default:
			return B3_ERROR;
		}
	}

	return B3_ERROR;
}
