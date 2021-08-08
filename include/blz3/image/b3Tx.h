/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2011, 2016 $
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

#pragma once

#ifndef B3_IMAGE_TX_H
#define B3_IMAGE_TX_H

#include "blz3/b3Config.h"
#include "blz3/system/b3FileAbstract.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3List.h"

#include <functional>

#ifdef HAVE_LIBTIFF
#	include "tiff.h"
#	include "tiffio.h"
#endif

/*************************************************************************
**                                                                      **
**                        Some useful defines                           **
**                                                                      **
*************************************************************************/

/** This macro returns a byte aligned byte offset computed from the x coordinate. */
#define TX_BBA(x) ((((x) +  7) & 0xfffffff8) >> 3)

/** This macro returns a word aligned byte offset computed from the x coordinate. */
#define TX_BWA(x) ((((x) + 15) & 0xfffffff0) >> 3)

/** This macro returns a long aligned byte offset computed from the x coordinate. */
#define TX_BLA(x) ((((x) + 31) & 0xffffffe0) >> 3)

#define TX_VBA(x)   (x)
#define TX_VWA(x) (((x) +  1) & 0xfffffffe)
#define TX_VLA(x) (((x) +  3) & 0xfffffffc)

#define B3_MAX_GRID (1 << (2 + 2 + 2))
#define b3ColorGridIndex(color) (\
	(((color) & 0xc00000) >> 18) |\
	(((color) & 0x00c000) >> 12) |\
	(((color) & 0x0000c0) >>  6))

#define B3_JPG_QUALITY       85
#define TX_CPU_MAX            8

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
	B3_TX_TYPE_START = 0, //!< The first type index for iterating types.
	B3_TX_ILBM      =  B3_TX_TYPE_START, //!< BW image with 16 bit alignment for each scan line (b3_u16).
	B3_TX_VGA       =  1, //!< Palette image with unsigned byte indices (b3_u08) and b3_pkd_color palette.
	B3_TX_RGB4      =  2, //!< Color channels in true color ARGB nibbles (b3_u16).
	B3_TX_RGB8      =  3, //!< Color channels in true color AARRGGBB nibbles (b3_pkd_color).
	B3_TX_FLOAT     =  4, //!< Color channels in floating point true color ARGB (b3_color).
	B3_TX_TYPE_COUNT      //!< The different type count.
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
	FT_EXR,                 //!< OpenEXR (high dynamice range).
	FT_PNG                  //!< PNG.
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
	B3_TX_ILLEGAL_DATATYPE,   //!< Internal datatype use illegal.
	B3_TX_WRONG_SIZE,         //!< The data size is not correct.
	B3_TX_STREAMING_ERROR     //!< Error occured while video encoding.
};

typedef b3Exception<b3_tx_error, 0x5458> b3TxException;

/*************************************************************************
**                                                                      **
**                        Image helper classes                          **
**                                                                      **
*************************************************************************/

/**
 * This class initializes a lookup table with squared values.
 */
class B3_PLUGIN b3TxQuad
{
	b3_pkd_color quad256[512];

	b3TxQuad()
	{
		b3_loop i;

		for (i = 0; i < 256; i++)
		{
			quad256[256 - i] = quad256[256 + i] = i * i;
		}
	}

	friend class b3ColorIndices;
};

/**
 * This class holds image palette indices.
 */
class B3_PLUGIN b3ColorIndices
{
	static b3TxQuad          m_TxQuad;

	static const b3_count    m_Max = 256;
	b3_count                 m_Num = 0;
	b3_index                 m_Indices[m_Max];

public:
	b3ColorIndices() = default;

	void            b3AddColorIndex(const b3_index index);
	b3_index        b3ColorIndex(
		const b3_pkd_color *,
		const b3_pkd_color) const;
};

#define CLASS_TEXTURE       0x20000000
#define TYPE_USUAL          0x00000001
#define USUAL_TEXTURE       (CLASS_TEXTURE|TYPE_USUAL)

