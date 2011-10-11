/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2011 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (proto types)
**
**	(C) Copyright 2011 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGE_TX_H
#define B3_IMAGE_TX_H

#include "blz3/b3Config.h"
#include "blz3/system/b3FileAbstract.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3List.h"

#include "tiff.h"
#include "tiffio.h"

/*************************************************************************
**                                                                      **
**                        Some useful defines                           **
**                                                                      **
*************************************************************************/

#define TX_BBA(x) ((((x) +  7) & 0xfffffff8) >> 3)
#define TX_BWA(x) ((((x) + 15) & 0xfffffff0) >> 3)
#define TX_BLA(x) ((((x) + 31) & 0xffffffe0) >> 3)

#define TX_VBA(x)   (x)
#define TX_VWA(x) (((x) +  1) & 0xfffffffe)
#define TX_VLA(x) (((x) +  3) & 0xfffffffc)

#define TX_RGB4_TO_RGB8(color) (\
	(((color) & 0x0f00) << 12) |\
	(((color) & 0x00f0) <<  8) |\
	(((color) & 0x000f) <<  4))

#define B3_MAX_GRID (1 << (2 + 2 + 2))
#define b3ColorGridIndex(color) (\
	(((color) & 0xc00000) >> 18) |\
	(((color) & 0x00c000) >> 12) |\
	(((color) & 0x0000c0) >>  6))

#define B3_TX_MAX_HISTGRM_DEPTH 8
#define B3_TX_MAX_HISTGRM       (1 << B3_TX_MAX_HISTGRM_DEPTH)

#define B3_JPG_QUALITY       85

/*************************************************************************
**                                                                      **
**                        SGI RLE structures                            **
**                                                                      **
*************************************************************************/

/**
 * This structure defines the SGI image file format header.
 */
struct HeaderSGI
{
	b3_u16	imagic;		/* stuff saved on disk . . */
	b3_u16 	type;
	b3_u16 	dim;
	b3_u16 	xsize;
	b3_u16 	ysize;
	b3_u16 	zsize;
	b3_u32 	min;
	b3_u32 	max;
	b3_u32	wastebytes;
	char 	name[80];
	b3_u32	colormap;
};

#define IMAGIC1 	0x01da
#define IMAGIC2 	0xda01

/*************************************************************************
**                                                                      **
**                        Imaging structures, classes and defines       **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists all possible image formats.
 */
enum b3_tx_type
{
	B3_TX_UNDEFINED = -1, //!< Undefined or empty image.
	B3_TX_ILBM      =  0, //!< BW image with 16 bit alignment for each scan line (b3_u16).
	B3_TX_VGA       =  1, //!< Palette image with unsigned byte indices (b3_u08) and b3_pkd_color palette.
	B3_TX_RGB4      =  2, //!< Color channels in true color ARGB nibbles (b3_u16).
	B3_TX_RGB8      =  3, //!< Color channels in true color AARRGGBB nibbles (b3_pkd_color).
	B3_TX_FLOAT     =  4  //!< Color channels in floating point true color ARGB (b3_color).
};

/*************************************************************************
**                                                                      **
**                        File format codes                             **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists all source file formats.
 */
enum b3_tx_filetype
{
	FT_UNKNOWN		=   0,  //!< unknown file format.
	FT_PCX4,                //!< PCX 16 colors.
	FT_PCX8,                //!< PCX 256 colors.
	FT_ILBM,                //!< IFF ILBM
	FT_ILBM_HAM,            //!< IFF ILBM hold&modify 6 bit.
	FT_ILBM_HAM8,           //!< IFF ILBM hold&modify 8 bit.
	FT_ILBM_EHB,            //!< IFF ILBM extra halfbrite.
	FT_ILBM_24,             //!< IFF ILBM true color.
	FT_RGB8,                //!< Impluse true color 24 bit,
	FT_RGB4,                //!< Impluse true color 12 bit.
	FT_MTV,                 //!< MTV.
	FT_YUV,                 //!< IFF ILBM YUV format.
	FT_TIFF,                //!< Tagged Image File Format.
	FT_TGA,                 //!< Targa true color.
	FT_GIF,                 //!< Graphics Interchange Format.
	FT_PPM6,                //!< Pixmap.
	FT_BMP,                 //!< Microsoft bitmap.
	FT_SGI_RLE,             //!< SGI RLE.
	FT_PPM,                 //!< Portable pixmap.
	FT_PGM,                 //!< Portable grey map.
	FT_PBM,                 //!< Portable bitmap.
	FT_JPEG,                //!< Joint Pictures Expert Group.
	FT_BMF,                 //!< BMF format.
	FT_PS,                  //!< PostScript.
	FT_EXR                  //!< OpenEXR (high dynamice range).
};

/**
 * This enumeration lists grey to b/w converting modes.
 */
