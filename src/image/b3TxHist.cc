/*
**
**	$Filename:	b3TxHist.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - image histogramme routines
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

#include <math.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**
**	Revision 1.3  2001/07/01 17:04:09  sm
**	- Solved some signed/unsigned mismatches
**	
**	Revision 1.2  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
**	Revision 1.6  2000/10/09 06:51:52  smork
**	- Main work: Changing scanning via b3ScanLoop to b3ScanBase
**	  o Adding some b3ScanContainer overridables
**	  o OnUpdateXXX() routines works fine with b3ScanBase
**	  o needed CXXXMainFrame updates done
**	- b3ScanBase gives the right b3OnUpdateXXX values back.
**	- Blizzard III Imaging: Palette and true color filtering and contrasting
**	  corrected in OnUpdateXXX() routines
**	- Blizzard III BC-Analyzer not using b3Page any more.
**	- View modes of CBlizzard IIIScrollView are enums now.
**	
**	Revision 1.5  2000/09/29 11:14:58  smork
**	- ICAP_GAMMA changed from TW_RANGE to TW_ONEVALUE. It's working
**	  now with correct values. So the release version works now
**	  properly.
**	- The histogramme was computed wrong! The black usage is computed
**	  black pixel divided by all pixel (and not white pixel).
**	- When computing white page ratio for gray images count middle
**	  values as dark values.
**	
**	Revision 1.4  2000/09/21 10:22:27  smork
**	- Setting Blizzard III projects to warning level 3: Found
**	  some uninitialized variables.
**	- changed b3Mutex from CMutex to CRITICAL_SECTION (thread
**	  synchronization only)
**	- introduced b3IPCMutex for process synchronization
**	
**	Revision 1.3  2000/09/15 08:29:15  smork
**	- Converting some defines to enums.
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

static const b3_u08 Bits[8] =
{
	128,64,32,16,8,4,2,1
};

/*****************************

bits     bwBitsSet bwBitsClear
00000000         0           8
00000001         1           7
00000011         2           6
00000100         1           7
   .
   .
01100100         3           5
01100101         4           4
   .
   .
11111110         7           1
11111111         8           0

*****************************/

static b3_bool bwInit = false;
static b3_u08  bwBitsSet[256];
static b3_u08  bwBitsClr[256];

/*************************************************************************
**                                                                      **
**                        histogramme routines                          **
**                                                                      **
*************************************************************************/

void b3Tx::b3SetWhiteRatio(b3_f64 newRatio)
{
	whiteRatio = newRatio;
}

// We need histogrammes to determine if the image is a white page. The
// histogramme is created blockwise to make sure that this block is
// complete in the L2 cache (higher performance). 
b3_bool b3Tx::b3StartHist()
{
	b3_index i;

	// allocate memory for histogramme
	if (histogramme == null)
	{
		histogramme = (b3_count *)b3Alloc(
			B3_TX_MAX_HISTGRM * sizeof(b3_count));
		if (histogramme == null) 
		{
			return false;
		}
	}

	// clear histogramme
	for (i = 0;i < B3_TX_MAX_HISTGRM;i++) 
	{
		histogramme[i] = 0;
	}
	return true;
}

void b3Tx::b3EndHist()
{
	if (histogramme != null) 
	{
		b3Free(histogramme);
	}
	histogramme = null;
}

// All what we need is: "Schwarze Weizen Frühstückskorn"!
b3_bool b3Tx::b3Histogramme()
{
	if (!b3StartHist()) 
	{
		return false;
	}
	return b3AddHist(0,0,xSize,ySize);
}

