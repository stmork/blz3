/*
**
**	$Filename:	b3TxScale.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (scaling)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#define not_SLOW

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"
#include "blz3/system/b3Log.h"

#include <math.h>
#include <malloc.h>

typedef struct
{
	b3_index     *rIndex;
	b3_index     *cIndex;
	b3_res        yMin,yMax;
	b3_coord      xSizeSrc,xSizeDst;
	b3_coord      ySizeSrc,ySizeDst;
	b3_pkd_color  pal[2];
	b3_tx_type    dstType;
	union
	{
		b3_u08       *cData;
		b3_pkd_color *lData;
	}                  src;
	union
	{
		b3_u08       *cData;
		b3_pkd_color *lData;
	}                  dst;
} b3RectInfo;

#define CPU_MAX      4
#define TX_PRIO      0

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/07/01 17:04:09  sm
**	- Solved some signed/unsigned mismatches
**
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
**	Revision 1.6  2000/10/11 11:43:24  smork
**	- Some updates done in b3DocBase
**	- b3TxTranformTable used for filtering and contrasting. It's
**	  much more faster now.
**	- Why blocks the image output on Blizzard III Scan when scanning?
**	
**	Revision 1.5  2000/10/02 08:12:52  smork
**	- Scanning B/W images with right palette (0xffffff/0x000000)
**	
**	Revision 1.4  2000/09/22 08:51:03  smork
**	- Try to compile libb3Tool.a (Un*x)
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
**                        some constant tables                          **
**                                                                      **
*************************************************************************/

static b3CPU        cpu;
static b3_pkd_color quad256[512];
static b3_bool      quadValid = false;

/*************************************************************************
**                                                                      **
**                        color index computing                         **
**                                                                      **
*************************************************************************/

b3ColorIndices::b3ColorIndices()
{
	unsigned long size = 256;

	num     = 0;
	indices = (b3_index *)b3Alloc (size * sizeof(b3_index));
	max     = (indices ? size : 0);
}

void b3ColorIndices::b3AddColorIndex(b3_index index)
{
	if (num < max)
	{
		indices[num++] = index;
	}
}

b3_index b3ColorIndices::b3ColorIndex(
	b3_pkd_color *palette,
	b3_pkd_color  color)
{
	b3_s32   r,rDiff;
	b3_s32   g,gDiff;
	b3_s32   b,bDiff;
	b3_s32   diff,actDiff;
	b3_index i,index = 0,act,pos;

	diff = 65536 * 3 + 1;
	r    = (b3_s32)((color & 0xff0000) >> 16);
	g    = (b3_s32)((color & 0x00ff00) >>  8);
	b    = (b3_s32)((color & 0x0000ff));
	for (i = 0;i < num;i++)
	{
		// Get actual color
		pos = indices[i];
		act = palette[pos];

		// Compute difference between "color" and test color "act".
		rDiff = (b3_s32)((act & 0xff0000) >> 16) - r;
		gDiff = (b3_s32)((act & 0x00ff00) >>  8) - g;
		bDiff = (b3_s32)((act & 0x0000ff))       - b;

		// Compute difference and test
		actDiff  =
			quad256[rDiff + 256] +
			quad256[gDiff + 256] +
			quad256[bDiff + 256];
		if (actDiff < diff)
		{
			// take smallest difference
			diff  = actDiff;
			index = pos;
		}
	}
	return index;
}

/*************************************************************************
**                                                                      **
**                        division table                                **
**                                                                      **
*************************************************************************/

static b3_bool          divTableValid = false;

#if 1

#define B3_TX_MAX_DIVIDENT_BITS 12
#define B3_TX_MAX_DIVIDENT      (1 << B3_TX_MAX_DIVIDENT_BITS)
#define B3_TX_MAX_DIVISOR_BITS   4
#define B3_TX_MAX_DIVISOR       (1 << B3_TX_MAX_DIVISOR_BITS)
#define B3_TX_DIV_INDEX(value,divisor) (((value) << B3_TX_MAX_DIVISOR_BITS) | (divisor))
#define B3_TX_DIV(value,divisor)\
	((((value) < B3_TX_MAX_DIVIDENT) && ((divisor) < B3_TX_MAX_DIVISOR)) ?\
		divTable[B3_TX_DIV_INDEX(value,divisor)] :\
		((value) / (divisor)))
#define B3_TX_DIV2(value,divisor)\
	(((value) < B3_TX_MAX_DIVIDENT) ?\
		divTable[B3_TX_DIV_INDEX(value,divisor)] :\
		((value) / (divisor)))
#define B3_TX_MUL255_DIV(value,divisor)\
	((((value) < B3_TX_MAX_DIVIDENT) && ((divisor) < B3_TX_MAX_DIVISOR)) ?\
		divTable255[B3_TX_DIV_INDEX(value,divisor)] :\
		((value) * 255 / (divisor)))
			
static b3_u08 divTable[B3_TX_MAX_DIVIDENT * B3_TX_MAX_DIVISOR];
static b3_u08 divTable255[B3_TX_MAX_DIVIDENT * B3_TX_MAX_DIVISOR];

void b3Tx::b3DivTableInit()
{
	b3_u32 value,divisor,result;

	for (value = 1;value < B3_TX_MAX_DIVIDENT;value++)
	{
		divTable[B3_TX_DIV_INDEX(value,0)] = 0xff;
		for (divisor = 1;divisor < B3_TX_MAX_DIVISOR;divisor++)
		{
			result = value / divisor;
			divTable[B3_TX_DIV_INDEX(value,divisor)] = (b3_u08)(result > 0xff ? 0xff : result);

			result = value * 255/ divisor;
			divTable255[B3_TX_DIV_INDEX(value,divisor)] = (b3_u08)(result > 0xff ? 0xff : result);
		}
	}
	divTableValid = true;
}

#else

#define B3_TX_DIV(value,divisor)        ((value) / (divisor))
#define B3_TX_DIV2(value,divisor)       ((value) / (divisor))
#define B3_TX_MUL255_DIV(value,divisor) ((value) * 255 / (divisor))

void b3Tx::b3DivTableInit()
{
	divTableValid = true;
}

#endif

/*************************************************************************
**                                                                      **
**                        bit table                                     **
**                                                                      **
*************************************************************************/

// Bit ordering in a byte. We see onto the byte starting from
// left. So the first bit is the left most bit (No. 7)
static const b3_u08 Bits[8] =
{
	128,64,32,16,8,4,2,1
};

// Now: Isn't it magic what the "Pixel Wizard" is doing here? If you
// don't know what's going up here. Don't matter! I want to explain it
// to you now.
//
// But first: You must have the right feeling to avoid memory accesses!
//            Don't use too much "bit nibbling"!
//            Try to use at least byte accesses!
//            Avoid floating point operations because of their memory needs!
//            The more memory you need the more time you need!
//
// NOTE: If you find the 0.35/0.51/0.14 triple this is the sensivity
//       of your eyes divided into red/green/blue. Yes - the most
//       sensivity is in the green spectral area. We need this triple
//       for converting color into grey scale. Everytime you see this
//       triple there is converted color into grey.