enum b3_tx_threshold
{
	B3_THRESHOLD_WHITE_RATIO = 0,  //!< Use white page ratio as separation limit.
	B3_THRESHOLD_USE         = 1,  //!< Use custom threshold for separation.
	B3_THRESHOLD_EQUALIZE    = 2   //!< Use histogramme equalization.
};

/*************************************************************************
**                                                                      **
**                        Exception error codes                         **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists all possible immage error codes.
 */
enum b3_tx_error
{
	B3_TX_ERROR = -1,         //!< General image processing error.
	B3_TX_OK    =  0,         //!< Image OK.
	B3_TX_MEMORY,             //!< Out of memory error.
	B3_TX_NOT_FOUND,          //!< File not found.
	B3_TX_NOT_SAVED,          //!< Disk full.
	B3_TX_UNSUPP,             //!< Unsupported sub file format.
	B3_TX_ERR_PACKING,        //!< Unknown packing algorithm.
	B3_TX_UNCOMPL,            //!< Uncomplete image file.
	B3_TX_ERR_HEADER,         //!< File header format error.
	B3_TX_UNKNOWN_FILETYPE,   //!< Unknown file type.
	B3_TX_UNKNOWN_DATATYPE,   //!< Unknown image representation.
	B3_TX_ILLEGAL_DATATYPE    //!< Internal datatype use illegal.
};

typedef b3Exception<b3_tx_error,0x5458> b3TxException;

/*************************************************************************
**                                                                      **
**                        Image helper classes                          **
**                                                                      **
*************************************************************************/

/**
 * This class initializes a lookup table with squared values.
 */
class b3TxQuad
{
	b3_pkd_color quad256[512];

	b3TxQuad()
	{
		b3_loop i;

		for (i = 0;i < 256;i++)
		{
			quad256[256 - i] =
				quad256[256 + i] = i * i;
		}
	}

	friend class b3ColorIndices;
};

/**
 * This class holds image palette indices.
 */
class B3_PLUGIN b3ColorIndices : public b3Mem
{
	static b3TxQuad  m_TxQuad;

	b3_count    num;
	b3_count    max;
	b3_index   *indices;
public:
	b3ColorIndices ();
	void      b3AddColorIndex(b3_index);
	b3_index  b3ColorIndex   (b3_pkd_color *,b3_pkd_color);
};

#define CLASS_TEXTURE       0x20000000
#define TYPE_USUAL          0x00000001
#define USUAL_TEXTURE       (CLASS_TEXTURE|TYPE_USUAL)

/*************************************************************************
**                                                                      **
**                        The image class itself!                       **
**                                                                      **
*************************************************************************/

/**
 * This big class represents one image in its best representation.
 * It has many convenience methods for manipulating the image.
 */
class B3_PLUGIN b3Tx : public b3Link<b3Tx>, public b3Mem
{
	static const b3_u08  m_Bits[8];
	static const b3_u08  m_RightMaskLeftByte[16];
	static const b3_u08  m_RightMaskRightByte[16];
	static const b3_u08  m_RightBorder[];
	static       b3_bool m_ErrorHandlerInstalled;

private:
	b3_pkd_color     *palette;
	b3_count         *histogramme;
	b3_u08           *data;
	b3_count          dSize,pSize;
	b3_tx_type        type;
	b3_tx_filetype    FileType;
	b3_f64            whiteRatio;
	b3Path            image_name;
	b3ColorIndices   *grid;

public:
	b3_res            xSize;       //!< The image width;
	b3_res            ySize;       //!< The image height;
	b3_res            depth;       //!< The image depth in bit;
	b3_count          ScanLines;   //!< Number of row to process during scanning.
	b3_res            xDPI;        //!< The pixel density in x direction.
	b3_res            yDPI;        //!< The pixel density in y direction.

	/**
	 * This constructor initializes an empty image.
	 */
	b3Tx();

	/**
	 * This constructor copies the given image as initialization.
	 *
	 * @param srcTx The source image.
	 * @see b3CopyTx()
	 */
	b3Tx(b3Tx *srcTx);

	/**
	 * This destructor deinitializes the image.
	 */
	~b3Tx           ();

	/**
	 * This method copies the data of the given source image.
	 *
	 * @param srcTx The source image to copy.
	 */
	void           b3Copy         (b3Tx *srcTx);

	/**
	 * This method allocates memory for bitmap data or palette data depending
	 * on the given image format specifications. Depending on the bit depth
	 * the format is chosen automatically:
	 *
	 * - Bit depth 0: The memory is freed with b3FreeTx().
	 * - Bit depth 1: The image is of type B3_TX_ILBM.
	 * - Bit depth 2-8: The image is palattized and of type B3_TX_VGA.
	 * - Bit depth 12 or 16: The image is of type B3_TX_RGB4.
	 * - Bit depth 24 or 32: The image is of type B3_TX_RGB8.
	 * - Bit depth 96 or 128: The image is of type B3_TX_FLOAT.
	 *
	 * There is now image data allocated in all other cases.
	 *
	 * @param xSize The new image width.
	 * @param ySize The new image height.
	 * @param depth The new image bit depth.
	 * @return True of success.
	 * @see b3_tx_type
	 */
	b3_bool        b3AllocTx      (const b3_res xSize, const b3_res ySize, const b3_res depth);

