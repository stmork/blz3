/*
**
**	$Filename:	b3Bitmap.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - bitmap conversion routines
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

#include "blz3/system/b3View.h"
#include <math.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3DIB                           **
**                                                                      **
*************************************************************************/

void CB3DIB::b3InitDIB(b3_res xSize,b3_res yExtend)
{
	// Set bitmap values
	bmiHeader.biSize          = sizeof(bmiHeader);
	bmiHeader.biWidth         = xSize;
	bmiHeader.biHeight        = yExtend;
	bmiHeader.biPlanes        =  1;
	bmiHeader.biBitCount      = 32;
	bmiHeader.biCompression   = BI_BITFIELDS;
	bmiHeader.biSizeImage     = xSize * yExtend * sizeof(b3_u32);
	bmiHeader.biXPelsPerMeter = 1;
	bmiHeader.biYPelsPerMeter = 1;
	bmiHeader.biClrUsed       = 0;
	bmiHeader.biClrImportant  = 0;

	// Set Color mask
	color.rgb[0] = 0x00ff0000;
	color.rgb[1] = 0x0000ff00;
	color.rgb[2] = 0x000000ff;

	// Now we are ready to draw onto the screen (TippEx on the CRT)
	b3PrintF (B3LOG_FULL,
		"### CLASS: b3BDIB # b3InitDIB(%ld,%ld # %ld (constant))\n",
		xSize,yExtend,32);
}

CB3DIB::operator BITMAPINFO *()
{
	return (BITMAPINFO *)&bmiHeader;
}

/*************************************************************************
**                                                                      **
**                        Device independend bitmap                     **
**                                                                      **
*************************************************************************/

CB3BitmapDIB::CB3BitmapDIB()
{
	// Init zero
	row   = null;
	dSize = 0;
	b3SetSize (0,0,0);
}

CB3BitmapDIB::CB3BitmapDIB(
	BITMAPINFO *new_dib,
	void       *new_row)
{
	// Init zero
	row   = null;
	dSize = 0;
	b3SetSize (0,0,0);

	if (!b3SetDIB(new_dib,new_row))
	{
		throw b3ErrorInitDIB();
	}
}

CB3BitmapDIB::operator BITMAPINFO *()
{
	return (BITMAPINFO *)&DIB.bmiHeader;
}

void CB3BitmapDIB::b3InitDIB()
{
	b3_u16 num;

	// Compute needed color depth
	if (xSize < 16)
	{
		return;
	}

	if (depth <= 8)
	{
		num    = (b3_u16)(depth == 1 ? 1 : 8);
		m_Mode = DIB_PAL_COLORS;
	}
	else
	{
		num    = 32;
		m_Mode = DIB_RGB_COLORS;
	}

	// This is the need for a 24-Bit-DIB. See VC++
	// documentation for what's going up here. This
	// needs a lot of memory but it's faaaast later.
	DIB.bmiHeader.biSize          = sizeof(DIB.bmiHeader);
	DIB.bmiHeader.biWidth         =  xSize;
	DIB.bmiHeader.biHeight        = (signed)ySize;
	DIB.bmiHeader.biPlanes        =  1;
	DIB.bmiHeader.biBitCount      = num;
	DIB.bmiHeader.biCompression   = (depth <= 8 ? BI_RGB : BI_BITFIELDS);
	DIB.bmiHeader.biSizeImage     = dSize;
	DIB.bmiHeader.biXPelsPerMeter = 1;
	DIB.bmiHeader.biYPelsPerMeter = 1;
	DIB.bmiHeader.biClrUsed       = 0;
	DIB.bmiHeader.biClrImportant  = 0;

	// Now we are ready to draw onto the screen (TippEx on the CRT)
	b3PrintF (B3LOG_FULL,
		"### CLASS: b3BDIB # b3InitDIB(%ld,%ld # %ld)\n",
		xSize,ySize,num);
}