/*************************************************************************
**                                                                      **
**                        bit counting...                               **
**                                                                      **
*************************************************************************/

#define BOOM

static b3_bool TxBitCountValid = false;
static b3_u08  TxBitCount[256];
static b3_u08  TxBitCountInv[256];
static b3_u08  TxMaskLeft[8] =
{
	0xff,		// 11111111
	0x7f,		// 01111111
	0x3f,		// 00111111
	0x1f,		// 00011111
	0x0f,		// 00001111
	0x07,		// 00000111
	0x03,		// 00000011
	0x01        // 00000001
};

static b3_u08 TxMaskRight[8] =
{
	0x00,		// 00000000
	0x80,		// 10000000
	0xc0,		// 11000000
	0xe0,		// 11100000
	0xf0,		// 11110000
	0xf8,		// 11111000
	0xfc,		// 11111100
	0xfe		// 11111110
};

static void BitCount()
{
	b3_index i;
	b3_u32   bit;
	b3_u08   count;

	if (!TxBitCountValid)
	{
		// For each possible value...
		for (i = 0;i < 256;i++)
		{
			count = 0;
			for (bit = 128;bit != 0;bit = bit >> 1)
			{
				if (i & bit)
				{
					count++;
				}
			}

			// Set number of set bits for each possible value [0;255]
			TxBitCount[i]    = count;
			TxBitCountInv[i] = (b3_u08)(8 - count);
		}

		// Now we can use the tables
		TxBitCountValid = true;
	}
}

#ifndef SLOW
// NOTE:
// Now the fast part begins here...

// If a line is to be scaled up (means: destination is bigger)...
static void b3ComputeLineBigger(
	b3_count *TxRowCounter,
	b3_count *TxRowCells,
	b3_count *rIndex,
	b3_u08   *src,
	b3_res    xDstSize)
{
	b3_coord xDst;
	b3_index index;

	for (xDst = 0;xDst < xDstSize;xDst++)
	{
		index = rIndex[xDst];
		if (src[index >> 3] & Bits[index & 7])
		{
			TxRowCounter[xDst]++;
		}
		TxRowCells[xDst]++;
	}
}

/*
** The "src" is the B/W source bitmap. One interval results into one single
** pixel in the destination image. The grey value is the quotient of the number
** of bits set to one inside the interval and the whole bit width of the
** interval. This quotient is multiplied by 255 to get the full range of grey
** palette. Due to the fact that any scaling factor is possible there can be any
** interval from any bit position inside a byte. So the start byte and end byte
** must be masked for bit counting. The bit counting is done by a table lookup
** which is faster than counting the bits itself and "eats" only 256 bytes of
** memory.
**
** Here are some examples:
** 
** src:  10010010 10010101 01111010 01010001 01001001 01101001 01001000 10100010
**
** *****************************************************************************
** This case is used if start byte and end byte is the same (1)
**
** interval:                [    ]
** mask left:              01111111
** mask right:             11111110 
** mask:                   01111110
** result:                 01111010
** count:                         5                                    =  5/ 6
**
** *****************************************************************************
** This is otherwise (2)
** 
** interval:                [           ]
** mask left:              01111111
** mask right:                      11111110 
** mask:                   01111111 11111110
** result:                 01111010 01010000
** count:                         5        2                           =  7/14
**
** interval:                [                             ]
** mask left:              01111111
** mask right:                                        11111000 
** mask:                   01111111 11111111 11111111 11111000
** result:                 01111010 01010001 01001001 01101000
** count:                         5        3        3        3         = 14/28
*/

// If a line is to be scaled down (means: destination is smaller)...
static void b3ComputeLineSmaller(
	b3_count *TxRowCounter,
	b3_count *TxRowCells,
	b3_count *rIndex,
	b3_u08   *src,
	b3_res    xDstSize)
{
	b3_coord  xDst;
	b3_res    xStart,xEnd,xByteStart,xByteEnd;
	b3_u08    mask;

	xEnd = rIndex[0];
	for (xDst = 0;xDst < xDstSize;xDst++)
	{
		xStart            = xEnd;
		xEnd              = rIndex[xDst + 1];
		TxRowCells[xDst] += (xEnd - xStart);

		// compute byte index;
		xByteStart = xStart >> 3;
		xByteEnd   = xEnd   >> 3;

		if (xByteStart == xByteEnd)
		{
			// Case (1)
			// start byte and end byte is the same byte
			mask = (b3_u08)(TxMaskLeft[xStart & 7] & TxMaskRight[xEnd & 7]);
			TxRowCounter[xDst] += TxBitCount[src[xByteStart] & mask];
		}
		else
		{
			// Case (2)
			// Start byte and end byte are not the same.
			// Process start byte first
			// Process the interval between start byte and end byte
			// Process end byte

			// Start byte
			mask = TxMaskLeft[xStart & 7];
			TxRowCounter[xDst] += TxBitCount[src[xByteStart] & mask];

			// count unmasked bytes between border...
			xByteStart++;
			while (xByteStart < xByteEnd)
			{
				TxRowCounter[xDst] += TxBitCount[src[xByteStart]];
				xByteStart++;
			}

			// End byte
			mask = TxMaskRight[xEnd & 7];
			if (mask != 0)
			{
				// xByteStart =!= xByteEnd
				TxRowCounter[xDst] += TxBitCount[src[xByteStart] & mask];
			}
		}
	}
}