/*************************************************************************
**                                                                      **
**                        The image variant data representation         **
**                                                                      **
*************************************************************************/

union b3_tx_data
{
private:
	void     *     m_Void;
	b3_u08    *    m_Bytes;
	b3_u16    *    m_Words;
	b3_pkd_color * m_Colors;
	b3_color   *   m_Floats;

public:
	inline b3_tx_data() noexcept
	{
		m_Void = nullptr;
	}

	inline b3_tx_data(b3_u08 * ptr) : m_Bytes(ptr)
	{
	}

	inline b3_tx_data(b3_u16 * ptr) : m_Words(ptr)
	{
	}

	inline b3_tx_data(b3_pkd_color * ptr) : m_Colors(ptr)
	{
	}

	inline b3_tx_data(b3_color * ptr) : m_Floats(ptr)
	{
	}

	inline operator b3_u08 * () const noexcept
	{
		return m_Bytes;
	}

	inline operator b3_u16 * () const noexcept
	{
		return m_Words;
	}

	inline operator b3_pkd_color * () const noexcept
	{
		return m_Colors;
	}

	inline operator b3_color * () const noexcept
	{
		return m_Floats;
	}

	inline operator void * () const noexcept
	{
		return m_Void;
	}

	inline b3_tx_data operator+(const b3_count sum) const
	{
		return b3_tx_data(m_Bytes + sum);
	}

	inline b3_tx_data operator++(int) noexcept
	{
		b3_tx_data actual(m_Bytes);
		m_Bytes++;
		return actual;
	}

	inline b3_u08 & operator*() const noexcept
	{
		return m_Bytes[0];
	}

	inline b3_u08 & operator[](const b3_count index) const noexcept
	{
		return m_Bytes[index];
	}

	inline bool operator== (const void * ptr) const noexcept
	{
		return m_Void == ptr;
	}

	inline bool operator!= (const void * ptr) const noexcept
	{
		return m_Void != ptr;
	}

	inline b3_tx_data & operator =(void * ptr) noexcept
	{
		m_Void = ptr;
		return *this;
	}
};

/*************************************************************************
**                                                                      **
**                        The image class itself!                       **
**                                                                      **
*************************************************************************/

class b3TxAlgorithms;
class b3TxExif;
class b3HeaderSGI;

/**
 * This big class represents one image in its best representation.
 * It has many convenience methods for manipulating the image.
 */
class B3_PLUGIN b3Tx : public b3Link<b3Tx>, public b3Mem
{
	static const b3Color m_RgbEyeStimulus;
	static const b3_u08  m_Bits[8];
	static const b3_u08  m_RightMaskLeftByte[16];
	static const b3_u08  m_RightMaskRightByte[16];
	static const b3_u08  m_RightBorder[];
	static       bool    m_ErrorHandlerInstalled;

private:
	b3Path            image_name;
	b3_pkd_color   *  palette     = nullptr;
	b3_count     *    histogramme = nullptr;
	b3ColorIndices  * grid        = nullptr;
	b3_tx_data        data;
	b3_f64            white_ratio;
	b3_size           dSize = 0, pSize = 0;
	b3_tx_type        type     = B3_TX_UNDEFINED;
	b3_tx_filetype    FileType = FT_UNKNOWN;

	static const b3_count B3_TX_MAX_HISTGRM_DEPTH  = 8;
	static const b3_count B3_TX_MAX_HISTGRM        = (1 << B3_TX_MAX_HISTGRM_DEPTH);

public:
	b3_res            xSize = 0;   //!< The image width;
	b3_res            ySize = 0;   //!< The image height;
	b3_res            depth = 0;   //!< The image depth in bit;
	b3_res            xDPI = 72;   //!< The pixel density in x direction.
	b3_res            yDPI = 72;   //!< The pixel density in y direction.
	b3_count          ScanLines;   //!< Number of row to process during scanning.

	/**
	 * This constructor initializes an empty image.
	 */
	b3Tx();