b3_size CB3BitmapDIB::b3ImageSize(
	b3_res x,
	b3_res y,
	b3_res d)
{
	b3_size  size;
	b3_count xBytes;

	// Compute actual and new memory needs. It can be possible
	// that the resolution changes but the buffer size doesn't
	// change. In this case we don't need a new memory block).
	switch (d)
	{
	case 1 :
		xBytes   = TX_BLA(x);
		size = xBytes * y * sizeof(b3_u08);
		break;
	case 2 :
	case 3 :
	case 4 :
	case 5 :
	case 6 :
	case 7 :
	case 8 :
		xBytes   = TX_VLA(x);
		size = xBytes * y * sizeof(b3_u08);
		break;
	default :
		size = x      * y * sizeof(b3_pkd_color);
		break;
	}
	return size;
}

// Set the resolution for this view.
b3_bool CB3BitmapDIB::b3SetSize(
	b3_res x,
	b3_res y,
	b3_res d)
{
	b3_size newField;
	b3_bool changed = false;

	newField = b3ImageSize(x,y,d);
	if (newField != dSize)
	{
		// Free old bitmap
		b3Free ((void *)row);

		// Signal reallocation
		row  = null;
	}

	// If resolution has changed we need a new Windows bitmap
	if ((xSize != x) || (ySize != y) || (depth != d))
	{
		changed = true;
	}

	// Set new resolution in every case.
	xSize = x;
	ySize = y;
	depth = d;
	dSize = newField;

	b3InitDIB();

	// Allocate new image buffer
	if (row == null)
	{
		row  = (b3_pkd_color *)b3Alloc(dSize);
		changed = true;
	}

	b3PrintF (B3LOG_FULL,
		"### CLASS: b3BDIB # b3SetSize():    %4ld x %4ld (%ld bits)%s\n",
		xSize,ySize,depth,changed ? " - changed" : "");
	return changed;
}

void CB3BitmapDIB::b3SetData(b3Tx *texture,b3_res yStart,b3_res yEnd)
{
	b3_index index;
	b3_coord y,yDiff;

	b3PrintF (B3LOG_FULL,
		"### CLASS: b3BDIB # b3SetData(): lines %ld - %ld\n",yStart,yEnd);
	yDiff = ySize - yStart;

	// Monochrome images
	if (depth == 1)
	{
		b3_u08   *src,*dst;
		b3_count  srcBytes = TX_BWA(xSize);
		b3_count  dstBytes = TX_BLA(xSize);

		b3PrintF (B3LOG_FULL,
			"### CLASS: b3BDIB # b3SetData(): monochrome mode\n");

		src   = (b3_u08 *)texture->b3GetData();
		dst   = (b3_u08 *)row;
		index = yDiff * dstBytes;
		for (y = yStart;y < yEnd;y++)
		{
			index -= dstBytes;
			memcpy (&dst[index],src,srcBytes);
			src   += srcBytes;
		}
	}

	// Palette images
	if ((depth > 1) && (depth <= 8))
	{
		b3_u08   *src,*dst;
		b3_count  srcBytes = TX_VBA(xSize);
		b3_count  dstBytes = TX_VLA(xSize);

		b3PrintF (B3LOG_FULL,
			"### CLASS: b3BDIB # b3SetData(): palette mode\n");

		src   = (b3_u08 *)texture->b3GetData();
		dst   = (b3_u08 *)row;
		index = yDiff * dstBytes;
		for (y = yStart;y < yEnd;y++)
		{
			index -= dstBytes;
			memcpy (&dst[index],src,srcBytes);
			src   += srcBytes;
		}
	}

	// RGB images
	if (depth > 8)
	{
		b3PrintF (B3LOG_FULL,
			"### CLASS: b3BDIB # b3SetData(): true color mode\n");

		index  = yDiff * xSize;
		for (y = yStart;y < yEnd;y++)
		{
			index -= xSize;
			texture->b3GetRow(&row[index],y);
		}
	}
}

static RGBQUAD paletteEGA[16] =
{
	{ 0x00,0x00,0x00,0 },
	{ 0xff,0xff,0xff,0 },
	{ 0x40,0x40,0x40,0 },
	{ 0xc0,0xc0,0xc0,0 },
	     			   
	{ 0x00,0x00,0x80,0 },
	{ 0x00,0x00,0xff,0 },
	{ 0x00,0x80,0x80,0 },
	{ 0x00,0xff,0xff,0 },
	     			   
	{ 0x00,0x80,0x00,0 },
	{ 0x00,0xff,0x00,0 },
	{ 0x80,0x80,0x00,0 },
	{ 0xff,0xff,0x00,0 },
	     			   
	{ 0x80,0x00,0x00,0 },
	{ 0xff,0x00,0x00,0 },
	{ 0x80,0x00,0x80,0 },
	{ 0xff,0x00,0xff,0 }
};