static unsigned int b3ScaleBW2Grey(void *ptr)
{
	b3RectInfo   *RectInfo;
	b3_tx_type    dstType;
	b3_u08       *src;
	b3_u08       *cDst;
	b3_pkd_color *lDst;
	b3_count     *rIndex;
	b3_count     *cIndex;
	b3_count     *TxRowCounter;
	b3_count     *TxRowCells;
	b3_res        xSrcSize;
	b3_res        ySrcSize,ySrc;
	b3_res        xDstSize,xDst;
	b3_res        yDstSize,yDst;
	b3_count      srcBytes,dstBytes;
	b3_res        yMin,yMax;
	void          (*ComputeLine)(
		b3_count *TxRowCounter,
		b3_count *TxRowCells,
		b3_count *rIndex,
		b3_u08   *src,
		b3_res    dstSize);

	// Init some tables
	BitCount();

	// ... and some values
	RectInfo = (b3RectInfo *)ptr;

	dstType  =  RectInfo->dstType;
	rIndex   =  RectInfo->rIndex;
	cIndex   =  RectInfo->cIndex;
	src      =  RectInfo->src.cData;
	yMin     =  RectInfo->yMin;
	yMax     =  RectInfo->yMax;
	xSrcSize =  RectInfo->xSizeSrc;
	xDstSize =  RectInfo->xSizeDst;
	ySrcSize =  RectInfo->ySizeSrc;
	yDstSize =  RectInfo->ySizeDst;
	srcBytes = TX_BWA(RectInfo->xSizeSrc);
	dstBytes =  RectInfo->xSizeDst;
	cDst     = &RectInfo->dst.cData[yMin * dstBytes];
	lDst     = &RectInfo->dst.lData[yMin * dstBytes];

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3ScaleBW2Grey(%5ld - %5ld)\n",
		yMin,yMax);

	// Alloc some memory
	TxRowCounter = (b3_count *)malloc(xDstSize * sizeof(b3_count));
	if (TxRowCounter == null)
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx   # b3ScaleBW2Grey(): "
			"Not enough memory for row counter\n");
		return 0;
	}
	TxRowCells   = (b3_count *)malloc(xDstSize * sizeof(b3_count));
	if (TxRowCells == null)
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx   # b3MakeItGrey(): "
			"Not enough memory for row cell sizes\n");
		free(TxRowCounter);
		return 0;
	}

	// Select right line computation
	ComputeLine = (xDstSize < xSrcSize ? b3ComputeLineSmaller : b3ComputeLineBigger);

	if (yDstSize < ySrcSize)
	{
		// Destination height is smaller (scale down)
		for (yDst = yMin;yDst < yMax;yDst++)
		{
			// clean row counter
			for (xDst = 0;xDst < xDstSize;xDst++)
			{
				TxRowCounter[xDst] = 0;
				TxRowCells[xDst]   = 0;
			}

			// scan interval
			for (ySrc = cIndex[yDst];ySrc < cIndex[yDst + 1];ySrc++)
			{
				ComputeLine(TxRowCounter,TxRowCells,rIndex,&src[ySrc * srcBytes],xDstSize);
			}

			// compute grey indices
			switch(dstType)
			{
			case B3_TX_VGA:
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					if (TxRowCells[xDst] > 0)
					{
						*cDst = (b3_u08)(255 - B3_TX_MUL255_DIV(TxRowCounter[xDst],TxRowCells[xDst]));
					}
					cDst++;
				}
				break;

			case B3_TX_RGB8:
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					if (TxRowCells[xDst] > 0)
					{
						b3_u08 value;

						value = (b3_u08)(255 - B3_TX_MUL255_DIV(TxRowCounter[xDst],TxRowCells[xDst]));
						*lDst = 0x010101 * value;
					}
					lDst++;
				}
				break;
			}
		}
	}
	else
	{
		// Destination height is bigger (scale up)
		for (yDst = yMin;yDst < yMax;yDst++)
		{
			// Clean row counter
			for (xDst = 0;xDst < xDstSize;xDst++)
			{
				TxRowCounter[xDst] = 0;
				TxRowCells[xDst]   = 0;
			}

			// Compute Line...
			ComputeLine(TxRowCounter,TxRowCells,rIndex,&src[cIndex[yDst] * srcBytes],xDstSize);

			// Compute grey indices
			switch(dstType)
			{
			case B3_TX_VGA:
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					if (TxRowCells[xDst] > 0)
					{
						*cDst = (b3_u08)(255 - B3_TX_MUL255_DIV(TxRowCounter[xDst],TxRowCells[xDst]));
					}
					cDst++;
				}
				break;

			case B3_TX_RGB8:
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					if (TxRowCells[xDst] > 0)
					{
						b3_u08 value;

						value = (b3_u08)(255 - B3_TX_MUL255_DIV(TxRowCounter[xDst],TxRowCells[xDst]));
						*lDst = 0x010101 * value;
					}
					lDst++;
				}
				break;
			}
		}
	}

	// Free willie
	free (TxRowCounter);
	free (TxRowCells);
	return 0;
}

#else	// SLOW

/*************************************************************************
**                                                                      **
**                        filter scale (no aliases)                     **
**                                                                      **
*************************************************************************/

static unsigned int b3ScaleBW2Grey(void *ptr)
{
	b3RectInfo   *RectInfo;
	b3_count     *rIndex;
	b3_count     *cIndex;
	b3_pkd_color *tx_pal;
	b3_pkd_color  pal[2];
	b3_u08       *src,*dst,byte;
	b3_u08        bit;
	b3_count      srcBytes,dstBytes;
	b3_index      value,index;
	b3_count      count,num;
	b3_coord      x,y,sx,sy,ix,iy,rx,cy;
	b3_res        xSize,yMin,yMax;

	RectInfo = (b3RectInfo *)ptr;

	rIndex   = RectInfo->rIndex;
	cIndex   = RectInfo->cIndex;
	dst      = RectInfo->dst.cData;
	src      = RectInfo->src.cData;
	yMin     = RectInfo->yMin;
	yMax     = RectInfo->yMax;
	xSize    = RectInfo->xSizeDst;
	pal[0]   = RectInfo->pal[0];
	pal[1]   = RectInfo->pal[1];
	srcBytes = TX_BWA(RectInfo->xSizeSrc);
	dstBytes = RectInfo->xSizeDst;

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3ScaleBW2Grey(%5ld - %5ld)\n",
		yMin,yMax);
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # palette %06lx %06lx\n",pal[0],pal[1]);


	// For each new pixel in x- and y-direction...
	index = yMin * dstBytes;
	for (y = yMin;y < yMax;y++)
	{
		for (x = 0;x < xSize;x++)
		{
			value  = 0;
			count  = 0;
			cy     = cIndex[y];

			// First:  Find sample area greater zero [ci(y)..ci(y+iy)[.
			// Second: Sample over this region
			for (iy = 1;cy == cIndex[y + iy];iy++);
			for (sy = cy;
			     sy < cIndex[y + iy];
				 sy++)
			{
				// Adjust bits and bytes
				rx    = rIndex[x];
				bit   = Bits[rx & 7];
				num   = sy * srcBytes + (rx >> 3);

				// First:  Find sample area greater zero [ri(x)..ri(x+ix)[.
				// Second: Sample over this region
				for (ix = 1;rx == rIndex[x + ix];ix++);
				for (sx = rx;
				     sx < rIndex[x + ix];
					 sx++)
				{
					byte   = src[num];
					value += pal[byte & bit ? 1 : 0];
					bit    = bit >> 1;
					if (bit == 0)
					{
						bit = 128;
						num++;
					}
					count++;
				}
			}

			// Correct color
			if (count > 0)
			{
				dst[index++] = (b3_u08)B3_TX_DIV(value,count);
			}
		}
	}
	return 0;
}

#endif