	/**
	 * This method frees all image data.
	 */
	void           b3FreeTx       ();

	/**
	 * This method returns the palette data if any.
	 *
	 * @return The palette data.
	 */
	b3_pkd_color  *b3GetPalette   ();

	/**
	 * This method sets the given color palette. The bit depth is adjusted according
	 * to the palette entry count. If the new bit depth is 1 the type is B3_TX_ILBM otherwise
	 * the type is B3_TX_VGA. The old palette is freed.
	 *
	 * @note The maximal supported palette size is 256 with bit depth 8. This is due to the
	 *       fact that b3_u08 indices were used.
	 *
	 * @param entries The new palette.
	 * @param numEntries The palette size.
	 */
	void           b3SetPalette   (const b3_pkd_color *entries, b3_count numEntries);

	/**
	 * This method returns the image as palette index data pointer.
	 *
	 * @return The image palette index data.
	 */
	inline b3_u08 *b3GetIndexData()
	{
		if (data == null)
		{
			return null;
		}
		if ((!b3IsPalette()) || (depth > 8))
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return reinterpret_cast<b3_u08 *>(data);
	}

	/**
	 * This method returns the image as palette index data pointer.
	 *
	 * @return The image palette index data.
	 */
	inline b3_u16 *b3GetHighColorData()
	{
		if (data == null)
		{
			return null;
		}
		if (!b3IsHighColor())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return reinterpret_cast<b3_u16 *>(data);
	}

	/**
	 * This method returns the image as true color data pointer.
	 *
	 * @return The image true color data.
	 */
	inline b3_pkd_color *b3GetTrueColorData()
	{
		if (data == null)
		{
			return null;
		}
		if (!b3IsTrueColor())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return reinterpret_cast<b3_pkd_color *>(data);
	}

	/**
	 * This method returns the image as HDR data pointer.
	 *
	 * @return The image HDR data pointer.
	 */
	inline b3_color *b3GetHdrData()
	{
		if (data == null)
		{
			return null;
		}
		if (!b3IsHdr())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return reinterpret_cast<b3_color *>(data);
	}

	/**
	 * This method sets the new image data in the given resolution. The old data is
	 * deallocated.
	 *
	 * @param newData The new image data.
	 * @param newXSize The new image width.
	 * @param newYSize The new image height.
	 */
	void           b3SetData      (const void *newData, const b3_res newXSize, const b3_res newYSize);

	/**
	 * This method returns the file name from which the image was loaded. If the
	 * image data was allocated via the b3AllocTx() method this file name is empty.
	 *
	 * @return The file name of the image.
	 */
	const char    *b3Name         ();

	/**
	 * This method sets a new file name.
	 *
	 * @param ImageName The new file name of the image.
	 */
	void           b3Name         (const char *ImageName);

	/**
	 * This method returns the pixel density.
	 *
	 * @param xDPI The resulting horizontal pixel density.
	 * @param yDPI The resulting vertical pixel density.
	 */
	void           b3GetResolution(b3_res &xDPI,b3_res &yDPI);

	/**
	 * This method fills a given row buffer with the hdr color of the
	 * given y coordinate. The row buffer must have enough entries for
	 * retrieving a full scane line with the width of the image.
	 *
	 * @param row The row buffer.
	 * @param y The y coordinate of the row.
	 */
	void           b3GetRow       (b3_color *row, const b3_coord  y);

	/**
	 * This method fills a given row buffer with the true color of the
	 * given y coordinate. The row buffer must have enough entries for
	 * retrieving a full scane line with the width of the image.
	 *
	 * @param row The row buffer.
	 * @param y The y coordinate of the row.
	 */
	void           b3GetRow       (b3_pkd_color *row, const b3_coord  y);

	/**
	 * This method returns a color value at the given coordinates.
	 *
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return The resulting color as b3_pkd_color value.
	 */
	b3_pkd_color   b3GetValue     (const b3_coord x, const b3_coord  y);

	/**
	 * This method returns a color value at the given coordinates. The
	 * color may be in HDR format.
	 *
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @result The resulting color as HDR value.
	 */
	b3Color b3GetHdrValue     (const b3_coord x, const b3_coord  y);

	/**
	 * This method returns the blue color channel as floating point value.
	 * color may be in HDR format.
	 *
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return The resulting blue channel as floting point value.
	 */
	b3_f32 b3GetBlue(b3_coord x,b3_coord  y);

