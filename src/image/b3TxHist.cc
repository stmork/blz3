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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                        some constant tables                          **
**                                                                      **
*************************************************************************/

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

class b3_tx_fill_bits
{
	static b3_tx_fill_bits m_FillBits;

	inline b3_tx_fill_bits()
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
	}

	b3_u08  bwBitsSet[256];
	b3_u08  bwBitsClr[256];

	friend class b3Tx;
};

b3_tx_fill_bits b3_tx_fill_bits::m_FillBits;

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
		B3_THROW(b3TxException,B3_TX_MEMORY);
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
				histogramme[0] += b3_tx_fill_bits::m_FillBits.bwBitsClr[cPtr[x]];
				histogramme[1] += b3_tx_fill_bits::m_FillBits.bwBitsSet[cPtr[x]];
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

	if (type == B3_TX_FLOAT)
	{
		b3_coord      x,y;
		b3_index      index;
		b3_color     *cPtr;
		b3_pkd_color  grey;
		b3_f64        r,g,b;

		cPtr  = (b3_color *)data;
		index = yStart * xSize;
		for (y = yStart;y < yStop;y++)
		{
			for (x = 0;x < xSize;x++)
			{
				r = cPtr[index].r *  89.25;
				g = cPtr[index].g * 130.05;
				b = cPtr[index].b *  35.7;
				index++;

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
	b3_u08        *bPtr;
	b3_u16        *sPtr;
	b3_pkd_color  *lPtr;
	b3_color      *cPtr;
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
		bPtr = (b3_u08 *)data;
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
					grey = *bPtr++;
				}
				else
				{
					color = palette[*bPtr++];
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

	case B3_TX_FLOAT:
		cPtr = (b3_color *)data;
		ptr  = dPtr;
		for (y = 0;y < ySize;y++)
		{
			byte =   0;
			bit  = 128;
			i    =   0;
			for (x = 0;x < xSize;x++)
			{
				r = cPtr[x].r *  89.25;
				g = cPtr[x].g * 130.05;
				b = cPtr[x].b *  35.7;

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

	default:
		break;
	}

	// Replace image information
	try
	{
		b3EndHist();
		b3Free(data);
		b3Free(palette);
	}
	catch (b3MemException &e)
	{
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   # b3Trans2BW(): foreign pointer found - not freeing.\n");
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   #               error code: %d\n",e.b3GetError());
		b3PrintF(B3LOG_NORMAL,
			"### CLASS: b3Tx   #               error msg:  %s\n",e.b3GetErrorMsg());
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