void b3Tx::b3MonoScaleToGrey(
	b3Tx     *Tx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	b3RectInfo    RectInfo[CPU_MAX];
	b3Thread      tx_thread[CPU_MAX];
	b3_pkd_color  pal[2];
	b3_pkd_color *tx_pal,color;
	b3_index      i;
	b3_count      NumCPU;
	b3_f64        r,g,b;

#if 1
	NumCPU = cpu.b3GetNumCPU();
	if (NumCPU > CPU_MAX)
	{
		NumCPU = CPU_MAX;
	}
#else
	NumCPU = 1;
#endif

	// compute new palette
	tx_pal = Tx->b3GetPalette();

	color  = tx_pal[0];
	r      = (color & 0xff0000) >> 16;
	g      = (color & 0x00ff00) >>  8;
	b      = (color & 0x0000ff);
	pal[0] = (b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14);

	// Compute new palette
	color  = tx_pal[1];
	r      = (color & 0xff0000) >> 16;
	g      = (color & 0x00ff00) >>  8;
	b      = (color & 0x0000ff);
	pal[1] = (b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14);

	// It doesn't worth multi threading
	// if the image is too small
	if ((Tx->xSize * Tx->ySize) < 1000000)
	{
		NumCPU = 1;
	}

	// Init rect infos
	for (i = 0;i < NumCPU;i++)
	{
		RectInfo[i].dstType   = type;
		RectInfo[i].rIndex    = rIndex;
		RectInfo[i].cIndex    = cIndex;
		RectInfo[i].src.cData = Tx->data;
		RectInfo[i].dst.cData = data;
		RectInfo[i].xSizeSrc  = Tx->xSize;
		RectInfo[i].xSizeDst  = xSize;
		RectInfo[i].ySizeSrc  = Tx->ySize;
		RectInfo[i].ySizeDst  = ySize;
		RectInfo[i].yMin      = ySize *  i      / NumCPU;
		RectInfo[i].yMax      = ySize * (i + 1) / NumCPU;
		RectInfo[i].pal[0]    = pal[0];
		RectInfo[i].pal[1]    = pal[1];
	}
	if (NumCPU > 1)
	{
		// start threads
		for (i = 0;i < NumCPU;i++)
		{
			tx_thread[i].b3Name("b3Tx - b3MonoScaleToGrey");
			tx_thread[i].b3Start(
				b3ScaleBW2Grey,
				(void *)&RectInfo[i],TX_PRIO);
		}

		// Wait for threads
		for (i = 0;i < NumCPU;i++)
		{
			tx_thread[i].b3Wait();
		}
	}
	else
	{
		// Simple call
		b3ScaleBW2Grey((void *)&RectInfo[0]);
	}
}

// If a line is to be scaled up (means: destination is bigger)...
static void b3RGB8ComputeLineBigger(
	b3_count     *TxRowCounter,
	b3_count     *TxRowCells,
	b3_count     *rIndex,
	b3_pkd_color *src,
	b3_res        xDstSize)
{
	b3_coord     xDst;
	b3_index     index;
	b3_index     i = 0;
	b3_pkd_color color;

	for (xDst = 0;xDst < xDstSize;xDst++)
	{
		index = rIndex[xDst];
		color = src[index];
		TxRowCounter[i++] += (color & 0xff0000) >> 16;
		TxRowCounter[i++] += (color & 0x00ff00) >>  8;
		TxRowCounter[i++] += (color & 0x0000ff);
		TxRowCells[xDst]++;
	}
}


// If a line is to be scaled down (means: destination is smaller)...
static void b3RGB8ComputeLineSmaller(
	b3_count     *TxRowCounter,
	b3_count     *TxRowCells,
	b3_count     *rIndex,
	b3_pkd_color *src,
	b3_res        xDstSize)
{
	b3_coord     xDst;
	b3_res       xStart,xEnd;
	b3_pkd_color color,diff;
	b3_coord     x;

	xEnd = rIndex[0];
	for (xDst = 0;xDst < xDstSize;xDst++)
	{
		xStart = xEnd;
		xEnd   = rIndex[xDst + 1];
		diff   = xEnd - xStart;
		for (x = xStart;x < xEnd;x++)
		{
			color  = src[x];
			TxRowCounter[0] += ((color & 0xff0000) >> 16);
			TxRowCounter[1] += ((color & 0x00ff00) >>  8);
			TxRowCounter[2] += ((color & 0x0000ff));
		}
		TxRowCounter     += 3;
		TxRowCells[xDst] += diff;
	}
}