	/**
	 * This method returns true if the given pixel is transparent.
	 * In case B3_TX_ILBM and B3_TX_VGA the color index must be 0. In
	 * all other cases there must be an alpha channel greater than 0.
	 *
	 * @param x The x coordinate to test.
	 * @param y The y coordinate to test.
	 * @return True if the pixel is transparent.
	 */
	b3_bool        b3IsBackground (const b3_coord x, const b3_coord  y);

	/**
	 * This method returns true if this instance holds an image.
	 *
	 * @return True if this instance holds an image.
	 */
	b3_bool        b3IsLoaded     ();

	/**
	 * This method returns true if this image is black/white.
	 *
	 * @return True if this image is b/w.
	 */
	inline b3_bool b3IsBW()
	{
		return (depth == 1) && (type == B3_TX_ILBM);
	}

	/**
	 * This method returns true if this image is high color.
	 *
	 * @return True if this image is high color.
	 */
	inline b3_bool b3IsHighColor()
	{
		return ((depth == 12) || (depth == 16)) && (type == B3_TX_RGB4);
	}

	/**
	 * This method returns true if this image is true color.
	 *
	 * @return True if this image is true color.
	 */
	inline b3_bool b3IsTrueColor()
	{
		return (depth >= 24) && ((type == B3_TX_RGB8) || (type == B3_TX_FLOAT));
	}

	/**
	 * This method returns true if this image is a high dynamic range image.
	 *
	 * @return True if this image is a high dynamic range image.
	 */
	inline b3_bool b3IsHdr()
	{
		return (depth >= 96) && (type == B3_TX_FLOAT);
	}

	/**
	 * This method returns true if this image is palettized.
	 *
	 * @return True if this image is palettized.
	 */
	inline b3_bool b3IsPalette()
	{
		return palette != null;
	}

	/**
	 * This method returns true if this image is a palettized grey image.
	 *
	 * @return True if this image is a palettized grey image.
	 */
	b3_bool        b3IsGreyPalette();

	// b3TxBlit.cc
	/**
	 * This method creates an image mask from a specified key color. If the image
	 * type is B3_TX_ILBM the image data itself is used. In all other cases the
	 * color mask must be equal to the pixel value to set the mask bit to one.
	 *
	 * @param mask The resulting mask buffer.
	 * @param BytesPerRow The number of bytes per row in the mask buffer.
	 * @param colorMask The color key.
	 */
	void           b3GetColorMask(
		b3_u08       *mask,
		b3_count      BytesPerRow,
		b3_pkd_color  colorMask);

	/**
	 * This method blits an image area from the source image into this image. The
	 * destination type must be of type B3_TX_RGB8.
	 *
	 * @param srcTx   The source image.
	 * @param xDstOff The x image offset in the destination image.
	 * @param yDstOff The y image offset in the destination image.
	 * @param xMax    The image width to blit.
	 * @param yMax    The image height to blit.
	 * @param xSrcOff The x image offset in the source image.
	 * @param ySrcOff The y image offset in the source image.
	 */
	void           b3Blit       (
		b3Tx     *srcTx,
		b3_coord  xDstOff,  b3_coord yDstOff,
		b3_res    xMax,     b3_res   yMax,
		b3_coord  xSrcOff=0,b3_coord ySrcOff=0);

	// b3TxImage.cc
	/**
	 * This method reduces the black components of a B/W image. At the right black
	 * to white border the last black pixel is converted into a white one until
	 * there is only one pixel left.
	 *
	 * @note This method works only on B3_TX_ILBM images.
	 *
	 * @param shrink The iteration count.
	 */
	void           b3Shrink     (b3_count shrink=1);

	/**
	 * This method removes any black border on the left and right side of the image.
	 *
	 * @note This method works only on B3_TX_ILBM images.
	 */
	void           b3RemoveBlackBorder();

	/**
	 * This method negates the color.
	 */
	void           b3Negate     ();

	/**
	 * This method turns left the image by 90 degrees.
	 */
	void           b3TurnLeft   ();

	/**
	 * This method turns the image upside down.
	 */
	void           b3Turn       ();

	/**
	 * This method turns right the image by 90 degrees.
	 */
	void           b3TurnRight  ();

	/**
	 * This method computes devignette on an given image. If the source image
	 * pointer is null this image instance is assumed. The
	 *
	 * @param xPos The x center position to devignette.
	 * @param yPos The y center position to devignette.
	 * @param scale The scale factor for the radius.
	 * @param sigma The sigma value of the Gauss bell.
	 * @param niveau The general niveau to subtract.
	 * @param slope The left to right slope to form.
	 * @param srcTx The source image.
	 * @return True on success.
	 */
	b3_bool        b3TxGauss    (
		b3_coord xPos,b3_coord yPos,
		b3_f64   scale,b3_f64 sigma,b3_f64 niveau,b3_f64 slope,
		b3Tx        *srcTx = null);