	/**
	 * This constructor copies the given image as initialization.
	 *
	 * @param srcTx The source image.
	 * @see b3Copy()
	 */
	explicit b3Tx(b3Tx * srcTx);

	/**
	 * This destructor deinitializes the image.
	 */
	virtual       ~b3Tx();

	/**
	 * This method copies the data of the given source image.
	 *
	 * @param srcTx The source image to copy.
	 */
	void           b3Copy(const b3Tx * srcTx);

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
	bool           b3AllocTx(const b3_res xSize, const b3_res ySize, const b3_res depth);

	/**
	 * This method frees all image data.
	 */
	void           b3FreeTx();

	/**
	 * This method fills in a test pattern into the pre allocated image buffer.
	 *
	 * @see b3AllocTx()
	 */
	void           b3TestPattern();

	/**
	 * This method returns the palette data if any.
	 *
	 * @return The palette data.
	 */
	[[nodiscard]]
	b3_pkd_color * b3GetPalette() const noexcept
	{
		return palette;
	}

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
	void           b3SetPalette(
		const b3_pkd_color * entries,
		const b3_count       numEntries);

	/**
	 * This method returns the image as palette index data pointer.
	 *
	 * @return The image palette index data.
	 */
	inline b3_u08 * b3GetIndexData() const
	{
		if (data == nullptr)
		{
			return nullptr;
		}
		if ((!b3IsPalette()) || (depth > 8))
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return data;
	}

	/**
	 * This method returns the image as palette index data pointer.
	 *
	 * @return The image palette index data.
	 */
	inline b3_u16 * b3GetHighColorData() const
	{
		if (data == nullptr)
		{
			return nullptr;
		}
		if (!b3IsHighColor())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return data;
	}

	/**
	 * This method returns the image as true color data pointer.
	 *
	 * @return The image true color data.
	 */
	inline b3_pkd_color * b3GetTrueColorData() const
	{
		if (data == nullptr)
		{
			return nullptr;
		}
		if (!b3IsTrueColor())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return data;
	}

	/**
	 * This method returns the image as HDR data pointer.
	 *
	 * @return The image HDR data pointer.
	 */
	inline b3_color * b3GetHdrData() const
	{
		if (data == nullptr)
		{
			return nullptr;
		}
		if (!b3IsHdr())
		{
			B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
		}
		return data;
	}

	/**
	 * This method sets the new image data. The old data is
	 * deallocated.
	 *
	 * @param newData The new image data.
	 * @param size The data size for verification against the internal dSize field.
	 */
	void           b3SetData(const b3_tx_data newData, const b3_size size);

	/**
	 * This method returns the file name from which the image was loaded. If the
	 * image data was allocated via the b3AllocTx() method this file name is empty.
	 *
	 * @return The file name of the image.
	 */
	[[nodiscard]]
	const char  *  b3Name() const noexcept
	{
		return image_name;
	}

	/**
	 * This method sets a new file name.
	 *
	 * @param ImageName The new file name of the image.
	 */
	void           b3Name(const char * ImageName);

	/**
	 * This method returns the pixel density.
	 *
	 * @param xDPI The resulting horizontal pixel density.
	 * @param yDPI The resulting vertical pixel density.
	 */
	void           b3GetResolution(b3_res & xDPI, b3_res & yDPI) const;

	/**
	 * This method fills a given row buffer with the hdr color of the
	 * given y coordinate. The row buffer must have enough entries for
	 * retrieving a full scane line with the width of the image.
	 *
	 * @param row The row buffer.
	 * @param y The y coordinate of the row.
	 */
	void           b3GetRow(b3_color * row, const b3_coord  y) const;

	/**
	 * This method fills a given row buffer with the true color of the
	 * given y coordinate. The row buffer must have enough entries for
	 * retrieving a full scane line with the width of the image.
	 *
	 * @param row The row buffer.
	 * @param y The y coordinate of the row.
	 */
	void           b3GetRow(b3_pkd_color * row, const b3_coord  y) const;

