/*
**
**	$Filename:	b3TxImage.c $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some useful imaging routines (rotation)
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3TxInclude.h"
#include "b3TxAlgorithms.h"

#include "blz3/base/b3Color.h"
#include <float.h>

#define not_SLOW_N_UGLY

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
	b3_count * line,
	b3_u08  * cPtr)
{
	b3_count     count, num;
	b3_index     i, x;
	b3_pkd_color bit;
	bool         last, act;

	// building RLE code
	bit   = 128;
	i     = 0;
	last  = true;	// true = last bit set/false = last bit cleared
	// So: We start with black block!
	count = 0;
	num   = 0;
	for (x = 0; x < xSize; x++)
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
	b3_count * line,
	b3_u08  * cPtr,
	b3_count  num,
	b3_count  xBytes)
{
	b3_index     x, k, i, b;
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
	for (x = 0; x < xBytes; x++)
	{
		cPtr[x] = 0;
	}

	for (k = 0; k < num; k += 2)
	{
		i      = count >> 3;
		bit    = m_Bits[count & 7];
		byte   = 0;
		for (b = 0; b < line[k]; b++)
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
		count   += line[k + 1];
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
	b3_count * line;
	b3_u08  * cPtr;
	b3_count  xBytes, num;
	b3_coord  y;

	if (!b3IsBW())
	{
		return;
	}
	line = b3TypedAlloc<b3_count>(ySize);
	if (line == null)
	{
		throw b3TxException(B3_TX_MEMORY);
	}

	cPtr   = (b3_u08 *)data;
	xBytes = TX_BWA(xSize);
	for (y = 0; y < ySize; y++)
	{
		num = b3BuildRLE(line, cPtr);

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

		b3BuildRow(line, cPtr, num, xBytes);
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
		b3_loop x, b;
		b3_u08  byte;

		for (x = 0; x < 256; x++)
		{
			// compute left mask
			byte = (b3_u08)x;
			for (b = 128; b != 0; b = b >> 1)
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
			for (b = 1; b != 0; b = b << 1)
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
	0xff, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const b3_u08 b3Tx::m_RightMaskRightByte[16] =
{
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe
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
	0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01
};


// This is the nice version...

void b3Tx::b3RemoveBlackBorder()
{
	b3_coord x, y;
	b3_res   xEnd;
	b3_count xBytes;
	b3_u08  * cPtr;
	bool      loop;

	// Do nothing if image has no width.
	if (!b3IsBW())
	{
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
	}
	if (xSize == 0)
	{
		B3_THROW(b3TxException, B3_TX_WRONG_SIZE);
	}

	// Init some values...
	xBytes = TX_BWA(xSize);
	xEnd   = xSize >> 3;

	// Mask right border (set bits outside the image to black)
	cPtr = (b3_u08 *)data;
	x    = xSize & 3;
	if (x != 0)
	{
		for (y = 0; y < ySize; y++)
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
	for (y = 0; y < ySize; y++)
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

void b3Tx::b3Shrink(const b3_count shrink)
{
	b3_coord  y;
	b3_count  xBytes, num;
	b3_index  k;
	b3_u08  * cPtr;
	b3_count * line;

	if (!b3IsBW())
	{
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
	}
	line = b3TypedAlloc<b3_count>(xSize);
	if (line == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	cPtr   = data;
	xBytes = TX_BWA(xSize);
	for (y = 0; y < ySize; y++)
	{
		num = b3BuildRLE(line, cPtr);

		// correcting RLE
		for (k = 0; k < num; k += 2)
		{
			if (line[k] > (b3_count)shrink)
			{
				line[k]   -= shrink;
				line[k + 1] += shrink;
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
					line[k + 1] += diff;
				}
			}
		}

		b3BuildRow(line, cPtr, num, xBytes);

		// Ready. Incrementing some things...
		cPtr += xBytes;
	}
	b3Free(line);
}

/*************************************************************************
**                                                                      **
**                        b3Tx negate colors                            **
**                                                                      **
*************************************************************************/