	/**
	 * This method filters each color component seperately. If the source
	 * image pointer is null this image is taken as source.
	 *
	 * @param redFilter The red filter value.
	 * @param greenFilter The green filter value.
	 * @param blueFilter The blue filter value.
	 * @param srcTx The source image.
	 * @return True on success.
	 */
	b3_bool        b3TxColorFilter(
		b3_f64       redFilter,
		b3_f64       greenFilter,
		b3_f64       blueFilter,
		b3Tx        *srcTx = null);

	/**
	 * This method controls brightness, constrast and gamma of the given source
	 * image. If the source image pointer is null this image instance is assumed.
	 *
	 * @param highlight The upper bound
	 * @param shadow The lower bound
	 * @param gamma the gamma curve between the bound values.
	 * @param srcTx The source image.
	 * @return True on success.
	 */
	b3_bool        b3TxContrast(
		b3_f64        highlight,
		b3_f64        shadow,
		b3_f64        gamma,
		b3Tx        *srcTx = null);

	/**
	 * This method converts the three color channels seperately using the source
	 * channel as index for lookup in the appropriate lookup table.
	 * If the source image pointer is null this image instance is assumed.
	 *
	 * @param rTable Red lookup table.
	 * @param gTable Green lookup table.
	 * @param bTable Blue lookup table.
	 * @param srcTx The source image.
	 * @return True on success.
	 * @bug This method is only implemented for B3_TX_RGB8 images as source and
	 *      destination. No type checking is performed yet.
	 */
	b3_bool        b3TxTransformTable(
		b3_pkd_color *rTable,
		b3_pkd_color *gTable,
		b3_pkd_color *bTable,
		b3Tx         *srcTx = null);

	/**
	 * This method converts the source image into a palettized destination image.
	 * The source image must be a true color image. Both images must have the same
	 * resolution. The destination image must be a palettized image with an initialized
	 * palette. All true color pixel values from the source image are converted into
	 * the best fitting palette index entries in the destination image.
	 *
	 * @param src The source image.
	 * @return True on successful reduce operation.
	 */
	b3_bool        b3TxReduce(b3Tx *src);

	// b3TxHist.cc
	/**
	 * This method computes a complete histogramme of the image. This
	 * method works only on images of type B3_TX_ILBM, B3_TX_VGA and
	 * B3_TX_RGB8.
	 *
	 * @throws b3TxException on out of memory.
	 * @return True on success.
	 */
	b3_bool        b3Histogramme();

	/**
	 * This method initializes the histogramme.
	 *
	 * @return True on success.
	 */
	b3_bool        b3StartHist  ();

	/**
	 * This method closes the histogramme counting.
	 */
	void           b3EndHist    ();

	/**
	 * This method computes the history for the given image area. This
	 * method works only on images of type B3_TX_ILBM, B3_TX_VGA and
	 * B3_TX_RGB8.
	 *
	 * @param xStart The lower x coordinate.
	 * @param yStart The lower y coordinete.
	 * @param xStop The upper x coordinate.
	 * @param yStop The upper y coordinate.
	 * @return True on success.
	 */
	b3_bool        b3AddHist    (
		b3_coord xStart,b3_coord yStart,
		b3_coord xStop, b3_coord yStop);

	/**
	 * This method returns true if most of the image is white. The ratio is given
	 * with the b3SetWhiteRatio() method.
	 *
	 * @return True if the image is mostly white.
	 * @see b3SetWhiteRatio().
	 */
	b3_bool        b3IsWhite    ();

	/**
	 * This method sets the white level for white iamge computation. This
	 * level is compared against the quotient of the black parts of the
	 * image and the whole pixel count.
	 *
	 * @param newRatio The new white level.
	 * @see b3IsWhite().
	 */
	void           b3SetWhiteRatio(b3_f64 newRatio);

	/**
	 * This method returns the histogramme data. The data is computed via b3Histogramme() or
	 * b3AddHistogramme() methods.
	 *
	 * @param buffer  The resulting histogramme buffer.
	 * @param entries The resulting number of histogramme entries.
	 * @return True on success.
	 */
	b3_bool        b3GetHistogramme  (b3_count *buffer, b3_count &entries);

	/**
	 * This method transforms the image to a B/W image. The given threshold
	 * divides the index space into the black and white half.
	 *
	 * @param threshold The separation threshold.
	 * @return True on success.
	 */
	b3_bool        b3TransToBW       (b3_index  threshold=128);