b3_bool CB3BitmapDIB::b3SetDIB(
	BITMAPINFO *new_dib,
	void       *new_row)
{
	b3_pkd_color *buffer;
	b3_index      i;
	b3_count      num_colors;

	if (new_dib == null)
	{
		return false;
	}

	// Reallocate memory
	buffer = (b3_pkd_color *)b3Alloc(new_dib->bmiHeader.biSizeImage);
	if (buffer == null)
	{
		return false;
	}
	b3Free(row);
	row = buffer;

	// Copy some values
	DIB.bmiHeader = new_dib->bmiHeader;
	xSize = DIB.bmiHeader.biWidth;
	ySize = abs(DIB.bmiHeader.biHeight);
	dSize = DIB.bmiHeader.biSizeImage;
	depth = DIB.bmiHeader.biBitCount;

	num_colors = 1 << depth;
	if (depth <= 8)
	{
		for (i = 0;i < num_colors;i++)
		{
			DIB.color.quad[i] = new_dib->bmiColors[i];
			b3PrintF (B3LOG_FULL,
				"### CLASS: b3BDIB # %3lu: %02x%02x%02x\n",i,
				new_dib->bmiColors[i].rgbRed,
				new_dib->bmiColors[i].rgbGreen,
				new_dib->bmiColors[i].rgbBlue);
			DIB.bmiHeader.biClrUsed = i;
			m_Mode = DIB_RGB_COLORS;
		}
	}

	// Check if image data is appended to header
	if (new_row == null)
	{
		new_row = (void *)&new_dib->bmiColors[depth > 8 ? DIB.bmiHeader.biClrUsed : num_colors];
	}
	memcpy(row,new_row,dSize);

	return true;
}

