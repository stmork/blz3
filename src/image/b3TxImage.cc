/*
**
**	$Filename:	b3TxImage.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some useful imaging routines (rotation)
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

#include "blz3/image/b3Tx.h"
#include "blz3/system/b3Log.h"
#include "blz3/base/b3Color.h"
#include <float.h>

#define not_SLOW_N_UGLY

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.17  2005/01/31 21:36:50  sm
**	- FLT_MAX is correct! And now: include <float.h> for that...
**
**	Revision 1.16  2005/01/31 21:20:19  sm
**	- Changed FLT_MAX to MAX_FLOAT and removed values.h include.
**	
**	Revision 1.15  2005/01/24 18:32:34  sm
**	- Removed some static variables and functions.
**	
**	Revision 1.14  2005/01/24 14:21:00  smork
**	- Moved some static variables.
**	
**	Revision 1.13  2005/01/04 15:13:59  smork
**	- Changed some data types.
**	
**	Revision 1.12  2005/01/02 19:15:25  sm
**	- Fixed signed/unsigned warnings
**	
**	Revision 1.11  2005/01/01 16:43:19  sm
**	- Fixed some aliasing warnings.
**	
**	Revision 1.10  2004/11/26 16:28:17  smork
**	- Equalizing VERBOSE print outs.
**	- pthread error messages only in _DEBUG version.
**	
**	Revision 1.9  2004/08/28 13:55:33  sm
**	- Added some mirror methods.
**	- Cleanup job.
**	
**	Revision 1.8  2004/08/09 10:09:05  sm
**	- Added color palette reduction and its tool.
**	
**	Revision 1.7  2003/02/22 17:21:34  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.6  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.5  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.4  2002/07/22 18:45:16  sm
**	- Further probing of texture stencil via alpha channel.
**	- Why does Mesa loose the first texture?
**	- Nasty uncr.
**	
**	Revision 1.3  2002/05/10 15:24:23  sm
**	- Corrected some exceptions in b3Tx
**	- Added double click support in list controls when creating
**	  a new shape.
**	- Some minor fixes done.
**	
**	Revision 1.2  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
**	Revision 1.8  2000/10/11 11:43:24  smork
**	- Some updates done in b3DocBase
**	- b3TxTranformTable used for filtering and contrasting. It's
**	  much more faster now.
**	- Why blocks the image output on Blizzard III Scan when scanning?
**	
**	Revision 1.7  2000/10/06 14:23:38  smork
**	- Palette handling for b3TxContrast() and b3TxColorFilter()
**	  added.
**	- Added Twain dialog button for Blizzard III Imaging
**	
**	Revi  2000/10/04 06:56:38  smork
**	- New virtual function in class b3ScanBase for retrieving
**	  wether to get scan parameter from class b3Scan or not.
**	- Introduced new CDlgImgContrast dialog in Blizzard III Imaging for
**	  correcting contrast, brightness and gamma.
**	
**	Revision 1.5  2000/10/02 06:14:28  smork
**	- Introduced Blizzard III Imaging. This is a case study
**	  for creating new Blizzard III applications. Any new
**	  student has to learn exact this task. The
**	  applecation is missing following features:
**	  o saving main frame window format
**	  o size & position of toolbar
**	- New controls introduced:
**	  o CBlizzard IIIShowGauss    for removing Gauss artefacts
**	                      from image
**	  o CBlizzard IIIShowColor    for showing HSV color selection
**	  o CBlizzard IIIHSVField     for slider legend of HSV color
**	                      selection
**	  Controls added to libIntGUI
**	- Added image anhancement for RGB8 images inside
**	  b3Tx:
**	  o b3TxGauss(...)       for removing Gauss arte-
**	                           facts
**	  O b3TxColorFilter(...) for image color filtering
**	
**	- PS:
**	  The CfM Twain32 5.20 driver works fine.
**	- PPS:
**	  It was nice to use MFC. Shall I change to server
**	  development? I don't think so...
**	
**	Revision 1.4  2000/09/26 08:26:17  smork
**	- Fixed a turn left bug in b3Tx: Bits set outside the right byte
**	  boundary have to be cleared to prevent setting bits before new
**	  image memory starts (memory underflow).
**	
**	Revision 1.3  2000/09/21 10:22:27  smork
**	- Setting Blizzard III projects to warning level 3: Found
**	  some uninitialized variables.
**	- changed b3Mutex from CMutex to CRITICAL_SECTION (thread
**	  synchronization only)
**	- introduced b3IPCMutex for process synchronization
**	
**	Revision 1.2  2000/08/14 11:11:23  smork
**	- Inserted change log
**	- Inserted author tag
**	- Cleaned up file header
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Tx imaging                                  **
**                                                                      **
*************************************************************************/