	/**
	 * This method transforms a given source image into a B/W image using a specified
	 * transformation mode. If the mode is B3_THRESHOLD_WHITE_PAGE the white page
	 * ratio set via the b3SetWhiteRatio() method is used to separate.  If
	 * B3_THRESHOLD_USE is used as mode the given ratio is used as custom value.
	 * IfB3_THRESHOLD_EQUALIZE is used the separation level is determined via the
	 * internal histogramme where as much black pixel exist as white pixel.
	 *
	 * @param srcTx The source image.
	 * @param ratio The relative separation level.
	 * @param mode The separation mode.
	 * @return True on success.
	 * @see b3_tx_threshold.
	 */
	b3_bool        b3TransToBW       (b3Tx *srcTx,
									  b3_f64 ratio=0.5,b3_tx_threshold mode = B3_THRESHOLD_USE);

	/**
	 * This method transforms a this image into a B/W image using a specified
	 * transformation mode. If the mode is B3_THRESHOLD_WHITE_PAGE the white page
	 * ratio set via the b3SetWhiteRatio() method is used to separate.  If
	 * B3_THRESHOLD_USE is used as mode the given ratio is used as custom value.
	 * IfB3_THRESHOLD_EQUALIZE is used the separation level is determined via the
	 * internal histogramme where as much black pixel exist as white pixel.
	 *
	 * @param ratio The relative separation level.
	 * @param mode The separation mode.
	 * @return True on success.
	 * @see b3_tx_threshold.
	 */
	b3_bool        b3TransToBW       (
		b3_f64 ratio=0.5,b3_tx_threshold mode = B3_THRESHOLD_USE);

	/**
	 * This method converts the required ratio and threshold mode into a direct
	 * color index. Values below this threshold index are black the other are
	 * white.
	 *
	 * @note The threshold ratio and the resulting color index are not the same!
	 * @param ratio The relative separation level.
	 * @param mode The separation mode.
	 * @return The computed color index as threshold.
	 */
	b3_index       b3ComputeThreshold(b3_f64 ratio,b3_tx_threshold mode);

	/**
	 * This method determines the file type depending on the given extension.
	 *
	 * @param extension The image file extension.
	 * @return The resulting file type.
	 * @see b3_tx_filetype
	 */
	static       b3_tx_filetype  b3GetFileType(const char *extension);

	/**
	 * This method returns the image file extension depending on the given file type.
	 *
	 * @param type The file type.
	 * @return The resulting image file extension
	 * @see b3_tx_filetype
	 */
	static const char           *b3GetExt(b3_tx_filetype type);

	/**
	 * This method returns the image file extension depending on the images file name.
	 *
	 * @return The resulting image file extension
	 * @see b3_tx_filetype
	 */
	const char                  *b3GetExt();

	/**
	 * This method loads an image from the given image file. The file type is determined
	 * automatically.
	 *
	 * @param ImageName The file name of the image.
	 * @param throwException Flags if an exception should be thrown on error.
	 * @return The result code if no exception is thrown anyway.
	 * @see b3_result.
	 */
	b3_result                    b3LoadImage(const char *ImageName,b3_bool throwException = false);

	/**
	 * This method loads an image from the given buffer filled with image file data.
	 *
	 * @param buffer The buffer with the image file data.
	 * @param size The size of the buffer.
	 * @return The result code of image parsing.
	 * @see b3_result.
	 */
	b3_result                    b3LoadImage(b3_u08 *buffer,b3_size size);

	/**
	 * This method saves the image to the given file name. The file type is determined from
	 * the file name. The method returns an error if:
	 *
	 * - The image is empty.
	 * - The extension shows an unsupported file type.
	 * - A file IO error occured.
	 *
	 * @param filename The image file name.
	 * @return The result code of image saving.
	 * @see b3_result.
	 */
	b3_result                    b3SaveImage(const char *filename);

	// b3TxScale.cc
	/**
	 * This method transforms the image to a grey image.
	 */
	void                         b3TransToGrey();

	/**
	 * This method scales the given source image to the dimension of this image.
	 * The image is filtered if the destination size is smaller than the source size.
	 * It is possible to enlarge in one direction and to make smaller in the other.
	 * The destination size and format must be initialized in this instance.
	 *
	 * @param srcTx The source image.
	 */
	void                         b3ScaleToGrey(b3Tx *srcTx);

	/**
	 * This method samples the given source image to the dimension of this image.
	 * It is possible to enlarge in one direction and to make smaller in the other.
	 * The destination size and format must be initialized in this instance.
	 *
	 * @param srcTx The source image.
	 */
	void                         b3Scale      (b3Tx *srcTx);

	// b3TxMirror.cc
	/**
	 * This method mirrors the image horizontally. The image is flipped
	 * along the y axis in the middle of the image.
	 */
	void                         b3MirrorHorizontal();

	/**
	 * This method mirrors the image vertically. The image is flipped
	 * along the x axis in the middle of the image.
	 */
	void                         b3MirrorVertical();

