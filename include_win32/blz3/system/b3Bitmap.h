/*
**
**	$Filename:	b3Bitmap.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - bitmap routines (proto types)
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_BITMAP_H
#define B3_SYSTEM_BITMAP_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Aux.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        some helpers for b3BitmapDIB                  **
**                                                                      **
*************************************************************************/

#define MAXCOL 128

struct CB3LogPal256
{
	LOGPALETTE   pal;
	PALETTEENTRY entry[256];
};

struct CB3DIB
{
	BITMAPINFOHEADER   bmiHeader;	// DIB not used actually
	union
	{
		RGBQUAD        quad[256];	// For palette entries
		b3_u32         rgb[3];		// color mask of DIB
		b3_u16         bw[256];		// palette entries
	}                  color;
public:
	void b3InitDIB(b3_res xSize,b3_res yExtend);
	     operator BITMAPINFO *();
};

/*************************************************************************
**                                                                      **
**                        Bitmap exceptions                             **
**                                                                      **
*************************************************************************/

enum b3_view_error
{
	B3_VIEW_ERROR = -1,
	B3_VIEW_OK    =  0,
	B3_VIEW_MEMORY,
	B3_VIEW_INIT_DIB,
	B3_VIEW_PALETTE
};

typedef b3Exception<b3_view_error,0x565745> b3ViewException;

/*************************************************************************
**                                                                      **
**                        Device independend bitmap                     **
**                                                                      **
*************************************************************************/

class CB3BitmapDIB : public b3Mem
{
	static const RGBQUAD CB3BitmapDIB::m_PaletteEGA[16];

protected:
	unsigned long    m_Mode;
	b3_res           xSize;
	b3_res           ySize;
	b3_res           depth;
	b3_size          dSize;			// size of data needed
	b3_pkd_color    *m_Row;			// data itself

public:
	struct CB3DIB    m_DIB;

	              CB3BitmapDIB();
	              CB3BitmapDIB(BITMAPINFO *new_dib,void *new_row);
	b3_size       b3ImageSize(b3_res xSize,b3_res ySize,b3_res depth);
	b3_bool       b3SetSize(b3_res xSize,b3_res ySize,b3_res depth);
	b3_bool       b3SetTx(b3Tx *tx);
	void          b3SetData(b3Tx *texture,b3_coord yStart,b3_coord yEnd);
	b3_bool       b3SetDIB(BITMAPINFO *new_dib,void *new_row);
	b3_bool       b3DIBtoTx(b3Tx *texture);
	HBITMAP       b3CreateBitmap(CDC *dc);
	              operator BITMAPINFO *();

protected:
	void          b3InitDIB();
};

/*************************************************************************
**                                                                      **
**                        Device dependend bitmap                       **
**                                                                      **
*************************************************************************/

class CB3BitmapDDB : protected CB3BitmapDIB
{
public:
	b3_bool           m_AutoMemDC;
protected:
	CDC               memDC;
	CBitmap           memBitmap;

private:
	CDC              *origDC;
	CBitmap          *oldBitmap;
	CPalette         *oldPalette;
	CPalette          memPalette;
	CB3LogPal256    m_LogPal256;


public:
	          CB3BitmapDDB ();
			  CB3BitmapDDB (CDC *srcDC);
	CDC      *b3GetDC     ();
	CBitmap  *b3GetBitmap ();
	CPalette *b3GetPalette();
	void      b3InitImage (b3Tx *srcImage,CDC *srcDC,b3_coord yStart = 0,b3_coord yEnd = 0);
	void      b3SetImage  (b3Tx *srcImage, b3_res xSize, b3_res ySize);
	b3_bool   b3InitBitmap(b3_res xSize,b3_res ySize);
	b3_bool   b3DIBtoDDB  ();
	b3_bool   b3DIBtoDDB  (CDC *dstDC);
	b3_bool   b3Transparent(b3RGB &filter,b3_coord xPos,b3_coord yPos,b3_res xSize,b3_res ySize);
	b3_bool   b3Transparent(b3RGB &filter,b3Rect &rectangle);
	void      b3Clear     ();

protected:
	void     b3DeinitBitmap();
	b3_bool  b3InitBitmap  ();
	void     b3SetPalette  (b3Tx *srcImage);

private:
	b3_bool  b3InitPalette();
};

#endif