// This method builds an RLE code of a BW image line. It builds the
// code by counting the pixel width of each black/white cell. The
// first cell is black in every case. If the line starts with white
// this first cell is zero. So even numbered RLE codes are black,
// odd numbers are white.
b3_count b3Tx::b3BuildRLE(
	b3_count *line,
	b3_u08   *cPtr)
{
	b3_count     count,num;
	b3_index     i,x;
	b3_pkd_color bit;
	b3_bool      last,act;

	// building RLE code
	bit   = 128;
	i     = 0;
	last  = true;	// true = last bit set/false = last bit cleared
						// So: We start with black block!
	count = 0;
	num   = 0;
	for (x = 0;x < xSize;x++)
	{
		// get RLE code, starting with black code
		act = (cPtr[i] & bit) != 0;
		if (act != last)
		{
			// Here an equal block ends and we have
			// to toggle the state...
			line[num++] = count;
			count       = 1;
			last        = act;
		}
		else
		{
			// "Equal case" increment block width.
			count++;
		}

		// shifting bit
		bit = bit >> 1;
		if (bit == 0)
		{
			bit = 128;
			i++;
		}
	}

	if (count > 0)
	{
		line[num++] = count;
	}
	return num;
}

void b3Tx::b3BuildRow(
	b3_count *line,
	b3_u08   *cPtr,
	b3_count  num,
	b3_count  xBytes)
{
	b3_index     x,k,i,b;
	b3_count     count;
	b3_pkd_color bit;
	b3_u08       byte;

	// building new line by using
	// modified RLE
	x     =   0;
	bit   = 128;
	byte  =   0;
	k     =   0;
	count =   0;

	// Clear line
	for (x = 0;x < xBytes;x++) cPtr[x] = 0;

	for (k = 0;k < num;k += 2)
	{
		i      = count >> 3;
		bit    = m_Bits[count & 7];
		byte   = 0;
		for (b = 0;b < line[k];b++)
		{
			byte |= bit;
			bit   = bit >> 1;
			if (bit == 0)
			{
				cPtr[i++] |= byte;
				byte       = 0;
				bit        = 128;
			}
		}
		cPtr[i] |= byte;
		count   += line[k];
		count   += line[k+1];
	}
}

#ifdef SLOW_N_UGLY

// The Black Border Removal is like the rotation routines: Don't look
// to bits - look at least to bytes! If all bits in a byte are set, so
// they have cleared all without exception. Otherwise look in a table
// to replace the left most/right most bits and stop looping (meaning:
// there are cleared bits found and behind these bits there is nothing
// to clear!
//
// The following code computes a RLE code. This RLE code is built bitwise
// After correcting this RLE code in an efficient manner the bitmap must
// be reconstructed bitwise again.
//
// So: Using bit arithmetic is not what a CPU want.
//     Do at least byte arithmetic!
//
//                                                        (Blitting Image)

void b3Tx::b3RemoveBlackBorder()
{
	b3_count *line;
	b3_u08   *cPtr;
	b3_count  xBytes,num;
	b3_coord  y;

	if (!b3IsBW()) return;
	line = (b3_count *)b3Alloc(ySize * sizeof(b3_count));
	if (line == null)
	{
		throw b3TxException(B3_TX_MEMORY);
	}
	
	cPtr   = (b3_u08 *)data;
	xBytes = TX_BWA(xSize);
	for (y = 0;y < ySize;y++)
	{
		num = b3BuildRLE(line,cPtr);

		// Now! Start removing left black block
		if (num == 1)
		{
			num = 2;
			line[1] = line[0];
			line[0] = 0;
		}
		if (num > 1)
		{
			line[1] += line[0];
			line[0]  = 0;
		}
		if (num > 2)
		{
			if (num & 1)
			{
				num--;
				line[num - 1] += line[num];
			}
		}

		b3BuildRow (line,cPtr,num,xBytes);
		cPtr += xBytes;
	}
}

#else

class b3_tx_border
{
	static b3_tx_border TxBorder;

	/********************************
	  left border
	0XXXXXXX -> 0XXXXXXX (end case)
	10XXXXXX -> 00XXXXXX      "
	110XXXXX -> 000XXXXX      "
	1110XXXX -> 0000XXXX      "
	11110XXX ->	00000XXX      "
	111110XX -> 000000XX      "
	1111110X -> 0000000X      "
	11111110 -> 00000000      "
	11111111 -> 00000000 (loop case)
	*********************************/
	       b3_u08       lRemoval[256];

	/********************************
	  right border
	XXXXXXX0 -> XXXXXXX0 (end case)
	XXXXXX01 -> XXXXXX00      "
	XXXXX011 -> XXXXX000      "
	XXXX0111 -> XXXX0000      "
	XXX01111 -> XXX00000      "
	XX011111 -> XX000000      "
	X0111111 -> X0000000      "
	01111111 -> 00000000      "
	11111111 -> 00000000 (loop case)
	*********************************/
	       b3_u08       rRemoval[256];
	
