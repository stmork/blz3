/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (proto types)
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGE_TX_H
#define B3_IMAGE_TX_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"

#include "tiff.h"
#include "tiffio.h"

enum b3_tx_type
{
	B3_TX_UNDEFINED = -1,
	B3_TX_ILBM      =  0,
	B3_TX_VGA       =  1,
	B3_TX_RGB4      =  2,
	B3_TX_RGB8      =  3
};

/*************************************************************************
**                                                                      **
**                        error codes or original file format           **
**                                                                      **
*************************************************************************/

enum b3_tx_filetype
{
	FT_ERR_UNSUPP   =  -6,			/* errors, loading textures */
	FT_ERR_PACKING	=  -5,
	FT_ERR_OPEN		=  -4,
	FT_ERR_UNCOMPL	=  -3,
	FT_ERR_HEADER	=  -2,
	FT_ERR_MEM		=  -1,
	FT_UNKNOWN		=   1,
	FT_PCX4			=   2,			/* original file types */
	FT_PCX8			=   3,
	FT_ILBM			=   4,
	FT_ILBM_HAM		=   5,
	FT_ILBM_EHB		=   6,
	FT_ILBM_24		=   7,
	FT_RGB8			=   8,
	FT_RGB4			=   9,
	FT_MTV			=  10,
	FT_YUV			=  11,
	FT_TIFF			=  12,
	FT_ILBM_HAM8	=  13,
	FT_TGA          =  14,
	FT_GIF			=  15,
	FT_PPM6			=  16,	// old fashioned representation, now FT_PPM
	FT_BMP			=  17,
	FT_SGI_RLE		=  18,
	FT_PPM			=  16,
	FT_PGM			=  19,
	FT_PBM			=  20,
	FT_JPEG			=  21,
	FT_BMF          =  22
};

enum b3_tx_threshold
{
	B3_THRESHOLD_WHITE_RATIO = 0,
	B3_THRESHOLD_USE         = 1,
	B3_THRESHOLD_EQUALIZE    = 2
};

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

// Auxiliary class for color indexing (private use of class b3Tx)
class b3ColorIndices : public b3Mem
{
	b3_count       num;
	b3_count       max;
	b3_index      *indices;
public:
	          b3ColorIndices ();
	void      b3AddColorIndex(b3_index);
	b3_index  b3ColorIndex   (b3_pkd_color *,b3_pkd_color);
};

class b3TxPoint
{
public:
	b3_coord x,y;

	b3TxPoint(b3_coord new_x = 0,b3_coord new_y = 0)
	{
		x = new_x;
		y = new_y;
	}
};

#define B3_MEASURE_EDGE 3

class b3Measure
{
public:
	b3_bool   valid;
	b3TxPoint left[B3_MEASURE_EDGE];
	b3TxPoint top[B3_MEASURE_EDGE];
	b3TxPoint right[B3_MEASURE_EDGE];
	b3TxPoint bottom[B3_MEASURE_EDGE];

	b3TxPoint points[(B3_MEASURE_EDGE - 1) * 4];
	b3_count  num;
public:
	     b3Measure();
	void b3Init(b3_res xSize,b3_res ySize,b3_res depth);
	void b3Print();
};

typedef enum
{
	B3_TX_ERROR = -1,
	B3_TX_OK    =  0,
	B3_TX_MEMORY,
	B3_TX_NOT_FOUND,
	B3_TX_NOT_SAVED
} b3_tx_error;

class b3TxException
{
protected:
	b3_tx_error error;

public:
	b3TxException(b3_tx_error error)
	{
		this->error = error;
	}

	b3_tx_error b3GetError()
	{
		return error;
	}
};

// one single image and its methods
class b3Tx : public b3Link<b3Tx>, public b3Mem
{
private:           
	b3_pkd_color     *palette;
	b3_count         *histogramme;
	b3_u08           *data;
	b3_count          dSize,pSize;
	b3_tx_type        type;
	b3_tx_filetype    FileType;
	b3_f64            whiteRatio;
	b3Path            name;
	b3ColorIndices   *grid;
	b3Measure         measure;

public:
	b3_res         xSize,ySize,depth;
	b3_count       ScanLines;
	b3_res         xDPI, yDPI;