	/**
	 * This method returns a color value at the given coordinates.
	 *
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return The resulting color as b3_pkd_color value.
	 */
	b3_pkd_color   b3GetValue(const b3_coord x, const b3_coord  y) const;

	/**
	 * This method returns a color value at the given coordinates. The
	 * color may be in HDR format.
	 *
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @result The resulting color as HDR value.
	 */
	const b3Color b3GetHdrValue(const b3_coord x, const b3_coord  y) const;

	/**
	 * This method returns the blue color channel as floating point value.
	 * color may be in HDR format.
	 *
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return The resulting blue channel as floting point value.
	 */
	b3_f32 b3GetBlue(b3_coord x, b3_coord  y) const;

	/**
	 * This method returns true if the given pixel is transparent.
	 * In case B3_TX_ILBM and B3_TX_VGA the color index must be 0. In
	 * all other cases there must be an alpha channel greater than 0.
	 *
	 * @param x The x coordinate to test.
	 * @param y The y coordinate to test.
	 * @return True if the pixel is transparent.
	 */
	bool     b3IsBackground(const b3_coord x, const b3_coord  y) const;

	/**
	 * This method returns true if this instance holds an image.
	 *
	 * @return True if this instance holds an image.
	 */
	bool     b3IsLoaded() const noexcept;

	/**
	 * This method returns true if this image is black/white.
	 *
	 * @return True if this image is b/w.
	 */
	[[nodiscard]]
	inline bool    b3IsBW() const noexcept
	{
		return (depth == 1) && (type == B3_TX_ILBM);
	}

	/**
	 * This method returns true if this image is high color.
	 *
	 * @return True if this image is high color.
	 */
	[[nodiscard]]
	inline bool    b3IsHighColor() const noexcept
	{
		return ((depth == 12) || (depth == 16)) && (type == B3_TX_RGB4);
	}

	/**
	 * This method returns true if this image is true color.
	 *
	 * @return True if this image is true color.
	 */
	[[nodiscard]]
	inline bool    b3IsTrueColor() const noexcept
	{
		return (depth >= 24) && ((type == B3_TX_RGB8) || (type == B3_TX_FLOAT));
	}

	/**
	 * This method returns true if this image is a high dynamic range image.
	 *
	 * @return True if this image is a high dynamic range image.
	 */
	[[nodiscard]]
	inline bool    b3IsHdr() const noexcept
	{
		return (depth >= 96) && (type == B3_TX_FLOAT);
	}

	/**
	 * This method returns true if this image is palettized.
	 *
	 * @return True if this image is palettized.
	 */
	inline bool    b3IsPalette() const noexcept
	{
		return palette != nullptr;
	}