	b3_tx_border()
	{
		b3_loop x,b;
		b3_u08  byte;

		for (x = 0;x < 256;x++)
		{
			// compute left mask
			byte = (b3_u08)x;
			for (b = 128;b != 0;b = b >> 1)
			{
				if (x & b)
				{
					byte -= b;
				}
				else
				{
					b = 0;
				}
			}
			lRemoval[x] = (b3_u08)(byte & 0xff);

			// compute right mask
			byte = (b3_u08)x;
			for (b = 1;b != 0;b = b << 1)
			{
				if (x & b)
				{
					byte -= b;
				}
				else
				{
					b = 0;
				}
			}
			rRemoval[x] = (b3_u08)(byte & 0xff);
		}
	}

	friend class b3Tx;
};

b3_tx_border b3_tx_border::TxBorder;

// That's fine here. It's warm and faaaast.

/********************************
Set right border for rotation
to prevent outside bits to be
converted into new image (memory
underflow!)

xSize & %1111 =  0: XXXXXXXX XXXXXXXX -> 11111111 11111111 (not used)
xSize & %1111 =  1: Xnnnnnnn nnnnnnnn -> 10000000 00000000
xSize & %1111 =  2: XXnnnnnn nnnnnnnn -> 11000000 00000000
xSize & %1111 =  3: XXXnnnnn nnnnnnnn -> 11100000 00000000
xSize & %1111 =  4: XXXXnnnn nnnnnnnn -> 11110000 00000000
xSize & %1111 =  5: XXXXXnnn nnnnnnnn -> 11111000 00000000
xSize & %1111 =  6: XXXXXXnn nnnnnnnn -> 11111100 00000000
xSize & %1111 =  7: XXXXXXXn nnnnnnnn -> 11111110 00000000

xSize & %1111 =  8: XXXXXXXX nnnnnnnn -> 11111111 00000000
xSize & %1111 =  9: XXXXXXXX Xnnnnnnn -> 11111111 10000000
xSize & %1111 = 10: XXXXXXXX XXnnnnnn -> 11111111 11000000
xSize & %1111 = 11: XXXXXXXX XXXnnnnn -> 11111111 11100000
xSize & %1111 = 12: XXXXXXXX XXXXnnnn -> 11111111 11110000
xSize & %1111 = 13: XXXXXXXX XXXXXnnn -> 11111111 11111000
xSize & %1111 = 14: XXXXXXXX XXXXXXnn -> 11111111 11111100
xSize & %1111 = 15: XXXXXXXX XXXXXXXn -> 11111111 11111110

********************************/
const b3_u08 b3Tx::m_RightMaskLeftByte[16] =
{
	0xff,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

const b3_u08 b3Tx::m_RightMaskRightByte[16] =
{
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe
};

/********************************
Set right border for right
correction (n=not inside image) in
case the pixel width is not
byte aligned

Xnnnnnnn -> X1111111
XXnnnnnn -> XX111111
XXXnnnnn -> XXX11111
XXXXnnnn -> XXXX1111
XXXXXnnn -> XXXXX111
XXXXXXnn -> XXXXXX11
XXXXXXXn -> XXXXXXX1
XXXXXXXX -> XXXXXXXX
********************************/
const b3_u08 b3Tx::m_RightBorder[] =
{
	0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01
};


// This is the nice version...

void b3Tx::b3RemoveBlackBorder()
{
	b3_coord x,y;
	b3_res   xEnd;
	b3_count xBytes;
	b3_u08   *cPtr;
	b3_bool   loop;

	// Do nothing if image has no width.
	if (!b3IsBW())
	{
		return;
	}
	if (xSize == 0)
	{
		return;
	}

	// Init some values...
	xBytes = TX_BWA(xSize);
	xEnd   = xSize >> 3;

	// Mask right border (set bits outside the image to black)
	cPtr = (b3_u08 *)data;
	x    = xSize & 3;
	if (x != 0)
	{
		for (y = 0;y < ySize;y++)
		{
			cPtr[xEnd] |= m_RightBorder[x];
			cPtr       += xBytes;
		}
	}			
	else
	{
		xEnd--;
	}

	// Do Black Border Removal!
	cPtr   = (b3_u08 *)data;
	for (y = 0;y < ySize;y++)
	{
		// remove left black border
		loop = true;
		x    = 0;
		do
		{
			loop    = cPtr[x] == 255;
			cPtr[x] = b3_tx_border::TxBorder.lRemoval[cPtr[x]];
			x++;
			if (x >= xBytes)
			{
				loop = false;
			}
		}
		while (loop);

		// remove right border
		loop    = true;
		x       = xEnd;
		do
		{
			loop    = cPtr[x] == 255;
			cPtr[x] = b3_tx_border::TxBorder.rRemoval[cPtr[x]];
			x--;
			if (x >= xBytes)
			{
				loop = false;
			}
		}
		while (loop);

		cPtr += xBytes;
	}
}

#endif

void b3Tx::b3Shrink(b3_count shrink)
{
	b3_coord  y;
	b3_count  xBytes,num;
	b3_index  k;
	b3_u08   *cPtr;
	b3_count *line;

	if (!b3IsBW()) return;
	line = (b3_count *)b3Alloc(xSize * sizeof(b3_count));
	if (line == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	cPtr   = (b3_u08 *)data;
	xBytes = TX_BWA(xSize);
	for (y = 0;y < ySize;y++)
	{
		num = b3BuildRLE(line,cPtr);

		// correcting RLE
		for (k = 0;k < num;k += 2)
		{
			if (line[k] > (b3_count)shrink)
			{
				line[k]   -= shrink;
				line[k+1] += shrink;
			}
			else
			{
				if (line[k] != 0)
				{
					b3_count diff;
		
					// Set block block to width of
					// one pixel and add the difference
					// to white block.
					diff       = line[k] - 1;
					line[k]    = 1;
					line[k+1] += diff;
				}
			}
		}

		b3BuildRow(line,cPtr,num,xBytes);

		// Ready. Incrementing some things...
		cPtr += xBytes;
	}
	b3Free (line);
}

/*************************************************************************
**                                                                      **
**                        b3Tx negate colors                            **
**                                                                      **
*************************************************************************/

// Negate image by turning all color bits to reverse.
void b3Tx::b3Negate()
{
	b3_pkd_color *lPtr;
	b3_u16       *sPtr;
	b3_u08       *cPtr;
	b3_count      xBytes,max;
	b3_index      i;
	b3_u08        cVal;

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3Negate()\n");
	if (b3IsBW())
	{
		cPtr   = (b3_u08 *)data;
		xBytes = TX_BWA(xSize);			 
		max    = xBytes * ySize;
		for (i = 0;i < max;i++)
		{
			cVal    = *cPtr;
			*cPtr++ = (b3_u08)(cVal ^ 0xff);
		}
	}

	switch(type)
	{
	case B3_TX_VGA:
		lPtr = palette;
		max  = 1 << depth;
		for (i = 0;i < max;i++)
		{
			*lPtr = *lPtr ^ 0xffffff;
			lPtr++;
		}
		break;

	case B3_TX_RGB4:
		sPtr = (b3_u16 *)data;
		max  = xSize * ySize;
		for (i = 0;i < max;i++)
		{
			*sPtr = (b3_u16)(*sPtr ^ 0xfff);
			sPtr++;
		}
		break;
		
	case B3_TX_RGB8:
		lPtr = (b3_pkd_color *)data;
		max  = xSize * ySize;
		for (i = 0;i < max;i++)
		{
			*lPtr = *lPtr ^ 0xffffff;
			lPtr++;
		}
		break;

	default:
		break;
	}
}

/*************************************************************************
**                                                                      **
**                        b3Tx turn right                               **
**                                                                      **
*************************************************************************/

#ifdef SLOW_N_UGLY

// Change This define to show, how a simple algorithm works. This one
// tests each bit for existance, but the CPU is optimized for at least
// one byte (= 8 pixel if you don't now...). So if a byte is zero then
// we have checked 8 pixel in only one CPU cycle! In Blizzard III we use 0-bits
// for white and 1-bits for black. The usual scan shows that only 2 percent
// of a page is black - a significant area is white. So the zero test hits
// very, very often. The other way, all bits in a byte set, we can process
// but will be found less in a page. An example for such a hit are patch
// codes or vertical aligned barcodes.
//
// So: We process a source scanline to convert them into a vertical scanrow.
//
//                                                          (Blitting Image)

void b3Tx::b3TurnRightILBM()
{
	b3_u08   *oldData,srcBit;
	b3_u08   *newData,dstBit,dstByte;
	b3_res    xNewSize,yNewSize;
	b3_count  xBytes,xNewBytes;
	b3_coord  srcPos,dstPos,srcStart,x,y;
	b3_u08   *ptrToFree;

	xBytes    = TX_BWA(xSize);
	xNewSize  = ySize;
	yNewSize  = xSize;
	xNewBytes = TX_BWA(xNewSize);
	dSize     = xNewBytes * yNewSize;
	newData   = (b3_u08 *)b3Alloc(dSize);
	if (newData == null)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TurnRightILBM() NOT ENOUGH MEMORY!\n");
		throw b3TxException(B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	ptrToFree = (b3_u08 *)data;
	data      = newData;

	// We compute the turned image by computing a horizontal
	// line of the new image. That is a vertical line of the old
	// image. Inside a new line computation the bit position does
	// not change concerning the vertical line (srcBit, srcPos).
	// The dstBit and dstPos pair floats over the new line as a
	// generating mask.
	srcBit    = 128;
	srcPos    =   0;
	for (y = 0;y < yNewSize;y++)
	{
		dstBit   = 128;
		dstPos   =   0;
		dstByte  =   0;
		srcStart = xBytes * (ySize - 1);
		for (x = 0;x < xNewSize;x++)
		{
			// copy bit from horizontal position into vertical
			// (The new horizontal line)
			if (oldData[srcPos + srcStart] & srcBit)
			{
				dstByte |= dstBit;
			}

			// increase values
			srcStart -= xBytes;
			dstBit    = dstBit >> 1;

			// handle destination bit underflow
			if (dstBit == 0)
			{
				newData[dstPos++] = dstByte;
				dstBit  = 128;
				dstByte =   0;
			}
		}
		// increase values
		newData += xNewBytes;
		srcBit = srcBit >> 1;

		// handle source bit underflow
		if (srcBit == 0)
		{
			srcBit = 128;
			srcPos++;
		}
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(ptrToFree);
}

#else

// This one is the fast one (The programmer was called "Blitting Image");

void b3Tx::b3TurnRightILBM()
{
	b3_u08   *oldData;
	b3_u08   *newData,dstBit,srcByte;
	b3_res    xNewSize,yNewSize;
	b3_count  xBytes,xNewBytes,xNewBlock;
	b3_coord  dstPos,x,y,srcBit;
	b3_u08   *ptrToFree;

	xBytes    = TX_BWA(xSize);
	xNewSize  = ySize;
	yNewSize  = xSize;
	xNewBytes = TX_BWA(xNewSize);
	dSize     = xNewBytes * yNewSize;
	newData   = (b3_u08 *)b3Alloc(dSize);
	if (newData == null)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TurnRightILBM() NOT ENOUGH MEMORY!\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	ptrToFree = (b3_u08 *)data;
	data      = newData;

	xNewBlock = xNewBytes << 3;
	for (y = ySize - 1;(long)y >= 0;y--)
	{
		// set start position
		dstPos = (b3_coord)(y >> 3);
		dstBit = m_Bits[y &  7];
		for (x = 0;x < xBytes;x++)
		{
			srcByte = oldData[x];
			switch (srcByte)
			{
			case   0 :
				// Special case
				// No bits set: copy nothing
				dstPos += xNewBlock;
				break;

			case 255 :
				// Special case:
				// All bits set: copy all bits without testing
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos += xNewBytes;
				break;

			default :
				// copy usual bit pattern
				for (srcBit = 128;srcBit != 0;srcBit = srcBit >> 1)
				{
					if (srcByte & srcBit)
					{
						newData[dstPos] |= dstBit;
					}
					dstPos += xNewBytes;
				}
				break;
			}
		}
		oldData += xBytes;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(ptrToFree);
}
#endif

void b3Tx::b3TurnRightVGA()
{
	b3_u08   *oldData;
	b3_u08   *newData;
	b3_res    xNewSize,yNewSize;
	b3_coord  srcPos,srcStart,srcInit,x,y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = (b3_u08 *)b3Alloc(dSize);
	if (newData == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	data      = newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0;y < yNewSize;y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0;x < xNewSize;x++)
		{
			*newData++ = oldData[srcStart];
			srcStart -= xSize;
		}
		srcPos++;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(oldData);
}

void b3Tx::b3TurnRightRGB4()
{
	b3_u16   *oldData;
	b3_u16   *newData;
	b3_res    xNewSize,yNewSize;
	b3_coord  srcPos,srcStart,srcInit,x,y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = (b3_u16 *)b3Alloc(dSize);
	if (newData == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u16 *)data;
	data      = (b3_u08 *)newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0;y < yNewSize;y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0;x < xNewSize;x++)
		{
			*newData++ = oldData[srcStart];
			srcStart -= xSize;
		}
		srcPos++;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(oldData);
}

void b3Tx::b3TurnRightRGB8()
{
	b3_pkd_color *oldData;
	b3_pkd_color *newData;
	b3_res        xNewSize,yNewSize;
	b3_coord      srcPos,srcStart,srcInit,x,y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = (b3_pkd_color *)b3Alloc(dSize);
	if (newData == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_pkd_color *)data;
	data      = (b3_u08 *)newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0;y < yNewSize;y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0;x < xNewSize;x++)
		{
			*newData++ = oldData[srcStart];
			srcStart -= xSize;
		}
		srcPos++;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(oldData);
}

void b3Tx::b3TurnRight()
{
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3Right()\n");
	switch (type)
	{
	case B3_TX_ILBM :
		b3TurnRightILBM();
		break;
	case B3_TX_VGA :
		b3TurnRightVGA();
		break;
	case B3_TX_RGB4 :
		b3TurnRightRGB4();
		break;
	case B3_TX_RGB8 :
		b3TurnRightRGB8();
		break;
	default:
		break;
	}
}

/*************************************************************************
**                                                                      **
**                        b3Tx turn left                                **
**                                                                      **
*************************************************************************/

// Excuse me for the missing comments here but this algorithm is the
// same like the TurnRightXXX part.

void b3Tx::b3TurnLeftILBM()
{
	b3_u08   *oldData;
	b3_u08   *newData,dstBit,srcByte;
	b3_res    xNewSize,yNewSize;
	b3_count  xBytes,xNewBytes,xNewBlock;
	b3_coord  dstPos,dstStart,x,y,srcBit,maskPos;
	b3_u08   *ptrToFree;

	xBytes    = TX_BWA(xSize);
	xNewSize  = ySize;
	yNewSize  = xSize;
	xNewBytes = TX_BWA(xNewSize);
	dSize     = xNewBytes * yNewSize;
	newData   = (b3_u08 *)b3Alloc(dSize);
	if (newData == null)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TurnLeftILBM() NOT ENOUGH MEMORY!\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	ptrToFree = (b3_u08 *)data;
	data      = newData;

	// Mask outside bits of last image
	// byte to be converted into new image
	// to prevent memory underflow.
	maskPos = xSize & 0xf;
	if ((maskPos != 0) && (xBytes >= 2))
	{
		b3_u08 *ptr;

		ptr = &oldData[xBytes - 2];
		for (y = 0;y < ySize;y++)
		{
			ptr[0] &= m_RightMaskLeftByte[maskPos];
			ptr[1] &= m_RightMaskRightByte[maskPos];
			ptr    += xBytes;
		}
	}

	dstStart  = (yNewSize - 1) * xNewBytes;
	xNewBlock = xNewBytes << 3;
	for (y = 0;y < ySize;y++)
	{
		// set start position
		dstPos = dstStart + (b3_coord)(y >> 3);
		dstBit = m_Bits[y &  7];
		for (x = 0;x < xBytes;x++)
		{
			srcByte = oldData[x];
			switch (srcByte)
			{
			case   0 :
				// Special case
				// No bits set: copy nothing
				dstPos -= xNewBlock;
				break;

			case 255 :
				// Special case:
				// All bits set: copy all bits without testing
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				newData[dstPos] |= dstBit;
				dstPos -= xNewBytes;
				break;

			default :
				// copy usual bit pattern
				for (srcBit = 128;srcBit != 0;srcBit = srcBit >> 1)
				{
					if (srcByte & srcBit)
					{
						newData[dstPos] |= dstBit;
					}
					dstPos -= xNewBytes;
				}
				break;
			}
		}
		oldData += xBytes;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(ptrToFree);
}

void b3Tx::b3TurnLeftVGA()
{
	b3_u08   *oldData;
	b3_u08   *newData;
	b3_res    xNewSize,yNewSize;
	b3_coord  srcPos,srcStart,x,y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = (b3_u08 *)b3Alloc(dSize);
	if (newData == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	data      = (b3_u08 *)newData;

	srcPos    = xSize - 1;
	for (y = 0;y < yNewSize;y++)
	{
		srcStart = srcPos;
		for (x = 0;x < xNewSize;x++)
		{
			*newData++  = oldData[srcStart];
			srcStart   += xSize;
		}
		srcPos--;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(oldData);
}

void b3Tx::b3TurnLeftRGB4()
{
	b3_u16   *oldData;
	b3_u16   *newData;
	b3_res    xNewSize,yNewSize;
	b3_coord  srcPos,srcStart,x,y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = (b3_u16 *)b3Alloc(dSize);
	if (newData == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u16 *)data;
	data      = (b3_u08 *)newData;

	srcPos    = xSize - 1;
	for (y = 0;y < yNewSize;y++)
	{
		srcStart = srcPos;
		for (x = 0;x < xNewSize;x++)
		{
			*newData++  = oldData[srcStart];
			srcStart   += xSize;
		}
		srcPos--;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(oldData);
}

void b3Tx::b3TurnLeftRGB8()
{
	b3_pkd_color *oldData;
	b3_pkd_color *newData;
	b3_res        xNewSize,yNewSize;
	b3_coord      srcPos,srcStart,x,y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = (b3_pkd_color *)b3Alloc(dSize);
	if (newData == null)
	{
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_pkd_color *)data;
	data      = (b3_u08 *)newData;

	srcPos    = xSize - 1;
	for (y = 0;y < yNewSize;y++)
	{
		srcStart = srcPos;
		for (x = 0;x < xNewSize;x++)
		{
			*newData++  = oldData[srcStart];
			srcStart   += xSize;
		}
		srcPos--;
	}
	xSize = xNewSize;
	ySize = yNewSize;
	b3Free(oldData);
}

void b3Tx::b3TurnLeft()
{
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3Left()\n");
	switch (type)
	{
	case B3_TX_ILBM :
		b3TurnLeftILBM();
		break;
	case B3_TX_VGA :
		b3TurnLeftVGA();
		break;
	case B3_TX_RGB4 :
		b3TurnLeftRGB4();
		break;
	case B3_TX_RGB8 :
		b3TurnLeftRGB8();
		break;
	default:
		break;
	}
}

/*************************************************************************
**                                                                      **
**                        b3Tx turn by 180 degrees                      **
**                                                                      **
*************************************************************************/

class b3_tx_turn
{
	static b3_tx_turn TxTurn;
	       b3_u08     Turnbytes[256];
	
	b3_tx_turn()
	{
		b3_coord bit,byte,turn;
		b3_loop  i;

		for(i = 0;i < 256;i++)
		{
			byte = (b3_u08)i;
			turn = 0;
			for (bit = 0;bit < 8;bit++)
			{
				turn  = turn << 1;
				turn |= byte &  1;
				byte  = byte >> 1;
			}
			Turnbytes[i] = (b3_u08)(turn & 0xff);
		}
	}

	friend class b3Tx;
};

b3_tx_turn b3_tx_turn::TxTurn;

void b3Tx::b3Turn()
{
	b3_u08        cBack,*cfPtr,*cbPtr,*cPtr;
	b3_u16        sBack,*sfPtr,*sbPtr;
	b3_pkd_color  lBack,*lfPtr,*lbPtr;
	b3_coord      y;
	b3_index      i,d;
	b3_count      max,size;

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3Turn()\n");
	size = xSize * ySize;
	max  = size >> 1;

	switch (type)
	{
	case B3_TX_ILBM :
		xSize = (xSize + 15) & 0xfffffff0;
		size = xSize >> 3;
		max  = size  >> 1;
		cPtr = data;
		// First: Turn horizontal
		for (y = 0;y < ySize;y++)
		{
			for (d = 0;d < depth;d++)
			{
				b3_coord x;

				// We turn two bytes at the same time
				// using tables. No bit nibbling is
				// needed here.
				//
				// We start from left and right simultanously.
				// Exchanging the bytes and turning the bits
				// inside these bytes using a lookup table.
				cfPtr = (b3_u08 *)cPtr;
				cbPtr = (b3_u08 *)(cfPtr + size);
				for (x = 0;x < max;x++)
				{
					cbPtr--;
					cBack  = b3_tx_turn::TxTurn.Turnbytes[*cbPtr];
					*cbPtr = b3_tx_turn::TxTurn.Turnbytes[*cfPtr];
					*cfPtr =  cBack;
					cfPtr++;
				}
				cPtr += size;
			}
		}

		// Second: Turn vertical
		max   = ySize >> 1;
		cfPtr = (b3_u08 *)data;
		cPtr  = (b3_u08 *)(data + size * ySize * depth);
		for (y = 0;y < max;y++)
		{
			cPtr -= (depth * size);
			for (d = 0;d < depth;d++)
			{
				b3_coord x;

				// Now exchange the two lines. Do it! Do it!
				cbPtr = (b3_u08 *)cPtr;
				for (x = 0;x < size;x++)
				{
					cBack  = *cbPtr;
					*cbPtr = *cfPtr;
					*cfPtr =  cBack;
					cfPtr++;
					cbPtr++;
				}
				cPtr += size;
			}
			cPtr -= (depth * size);
		}
		break;
	case B3_TX_VGA :
		cfPtr = (b3_u08 *)data;
		cbPtr = (b3_u08 *)(cfPtr + size);
		for (i = 0;i < max;i++)
		{
			cbPtr--;
			cBack  = *cbPtr;
			*cbPtr = *cfPtr;
			*cfPtr =  cBack;
			cfPtr++;
		}
		break;

	case B3_TX_RGB4 :
		sfPtr = (b3_u16 *)data;
		sbPtr = (b3_u16 *)(sfPtr + size);
		for (i = 0;i < max;i++)
		{
			sbPtr--;
			sBack  = *sbPtr;
			*sbPtr = *sfPtr;
			*sfPtr =  sBack;
			sfPtr++;
		}
		break;

	case B3_TX_RGB8 :
		lfPtr = (b3_pkd_color *)data;
		lbPtr = (b3_pkd_color *)(lfPtr + size);
		for (i = 0;i < max;i++)
		{
			lbPtr--;
			lBack  = *lbPtr;
			*lbPtr = *lfPtr;
			*lfPtr =  lBack;
			lfPtr++;
		}
		break;

	default:
		break;
	}
}

b3_bool b3Tx::b3TxGauss(
	long   xPos,
	long   yPos,
	b3_f64 scale,
	b3_f64 sigma,
	b3_f64 niveau,
	b3_f64 slope,
	b3Tx *src)
{
	b3_coord      x,xHalf;
	b3_coord      y,yHalf;
	b3_pkd_color *srcPtr,srcColor;
	b3_pkd_color *dstPtr,dstColor;
	b3_s32        r,g,b,sub;
	b3_f64        radius,xDiff,yDiff;
	b3_f64        value,level,denom;

	if (!b3IsTrueColor())
	{
		return false;
	}
	if (src == null)
	{
		src = this;
	}

	denom  = -2.0 * sigma * sigma;

	xHalf = xSize >> 1;
	yHalf = ySize >> 1;
	srcPtr = (b3_pkd_color *)src->b3GetData();
	dstPtr = (b3_pkd_color *)data;
	for (y = 0;y < (long)ySize;y++)
	{
		for (x = 0;x < (long)xSize;x++)
		{
			srcColor  = *srcPtr++;

			// Extract colors
			r = (srcColor & 0xff0000) >> 16;
			g = (srcColor & 0x00ff00) >>  8;
			b = (srcColor & 0x0000ff);

			// Computing distance to center
			xDiff  = (b3_f64)(x - xPos) / (b3_f64)xHalf;
			yDiff  = (b3_f64)(y - yPos) / (b3_f64)yHalf;
			radius = xDiff * xDiff + yDiff * yDiff;

			// Computing Gauss value and left to right ramp
			value  = exp(radius / denom) * scale;
			level  = xDiff * slope + niveau;

			// Subtract color values
			sub    = (long)(255 * (level - value));
			r     += sub;
			g     += sub;
			b     += sub;

			// Upper clip
			if (r <   0) r =   0;
			if (g <   0) g =   0;
			if (b <   0) b =   0;

			// Upper clip
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			// Combine colors
			dstColor = (b3_pkd_color)((r << 16) | (g << 8) | b);
			*dstPtr++ = dstColor;
		}
	}
	return true;
}

b3_bool b3Tx::b3TxTransformTable(
	b3_pkd_color *rTable,
	b3_pkd_color *gTable,
	b3_pkd_color *bTable,
	b3Tx         *srcTx)
{
	b3_pkd_color *srcPtr = null;
	b3_pkd_color *dstPtr = null;
	b3_index      num    = 0,i;
	b3_pkd_color  r,g,b,srcColor;

	if (srcTx == null)
	{
		srcTx = this;
	}

	if (b3IsPalette())
	{
		srcPtr = srcTx->b3GetPalette();
		dstPtr = palette;
		num    = 1 << depth;

		if (srcTx != this)
		{
			// We have to copy the data, too.
			// But only if srcTx and dstTx are different.
			memcpy(data,srcTx->b3GetData(),dSize);
		}
	}

	if (b3IsTrueColor())
	{

		srcPtr = (b3_pkd_color *)srcTx->b3GetData();
		dstPtr = (b3_pkd_color *)data;
		num    = xSize * ySize;
	}

	// Do the loop!
	for (i = 0;i < num;i++)
	{
		srcColor = *srcPtr++;

		// Extract colors
		r = rTable[(srcColor & 0xff0000) >> 16];
		g = gTable[(srcColor & 0x00ff00) >>  8];
		b = bTable[(srcColor & 0x0000ff)];

		// Combine colors
		*dstPtr++ = (r << 16) | (g << 8) | b;
	}

	// True if we have initialized info
	return num > 0;
}

b3_bool b3Tx::b3TxColorFilter(
	b3_f64  fr,
	b3_f64  fg,
	b3_f64  fb,
	b3Tx   *src)
{
	b3_pkd_color filter_r[256];
	b3_pkd_color filter_g[256];
	b3_pkd_color filter_b[256];
	b3_index     i;

	for (i = 0;i < 256;i++)
	{
		filter_r[i] = (b3_pkd_color)floor(i * fr + 0.5);
		filter_g[i] = (b3_pkd_color)floor(i * fg + 0.5);
		filter_b[i] = (b3_pkd_color)floor(i * fb + 0.5);
	}

	return b3TxTransformTable(
		filter_r,
		filter_g,
		filter_b,
		src);
}

b3_f64 b3Tx::b3Gamma(b3_f64 h,b3_f64 s,b3_f64 gamma,b3_f64 value,b3_f64 scale)
{
	b3_f64 diff = h - s;
	b3_f64 result;

	value /= scale;
	if (diff <= 0.0)
	{
		return value < s ? 0.0 : scale;
	}

	if (value <= s)
	{
		return 0.0;
	}
	if (value >= h)
	{
		return scale;
	}

	result = (value - s) / diff;
	return pow(result,1.0 / gamma) * scale;
}

b3_bool b3Tx::b3TxContrast(
	b3_f64  h,
	b3_f64  s,
	b3_f64  gamma,
	b3Tx   *src)
{
	b3_pkd_color correction_table[256];
	b3_index     i;

	// Build correction table
	for (i = 0;i < 256;i++)
	{
		correction_table[i] =
			(b3_pkd_color)floor(b3Gamma(h,s,gamma,i,255.0) + 0.5);
	}

	return b3TxTransformTable(
		correction_table,
		correction_table,
		correction_table,src);
}

b3_bool b3Tx::b3TxReduce(b3Tx *src)
{
	b3_index      i,p,count,index;
	b3_pkd_color *srcPtr = (b3_pkd_color *)src->b3GetData(),color;
	b3_u08       *dstPtr = (b3_u08 *)b3GetData();

	if (!src->b3IsTrueColor())
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx # b3TxReduce(): source image is no true color image!\n");
		return false;
	}

	if (!b3IsPalette())
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx # b3TxReduce(): destination image has no palette!\n");
		return false;
	}

	if ((src->xSize != xSize) || (src->ySize != ySize))
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx # b3TxReduce(): Both images have different resolution!\n");
		return false;
	}

	count = xSize * ySize;
	for (i = 0;i < count;i++)
	{
		b3_f32 dist_min = FLT_MAX;

		color = *srcPtr++;
		index = pSize;
		for (p = 0;p < pSize;p++)
		{
			b3Color srcColor(color);
			b3Color palColor(palette[p]);
			b3Color diff = srcColor - palColor;

			diff *= diff;

			b3_f32 dist = sqrt(diff[b3Color::R] + diff[b3Color::G] + diff[b3Color::B]);
			if (dist < dist_min)
			{
				dist_min = dist;
				index = p;
			}
		}
		*dstPtr++ = index;
	}
	return true;
}