static unsigned int b3RGB8ScaleToRGB8(void *ptr)
{
	b3RectInfo  *RectInfo;
	b3_tx_type  dstType;
	b3_pkd_color *src;
	b3_pkd_color *dst;
	b3_count     *rIndex;
	b3_count     *cIndex;
	b3_count     *TxRowCounter;
	b3_count     *TxRowCells;
	b3_res        xSrcSize,ySrcSize;
	b3_res        xDstSize,yDstSize;
	b3_coord      ySrc;
	b3_coord      xDst,yDst;
	b3_count      srcBytes,dstBytes;
	b3_index      i;
	b3_pkd_color  color;
	b3_count      divisor;
	b3_res        yMin,yMax;
	void          (*ComputeLine)(
		b3_count     *TxRowCounter,
		b3_count     *TxRowCells,
		b3_count     *rIndex,
		b3_pkd_color *src,
		b3_res        dstSize);

	// ... and some values
	RectInfo = (b3RectInfo *)ptr;

	dstType  =  RectInfo->dstType;
	rIndex   =  RectInfo->rIndex;
	cIndex   =  RectInfo->cIndex;
	src      =  RectInfo->src.lData;
	yMin     =  RectInfo->yMin;
	yMax     =  RectInfo->yMax;
	xSrcSize =  RectInfo->xSizeSrc;
	xDstSize =  RectInfo->xSizeDst;
	ySrcSize =  RectInfo->ySizeSrc;
	yDstSize =  RectInfo->ySizeDst;
	srcBytes =  RectInfo->xSizeSrc;
	dstBytes =  RectInfo->xSizeDst;
	dst      = &RectInfo->dst.lData[yMin * dstBytes];

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3ColorScaleToGrey(%5ld - %5ld)\n",
		yMin,yMax);

	// Alloc some memory
	TxRowCounter = (b3_count *)malloc(xDstSize * sizeof(b3_count) * 3);
	if (TxRowCounter == null)
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx   # b3ColorScaleToGrey(): "
			"Not enough memory for row counter\n");
		return 0;
	}
	TxRowCells   = (b3_count *)malloc(xDstSize * sizeof(b3_count));
	if (TxRowCells == null)
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx   # b3MakeItGrey(): "
			"Not enough memory for row cell sizes\n");
		free(TxRowCounter);
		return 0;
	}

	// Select right line computation
	ComputeLine = (xDstSize < xSrcSize ? b3RGB8ComputeLineSmaller : b3RGB8ComputeLineBigger);

	if (yDstSize < ySrcSize)
	{
		// Destination height is smaller (scale down)
		for (yDst = yMin;yDst < yMax;yDst++)
		{
			// clean row counter
			i = 0;
			for (xDst = 0;xDst < xDstSize;xDst++)
			{
				TxRowCounter[i++] = 0;
				TxRowCounter[i++] = 0;
				TxRowCounter[i++] = 0;
				TxRowCells[xDst]  = 0;
			}

			// scan interval
			for (ySrc = cIndex[yDst];ySrc < cIndex[yDst + 1];ySrc++)
			{
				ComputeLine(TxRowCounter,TxRowCells,rIndex,&src[ySrc * srcBytes],xDstSize);
			}

			// compute grey indices
			switch(dstType)
			{
			case B3_TX_VGA:
				i = 0;
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					if (TxRowCells[xDst] > 0)
					{
						color  =
							B3_TX_DIV(TxRowCounter[i],TxRowCells[xDst]) << 16;
						i++;
						color |=
							B3_TX_DIV(TxRowCounter[i],TxRowCells[xDst]) <<  8;
						i++;
						color |=
							B3_TX_DIV(TxRowCounter[i],TxRowCells[xDst]);
						i++;
					}
					dst++;
				}
				break;

			case B3_TX_RGB8:
				i = 0;
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					divisor = TxRowCells[xDst];
					if (divisor > 0)
					{
						if (divisor < B3_TX_MAX_DIVISOR)
						{
							color  = B3_TX_DIV2(TxRowCounter[i],divisor) << 16; i++;
							color |= B3_TX_DIV2(TxRowCounter[i],divisor) <<  8; i++;
							color |= B3_TX_DIV2(TxRowCounter[i],divisor);       i++;
						}
						else
						{
							color  = ((TxRowCounter[i] / divisor) & 0xff) << 16; i++;
							color |= ((TxRowCounter[i] / divisor) & 0xff) <<  8; i++;
							color |= ((TxRowCounter[i] / divisor) & 0xff);       i++;
						}
						*dst = color;
					}
					dst++;
				}
				break;
			}
		}
	}
	else
	{
		// Destination height is bigger (scale up)
		for (yDst = yMin;yDst < yMax;yDst++)
		{
			// Clean row counter
			i = 0;
			for (xDst = 0;xDst < xDstSize;xDst++)
			{
				TxRowCounter[i++] = 0;
				TxRowCounter[i++] = 0;
				TxRowCounter[i++] = 0;
				TxRowCells[xDst]  = 0;
			}

			// Compute Line...
			ComputeLine(TxRowCounter,TxRowCells,rIndex,&src[cIndex[yDst] * srcBytes],xDstSize);

			// Compute grey indices
			switch(dstType)
			{
			case B3_TX_VGA:
				i = 0;
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					if (TxRowCells[xDst] > 0)
					{
						color  =
							(B3_TX_DIV(TxRowCounter[i],TxRowCells[xDst]) & 0xff) << 16;
						i++;
						color |=
							(B3_TX_DIV(TxRowCounter[i],TxRowCells[xDst]) & 0xff) <<  8;
						i++;
						color |=
							(B3_TX_DIV(TxRowCounter[i],TxRowCells[xDst]) & 0xff);
						i++;
					}
					dst++;
				}
				break;

			case B3_TX_RGB8:
				i = 0;
				for (xDst = 0;xDst < xDstSize;xDst++)
				{
					divisor = TxRowCells[xDst];
					if (divisor > 0)
					{
						if (divisor < B3_TX_MAX_DIVISOR)
						{
							color  = B3_TX_DIV2(TxRowCounter[i],divisor) << 16; i++;
							color |= B3_TX_DIV2(TxRowCounter[i],divisor) <<  8; i++;
							color |= B3_TX_DIV2(TxRowCounter[i],divisor);       i++;
						}
						else
						{
							color  = ((TxRowCounter[i] / divisor) & 0xff) << 16; i++;
							color |= ((TxRowCounter[i] / divisor) & 0xff) <<  8; i++;
							color |= ((TxRowCounter[i] / divisor) & 0xff);       i++;
						}
						*dst = color;
					}
					dst++;
				}
				break;
			}
		}
	}

	// Free willie
	free (TxRowCounter);
	free (TxRowCells);
	return 0;
}

void b3Tx::b3ColorScaleToGrey(
	b3Tx     *srcTx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	b3RectInfo RectInfo[CPU_MAX];
	b3Thread   tx_thread[CPU_MAX];
	b3_index   i;
	b3_count   NumCPU;

#if 1
	NumCPU = cpu.b3GetNumCPU();
	if (NumCPU > CPU_MAX)
	{
		NumCPU = CPU_MAX;
	}
#else
	NumCPU = 1;
#endif

	// It doesn't worth multi threading
	// if the image is too small
	if ((srcTx->xSize * srcTx->ySize) < 250000)
	{
		NumCPU = 1;
	}

	// Init rect infos
	for (i = 0;i < NumCPU;i++)
	{
		RectInfo[i].dstType   = type;
		RectInfo[i].rIndex    = rIndex;
		RectInfo[i].cIndex    = cIndex;
		RectInfo[i].src.lData = (b3_pkd_color *)srcTx->data;
		RectInfo[i].dst.lData = (b3_pkd_color *)data;
		RectInfo[i].xSizeSrc  = srcTx->xSize;
		RectInfo[i].xSizeDst  = xSize;
		RectInfo[i].ySizeSrc  = srcTx->ySize;
		RectInfo[i].ySizeDst  = ySize;
		RectInfo[i].yMin      = ySize *  i      / NumCPU;
		RectInfo[i].yMax      = ySize * (i + 1) / NumCPU;
		RectInfo[i].pal[0]    = 0;
		RectInfo[i].pal[1]    = 0;
	}

	if (NumCPU > 1)
	{
		// start threads
		for (i = 0;i < NumCPU;i++)
		{
			tx_thread[i].b3Name("b3Tx - b3ColorScaleToGrey");
			tx_thread[i].b3Start(
				b3RGB8ScaleToRGB8,
				(void *)&RectInfo[i],TX_PRIO);
		}

		// Wait for threads
		for (i = 0;i < NumCPU;i++)
		{
			tx_thread[i].b3Wait();
		}
	}
	else
	{
		// Simple call
		b3RGB8ScaleToRGB8((void *)&RectInfo[0]);
	}
}