b3_bool b3Tx::b3AddHist(
	b3_coord xStart,
	b3_coord yStart,
	b3_coord xStop,
	b3_coord yStop)
{
	b3_u08 *cPtr;

	// Compute bit set/cleared table
	if (!bwInit)
	{
		b3_coord      x;
		b3_pkd_color  bit;
		b3_u08        set,clr;

		for (x = 0;x < 256;x++)
		{
			set = 0;
			clr = 0;
			for (bit = 128;bit != 0;bit = bit >> 1)
			{
				if (x & bit) set++;
				else         clr++;
			}
			bwBitsSet[x] = set;
			bwBitsClr[x] = clr;
		}
		bwInit = true;
	}
		
	// if Tx is type of direct palette entries
	if (type == B3_TX_VGA)
	{
		b3_coord      x,y;
		b3_index      index;
		b3_pkd_color  color,grey;
		b3_f64        r,g,b;

		xStop += xStart;
		yStop += yStart;
		if (b3IsGreyPalette())
		{
			cPtr = &data[yStart * xSize];
			for (y = yStart;y < yStop;y++)
			{
				for (x = xStart;x < xStop;x++) 
				{
					index = cPtr[x] ;
					histogramme[index]++;
				}
				cPtr += xSize;
			}
		}
		else
		{
			cPtr = &data[yStart * xSize];
			for (y = yStart;y < yStop;y++)
			{
				for (x = xStart;x < xStop;x++) 
				{
					color = palette[cPtr[x]];
					r     = ((color & 0xff0000) >> 16) * 0.35;
					g     = ((color & 0x00ff00) >>  8) * 0.51;
					b     = ((color & 0x0000ff))       * 0.14;

					grey = (b3_pkd_color)(r + g + b);
					if (grey >= B3_TX_MAX_HISTGRM)
					{
						grey = B3_TX_MAX_HISTGRM - 1;
					}

					index = cPtr[x] ;
					histogramme[grey]++;
				}
				cPtr += xSize;
			}
		}
		return true;
	}

	// if Tx is type of interleaved
	// only b/w is supported yet
	if ((type == B3_TX_ILBM) && (depth == 1))
	{
		b3_coord x,y;
		b3_count xBytes;

		xStop += xStart;
		yStop += yStart;
		xBytes = TX_BWA(xSize);
		for (y = yStart;y < yStop;y++)
		{
			cPtr = &data[y * xBytes];
			for (x = 0;x < xBytes;x++)
			{
				histogramme[0] += bwBitsClr[cPtr[x]];
				histogramme[1] += bwBitsSet[cPtr[x]];
			}
		}
		return true;
	}

	if (type == B3_TX_RGB8)
	{
		b3_coord       x,y;
		b3_index       index;
		b3_pkd_color  *lPtr,color,grey;
		b3_f64         r,g,b;

		lPtr  = (b3_pkd_color *)data;
		index = yStart * xSize;
		for (y = yStart;y < yStop;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				color = lPtr[index++];
				r     = ((color & 0xff0000) >> 16) * 0.35;
				g     = ((color & 0x00ff00) >>  8) * 0.51;
				b     = ((color & 0x0000ff))       * 0.14;

				grey = (b3_pkd_color)(r + g + b);
				if (grey >= B3_TX_MAX_HISTGRM)
				{
					grey = B3_TX_MAX_HISTGRM - 1;
				}

				histogramme[grey]++;
			}
		}
		return true;
	}
	return false;
}

b3_bool b3Tx::b3IsWhite()
{
	b3_count black  = 0;
	b3_count white  = 0;
	b3_count middle = 0;
	b3_index together;
	b3_index i,s1,s2,s3;
	b3_f64   ratio;
	b3_bool  IsWhite;

	if (histogramme == null) 
	{
		return false;
	}
	if (depth > B3_TX_MAX_HISTGRM_DEPTH) 
	{
		return false;
	}

	if (depth == 1)
	{
		white = histogramme[0];
		black = histogramme[1];
	}
	if (depth == 2)
	{
		black  = histogramme[0];
		white  = histogramme[3];
		middle = histogramme[1] + histogramme[2];
	}
	if (depth > 2)
	{
		s3 = 1 << depth; // number of colors
		s1 = s3 >> 2;    // a quarter of s3
		s2 = s3 - s1;    // three quarters of s3
		for (i =  0;i < s1;i++) black  += histogramme[i];
		for (i = s1;i < s2;i++) middle += histogramme[i];
		for (i = s2;i < s3;i++) white  += histogramme[i];
	}

	// Compute ratio
	together = black + white + middle;
	IsWhite = ((black == 0) || (white == 0));
	if (!IsWhite)
	{
		ratio = (b3_f64)(black + middle) / (b3_f64)together;
		if (ratio >= 1.0) ratio = 1.0 / ratio;	   
		IsWhite = (ratio < whiteRatio);
	}
	else
	{
		ratio = 0;
	}
	b3PrintF(B3LOG_DEBUG,"### CLASS: b3Tx:  # Is%sWhite (%1.4f)\n",
		IsWhite ? "" : "Not",ratio);

	return IsWhite;
}