	// b3TxDeskew.cc
	/**
	 * This method deskews the B/W image.
	 *
	 * @warning This method is not implemented yet.
	 */
	void                         b3Deskew     ();

	// b3TxSaveTIFF.cc, b3TxLoadTIFF.cc

	/**
	 * This method saves the image as TIFF file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result      b3SaveTIFF (const char *ImageName);

	// b3TxSaveXXX.cc

	/**
	 * This method saves the image as JPEG file.
	 *
	 * @param ImageName The file name to save into.
	 * @param quality The JPEG quality level.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result      b3SaveJPEG (const char *ImageName,b3_u32 quality = B3_JPG_QUALITY);

	/**
	 * This method saves the image as PostScript file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result      b3SavePS   (const char *ImageName);

	/**
	 * This method saves the image as OpenEXR file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result      b3SaveEXR  (const char *ImageName);

	/**
	 * This method saves the image as RGB8 file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result      b3SaveRGB8 (const char *ImageName);

	/**
	 * This method saves the image as TGA file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result      b3SaveTGA  (const char *ImageName);

private:
	// b3TxScale.cc
	static unsigned int b3ScaleBW2BW(void *ptr);
	static unsigned int b3ScaleBW2Grey(void *ptr);
	static unsigned int b3RGB8ScaleToRGB8(void *ptr);
	static unsigned int b3FloatScaleToRGB8(void *ptr);

	static void         b3FloatComputeLineSmaller(
		b3_color *TxRowCounter,
		b3_count *TxRowCells,
		b3_count *rIndex,
		b3_color *src,
		b3_res    xDstSize);
	static void         b3FloatComputeLineBigger(
		b3_color *TxRowCounter,
		b3_count *TxRowCells,
		b3_count *rIndex,
		b3_color *src,
		b3_res    xDstSize);

	static void         b3RGB8ComputeLineSmaller(
		b3_count     *TxRowCounter,
		b3_count     *TxRowCells,
		b3_count     *rIndex,
		b3_pkd_color *src,
		b3_res        xDstSize);
	static void         b3RGB8ComputeLineBigger(
		b3_count     *TxRowCounter,
		b3_count     *TxRowCells,
		b3_count     *rIndex,
		b3_pkd_color *src,
		b3_res        xDstSize);

	static void         b3ComputeLineSmaller(
		b3_count *TxRowCounter,
		b3_count *TxRowCells,
		b3_count *rIndex,
		b3_u08   *src,
		b3_res    xDstSize);
	static void         b3ComputeLineBigger(
		b3_count *TxRowCounter,
		b3_count *TxRowCells,
		b3_count *rIndex,
		b3_u08   *src,
		b3_res    xDstSize);

private:
	/**
	 * This method returns the image data.
	 *
	 * @return The image data.
	 */
	inline void *b3GetVoidData()
	{
		return data;
	}

	// b3TxTurn.cc
	void           b3TurnLeftILBM();
	void           b3TurnLeftVGA();
	void           b3TurnLeftRGB4();
	void           b3TurnLeftRGB8();
	void           b3TurnLeftFloat();

	// b3TxTurn.cc
	void           b3TurnRightILBM();
	void           b3TurnRightVGA();
	void           b3TurnRightRGB4();
	void           b3TurnRightRGB8();
	void           b3TurnRightFloat();