	/**
	 * This method returns true if this image is a palettized grey image.
	 *
	 * @return True if this image is a palettized grey image.
	 */
	[[nodiscard]]
	bool           b3IsGreyPalette() const noexcept;

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
		b3_u08    *   mask,
		b3_count      BytesPerRow,
		b3_pkd_color  colorMask) const ;

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
	void           b3Blit(const b3Tx * srcTx,
		b3_coord  xDstOff,     b3_coord yDstOff,
		b3_res    xMax,        b3_res   yMax,
		b3_coord  xSrcOff = 0, b3_coord ySrcOff = 0);

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
	void           b3Shrink(const b3_count shrink = 1);

	/**
	 * This method removes any black border on the left and right side of the image.
	 *
	 * @note This method works only on B3_TX_ILBM images.
	 */
	void           b3RemoveBlackBorder();

	/**
	 * This method negates the color.
	 */
	bool           b3Negate();

	/**
	 * This method turns left the image by 90 degrees.
	 */
	void           b3TurnLeft();

	/**
	 * This method turns the image upside down.
	 */
	void           b3Turn();

	/**
	 * This method turns right the image by 90 degrees.
	 */
	void           b3TurnRight();

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
	bool     b3TxGauss(const b3_coord xPos,
		b3_coord yPos,
		b3_f64   scale,
		b3_f64   sigma,
		b3_f64   niveau,
		b3_f64   slope,
		const b3Tx  *  srcTx = nullptr);

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
	bool     b3TxColorFilter(
		const b3_f32 redFilter,
		const b3_f32 greenFilter,
		const b3_f32 blueFilter,
		const b3Tx * srcTx = nullptr);

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
	bool     b3TxContrast(
		b3_f64       highlight,
		b3_f64       shadow,
		b3_f64       gamma,
		const b3Tx * srcTx = nullptr);

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
	bool     b3TxTransformTable(
		b3_pkd_color * rTable,
		b3_pkd_color * gTable,
		b3_pkd_color * bTable,
		const b3Tx  *  srcTx = nullptr);

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
	bool           b3TxReduce(const b3Tx * src);

	// b3TxHist.cc
	/**
	 * This method computes a complete histogramme of the image. This
	 * method works only on images of type B3_TX_ILBM, B3_TX_VGA and
	 * B3_TX_RGB8.
	 *
	 * @throws b3TxException on out of memory.
	 * @return True on success.
	 */
	bool           b3Histogramme();

	/**
	 * This method initializes the histogramme.
	 *
	 * @return True on success.
	 */
	bool           b3StartHist();

	/**
	 * This method closes the histogramme counting.
	 */
	void           b3EndHist();

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
	bool           b3AddHist(
		b3_coord xStart,
		b3_coord yStart,
		b3_coord xStop,
		b3_coord yStop);

	/**
	 * This method returns true if most of the image is white. The ratio is given
	 * with the b3SetWhiteRatio() method.
	 *
	 * @return True if the image is mostly white.
	 * @see b3SetWhiteRatio().
	 */
	bool           b3IsWhite() const;

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
	bool           b3GetHistogramme(b3_count * buffer, b3_count & entries) const;

	/**
	 * This method transforms the image to a B/W image. The given threshold
	 * divides the index space into the black and white half.
	 *
	 * @param threshold The separation threshold.
	 * @return True on success.
	 */
	bool           b3TransToBW(b3_index  threshold = 128);

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
	bool           b3TransToBW(
		const b3Tx * srcTx,
		const b3_f64 ratio = 0.5, const b3_tx_threshold mode = B3_THRESHOLD_USE);

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
	bool           b3TransToBW(
		const b3_f64          ratio = 0.5,
		const b3_tx_threshold mode  = B3_THRESHOLD_USE);

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
	b3_index       b3ComputeThreshold(
		const b3_f64          ratio,
		const b3_tx_threshold mode);

	/**
	 * This method determines the file type depending on the given extension.
	 *
	 * @param extension The image file extension.
	 * @return The resulting file type.
	 * @see b3_tx_filetype
	 */
	static       b3_tx_filetype  b3GetFileType(const char * extension);

	inline const b3Path & b3GetFilename() const
	{
		return image_name;
	}

	/**
	 * This method returns the image file extension depending on the given file type.
	 *
	 * @param type The file type.
	 * @return The resulting image file extension
	 * @see b3_tx_filetype
	 */
	static const char      *     b3GetExt(b3_tx_filetype type);

	/**
	 * This method returns the image file extension depending on the images file name.
	 *
	 * @return The resulting image file extension
	 * @see b3_tx_filetype
	 */
	const char         *         b3GetExt() const;

	/**
	 * This method loads an image from the given image file. The file type is determined
	 * automatically.
	 *
	 * @param ImageName The file name of the image.
	 * @param throwException Flags if an exception should be thrown on error.
	 * @return The result code if no exception is thrown anyway.
	 * @see b3_result.
	 */
	b3_result                    b3LoadImage(const char * ImageName, bool    throwException = false);

	/**
	 * This method loads an image from the given buffer filled with image file data.
	 *
	 * @param buffer The buffer with the image file data.
	 * @param size The size of the buffer.
	 * @return The result code of image parsing.
	 * @see b3_result.
	 */
	b3_result                    b3LoadImage(const b3_u08 * buffer, b3_size size);

	/**
	 * This method saves the image to the given file name. The file type is determined from
	 * the file name. The method returns an error if:
	 *
	 * - The image is empty.
	 * - The extension shows an unsupported file type.
	 * - A file IO error occured.
	 *
	 * @param filename The image file name.
	 * @param exif The optional EXIF description to write if it is not nullptr.
	 * @return The result code of image saving.
	 * @see b3_result.
	 */
	b3_result                    b3SaveImage(
		const char * filename,
		b3TxExif  *  exif = nullptr);

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
	void                         b3ScaleToGrey(b3Tx * srcTx);

	/**
	 * This method samples the given source image to the dimension of this image.
	 * It is possible to enlarge in one direction and to make smaller in the other.
	 * The destination size and format must be initialized in this instance.
	 *
	 * @param srcTx The source image.
	 */
	void                         b3Scale(b3Tx * srcTx);

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
	void                         b3Deskew();

	/**
	 * This method saves the image as TIFF file.
	 *
	 * @param ImageName The file name to save into.
	 * @param exif The optional EXIF description to write if it is not nullptr.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SaveTIFF(
		const char   *   ImageName,
		b3TxExif    *    exif = nullptr);

	/**
	 * This method saves the image as JPEG file.
	 *
	 * @param ImageName The file name to save into.
	 * @param quality The JPEG quality level.
	 * @param exif The optional EXIF description to write if it is not nullptr.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SaveJPEG(
		const char   *   ImageName,
		const b3_u32     quality = B3_JPG_QUALITY,
		b3TxExif    *    exif = nullptr);

	/**
	 * This method saves the image as PostScript file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SavePS(const char * ImageName);

#ifdef BLZ3_USE_OPENEXR
	/**
	 * This method saves the image as OpenEXR file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SaveEXR(const char * ImageName);
#endif

	/**
	 * This method saves the image as RGB8 file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SaveRGB8(const char * ImageName);

	/**
	 * This method saves the image as RGB8 file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SavePNG(const char * ImageName);

	/**
	 * This method saves the image as TGA file.
	 *
	 * @param ImageName The file name to save into.
	 * @return The result code.
	 * @see b3_result.
	 */
	b3_result b3SaveTGA(const char * ImageName);

	static inline b3_f32 b3ToGrey(const b3_pkd_color input)
	{
		b3Color grey(input);

		grey *= m_RgbEyeStimulus;

		// Set bit if over threshold
		return grey[b3Color::R] + grey[b3Color::G] + grey[b3Color::B];
	}

	static inline b3_f32 b3ToGrey(const b3Color & input)
	{
		b3Color grey = input * m_RgbEyeStimulus;

		// Set bit if over threshold
		return grey[b3Color::R] + grey[b3Color::G] + grey[b3Color::B];
	}

	static inline b3_index b3ColorToIndex(const b3_pkd_color color)
	{
		return
			((color & 0xe00000) >> 16) |
			((color & 0x00e000) >> 11) |
			((color & 0x0000c0) >>  6);
	}

	static inline b3_pkd_color b3IndexToColor(const b3_index index)
	{
		return
			((index & 0xe0) << 16) |
			((index & 0x1c) << 11) |
			((index & 0x03) <<  6);
	}