b3_bool CB3BitmapDIB::b3DIBtoTx(b3Tx *texture)
{
	b3_pkd_color  *palette;
	b3_pkd_color  *lData;
	b3_u08        *cData,*cRow;
	void          *data;
	b3RGB          color;
	b3_index	   i,index;
	b3_count       xMax,xBytes;
	b3_coord       x,y;
	b3_index       subOffset,addOffset,rowOffset;

	if (texture == null)
	{
		return false;
	}
	texture->b3AllocTx(xSize,ySize,depth);
	data    = texture->b3GetData();
	palette = texture->b3GetPalette();

	if (palette != null)
	{
		for (i = 0;i < (b3_count)DIB.bmiHeader.biClrUsed;i++)
		{
			color.r    = DIB.color.quad[i].rgbRed;
			color.g    = DIB.color.quad[i].rgbGreen;
			color.b    = DIB.color.quad[i].rgbBlue;
			palette[i] = color;
		}
	}

	if (data == null)
	{
		return false;
	}

//	return true;

	switch (depth)
	{
	case  1:
		xBytes     = TX_BWA(xSize);
		cRow       = (b3_u08 *)row;
		cData      = (b3_u08 *)data;
		rowOffset  = TX_BLA(xSize);
		palette[0] = 0xffffff;
		palette[1] = 0x000000;
		if (DIB.bmiHeader.biHeight > 0)
		{
			subOffset = xBytes;
			addOffset = 0;
			cData    += (xBytes * ySize);
		}
		else
		{
			subOffset = 0;
			addOffset = xBytes;
		}

		// Copy loop
		for (y = 0;y < ySize;y++)
		{
			cData -= subOffset;
			for (x = 0;x < xBytes;x++)
			{
				cData[x] = (b3_pkd_color)(cRow[x] ^ 0xff);
			}
			cData += addOffset;
			cRow  += rowOffset;
		}
		break;

	case  4:
		xMax  = xSize & 0xfffffffe;
		index = 0;
		cRow  = (b3_u08 *)row;
		cData = (b3_u08 *)data;
		if (DIB.bmiHeader.biHeight > 0)
		{
			subOffset = xSize;
			addOffset = 0;
			cData    += (xSize * ySize);
		}
		else
		{
			subOffset = 0;
			addOffset = xSize;
		}

		// Copy loop
		for (y = 0;y < ySize;y++)
		{
			cData -= subOffset;
			for (x = 0;x < xMax;x+=2)
			{
				cData[x]   = (b3_u08)((cRow[index] & 0xf0) >> 4);
				cData[x+1] = (b3_u08)( cRow[index] & 0x0f);
				index++;
			}
			if (xSize & 1)
			{
				cData[x]   = (b3_u08)((cRow[index] & 0xf0) >> 4);
			}
			cData += addOffset;
		}
		break;

	case  8:
		cData = (b3_u08 *)data;
		cRow  = (b3_u08 *)row;
		if (DIB.bmiHeader.biHeight > 0)
		{
			subOffset = xSize;
			addOffset = 0;
			cData    += (xSize * ySize);
		}
		else
		{
			subOffset = 0;
			addOffset = xSize;
		}

		// Copy loop
		for (y = 0;y < ySize;y++)
		{
			cData -= subOffset;
			for (x = 0;x < xSize;x++)
			{
				cData[x] = *cRow++;
			}
			cData += addOffset;
		}
		break;

	case 15:
		break;

	case 16:
		break;

	case 24:
		index = 0;
		lData = (b3_pkd_color *)data;
		cRow  = (b3_u08 *)row;
		if (DIB.bmiHeader.biHeight > 0)
		{
			subOffset = xSize;
			addOffset = 0;
			lData    += (xSize * ySize);
		}
		else
		{
			subOffset = 0;
			addOffset = xSize;
		}

		// Copy loop
		for (y = 0;y < ySize;y++)
		{
			lData -= subOffset;
			for (x = 0;x < xSize;x++)
			{
				color.b  = *cRow++;
				color.g  = *cRow++;
				color.r  = *cRow++;
				lData[x] = color;
			}
			lData += addOffset;
		}
		break;

	case 32:
		index = 0;
		lData = (b3_pkd_color *)data;
		cRow  = (b3_u08 *)row;
		if (DIB.bmiHeader.biHeight > 0)
		{
			subOffset = xSize;
			addOffset = 0;
			lData    += (xSize * ySize);
		}
		else
		{
			subOffset = 0;
			addOffset = xSize;
		}

		// Copy loop
		for (y = 0;y < ySize;y++)
		{
			lData -= subOffset;
			for (x = 0;x < xSize;x++)
			{
				color.b  = *cRow++;
				color.g  = *cRow++;
				color.r  = *cRow++;
				lData[x] = color;
			}
			lData += addOffset;
		}
		break;

	default:
		return false;
	}

	return true;
}

/*************************************************************************
**                                                                      **
**                        Device dependend bitmap                       **
**                                                                      **
*************************************************************************/

CB3BitmapDDB::CB3BitmapDDB()
{
	oldBitmap      = null;
	origDC         = null;
	oldPalette     = null;
	m_AutoMemDC    = true;

	// Init palette
	if (!b3InitPalette())
	{
		throw b3PaletteError();
	}
}

CB3BitmapDDB::CB3BitmapDDB(CDC *srcDC)
{
	oldBitmap      = null;
	origDC         = srcDC;
	oldPalette     = null;
	m_AutoMemDC    = true;
	b3InitBitmap(0,0);

	// Init palette
	if (!b3InitPalette())
	{
		throw b3PaletteError();
	}
}

b3_bool CB3BitmapDDB::b3InitPalette()
{
	b3_bool result;
	m_LogPal256.pal.palVersion    = 0x300;
	m_LogPal256.pal.palNumEntries = 256;
	result = (memPalette.CreatePalette(&m_LogPal256.pal) != FALSE);

	b3PrintF (B3LOG_FULL,
		"### CLASS: b3BDDB # b3InitPalette() # %s\n",result ? "successful" : "error");
	return result;
}

CDC *CB3BitmapDDB::b3GetDC()
{
	return (memDC.m_hDC != 0 ? &memDC : null);
}