b3_bool b3Tx::b3GetHistogramme(b3_count *buffer,b3_count &entries)
{
	b3_index i;

	entries = 0;
	if (histogramme != null)
	{
		// Copy entries
		entries = 1 << (depth <= B3_TX_MAX_HISTGRM_DEPTH ? depth : B3_TX_MAX_HISTGRM_DEPTH);
		for (i = 0;i < entries;i++)
		{
			buffer[i] = histogramme[i];
		}
	}
	return histogramme != null;
}

/*************************************************************************
**                                                                      **
**                        transform texture into black/white            **
**                                                                      **
*************************************************************************/

b3_bool b3Tx::b3TransToBW(b3Tx *srcTx,b3_f64 ratio,b3_tx_threshold mode)
{
	b3Copy(srcTx);
	return b3TransToBW(ratio,mode);
}

b3_bool b3Tx::b3TransToBW(b3_f64 ratio,b3_tx_threshold mode)
{
	b3_index threshold;
	b3_bool  result;

	result = b3Histogramme();
	if (result)
	{
		threshold = b3ComputeThreshold(ratio,mode);
		if (threshold >= 0)
		{
			result = b3TransToBW(threshold);
		}
		else
		{
			result = false;
		}
	}
	return result;
}

b3_index b3Tx::b3ComputeThreshold(b3_f64 ratio,b3_tx_threshold mode)
{
	b3_count limit,count=0,max;
	b3_index i,threshold = 0;
	b3_bool  compute_threshold = true;

	if (histogramme == null)
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Tx   # b3ComputeThreshold() failed.\n");
		threshold = -1;
	}
	else
	{
		// Compute threshold;
		max = 1 << (depth <= B3_TX_MAX_HISTGRM_DEPTH ? depth : B3_TX_MAX_HISTGRM_DEPTH);
		switch (mode)
		{
		case B3_THRESHOLD_WHITE_RATIO:
			ratio = whiteRatio;
			break;

		case B3_THRESHOLD_EQUALIZE:
			ratio = 0.5;
			break;

		case B3_THRESHOLD_USE:
		default:
			threshold         = (long)floor(ratio * (max - 1));
			compute_threshold = false;
			break;
		}

		// Search inside histogramme for a threshold
		if (compute_threshold)
		{
			limit = (b3_count)(xSize * ySize * ratio);
			for (i = 0;(i < max) && (count < limit);i++)
			{
				count += histogramme[i];
			}
			threshold = i;
		}
		b3PrintF(B3LOG_FULL,
			"### CLASS: b3Tx   # b3ComputeThreshold() using %ld as threshold.\n",threshold);
		b3PrintF(B3LOG_FULL,
			"### CLASS: b3Tx   # b3ComputeThreshold() %ld histogramme entries.\n",max);
		b3PrintF(B3LOG_FULL,
			"### CLASS: b3Tx   # b3ComputeThreshold() ratio: %3.2f%%.\n",ratio * 100.0);
	}
	return threshold;
}