private:
	// b3TxScale.cc
	static bool         b3ScaleBW2BW(void * ptr);
	static bool         b3ScaleBW2Grey(void * ptr);
	static bool         b3RGB8ScaleToRGB8(void * ptr);
	static bool         b3FloatScaleToRGB8(void * ptr);

	static void         b3FloatComputeLineSmaller(
		b3_color    *    TxRowCounter,
		b3_count    *    TxRowCells,
		const b3_count * rIndex,
		const b3_color * src,
		const b3_res     xDstSize);
	static void         b3FloatComputeLineBigger(
		b3_color    *    TxRowCounter,
		b3_count    *    TxRowCells,
		const b3_count * rIndex,
		const b3_color * src,
		const b3_res     xDstSize);

	static void         b3RGB8ComputeLineSmaller(
		b3_count      *      TxRowCounter,
		b3_count      *      TxRowCells,
		const b3_count   *   rIndex,
		const b3_pkd_color * src,
		const b3_res         xDstSize);
	static void         b3RGB8ComputeLineBigger(
		b3_count      *      TxRowCounter,
		b3_count      *      TxRowCells,
		const b3_count   *   rIndex,
		const b3_pkd_color * src,
		const b3_res         xDstSize);

	static void         b3ComputeLineSmaller(
		b3_count    *    TxRowCounter,
		b3_count    *    TxRowCells,
		const b3_count * rIndex,
		const b3_u08  *  src,
		const b3_res     xDstSize);
	static void         b3ComputeLineBigger(
		b3_count    *    TxRowCounter,
		b3_count    *    TxRowCells,
		const b3_count * rIndex,
		const b3_u08  *  src,
		const b3_res     xDstSize);