CBitmap *CB3BitmapDDB::b3GetBitmap()
{
	return &memBitmap;
}

CPalette *CB3BitmapDDB::b3GetPalette()
{
	return (memPalette.m_hObject != 0 ? &memPalette : null);
}

// This method frees the DDB data we needed
void CB3BitmapDDB::b3DeinitBitmap()
{
	// Delete bitmap
	if (oldBitmap != null) 
	{
		memDC.SelectObject(oldBitmap);
		memBitmap.DeleteObject();
		oldBitmap = null;
	}
	
	// Delete palette
	if (oldPalette != null)
	{
		memDC.SelectPalette (oldPalette,false);
		oldPalette = null;
	}

	memDC.DeleteDC();
}

// This method initializes a DIB and a DDB. We setup a DIB by
// copying the Blizzard III image data into the DIB. Later in OnUpdate()
// we copy the DIB data into the DDB data. The OnDraw routine
// copies the DDB data on the screen. This is the moment when
// we see what we want to see...
b3_bool CB3BitmapDDB::b3InitBitmap()
{
	b3_bool success = false;

	// We should free a previous session first...
	b3DeinitBitmap();

	// Now get active DC
	if (origDC != null)
	{
		// Make this DC compatible to the DC of the actual view
		if (memDC.CreateCompatibleDC(origDC))
		{
			// Create a DDB by using the new DC
			if (memBitmap.CreateCompatibleBitmap (origDC,xSize,ySize))
			{
				oldBitmap = memDC.SelectObject(&memBitmap);

				success = true;
			}
		}
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitBitmap(): success = %s\n",
		success ? "True" : "False");
	return success;
}

void CB3BitmapDDB::b3SetPalette(b3Tx *texture)
{
	b3_count num = 0;
	b3_bool  useNewPalette = false;

	// Free old palette if existing
	if (oldPalette != null)
	{
		memDC.SelectPalette (oldPalette,false);
		oldPalette = null;
	}
	
	// Do nothing on empty image.
	if ((texture->xSize == 0) || (texture->ySize == 0) || (texture->depth == 0))
	{
		return;
	}

	// Allocate new palette
	if (depth <= 8)
	{			
		b3_index      i;
		b3_pkd_color *palette = texture->b3GetPalette();

		// Set palette entries
		num = 1 << depth;
		for (i = 0;i < num;i++)
		{
			b3_u08 r,g,b;

			// Get RGB from actual Blizzard III palette
			r = (b3_u08)((palette[i] & 0xff0000) >> 16);
			g = (b3_u08)((palette[i] & 0x00ff00) >>  8);
			b = (b3_u08)( palette[i] & 0x0000ff);

			// Copy RGB values into Windows palette
			m_LogPal256.entry[i].peRed   = r;
			m_LogPal256.entry[i].peGreen = g;
			m_LogPal256.entry[i].peBlue  = b;
			m_LogPal256.entry[i].peFlags = PC_NOCOLLAPSE;

			// Set Palette indices
			switch (m_Mode)
			{
			case DIB_PAL_COLORS:
				DIB.color.bw[i] = (b3_u16)i;
				break;

			case DIB_RGB_COLORS:
				DIB.color.quad[i].rgbRed      = r;
				DIB.color.quad[i].rgbGreen    = g;
				DIB.color.quad[i].rgbBlue     = b;
				DIB.color.quad[i].rgbReserved = 0;
				break;
			}
		}

		// Set right values into DIB (palette)
		DIB.bmiHeader.biClrUsed       = num;
		DIB.bmiHeader.biClrImportant  = num;

		// Signal new palette
		useNewPalette = true;
		b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3SetPalette(): %06lx - %06lx (%ld colors) (palettized image)\n",
			palette[0],palette[num-1],num);
	}
	else
	{
		// Check if we are able to use palettes in
		// actual viedeo mode.
		b3_count ColorNum  = 0;
		b3_count ColorBits = 0;
		b3_index i;

		// Get some color values
		if (origDC != null)
		{
			ColorNum  = origDC->GetDeviceCaps(SIZEPALETTE);
			ColorBits = origDC->GetDeviceCaps(BITSPIXEL);
		}

		// Set right values into DIB (RGB)
		DIB.bmiHeader.biClrUsed       = 0;
		DIB.bmiHeader.biClrImportant  = 0;

		DIB.color.rgb[0] = 0x00ff0000;
		DIB.color.rgb[1] = 0x0000ff00;
		DIB.color.rgb[2] = 0x000000ff;

		// Check if we must seup a custom color palette
		// for true color images on palette screen
		if (ColorBits <= 8)
		{
			num = 64;
			for (i = 0;i < num;i++)
			{
				// Copy RGB values into Windows palette
				m_LogPal256.entry[i].peRed   = (b3_u08)((i & 0x30) << 2);
				m_LogPal256.entry[i].peGreen = (b3_u08)((i & 0x0c) << 4);
				m_LogPal256.entry[i].peBlue  = (b3_u08)((i & 0x03) << 6);
				m_LogPal256.entry[i].peFlags = PC_NOCOLLAPSE;
			}

			// Signal new palette
			useNewPalette = true;
			b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3SetPalette(): (%ld colors) (palettized screen)\n",
				num);
		}
	}

	// We need a new palette, so say it Windows...
	if (useNewPalette)
	{
		// Update palette instance
		memPalette.SetPaletteEntries(0,num,m_LogPal256.entry);
		b3PrintF (B3LOG_FULL,
			"### CLASS: b3BDDB # b3SetPalette() # %3ld colors set\n",num);

		// Set palette to DDB
		if (memDC.m_hDC != 0)
		{
			oldPalette = memDC.SelectPalette (&memPalette,false);
			memDC.RealizePalette();

			b3PrintF (B3LOG_FULL,
				"### CLASS: b3BDDB # b3SetPalette() # selecting palette into memory DC\n");
		}

		// Set palette to real ScrollView
		if (origDC != null)
		{
			origDC->SelectPalette (&memPalette,false);
			origDC->RealizePalette();
			
			b3PrintF (B3LOG_FULL,
				"### CLASS: b3BDDB # b3SetPalette() # selecting palette into original DC\n");
		}
	}
}