// Negate image by turning all color bits to reverse.
bool b3Tx::b3Negate()
{
	b3_color   *   cPtr;
	b3_pkd_color * lPtr;
	b3_u16    *    sPtr;
	b3_u08    *    bPtr;
	b3_count       xBytes, max;
	b3_index       i;

	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Negate()\n");
	if (b3IsBW())
	{
		bPtr   = data;
		xBytes = TX_BWA(xSize);
		max    = xBytes * ySize;
		for (i = 0; i < max; i++)
		{
			*bPtr++ ^= 0xff;
		}
	}

	switch (type)
	{
	case B3_TX_VGA:
		lPtr = palette;
		max  = 1 << depth;
		for (i = 0; i < max; i++)
		{
			*lPtr++ ^= 0xffffff;
		}
		break;

	case B3_TX_RGB4:
		sPtr = data;
		max  = xSize * ySize;
		for (i = 0; i < max; i++)
		{
			*sPtr++ ^= 0xfff;
		}
		break;

	case B3_TX_RGB8:
		lPtr = data;
		max  = xSize * ySize;
		for (i = 0; i < max; i++)
		{
			*lPtr++ ^= 0xffffff;
		}
		break;

	case B3_TX_FLOAT:
		cPtr = data;
		max  = xSize * ySize;
		for (i = 0; i < max; i++)
		{
			cPtr->r = 1.0 - cPtr->r;
			cPtr->g = 1.0 - cPtr->g;
			cPtr->b = 1.0 - cPtr->b;
			cPtr++;
		}
		break;

	default:
		return false;
	}
	return true;
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
	b3_u08  * oldData, srcBit;
	b3_u08  * newData, dstBit, dstByte;
	b3_res    xNewSize, yNewSize;
	b3_count  xBytes, xNewBytes;
	b3_coord  srcPos, dstPos, srcStart, x, y;
	b3_u08  * ptrToFree;

	xBytes    = TX_BWA(xSize);
	xNewSize  = ySize;
	yNewSize  = xSize;
	xNewBytes = TX_BWA(xNewSize);
	dSize     = xNewBytes * yNewSize;
	newData   = b3TypedAlloc<b3_u08>(dSize);
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
	for (y = 0; y < yNewSize; y++)
	{
		dstBit   = 128;
		dstPos   =   0;
		dstByte  =   0;
		srcStart = xBytes * (ySize - 1);
		for (x = 0; x < xNewSize; x++)
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
	b3_u08  * oldData;
	b3_u08  * newData, dstBit, srcByte;
	b3_res    xNewSize, yNewSize;
	b3_count  xBytes, xNewBytes, xNewBlock;
	b3_coord  dstPos, x, y, srcBit;
	b3_u08  * ptrToFree;

	xBytes    = TX_BWA(xSize);
	xNewSize  = ySize;
	yNewSize  = xSize;
	xNewBytes = TX_BWA(xNewSize);
	dSize     = xNewBytes * yNewSize;
	newData   = b3TypedAlloc<b3_u08>(dSize);
	if (newData == nullptr)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TurnRightILBM() NOT ENOUGH MEMORY!\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	ptrToFree = (b3_u08 *)data;
	data      = newData;

	xNewBlock = xNewBytes << 3;
	for (y = ySize - 1; (long)y >= 0; y--)
	{
		// set start position
		dstPos = (b3_coord)(y >> 3);
		dstBit = m_Bits[y &  7];
		for (x = 0; x < xBytes; x++)
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
				for (srcBit = 128; srcBit != 0; srcBit = srcBit >> 1)
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
	b3_u08  * oldData;
	b3_u08  * newData;
	b3_res    xNewSize, yNewSize;
	b3_coord  srcPos, srcStart, srcInit, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_u08>(dSize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	data      = newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0; x < xNewSize; x++)
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
	b3_u16  * oldData;
	b3_u16  * newData;
	b3_res    xNewSize, yNewSize;
	b3_coord  srcPos, srcStart, srcInit, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_u16>(xSize * ySize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u16 *)data;
	data      = (b3_u08 *)newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0; x < xNewSize; x++)
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
	b3_pkd_color * oldData;
	b3_pkd_color * newData;
	b3_res        xNewSize, yNewSize;
	b3_coord      srcPos, srcStart, srcInit, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_pkd_color>(xSize * ySize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = data;
	data      = newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0; x < xNewSize; x++)
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

void b3Tx::b3TurnRightFloat()
{
	b3_color * oldData;
	b3_color * newData;
	b3_res    xNewSize, yNewSize;
	b3_coord  srcPos, srcStart, srcInit, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_color>(xSize * ySize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_color *)data;
	data      = (b3_u08 *)newData;

	srcPos    = 0;
	srcInit   = xSize * (ySize - 1);
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcInit + srcPos;
		for (x = 0; x < xNewSize; x++)
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
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Right()\n");
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
	case B3_TX_FLOAT :
		b3TurnRightFloat();
		break;

	default:
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
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
	b3_u08  * oldData;
	b3_u08  * newData, dstBit, srcByte;
	b3_res    xNewSize, yNewSize;
	b3_count  xBytes, xNewBytes, xNewBlock;
	b3_coord  dstPos, dstStart, x, y, srcBit, maskPos;
	b3_u08  * ptrToFree;

	xBytes    = TX_BWA(xSize);
	xNewSize  = ySize;
	yNewSize  = xSize;
	xNewBytes = TX_BWA(xNewSize);
	dSize     = xNewBytes * yNewSize;
	newData   = b3TypedAlloc<b3_u08>(dSize);
	if (newData == nullptr)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TurnLeftILBM() NOT ENOUGH MEMORY!\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
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
		b3_u08 * ptr;

		ptr = &oldData[xBytes - 2];
		for (y = 0; y < ySize; y++)
		{
			ptr[0] &= m_RightMaskLeftByte[maskPos];
			ptr[1] &= m_RightMaskRightByte[maskPos];
			ptr    += xBytes;
		}
	}

	dstStart  = (yNewSize - 1) * xNewBytes;
	xNewBlock = xNewBytes << 3;
	for (y = 0; y < ySize; y++)
	{
		// set start position
		dstPos = dstStart + (b3_coord)(y >> 3);
		dstBit = m_Bits[y &  7];
		for (x = 0; x < xBytes; x++)
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
				for (srcBit = 128; srcBit != 0; srcBit = srcBit >> 1)
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
	b3_u08  * oldData;
	b3_u08  * newData;
	b3_res    xNewSize, yNewSize;
	b3_coord  srcPos, srcStart, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_u08>(dSize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u08 *)data;
	data      = (b3_u08 *)newData;

	srcPos    = xSize - 1;
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcPos;
		for (x = 0; x < xNewSize; x++)
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
	b3_u16  * oldData;
	b3_u16  * newData;
	b3_res    xNewSize, yNewSize;
	b3_coord  srcPos, srcStart, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_u16>(xSize * ySize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_u16 *)data;
	data      = (b3_u08 *)newData;

	srcPos    = xSize - 1;
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcPos;
		for (x = 0; x < xNewSize; x++)
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
	b3_pkd_color * oldData;
	b3_pkd_color * newData;
	b3_res        xNewSize, yNewSize;
	b3_coord      srcPos, srcStart, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_pkd_color>(xSize * ySize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = (b3_pkd_color *)data;
	data      = (b3_u08 *)newData;

	srcPos    = xSize - 1;
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcPos;
		for (x = 0; x < xNewSize; x++)
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

void b3Tx::b3TurnLeftFloat()
{
	b3_color * oldData;
	b3_color * newData;
	b3_res    xNewSize, yNewSize;
	b3_coord  srcPos, srcStart, x, y;

	xNewSize  = ySize;
	yNewSize  = xSize;
	newData   = b3TypedAlloc<b3_color>(xSize * ySize);
	if (newData == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	// change data pointer
	oldData   = data;
	data      = newData;

	srcPos    = xSize - 1;
	for (y = 0; y < yNewSize; y++)
	{
		srcStart = srcPos;
		for (x = 0; x < xNewSize; x++)
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
	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Left()\n");
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
	case B3_TX_FLOAT :
		b3TurnLeftFloat();
		break;

	default:
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
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
		b3_coord bit, byte, turn;
		b3_loop  i;

		for (i = 0; i < 256; i++)
		{
			byte = (b3_u08)i;
			turn = 0;
			for (bit = 0; bit < 8; bit++)
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
	b3_u08        bBack, *bfPtr, *bbPtr, *bPtr;
	b3_u16        sBack, *sfPtr, *sbPtr;
	b3_pkd_color  lBack, *lfPtr, *lbPtr;
	b3_color      cBack, *cfPtr, *cbPtr;
	b3_coord      y;
	b3_index      i, d;
	b3_count      max, size;

	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Turn()\n");
	size = xSize * ySize;
	max  = size >> 1;

	switch (type)
	{
	case B3_TX_ILBM :
		xSize = (xSize + 15) & 0xfffffff0;
		size = xSize >> 3;
		max  = size  >> 1;
		bPtr = data;
		// First: Turn horizontal
		for (y = 0; y < ySize; y++)
		{
			for (d = 0; d < depth; d++)
			{
				b3_coord x;

				// We turn two bytes at the same time
				// using tables. No bit nibbling is
				// needed here.
				//
				// We start from left and right simultanously.
				// Exchanging the bytes and turning the bits
				// inside these bytes using a lookup table.
				bfPtr = (b3_u08 *)bPtr;
				bbPtr = (b3_u08 *)(bfPtr + size);
				for (x = 0; x < max; x++)
				{
					bbPtr--;
					bBack  = b3_tx_turn::TxTurn.Turnbytes[*bbPtr];
					*bbPtr = b3_tx_turn::TxTurn.Turnbytes[*bfPtr];
					*bfPtr =  bBack;
					bfPtr++;
				}
				bPtr += size;
			}
		}

		// Second: Turn vertical
		max   = ySize >> 1;
		bfPtr = (b3_u08 *)data;
		bPtr  = (b3_u08 *)(data + size * ySize * depth);
		for (y = 0; y < max; y++)
		{
			bPtr -= (depth * size);
			for (d = 0; d < depth; d++)
			{
				b3_coord x;

				// Now exchange the two lines. Do it! Do it!
				bbPtr = (b3_u08 *)bPtr;
				for (x = 0; x < size; x++)
				{
					bBack  = *bbPtr;
					*bbPtr = *bfPtr;
					*bfPtr =  bBack;
					bfPtr++;
					bbPtr++;
				}
				bPtr += size;
			}
			bPtr -= (depth * size);
		}
		break;

	case B3_TX_VGA :
		bfPtr = (b3_u08 *)data;
		bbPtr = (b3_u08 *)(bfPtr + size);
		for (i = 0; i < max; i++)
		{
			bbPtr--;
			bBack  = *bbPtr;
			*bbPtr = *bfPtr;
			*bfPtr =  bBack;
			bfPtr++;
		}
		break;

	case B3_TX_RGB4 :
		sfPtr = (b3_u16 *)data;
		sbPtr = (b3_u16 *)(sfPtr + size);
		for (i = 0; i < max; i++)
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
		for (i = 0; i < max; i++)
		{
			lbPtr--;
			lBack  = *lbPtr;
			*lbPtr = *lfPtr;
			*lfPtr =  lBack;
			lfPtr++;
		}
		break;

	case B3_TX_FLOAT :
		cfPtr = (b3_color *)data;
		cbPtr = (b3_color *)(cfPtr + size);
		for (i = 0; i < max; i++)
		{
			cbPtr--;
			cBack  = *cbPtr;
			*cbPtr = *cfPtr;
			*cfPtr =  cBack;
			cfPtr++;
		}
		break;

	default:
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
	}
}

bool b3Tx::b3TxGauss(
	long   xPos,
	long   yPos,
	b3_f64 scale,
	b3_f64 sigma,
	b3_f64 niveau,
	b3_f64 slope,
	const b3Tx * src)
{
	if (src == nullptr)
	{
		src = this;
	}
	if ((src->xSize != xSize) || (src->ySize != ySize))
	{
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
	}

	switch (type)
	{
	case B3_TX_RGB8:
		switch (src->type)
		{
		case B3_TX_VGA:
			b3TxAlgorithms::b3Gauss<b3_u08, b3_pkd_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope,
				[src] (const b3_u08 index)
			{
				return src->palette[index];
			});
			break;
		case B3_TX_RGB4:
			b3TxAlgorithms::b3Gauss<b3_u16, b3_pkd_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope);
			break;
		case B3_TX_RGB8:
			b3TxAlgorithms::b3Gauss<b3_pkd_color, b3_pkd_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope);
			break;
		case B3_TX_FLOAT:
			b3TxAlgorithms::b3Gauss<b3_color, b3_pkd_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope);
			break;

		default:
			B3_THROW(b3TxException, B3_TX_UNSUPP);
		}
		break;

	case B3_TX_FLOAT:
		switch (src->type)
		{
		case B3_TX_VGA:
			b3TxAlgorithms::b3Gauss<b3_u08, b3_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope,
				[src] (const b3_u08 index)
			{
				return src->palette[index];
			});
			break;
		case B3_TX_RGB4:
			b3TxAlgorithms::b3Gauss<b3_u16, b3_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope);
			break;
		case B3_TX_RGB8:
			b3TxAlgorithms::b3Gauss<b3_pkd_color, b3_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope);
			break;
		case B3_TX_FLOAT:
			b3TxAlgorithms::b3Gauss<b3_color, b3_color>(
				src,   this,
				xPos,  yPos,
				scale, sigma, niveau, slope);
			break;

		default:
			B3_THROW(b3TxException, B3_TX_UNSUPP);
		}
		break;

	default:
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
	return true;
}

bool b3Tx::b3TxTransformTable(
	b3_pkd_color * rTable,
	b3_pkd_color * gTable,
	b3_pkd_color * bTable,
	const b3Tx  *  srcTx)
{
	b3_pkd_color * srcPtr = nullptr;
	b3_pkd_color * dstPtr = nullptr;
	b3_index       num    = 0, i;
	b3_pkd_color   r, g, b, srcColor;

	if (srcTx == nullptr)
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
			memcpy(data, srcTx->b3GetVoidData(), dSize);
		}
	}

	if (b3IsTrueColor())
	{

		srcPtr = srcTx->b3GetTrueColorData();
		dstPtr = b3GetTrueColorData();
		num    = xSize * ySize;
	}

	// Do the loop!
	for (i = 0; i < num; i++)
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