private:
	/**
	 * This method returns the image data.
	 *
	 * @return The image data.
	 */
	inline b3_tx_data b3GetVoidData() const noexcept
	{
		return data;
	}

	template<typename T> inline T b3Get(const b3_coord x, const b3_coord y) const
	{
		const T * ptr = data;

		return ptr[y * xSize + x];
	}

	static inline b3_pkd_color b3Convert(const b3_u16 px) noexcept
	{
		b3_u16       mask    = 0xf000;
		b3_pkd_color result  = 0;

		while (mask != 0)
		{
			const b3_u16 nibble = px & mask;

			result  |= nibble;
			result <<= 4;
			result  |= nibble;
			mask   >>= 4;
		}
		return result;
	}

	static b3_res b3ClipBlit(
		b3_coord  &  src_offset,
		b3_coord  &  dst_offset,
		const b3_res src_size,
		const b3_res dst_size,
		const b3_res blit_size);

	// b3TxImage.cc
	void           b3TurnLeftILBM();
	void           b3TurnRightILBM();

	// b3TxScale.cc
	void           b3ScaleFilteredFromBW(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleFilteredFromVGA(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3VGAScaleToVGA(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3VGAScaleToRGB8(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleFilteredFromTrueColor(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleFilteredFromFloat(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);

	void           b3ScaleUnfilteredFromBW(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleUnfilteredFromILBM(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleUnfilteredFromVGA(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleUnfilteredFromHighColor(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleUnfilteredFromTrueColor(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	void           b3ScaleUnfilteredFromFloat(
		const b3Tx   *   srcTx,
		const b3_index * rIndex,
		const b3_index * cIndex);
	b3_index       b3ILBMPlaneValue(const b3_coord x, const b3_coord y) const;

	// b3Tx.cc
	b3_pkd_color   b3ILBMValue(const b3_coord x, const b3_coord y) const;

	// b3Tx.cc
	void           b3CopyILBMtoVGA(b3_u08     *    row, b3_coord y) const;
	void           b3CopyILBMtoRGB8(b3_pkd_color * row, b3_coord y) const;
	void           b3CopyILBMtoFloat(b3_color   *  row, b3_coord y) const;

	// b3Tx.cc
	void           b3GetILBM(b3_pkd_color * row, b3_coord y) const;

	// b3TxImage.cc
	b3_count       b3BuildRLE(b3_count * row, b3_u08 * rle);
	void           b3BuildRow(b3_count * row, b3_u08 * rle, b3_count codeNum, b3_count byteNum);
	static b3_f64  b3Gamma(b3_f64 h, b3_f64 s, b3_f64 gamma, b3_f64 value, b3_f64 scale = 1.0);

#ifdef HAVE_LIBTIFF
	// b3TxLoadTIFF.cc
	b3_result      b3LoadTIFF(const char * ImageName);
	b3_result      b3LoadTIFF(const char * ImageName,
		const b3_u08 * ImageBuffer,
		const b3_size  BufferSize);
	long           b3TIFFPalette(TIFF * handle, short PaletteMode);
	long           b3TIFFDecode(TIFF * handle, short PlanarConfig);
	long           b3TIFFAlloc();

	// b3TxSaveTIFF.cc
	b3_result      b3SaveTIFFFax(TIFF * handle);
	b3_result      b3SaveTIFFPalette(TIFF * handle);
	b3_result      b3SaveTIFFTrueColor(TIFF * handle);
	b3_result      b3SaveTIFFRealColor(TIFF * handle);

	static void    b3TIFFErrorHandler(const char * module, const char * fmt, va_list args);
	static void    b3TIFFWarnHandler(const char * module, const char * fmt, va_list args);
	static tsize_t b3ReadProc(thandle_t fd, tdata_t buf, tsize_t size);
	static tsize_t b3WriteProc(thandle_t fd, tdata_t buf, tsize_t size);
	static toff_t  b3SeekProc(thandle_t fd, toff_t off, int whence);
	static int     b3CloseProc(thandle_t fd);
	static toff_t  b3SizeProc(thandle_t fd);
	static int     b3MMapProc(thandle_t fd, tdata_t * pbase, toff_t * psize);
	static void    b3UnmapProc(thandle_t fd, tdata_t base, toff_t size);
#endif

	// b3TxScale.cc (color indexing)
	void           b3ColorGrid();
	b3_index       b3ColorIndex(b3_pkd_color color);

	// b3TxEasy.cc
	b3_result      b3ParseRAW(const b3_u08 * buffer, b3_res x, b3_res y, b3_s32 type);
	b3_result      b3ParseBMP(const b3_u08 * buffer);
	b3_result      b3ParseMTV(const b3_u08 * buffer);
	b3_result      b3ParseBMF(const b3_u08 * buffer, b3_size buffer_size);

	// b3TxIFF.cc
	b3_result      b3ParseIFF_ILBM(const b3_u08 * buffer, b3_size buffer_size);
	b3_result      b3ParseIFF_RGB8(const b3_u08 * buffer, b3_size buffer_size);
	b3_result      b3ParseIFF_RGB4(const b3_u08 * buffer, b3_size buffer_size);
	b3_result      b3ParseIFF_YUVN(const b3_u08 * buffer, b3_size buffer_size);
	void           b3EHBPalette();
	void           b3ConvertILBMLine(b3_u08 * Line, b3_u08 * Interleave, b3_res xMax, b3_count Planes);
	void           b3HamPalette(bool    HAM8);

	static b3_u32  b3ShiftCount(b3_count Count);

	// b3TxGIF.cc
	b3_result      b3ParseGIF(const b3_u08 * buffer);

#ifdef BLZ3_USE_OPENEXR
	// b3TxEXR.cc
	b3_result      b3ParseOpenEXR(b3_u08 * buffer, b3_size size);
#endif

	// b3TxPCX.cc
	b3_result      b3ParsePCX4(const b3_u08 * buffer);
	b3_result      b3ParsePCX8(const b3_u08 * buffer);

	// b3TxIMG.cc
	b3_result      b3ParseSGI(const b3_u08 * buffer);
	void           b3ParseSGI3(const b3HeaderSGI * HeaderSGI, const b3_u08 * Data);

	static void    b3ConvertSGILine(b3_u16 * buffer, b3_offset offset, b3_size size, b3_count bytes);
	static void    b3UnpackSGI(
		b3_u08     *    buffer,
		const void   *  inPtr,
		b3_count        count,
		b3_count        bytes,
		const b3_offset offset);

	// b3TxTGA.cc
	b3_result      b3ParseTGA(const b3_u08 * buffer);

	// b3TxJPG.cc
	b3_result      b3ParseJPEG(const b3_u08 * buffer, b3_size buffer_size);

	// b3TxPNG.cc
	b3_result      b3ParsePNG(const b3_u08 * buffer, b3_size buffer_size);

	friend class b3TxAlgorithms;
};

#endif