// This is the main update view. Call b3Tx-class to get the
// image information in DIB format then copy it into DDB-format
// using SetDIBitsToDevice(). The drawing into the visible
// device is doing b3View::b3DrawView().
// (--< Call from Cb3View::OnUpdate() >--)
void CB3BitmapDDB::b3InitImage(
	b3Tx     *texture,
	CDC      *srcDC,
	b3_coord  yStart,
	b3_coord  yEnd)
{
	b3_bool success;
 
	// Has the resolution changed since the last call?
	b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitImage() - start\n");
	origDC = srcDC;
	if (b3SetSize (texture->xSize,texture->ySize,texture->depth))
	{
		success = true;
	}
	else
	{
		// Nothing has changed nothing to be initialized.
		// But if we have an image smaller than 16 pixel
		// we should do nothing.
		success = (xSize >= 16);
	}

	if (success)
	{
		if (m_AutoMemDC)
		{
			b3InitBitmap();
		}

		// Get image data row by row
		// Convert Blizzard III texture format into 24-Bit-DIB.	 
		if (srcDC->IsPrinting())
		{
			m_Mode = DIB_RGB_COLORS;
		}
		b3SetPalette(texture);

		// Setup which lines are to be copied...
		if ((yStart == 0) && (yEnd == 0))
		{
			yStart = 0;
			yEnd   = ySize;
		}

#ifdef _DEBUG
		int caps = srcDC->GetDeviceCaps(RASTERCAPS);
		b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitImage(): RC_DIBTODEV   %ssupported\n",
			caps & RC_DIBTODEV   ? "" : "not ");
		b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitImage(): RC_STRETCHDIB %ssupported\n",
			caps & RC_STRETCHDIB ? "" : "not ");
		b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitImage(): RC_BITBLT     %ssupported\n",
			caps & RC_BITBLT     ? "" : "not ");
		b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitImage(): RC_PALETTE    %ssupported\n",
			caps & RC_PALETTE    ? "" : "not ");