void b3Tx::b3ColorGrid()
{
	b3_index i,index;
	b3_count count = 0;

	// Compare palette entries.
	for (i = 0;i < pSize;i++)
	{
		if (palette[i] == (b3_pkd_color)(0x010101 * i))
		{
			count++;
		}
	}

	// Check if every entry is like you want.
	if (count == 256)
	{
		// OK, we have a grey scale palette.
		// Return and be happy!
		return;
	}

	// Allocate grid
	try
	{
		grid = (b3ColorIndices *)new b3ColorIndices[B3_MAX_GRID];
	}
	catch (...)
	{
		grid = null;
	}

	// Now initialize grid
	if (grid != null)
	{
		// compute squares in range [0;255]
		if (!quadValid)
		{
			for (i = 0;i < 256;i++)
			{
				quad256[256 - i] =
				quad256[256 + i] = i * i;
			}
			quadValid = true;
		}

		// "Check in" palette
		for (i = 0;i < pSize;i++)
		{
			index = b3ColorGridIndex(palette[i]);
			grid[index].b3AddColorIndex(i);
		}
	}
}

b3_index b3Tx::b3ColorIndex(b3_pkd_color color)
{
	if (grid != null)
	{
		b3_index index;

		index = b3ColorGridIndex(color);
		return grid[index].b3ColorIndex(palette,color);
	}
	else
	{
		b3_index index;
		b3_f64   r,g,b;

		// Extract RGB
		r     = (color & 0xff0000) >> 16;
		g     = (color & 0x00ff00) >>  8;
		b     = (color & 0x0000ff);

		// Compute grey index
		index = (b3_index)(r * 0.35 + g * 0.51 + b * 0.14);

		// Check against bounds and return
		return index > 255 ? 255 : index;
	}
}

void b3Tx::b3VGAScaleToVGA(
	b3Tx     *srcTx,
	b3_count *rIndex,
	b3_index *cIndex)
{
	b3_coord     x,y,sx,sy,ix,iy;
	b3_count     count=0;
	b3_index     index=0;
	b3_pkd_color rVal,gVal,bVal,value,color;

	memcpy (palette,srcTx->b3GetPalette(),pSize * sizeof(b3_pkd_color));
	b3ColorGrid();
	for (y = 0;y < ySize;y++)
	{
		for (x = 0;x < xSize;x++)
		{
			rVal  = 0;
			gVal  = 0;
			bVal  = 0;
			count = 0;

			// First:  Find sample area greater zero [ci(y)..ci(y+iy)[.
			// Second: Sample over this region
			for (iy = 1;cIndex[y] == cIndex[y + iy];iy++);
			for (sy = cIndex[y];
				 sy < cIndex[y + iy];
				 sy++)
			{
				// First:  Find sample area greater zero [ri(x)..ri(x+ix)[.
				// Second: Sample over this region
				for (ix = 1;rIndex[x] == rIndex[x + ix];ix++);
				for (sx = rIndex[x];
					 sx < rIndex[x + ix];
					 sx++)
				{
					color = srcTx->b3GetValue(sx,sy);
					rVal += (color & 0xff0000) >> 16;
					gVal += (color & 0x00ff00) >>  8;
					bVal += (color & 0x0000ff);
					count++;
				}
			}

			// Correct color
			rVal /= count;
			gVal /= count;
			bVal /= count;

			// Convert into packed RGB
			value = (rVal << 16) | (gVal << 8) | bVal;

			// Get palette index of "value"
			data[index++] = (b3_u08)b3ColorIndex(value);
		}
	}
	// Free grid
	if (grid)
	{
		delete [] grid;
		grid = null;
	}
}

void b3Tx::b3VGAScaleToRGB8(
	b3Tx     *srcTx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	b3_u08       *cSrc;
	b3_pkd_color *lDst;
	b3_pkd_color *palette;
	b3_coord       x,y,sx,sy,ix,iy;
	b3_count      count=0;
	b3_pkd_color  rVal,gVal,bVal,color;

	lDst    = (b3_pkd_color *)data;
	cSrc    = (b3_u08 *)srcTx->b3GetData();
	palette = srcTx->b3GetPalette();
	for (y = 0;y < ySize;y++)
	{
		for (x = 0;x < xSize;x++)
		{
			rVal  = 0;
			gVal  = 0;
			bVal  = 0;
			count = 0;

			// First:  Find sample area greater zero [ci(y)..ci(y+iy)[.
			// Second: Sample over this region
			for (iy = 1;cIndex[y] == cIndex[y + iy];iy++);
			for (sy = cIndex[y];
				 sy < cIndex[y + iy];
				 sy++)
			{
				// First:  Find sample area greater zero [ri(x)..ri(x+ix)[.
				// Second: Sample over this region
				for (ix = 1;rIndex[x] == rIndex[x + ix];ix++);
				for (sx = rIndex[x];
					 sx < rIndex[x + ix];
					 sx++)
				{
					color = palette[cSrc[sx + sy * srcTx->xSize]];
					rVal += (color & 0xff0000) >> 16;
					gVal += (color & 0x00ff00) >>  8;
					bVal += (color & 0x0000ff);
					count++;
				}
			}

			// Get palette index of "value"
			if (count < B3_TX_MAX_DIVISOR)
			{
				*lDst++ =
					(B3_TX_DIV2(rVal,count) << 16) |
					(B3_TX_DIV2(gVal,count) <<  8) |
					 B3_TX_DIV2(bVal,count);
			}
			else
			{
				*lDst++ =
					((rVal / count) << 16) |
					((gVal / count) <<  8) |
					 (bVal / count);
			}
		}
	}
	// Free grid
	if (grid)
	{
		delete [] grid;
		grid = null;
	}
}

void b3Tx::b3VGAScaleToGrey(
	b3Tx     *srcTx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	switch (type)
	{
	case B3_TX_VGA:
		b3VGAScaleToVGA(srcTx,rIndex,cIndex);
		break;

	case B3_TX_RGB8:
		b3VGAScaleToRGB8(srcTx,rIndex,cIndex);
		break;
	}
}

void b3Tx::b3ScaleToGrey(b3Tx *srcTx)
{
	b3_coord  x,y;
	b3_count *rIndex,*cIndex;

	// Check if there is nothing to do
	if (srcTx->type == B3_TX_UNDEFINED)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3ScaleToGrey(): source image (0x%p) of undefined type!\n",
			srcTx);
		return;
	}
	if ((xSize <= 0) || (ySize <= 0))
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3ScaleToGrey(): destination image (0x%p) without extend (%ldx%ld)!\n",
			this,xSize,ySize);
		return;
	}
	rIndex = (b3_count *)b3Alloc((xSize + 1) * sizeof(b3_count));
	cIndex = (b3_count *)b3Alloc((ySize + 1) * sizeof(b3_count));

	if (!divTableValid)
	{
		b3DivTableInit();
	}

	// Compute resampled start coordinates
	b3PrintT("ScaleToGrey: start");
	for (x = 0;x <= xSize;x++) rIndex[x] = x * srcTx->xSize / xSize;
	for (y = 0;y <= ySize;y++) cIndex[y] = y * srcTx->ySize / ySize;
 	// NOTE:     ^^ This Lower/Equal preserves the resampler from overflow!
	//              Very vecessary in "grey" resampling...

	switch (srcTx->depth)
	{
	case 1 :
		b3MonoScaleToGrey (srcTx,rIndex,cIndex);
		break;

	case 24:
		b3ColorScaleToGrey(srcTx,rIndex,cIndex);
		break;

	default :
		b3VGAScaleToGrey(srcTx,rIndex,cIndex);
		break;
	}
	b3PrintT("ScaleToGrey: stop");

	b3Free ((void *)rIndex);
	b3Free ((void *)cIndex);
}