	               b3Tx           ();
	               b3Tx           (b3Tx *srcTx);
	              ~b3Tx           ();

	void           b3Copy         (b3Tx *srcTx);
	b3_bool        b3AllocTx      (b3_res xSize,b3_res ySize,b3_res depth);
	void           b3FreeTx       ();
	b3_pkd_color  *b3GetPalette   ();
	void           b3SetPalette   (b3_pkd_color *entries,b3_count numEntries);
	void          *b3GetData      ();
	void           b3SetData      (void *newData,b3_res newXSize,b3_res newYSize);
	b3Measure     *b3GetMeasure   ();
	char          *b3Name         ();
	void           b3Name         (const char *ImageName);
	void           b3GetResolution(b3_res &xDPI,b3_res &yDPI);
	void           b3GetRow       (b3_pkd_color *row,b3_coord  y);
	b3_pkd_color   b3GetValue     (b3_coord x,b3_coord  y);
	b3_bool        b3IsLoaded     ();
	b3_bool        b3IsBW         ();
	b3_bool        b3IsTrueColor  ();
	b3_bool        b3IsPalette    ();
	b3_bool        b3IsGreyPalette();

	// b3TxBlit.cc
	void           b3GetColorMask(
		b3_u08       *mask,
		b3_count      BytesPerRow,
		b3_pkd_color  colorMask);
	void           b3Blit       (
		b3Tx     *srcTx,
		b3_coord  xDstOff,  b3_coord yDstOff,
		b3_res    xMax,     b3_res   yMax,
		b3_coord  xSrcOff=0,b3_coord ySrcOff=0);

	// b3TxImage.cc
	void           b3Shrink     (b3_count shrink=1);
	void           b3RemoveBlackBorder();
	void           b3Negate     ();
	void           b3TurnLeft   ();
	void           b3Turn       ();
	void           b3TurnRight  ();
	b3_bool        b3TxGauss    (
		b3_coord xPos,b3_coord yPos,
		b3_f64   scale,b3_f64 sigma,b3_f64 niveau,b3_f64 slope,
		b3Tx        *srcTx = null);
	b3_bool        b3TxColorFilter(
		b3_f64         red_filter,b3_f64 green_filter,b3_f64 blue_filter,
		b3Tx        *srcTx = null);
	b3_bool        b3TxContrast(
		b3_f64         highlight,b3_f64 shadow,b3_f64 gamma,
		b3Tx        *srcTx = null);
	b3_bool        b3TxTransformTable(
		b3_pkd_color *rTable,
		b3_pkd_color *gTable,
		b3_pkd_color *bTable,
		b3Tx         *srcTx = null);

	// b3TxHist.cc
	b3_bool        b3Histogramme();
	b3_bool        b3StartHist  ();
	void           b3EndHist    ();
	b3_bool        b3AddHist    (
		b3_coord xStart,b3_coord yStart,
		b3_coord xStop, b3_coord yStop);
	b3_bool        b3IsWhite    ();
	void           b3SetWhiteRatio(b3_f64 newRatio);
	b3_bool        b3GetHistogramme  (b3_count *hist_buffer,b3_count &entries);
	b3_bool        b3TransToBW       (b3_index  threshold=128);
	b3_bool        b3TransToBW       (b3Tx *srcTx,
		b3_f64 ratio=0.5,b3_tx_threshold mode = B3_THRESHOLD_USE);
	b3_bool        b3TransToBW       (
		b3_f64 ratio=0.5,b3_tx_threshold mode = B3_THRESHOLD_USE);
	b3_index       b3ComputeThreshold(b3_f64 ratio,b3_tx_threshold mode);