bool b3Tx::b3TxColorFilter(
	b3_f64  fr,
	b3_f64  fg,
	b3_f64  fb,
	const b3Tx  * src)
{
	b3_pkd_color filter_r[256];
	b3_pkd_color filter_g[256];
	b3_pkd_color filter_b[256];
	b3_index     i;

	for (i = 0; i < 256; i++)
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

b3_f64 b3Tx::b3Gamma(b3_f64 h, b3_f64 s, b3_f64 gamma, b3_f64 value, b3_f64 scale)
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
	return pow(result, 1.0 / gamma) * scale;
}

bool b3Tx::b3TxContrast(
	b3_f64  h,
	b3_f64  s,
	b3_f64  gamma,
	const b3Tx  * src)
{
	b3_pkd_color correction_table[256];
	b3_index     i;

	// Build correction table
	for (i = 0; i < 256; i++)
	{
		correction_table[i] =
			(b3_pkd_color)round(b3Gamma(h, s, gamma, i, 255.0));
	}

	return b3TxTransformTable(
			correction_table,
			correction_table,
			correction_table, src);
}

bool b3Tx::b3TxReduce(const b3Tx * src)
{
	b3_index       i, p, count, index;
	b3_pkd_color * srcPtr = src->b3GetTrueColorData(), color;
	b3_u08    *    dstPtr = b3GetIndexData();

	if (!src->b3IsTrueColor())
	{
		b3PrintF(B3LOG_NORMAL, "### CLASS: b3Tx # b3TxReduce(): source image is no true color image!\n");
		return false;
	}

	if (!b3IsPalette())
	{
		b3PrintF(B3LOG_NORMAL, "### CLASS: b3Tx # b3TxReduce(): destination image has no palette!\n");
		return false;
	}

	if ((src->xSize != xSize) || (src->ySize != ySize))
	{
		b3PrintF(B3LOG_NORMAL, "### CLASS: b3Tx # b3TxReduce(): Both images have different resolution!\n");
		return false;
	}

	count = xSize * ySize;
	for (i = 0; i < count; i++)
	{
		b3_f32 dist_min = FLT_MAX;

		color = *srcPtr++;
		index = pSize;
		for (p = 0; p < pSize; p++)
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