/*************************************************************************
**                                                                      **
**                        simple scale (alias rich)                     **
**                                                                      **
*************************************************************************/

static unsigned int b3ScaleBW2BW(void *ptr)
{
	b3RectInfo  *RectInfo;
	b3_count    *rIndex;
	b3_count    *cIndex;
	b3_u08      *src,*dst;
	b3_coord    x,y,rx;
	b3_res      yMin,yMax,xSize;
	b3_u32      dstBit,srcBit;
	b3_u08      dstByte;
	b3_count    dstBytes,srcBytes,num;
	b3_index    index;

	RectInfo = (b3RectInfo *)ptr;

	rIndex   = RectInfo->rIndex;
	cIndex   = RectInfo->cIndex;
	dst      = RectInfo->dst.cData;
	src      = RectInfo->src.cData;
	yMin     = RectInfo->yMin;
	yMax     = RectInfo->yMax;
	xSize    = RectInfo->xSizeDst;
	srcBytes = TX_BWA(RectInfo->xSizeSrc);
	dstBytes = TX_BWA(RectInfo->xSizeDst);

	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # b3ScaleBW2BW(%5ld - %5ld)\n",
		yMin,yMax);

	dst += (yMin * dstBytes);
	for (y = yMin;y < yMax;y++)
	{
		num = cIndex[y] * srcBytes;

		dstBit  = 128;
		dstByte =   0;
		index   =   0;
		for (x = 0;x < xSize;x++)
		{
			rx     = rIndex[x];
			srcBit = Bits[rx & 7];

			// Copy bit
			if (src[num + (rx >> 3)] & srcBit)
			{
				dstByte |= dstBit;
			}

			// Shift bit right...
			dstBit = dstBit >> 1;
			if (dstBit == 0)
			{
				dst[index++] = dstByte;
				dstBit       = 128;
				dstByte      =   0;
			}
		}
		dst += dstBytes;
	}
	return 0;
}

void b3Tx::b3MonoScale(
	b3Tx     *Tx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	b3_coord      x,y,rx;
	b3_count      bytes,num;
	b3_index      value,index=0;
	b3_pkd_color  pal[2];
	b3_pkd_color *tx_pal,color;
	b3_u08       *cData,bit;
	b3_pkd_color *lData;
	b3_f64         r,g,b;
	b3RectInfo     RectInfo[CPU_MAX];
	b3_index       i;
	b3_count       NumCPU;

#if 1
	NumCPU = cpu.b3GetThreads();
	if (NumCPU > CPU_MAX)
	{
		NumCPU = CPU_MAX;
	}
#else
	NumCPU = 1;
#endif

	cData  = (b3_u08 *)Tx->b3GetData();
	tx_pal = Tx->b3GetPalette();

	color  = tx_pal[0];
	r      = (color & 0xff0000) >> 16;
	g      = (color & 0x00ff00) >>  8;
	b      = (color & 0x0000ff);
	pal[0] = (b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14);

	color  = tx_pal[1];
	r      = (color & 0xff0000) >> 16;
	g      = (color & 0x00ff00) >>  8;
	b      = (color & 0x0000ff);
	pal[1] = (b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14);
	bytes  = TX_BWA(Tx->xSize);
	b3PrintF(B3LOG_FULL,"### CLASS: b3Tx   # palette %06lx %06lx\n",pal[0],pal[1]);

	if (type == B3_TX_VGA)
	{
		for (y = 0;y < ySize;y++)
		{
			num = cIndex[y] * bytes;
			for (x = 0;x < xSize;x++)
			{
				rx  = rIndex[x];
				bit = Bits[rx & 7];
				value = pal[cData[num + (rx >> 3)] & bit ? 1 : 0];

				data[index++] = (b3_u08)value;
			}
		}
	}

	if (type == B3_TX_RGB8)
	{	
		lData = (b3_pkd_color *)data;
		for (y = 0;y < ySize;y++)
		{
			num = cIndex[y] * bytes;
			for (x = 0;x < xSize;x++)
			{
				rx  = rIndex[x];
				bit = Bits[rx & 7];

				value = tx_pal[cData[num + (rx >> 3)] & bit ? 1 : 0];

				lData[index++] = value;
			}
		}
	}
	if (type == B3_TX_ILBM)
	{
		b3Thread tx_thread[CPU_MAX];

		// It doesn't worth multi threading
		// if the image is too small
		if ((xSize * ySize) < 300000)
		{
			NumCPU = 1;
		}

		// Init rect infos
		for (i = 0;i < NumCPU;i++)
		{
			RectInfo[i].rIndex    = rIndex;
			RectInfo[i].cIndex    = cIndex;
			RectInfo[i].src.cData = Tx->data;
			RectInfo[i].dst.cData = data;
			RectInfo[i].xSizeSrc  = Tx->xSize;
			RectInfo[i].xSizeDst  = xSize;
			RectInfo[i].ySizeSrc  = Tx->ySize;
			RectInfo[i].ySizeDst  = ySize;
			RectInfo[i].yMin      = ySize *  i      / NumCPU;
			RectInfo[i].yMax      = ySize * (i + 1) / NumCPU;
		}
		if (NumCPU > 1)
		{
			// start threads
			for (i = 0;i < NumCPU;i++)
			{
				tx_thread[i].b3Name("b3Tx - b3MonoScale");
				tx_thread[i].b3Start(
					b3ScaleBW2BW,
					(void *)&RectInfo[i],TX_PRIO);
			}

			// Wait for threads
			for (i = 0;i < NumCPU;i++)
			{
				tx_thread[i].b3Wait();
			}
		}
		else
		{
			// Simple call
			b3ScaleBW2BW((void *)&RectInfo[0]);
		}
	}
}

void b3Tx::b3ColorScale(
	b3Tx     *Tx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	b3_coord       x,y;
	b3_count      num;
	b3_pkd_color *lSrc,*lDst;

	lSrc = (b3_pkd_color *)Tx->b3GetData();
	lDst = (b3_pkd_color *)data;

	for (y = 0;y < ySize;y++)
	{
		num = cIndex[y] * Tx->xSize;
		for (x = 0;x < xSize;x++)
		{
			*lDst++ = lSrc[num + rIndex[x]];
		}
	}
}