b3_bool b3Tx::b3TransToBW(b3_index threshold)
{
	b3_u08        *dPtr,*ptr;
	b3_pkd_color  *pPtr;
	b3_u08        *cPtr;
	b3_u16        *sPtr;
	b3_pkd_color  *lPtr;
	b3_coord       x,y;
	b3_count       xBytes,newSize;
	b3_index       i,grey;
	b3_u08         byte;
	b3_f64         r,g,b;
	b3_pkd_color   color,bit;
	b3_bool        grey_palette;
	b3_bool        result = false;

	// Check if nothing is to do...
	if (depth == 1)
	{
		return result;
	}

	// First allocate new buffer;
	xBytes  = TX_BWA(xSize);
	newSize = xBytes * ySize;
	dPtr    = (b3_u08 *)b3Alloc(newSize);
	if (dPtr == null)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TransToBW(): Not enogh memory for new image buffer!\n");
		return result;
	}

	// alloc new palette
	pPtr = (b3_pkd_color *)b3Alloc(2 * sizeof(b3_pkd_color));
	if (pPtr == null)
	{
		b3Free (dPtr);
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3TransToBW(): Not enogh memory for new palette!\n");
		return result;
	}
	else
	{
		// init BW
		pPtr[0] = 0xffffff;
		pPtr[1] = 0x000000;
	}

	grey_palette = b3IsGreyPalette();
	switch (type)
	{
	case B3_TX_VGA:
		cPtr = (b3_u08 *)data;
		ptr  = dPtr;
		for (y = 0;y < ySize;y++)
		{
			byte =   0;
			bit  = 128;
			i    =   0;
			for (x = 0;x < xSize;x++)
			{
				if (grey_palette)
				{
					grey = *cPtr++;
				}
				else
				{
					color = palette[*cPtr++];
					r = ((color & 0xff0000) >> 16) * 0.35;
					g = ((color & 0x00ff00) >>  8) * 0.51;
					b = ((color & 0x0000ff))       * 0.14;

					grey = (b3_pkd_color)(r + g + b);
				}

				// Set bit if over threshold
				if (grey < threshold)
				{
					byte |= bit;
				}

				// Shift bit
				bit = bit >> 1;
				if (bit == 0)
				{
					ptr[i] = byte;
					bit    = 128;
					byte   =   0;
					i++;
				}
			}

			// Put cached byte into memory
			if (i < xBytes)
			{
				ptr[i] = byte;
			}
			ptr += xBytes;
		}
		result = true;
		break;

	case B3_TX_RGB4:
		sPtr = (b3_u16 *)data;
		ptr  = dPtr;
		for (y = 0;y < ySize;y++)
		{
			byte =   0;
			bit  = 128;
			i    =   0;
			for (x = 0;x < xSize;x++)
			{
				color = *sPtr++;
				r = ((color & 0xf00) >> 4) * 0.35;
				g = ((color & 0x0f0))      * 0.51;
				b = ((color & 0x00f) << 4) * 0.14;

				// Set bit if over threshold
				if ((r + g + b) < threshold)
				{
					byte |= bit;
				}

				// Shift bit
				bit = bit >> 1;
				if (bit == 0)
				{
					ptr[i] = byte;
					bit    = 128;
					byte   =   0;
					i++;
				}
			}

			// Put cached byte into memory
			if (i < xBytes)
			{
				ptr[i] = byte;
			}
			ptr += xBytes;
		}
		result = true;
		break;

	case B3_TX_RGB8:
		lPtr = (b3_pkd_color *)data;
		ptr  = dPtr;
		for (y = 0;y < ySize;y++)
		{
			byte =   0;
			bit  = 128;
			i    =   0;
			for (x = 0;x < xSize;x++)
			{
				color = *lPtr++;
				r = ((color & 0xff0000) >> 16) * 0.35;
				g = ((color & 0x00ff00) >>  8) * 0.51;
				b = ((color & 0x0000ff))       * 0.14;

				// Set bit if over threshold
				if ((r + g + b) < threshold)
				{
					byte |= bit;
				}

				// Shift bit
				bit = bit >> 1;
				if (bit == 0)
				{
					ptr[i] = byte;
					bit    = 128;
					byte   =   0;
					i++;
				}
			}

			// Put cached byte into memory
			if (i < xBytes)
			{
				ptr[i] = byte;
			}
			ptr += xBytes;
		}
		result = true;
		break;
	}

	// Replace image information
	try
	{
		b3EndHist();
		b3Free(data);
		b3Free(palette);
	}
	catch (b3MemException *e)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3Trans2BW(): foreign pointer found - not freeing.\n");
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   #               error code: %d\n",e->b3GetError());
	}
	catch (...)
	{
		result = false;
	}
	data    = dPtr;
	palette = pPtr;
	dSize   = newSize;
	pSize   = 2;
	depth   = 1;
	type    = B3_TX_ILBM;

	return result;
}