	// b3TxSaveTIFF.cc, b3TxLoadTIFF.cc
	b3_tx_type     b3ParseTexture(b3_u08 *buffer,b3_size size);
	b3_result      b3Save       (const char *ImageName);
	b3_result      b3LoadTIFF   (const char *ImageName);
	b3_result      b3LoadTIFF   (const char *ImageName,
		const b3_u08 *ImageBuffer,
		b3_size       BufferSize);

	// b3TxScale.cc
	void           b3TransToGrey();
	void           b3ScaleToGrey(b3Tx *srcTx);
	void           b3Scale      (b3Tx *srcTx);

	// b3TxDeskew.cc
	void           b3Deskew     ();

private:
	// b3TxTurn.cc
	void           b3TurnLeftILBM();
	void           b3TurnLeftVGA();
	void           b3TurnLeftRGB4();
	void           b3TurnLeftRGB8();

	// b3TxTurn.cc
	void           b3TurnRightILBM();
	void           b3TurnRightVGA();
	void           b3TurnRightRGB4();
	void           b3TurnRightRGB8();

	// b3TxScale.cc
	void           b3DivTableInit    ();
	void           b3MonoScaleToGrey (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3VGAScaleToGrey  (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3VGAScaleToVGA   (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3VGAScaleToRGB8  (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ColorScaleToGrey(b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);

	void           b3MonoScale       (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3VGAScale        (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);
	void           b3ColorScale      (b3Tx *srcTx,b3_index *rIndex,b3_index *cIndex);

	// b3Tx.cc
	b3_pkd_color   b3ILBMValue(b3_coord x,b3_coord y);
	b3_pkd_color   b3RGB4Value(b3_coord x,b3_coord y);
	b3_pkd_color   b3RGB8Value(b3_coord x,b3_coord y);
	b3_pkd_color   b3VGAValue (b3_coord x,b3_coord y);

	// b3Tx.cc
	void           b3GetILBM  (b3_pkd_color *row,b3_coord y);
	void           b3GetRGB4  (b3_pkd_color *row,b3_coord y);
	void           b3GetRGB8  (b3_pkd_color *row,b3_coord y);
	void           b3GetVGA   (b3_pkd_color *row,b3_coord y);

	// b3TxImage.cc
	b3_count       b3BuildRLE (b3_count *row,b3_u08 *rle);
	void           b3BuildRow (b3_count *row,b3_u08 *rle,b3_count codeNum,b3_count byteNum);

	// b3TxLoadTIFF.cc
	long           b3TIFFPalette(TIFF *handle,short PaletteMode);
	long           b3TIFFDecode (TIFF *handle,short PlanarConfig);
	long           nauiTIFFAlloc  ();

	// maioTxSaveTIFF.cc
	void           b3GetSampleValues  (long &BitsPerPixel,long &SamplesPerPixel);
	b3_result      b3SaveTIFFFax      (TIFF *handle);
	b3_result      b3SaveTIFFPalette  (TIFF *handle);
	b3_result      b3SaveTIFFTrueColor(TIFF *handle);

	// b3TxScale.cc (color indexing)
	void           b3ColorGrid();
	b3_index       b3ColorIndex(b3_pkd_color color);

	// b3TxDeskew.cc (deskew page)
	void           b3DeskewILBM();
	void           b3DeskewRGB4();
	void           b3DeskewRGB8();
	void           b3DeskewVGA();

	// b3TxEasy.cc
	long           b3ParseRAW (b3_u08 *buffer,long,long,long);
	long           b3ParseBMP (b3_u08 *buffer);
	long           b3ParseMTV (b3_u08 *buffer);
	long           b3ParseBMF (b3_u08 *buffer,unsigned long);

	// b3TxIFF.cc
	b3_tx_type     b3ParseIFF_ILBM (b3_u08 *buffer,b3_size buffer_size);
	b3_tx_type     b3ParseIFF_RGB8 (b3_u08 *buffer,b3_size buffer_size);
	b3_tx_type     b3ParseIFF_RGB4 (b3_u08 *buffer,b3_size buffer_size);
	b3_tx_type     b3ParseIFF_YUVN (b3_u08 *buffer,b3_size buffer_size);
	b3_tx_type     b3EHBPalette();
	void           b3ConvertILBMLine (b3_u08 *Line,b3_u08 *Interleave,b3_res xMax,b3_count Planes);
	b3_tx_type     b3HamPalette (b3_bool HAM8);
	b3_bool        b3CalcYUVTable();

	// b3TxGIF.cc
	b3_tx_type     b3ParseGIF  (b3_u08 *buffer);

	// b3TxPCX.cc
	b3_tx_type     b3ParsePCX4 (b3_u08 *buffer);
	b3_tx_type     b3ParsePCX8 (b3_u08 *buffer);

	// b3TxIMG.cc
	b3_tx_type     b3ParseIMG  (b3_u08 *buffer);

	// b3TxTGA.cc
	b3_tx_type     b3ParseTGA  (b3_u08 *buffer);

	// b3TxJPG.cc
	b3_tx_type     b3ParseJPEG (b3_u08 *buffer,b3_size buffer_size);
};

extern b3_f64        b3Gamma(b3_f64 h,b3_f64 s,b3_f64 gamma,b3_f64 value,b3_f64 scale=1.0);

/*************************************************************************
**                                                                      **
**                        TIFF structures and definitions               **
**                                                                      **
*************************************************************************/

#define TIFF_INTEL    0x4949
#define TIFF_MOTOROLA 0x4d4d

struct HeaderTIFF
{
	short          TypeCPU;
	short          VersionTIFF;
	long           FirstTag;
};

struct TagTIFF
{
	unsigned short Code;
	unsigned short Type;
	long           Data[2];
};

/*************************************************************************
**                                                                      **
**                        TIFF class definitions                        **
**                                                                      **
*************************************************************************/

typedef void (*b3TravFunc)(b3Head *,b3Node *,void *);

class b3TIFF_Dir;

class b3TIFF_Strip : public b3Link<b3TIFF_Strip>
{
	unsigned long   end;
	char           *buffer;
	long            size;
	long            offset;
public:
	     b3TIFF_Strip();
	void b3Init(char *);
	long b3Size(long);
	long b3Offset();
	long b3WriteData (b3File *,long);
	long b3OrgTags   (long);
	long b3OrgStrips (long);
	void b3Traverse(
		b3Base<class T> *head,
		void             (*func)(b3Base<class T> *,b3Link<class T> *,void *),
		void              *ptr);
};

class b3TIFF_Entry : public b3Link<b3TIFF_Entry>, public b3Mem
{
	char           *TIFF;
	unsigned long   end;
	struct TagTIFF  tag;
	void           *buffer;		// buffer to data
	long           *ptr;		// new offsets of strips
	long            size;		// size of data
	long            offset;		// offset of this tag
	long            act;        // offset of buffer (if buffer != null)

public:
	             b3TIFF_Entry(b3TIFF_Dir *,struct TagTIFF *,char *);
				~b3TIFF_Entry();
	       long  b3WriteTag  (b3File *,b3Base<b3TIFF_Strip> *,long,long);
	       long  b3WriteData (b3File *,long);
		   void  b3Traverse  (
			   b3Base<class T> *head,
			   void             (*func)(b3Base<class T> *,b3Link<class T> *,void *),
			   void              *ptr);
		   void  b3RemoveIFW (b3Base<b3TIFF_Entry> *);
		   long  b3OrgTags   (long);
		   long  b3OrgStrips (long);
	friend int   b3SortTags  (b3TIFF_Entry *a,b3TIFF_Entry *b,void *ptr);

public:
	class b3Error {};
};

class b3TIFF_Dir : public b3Link<b3TIFF_Dir>
{
	b3Base<b3TIFF_Entry>  tags;
	b3Base<b3TIFF_Strip>  strips;
	unsigned long             end;
	b3TIFF_Strip           *stripArray;
	long                      num;			// number of tags inside directory
	long                      stripNum;		// number of strips
	long                      offset;		// offset of this dir structure
	long                      next;			// offset of next dir structure
public:
	                b3TIFF_Dir(char *,struct TagTIFF *,long);
	               ~b3TIFF_Dir();
	void            b3Append (b3TIFF_Strip *);
	void            b3Append (b3TIFF_Entry *);
	b3TIFF_Strip *b3FirstStrip();
	b3TIFF_Entry *b3FirstEntry();
	void            b3TravRemoveIFW();
	void            b3Traverse(
		b3Base<class T>  *head,
		void              (*func)(b3Base<class T> *,b3Link<class T> *,void *),
		void               *ptr);
	long            b3StripInc();
	long            b3TagInc();
	long            b3Offset();
	void            b3SetStrips(b3TIFF_Strip *);
	long            b3OrgTags   (long);
	void            b3OrgStrips ();
	long            b3WriteTags(b3File *,long);
public:
	class           b3Error {};
};

class b3TIFF : public b3Link<b3TIFF>, public b3Mem
{
	b3Base<b3TIFF_Dir>  dirs;
	unsigned long           end;
	struct HeaderTIFF       head;
	long                    offset;	// offset of this header (= 0)

public:
	      b3TIFF      (struct HeaderTIFF *);
		  b3TIFF      ();
		 ~b3TIFF      ();
	long  b3DirCount  ();
	void  b3Remove    (b3TIFF *);
	void  b3Append    (b3TIFF *);
	void  b3Write     (char *);

	long  b3OrgTags   (long);
	void  b3OrgStrips ();
	void  b3TravRemoveIFW();
	void  b3Traverse  (
		b3Base<class T>  *head,
		void              (*func)(b3Base<class T> *,b3Link<class T> *,void *),
		void               *ptr);

public:
	class b3Error {};
};

typedef void (*b3LogTiffFunc)(const char *output,void *ptr);

extern b3_u16  GetIntelShort (void *);
extern b3_u32  GetIntelLong  (void *);
extern b3_u16  GetMotShort   (void *);
extern b3_u32  GetMotLong    (void *);
extern b3_size ChangeWord    (void *);
extern b3_size ChangeLong    (void *);
extern b3_u16  GetShort      (void *);
extern b3_u32  GetLong       (void *);

extern void          b3SetLogTiffFunc(b3LogTiffFunc log_func,void *ptr = null);

// b3Node classes
#define CLASS_TIFF_HEAD  0x40000
#define CLASS_TIFF_DIR   0x30000
#define CLASS_TIFF_TAG   0x20000
#define CLASS_TIFF_STRIP 0x10000

// TIFF tags
#define TIFF_NEWSUBFILE	0x00fe
#define TIFF_XSIZE		0x0100
#define TIFF_YSIZE		0x0101
#define TIFF_BPS		0x0102
#define TIFF_COMPR		0x0103
#define TIFF_COLORTYPE	0x0106
#define TIFF_DOCNAME	0x010d
#define TIFF_MODEL		0x0110
#define TIFF_STRIPS		0x0111
#define TIFF_SAMPLES	0x0115
#define TIFF_ROWSPERSTR	0x0116
#define TIFF_STRIPBYTES	0x0117
#define TIFF_XRES		0x011a
#define TIFF_YRES		0x011b
#define TIFF_PLANARCONF	0x011c
#define TIFF_RESUNIT	0x0128
#define TIFF_SOFTWARE	0x0131
#define TIFF_PALETTE	0x0140
#define TIFF_IFW1       0x80a4
#define TIFF_IFW2       0x80a6

// TIFF data types
#define MODE_BYTE		1
#define MODE_ASCII		2
#define MODE_SHORT		3
#define MODE_LONG		4
#define MODE_RATIONAL	5

// TAF token
#define TAF_MAGIC_LEN(s)     (long)((strlen(s) + 4) & (~3))
#define TAF_MAGIC            "Dr. Materna GmbH\0\0\0\0\0"
#define TAF_START            0xdeadcafe
#define TAF_SEPARATOR        0xbadc0ded
#define TAF_END              0xdeadbeef

// Puhh!

#endif