void b3Tx::b3VGAScale(
	b3Tx     *Tx,
	b3_count *rIndex,
	b3_count *cIndex)
{
	b3_coord      x,y;
	b3_count      num;
	b3_pkd_color *pSrc,*pDst,*lDst;
	b3_u08       *cSrc,*cDst;

	// copying palette
	if (depth == Tx->depth)
	{
		pSrc = (b3_pkd_color *)Tx->b3GetPalette();
		pDst = (b3_pkd_color *)palette;
		num  = 1 << depth;
		for (x = 0;x < num;x++)
		{
			*pDst++ = *pSrc++;
		}
	}

	// scaling textures
	if (type == B3_TX_VGA)
	{
		cSrc = (b3_u08 *)Tx->b3GetData();
		cDst = (b3_u08 *)data;
		for (y = 0;y < ySize;y++)
		{
			num = cIndex[y] * Tx->xSize;
			for (x = 0;x < xSize;x++)
			{
				*cDst++ = cSrc[num + rIndex[x]];
			}
		}
	}

	// scaling textures
	if (type == B3_TX_RGB8)
	{
		pSrc = (b3_pkd_color *)Tx->b3GetPalette();
		cSrc = (b3_u08 *)Tx->b3GetData();
		lDst = (b3_pkd_color *)data;
		for (y = 0;y < ySize;y++)
		{
			num = cIndex[y] * Tx->xSize;
			for (x = 0;x < xSize;x++)
			{
				*lDst++ = pSrc[cSrc[num + rIndex[x]]];
			}
		}
	}
}

void b3Tx::b3Scale(b3Tx *srcTx)
{
	b3_coord      x,y,sx,sy;
	b3_index      index = 0,value;
	b3_count      count;
	b3_pkd_color  color;
	b3_f64        r,g,b;
	b3_count     *rIndex,*cIndex;

	// Check if there is nothing to do
	if (srcTx->type == B3_TX_UNDEFINED)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3Scale(): source image (0x%p) of undefined type!\n",
			srcTx);
		return;
	}
	if ((xSize <= 0) || (ySize <= 0))
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3Scale(): destination image (0x%p) without extend (%ldx%ld)!\n",
			this,xSize,ySize);
		return;
	}
	rIndex = (b3_count *)b3Alloc((xSize + 1) * sizeof(b3_count));
	cIndex = (b3_count *)b3Alloc((ySize + 1) * sizeof(b3_count));

	// Compute resampled start coordinates
	for (x = 0;x <= xSize;x++) rIndex[x] = x * srcTx->xSize / xSize;
	for (y = 0;y <= ySize;y++) cIndex[y] = y * srcTx->ySize / ySize;
	// NOTE:     ^^ This Lower/Equal preserves the resampler from overflow!

	switch (srcTx->depth)
	{
	case 1:
		b3MonoScale  (srcTx,rIndex,cIndex);
		break;

	case 24:
		b3ColorScale (srcTx,rIndex,cIndex);
		break;

	default:
		if (srcTx->type == B3_TX_VGA) 
		{
			b3VGAScale(srcTx,rIndex,cIndex);
		}
		else for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				value  = 0;
				count  = 0;
				for (sy = cIndex[y];
				     sy < cIndex[y+1];
					 sy++)
				{
					for (sx = rIndex[x];
					     sx < rIndex[x+1];
						 sx++)
					{
						color = srcTx->b3GetValue(sx,sy);
						r = (color & 0xff0000) >> 16;
						g = (color & 0x00ff00) >>  8;
						b = (color & 0x0000ff);
						value += (b3_index)(r * 0.35 + g * 0.51 + b * 0.14);
						count++;
					}
				}

				data[index++] = (b3_u08)(value / count);
			}
		}
		break;
	}

	b3Free ((void *)rIndex);
	b3Free ((void *)cIndex);
}

/*************************************************************************
**                                                                      **
**                        transform texture into grey scale             **
**                                                                      **
*************************************************************************/

void b3Tx::b3TransToGrey()
{
	b3_u08       *cPtr;
	b3_u16       *sPtr;
	b3_pkd_color *lPtr;
	b3_pkd_color *pPtr;
	b3_size       newSize;
	b3_coord      x,y;
	b3_f64        r,g,b;

	// First allocate new buffer;
	newSize = xSize * ySize;
	cPtr = (b3_u08 *)b3Alloc(newSize);
	if (cPtr == null)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TransToGrey(): Not enogh memory for new image buffer!\n");
		return;
	}

	// alloc new palette
	pPtr = (b3_pkd_color *)b3Alloc(256 * sizeof(b3_pkd_color));
	if (pPtr == null)
	{
		b3Free (cPtr);
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TransToGrey(): Not enogh memory for new palette!\n");
		return;
	}
	// init grey ramp
	for (x = 0;x < 256;x++) pPtr[x] = 0x010101 * x;

	switch (type)
	{
	case B3_TX_ILBM:
		b3Free(data);
		data = (b3_u08 *)cPtr;
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				b3_pkd_color color;

				color = b3GetValue(x,y);
				r = ((color & 0xff0000) >> 16) * 0.35;
				g = ((color & 0x00ff00) >>  8) * 0.51;
				b = ((color & 0x0000ff))       * 0.14;

				*cPtr++ = (b3_u08)(r + b + g);
			}
		}
		break;

	case B3_TX_VGA:
		sPtr = (b3_u16 *)data;
		b3Free(data);
		data = (b3_u08 *)cPtr;
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				b3_pkd_color color;

				color = palette[*sPtr++];
				r = ((color & 0xff0000) >> 16) * 0.35;
				g = ((color & 0x00ff00) >>  8) * 0.51;
				b = ((color & 0x0000ff))       * 0.14;

				*cPtr++ = (b3_u08)(r + b + g);
			}
		}
		break;

	case B3_TX_RGB4:
		sPtr = (b3_u16 *)data;
		b3Free(data);
		data = (b3_u08 *)cPtr;
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				b3_u16 color;

				color = *sPtr++;
				r = ((color & 0xf00) >> 4) * 0.35;
				g = ((color & 0x0f0))      * 0.51;
				b = ((color & 0x00f) << 4) * 0.14;

				*cPtr++ = (b3_u08)(r + b + g);
			}
		}
		break;

	case B3_TX_RGB8:
		lPtr = (b3_pkd_color *)data;
		b3Free(data);
		data = (b3_u08 *)cPtr;
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				b3_pkd_color color;

				color = *lPtr++;
				r = ((color & 0xff0000) >> 16) * 0.35;
				g = ((color & 0x00ff00) >>  8) * 0.51;
				b = ((color & 0x0000ff))       * 0.14;
				*cPtr++ = (b3_u08)(r + b + g);
			}
		}
		break;
	}

	// replace old palette
	b3Free(palette);
	palette = pPtr;
	depth   = 8;
	type    = B3_TX_VGA;
}