	// b3TxScale.cc
	void           b3ScaleFilteredFromBW (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleFilteredFromVGA  (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3VGAScaleToVGA   (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3VGAScaleToRGB8  (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleFilteredFromColor(b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleFilteredFromFloat(b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);

	void           b3ScaleUnfilteredFromBW       (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleUnfilteredFromILBM     (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleUnfilteredFromVGA      (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleUnfilteredFromColor    (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ScaleUnfilteredFromFloat    (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	b3_index       b3ILBMPlaneValue  (b3_coord x,b3_coord y);

	// b3Tx.cc
	b3_pkd_color   b3ILBMValue (b3_coord x,b3_coord y);
	b3_pkd_color   b3RGB4Value (b3_coord x,b3_coord y);
	b3_pkd_color   b3RGB8Value (b3_coord x,b3_coord y);
	b3_pkd_color   b3VGAValue  (b3_coord x,b3_coord y);
	b3_pkd_color   b3FloatValue(b3_coord x,b3_coord y);

	// b3Tx.cc
	void           b3CopyILBMtoVGA  (b3_u08       *row, b3_coord y);
	void           b3CopyILBMtoRGB8 (b3_pkd_color *row, b3_coord y);
	void           b3CopyILBMtoFloat(b3_color     *row, b3_coord y);

	// b3Tx.cc
	void           b3GetILBM  (b3_pkd_color *row,b3_coord y);
	void           b3GetRGB4  (b3_pkd_color *row,b3_coord y);
	void           b3GetRGB8  (b3_pkd_color *row,b3_coord y);
	void           b3GetVGA   (b3_pkd_color *row,b3_coord y);
	void           b3GetFloat (b3_pkd_color *row,b3_coord y);

	// b3TxImage.cc
	b3_count       b3BuildRLE (b3_count *row,b3_u08 *rle);
	void           b3BuildRow (b3_count *row,b3_u08 *rle,b3_count codeNum,b3_count byteNum);
	static b3_f64  b3Gamma(b3_f64 h,b3_f64 s,b3_f64 gamma,b3_f64 value,b3_f64 scale=1.0);

	// b3TxLoadTIFF.cc
	b3_result      b3LoadTIFF (const char *ImageName);
	b3_result      b3LoadTIFF (const char *ImageName,
							   const b3_u08  *ImageBuffer,
							   const b3_size  BufferSize);
	long           b3TIFFPalette(TIFF *handle,short PaletteMode);
	long           b3TIFFDecode (TIFF *handle,short PlanarConfig);
	long           b3TIFFAlloc  ();

	// b3TxSaveTIFF.cc
	void           b3GetSampleValues  (long &BitsPerPixel,long &SamplesPerPixel);
	b3_result      b3SaveTIFFFax      (TIFF *handle);
	b3_result      b3SaveTIFFPalette  (TIFF *handle);
	b3_result      b3SaveTIFFTrueColor(TIFF *handle);
	b3_result      b3SaveTIFFRealColor(TIFF *handle);

	static void    b3TIFFErrorHandler(const char *module,const char *fmt,va_list args);
	static void    b3TIFFWarnHandler( const char *module,const char *fmt,va_list args);
	static tsize_t b3ReadProc( thandle_t fd, tdata_t buf, tsize_t size);
	static tsize_t b3WriteProc(thandle_t fd, tdata_t buf, tsize_t size);
	static toff_t  b3SeekProc( thandle_t fd, toff_t off, int whence);
	static int     b3CloseProc(thandle_t fd);
	static toff_t  b3SizeProc( thandle_t fd);
	static int     b3MMapProc( thandle_t fd, tdata_t* pbase, toff_t* psize);
	static void    b3UnmapProc(thandle_t fd, tdata_t base, toff_t size);

	// b3TxScale.cc (color indexing)
	void           b3ColorGrid();
	b3_index       b3ColorIndex(b3_pkd_color color);

	// b3TxDeskew.cc (deskew page)
	void           b3DeskewILBM();
	void           b3DeskewRGB4();
	void           b3DeskewRGB8();
	void           b3DeskewVGA();

	// b3TxEasy.cc
	b3_result      b3ParseRAW (b3_u08 *buffer,b3_res x,b3_res y,b3_s32 type);
	b3_result      b3ParseBMP (b3_u08 *buffer);
	b3_result      b3ParseMTV (b3_u08 *buffer);
	b3_result      b3ParseBMF (b3_u08 *buffer,b3_size buffer_size);

	// b3TxIFF.cc
	b3_result      b3ParseIFF_ILBM (b3_u08 *buffer,b3_size buffer_size);
	b3_result      b3ParseIFF_RGB8 (b3_u08 *buffer,b3_size buffer_size);
	b3_result      b3ParseIFF_RGB4 (b3_u08 *buffer,b3_size buffer_size);
	b3_result      b3ParseIFF_YUVN (b3_u08 *buffer,b3_size buffer_size);
	void           b3EHBPalette();
	void           b3ConvertILBMLine (b3_u08 *Line,b3_u08 *Interleave,b3_res xMax,b3_count Planes);
	void           b3HamPalette (b3_bool HAM8);

	static b3_u32  b3ShiftCount(b3_count Count);

	// b3TxGIF.cc
	b3_result      b3ParseGIF  (b3_u08 *buffer);

#ifdef BLZ3_USE_OPENEXR
	// b3TxEXR.cc
	b3_result      b3ParseOpenEXR  (b3_u08 *buffer, b3_size size);
#endif

	// b3TxPCX.cc
	b3_result      b3ParsePCX4 (b3_u08 *buffer);
	b3_result      b3ParsePCX8 (b3_u08 *buffer);

	// b3TxIMG.cc
	b3_result      b3ParseSGI  (b3_u08 *buffer);
	void           b3ParseSGI3 (HeaderSGI *HeaderSGI,b3_u08 *Data);

	static void    b3ConvertSGILine(b3_u16 *buffer,b3_offset offset,b3_size size,b3_count bytes);
	static void    b3UnpackSGI (b3_u08 *buffer,void *inPtr,b3_count count,b3_count bytes,b3_offset offset);

	// b3TxTGA.cc
	b3_result      b3ParseTGA  (b3_u08 *buffer);

	// b3TxJPG.cc
	b3_result      b3ParseJPEG (b3_u08 *buffer,b3_size buffer_size);
};


#endif