#endif
		b3PrintF (B3LOG_FULL,
			"### CLASS: b3BDDB # b3InitImage(): (%ld x %ld) -> (%ld - %ld)\n",
			xSize,ySize,yStart,yEnd);

		// Copy image data
		b3SetData(texture,yStart,yEnd);

		if (m_AutoMemDC)
		{
			b3DIBtoDDB(&memDC);
		}
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3InitImage() - stop\n");
	origDC = null;
}

b3_bool CB3BitmapDDB::b3InitBitmap(
	b3_res xNewSize,
	b3_res yNewSize)
{
	b3_bool result = true;

	if ((xNewSize != xSize) || (yNewSize != ySize))
	{
		xSize = xNewSize;
		ySize = yNewSize;
		result = b3InitBitmap();
	}

	return result;
}

b3_bool CB3BitmapDDB::b3DIBtoDDB()
{
	CDC     *pDC;
	b3_bool  result = false;

	pDC = b3GetDC();
	if (pDC != null)
	{
		result = b3DIBtoDDB(pDC);
	}
	return result;
}

b3_bool CB3BitmapDDB::b3DIBtoDDB(CDC *pDC)
{
	b3_res lines;

	lines = ::StretchDIBits(
		pDC->m_hDC,		// where to draw
		0,0,			// dst position
		xSize,ySize,	// dst size
		0,0,			// src position
		xSize,ySize,	// src size
		row,			// where is the bitmap data
		DIB,			// bitmap info
		m_Mode,
		SRCCOPY);
	b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3DIBtoDDB();\n");
	return lines > 0;
}

// Clearing memory bitmap
void CB3BitmapDDB::b3Clear()
{
	memDC.BitBlt(0,0,xSize,ySize,&memDC,0,0,WHITENESS);
	b3PrintF (B3LOG_FULL,"### CLASS: b3BDDB # b3Clear();\n");
}

b3_bool CB3BitmapDDB::b3Transparent(b3RGB &filter,b3Rect &rect)
{
	return b3Transparent(filter,rect.x1,rect.y1,rect.b3Width(),rect.b3Height());
}

b3_bool CB3BitmapDDB::b3Transparent(
	b3RGB    &filter,
	b3_coord  xPos,
	b3_coord  yPos,
	b3_res    xSize,
	b3_res    ySize)
{
	b3_index i;
	b3_count max,lines;
	b3_pkd_color  src,dst;
	b3_pkd_color  rMulTable[256];
	b3_pkd_color  gMulTable[256];
	b3_pkd_color  bMulTable[256];

	// Init bitmap
	b3SetSize(xSize,ySize,32);
	if (b3InitBitmap())
	{
		// Get bitmap
		if (memDC.BitBlt ( 0, 0, xSize, ySize, origDC, xPos, yPos, SRCCOPY ))
		{
			lines = ::GetDIBits (
				origDC->m_hDC,		// From where to get data
				HBITMAP(memBitmap),	// The bitmap
				0, ySize,			// Starting scan line, number of scan lines
				row,				// Where to put the data
				DIB,				// DIB info
				DIB_RGB_COLORS );	// Always RGB

			// Init multiplication table
			for (i = 0;i < 256;i++)
			{
				rMulTable[i] = ((i * filter.r) & 0x00ff00) << 8;
				gMulTable[i] =  (i * filter.g) & 0x00ff00;
				bMulTable[i] = ((i * filter.b) & 0x00ff00) >> 8;
			}

			// Do the filtering
			max = xSize * ySize;
			for (i = 0;i < max;i++)
			{
				src     = row[i];
				dst     = bMulTable[src & 0xff];
				src     = src >> 8;
				dst    |= gMulTable[src & 0xff];
				src     = src >> 8;
				dst    |= rMulTable[src & 0xff];
				row[i]  = dst;
			}
		}
	}

	// Put bitmap
	lines = ::SetDIBits(
		origDC->m_hDC,		// Where to put data
		HBITMAP(memBitmap),	// The bitmap
		0, ySize,			// Starting scan line, number of scan lines
		row,				// The data to put itself
		DIB,				// DIB info
		DIB_RGB_COLORS );	// Always RGB
	origDC->BitBlt(
		xPos,yPos,			// Dest position
		xSize,ySize,		// Dest/Src size
		&memDC,				// From where to copy
		0,0,				// Src position
		SRCCOPY);			// Copy!
	return false;
}
