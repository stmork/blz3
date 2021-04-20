/*
**
**	$Filename:	b3TxGIF.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading GIF images
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
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
**                        GIF - decoder                                 **
**                                                                      **
*************************************************************************/

class b3GifDecoder
{
	static const b3_res m_GifFirstRow[4];
	static const b3_res m_GifNextRow[4];
	static const b3_res m_GifMask[13];

public:
	b3_size bitsleft, availbytes, currbyte;

public:
	b3GifDecoder()
	{
		bitsleft   = 0;
		availbytes = 0;
		currbyte   = 0;
	}

	b3_u32 b3GetNextGifCode(const b3_u08 ** Data, b3_size currsize)
	{
		b3_size code;

		if (bitsleft == 0)
		{
			if (availbytes <= 0)
			{
				availbytes = **Data;
				*Data += 1;
			}

			currbyte = **Data;
			*Data   += 1;
			bitsleft = 8;
			availbytes--;
		}

		code = currbyte >> (8 - bitsleft);
		while (currsize > bitsleft)
		{
			if (availbytes <= 0)
			{
				availbytes = **Data;
				*Data     += 1;
			}

			currbyte = **Data;
			*Data    += 1;
			code     |= currbyte << bitsleft;
			bitsleft += 8;
			availbytes--;
		}

		bitsleft -= currsize;
		return (code & m_GifMask[currsize]);
	}

	friend class b3Tx;
};

const b3_res b3GifDecoder::m_GifFirstRow[4]
{
	4, 2, 1, 0
};

const b3_res b3GifDecoder::m_GifNextRow[4]
{
	8, 8, 4, 2
};

const b3_res b3GifDecoder::m_GifMask[13]
{
	0x000, 0x001, 0x003, 0x007, 0x00F,
	0x01F, 0x03F, 0x07F, 0x0FF, 0x1FF,
	0x3FF, 0x7FF, 0xFFF
};

b3_result b3Tx::b3ParseGIF(const b3_u08 * buffer)
{
	b3_index       i;
	b3_count       Colors;
	b3_size        currsize;
	b3_u32         c;
	b3_pkd_color   t;
	const b3_u08    *    pPtr = buffer;
	b3_u08    *    out;
	b3_u08    *    sp;
	b3_u08         suffix[4096];
	b3_u08         charstack[4096];
	b3_u08         fc;
	b3_u32         prefix[4096];
	b3_coord       xk, yk;
	b3_res         xNewSize, yNewSize, NewDepth;
	b3_u32         status, code, oldcode, clearcode;
	b3_u32         endcode, newcodes, maxcode, codecnt, size;
	b3_bool        interlaced;
	b3GifDecoder   decoder;

	b3PrintF(B3LOG_FULL, "IMG GIF  # b3ParseGIF(%s)\n",
		(const char *)image_name);

	NewDepth = (pPtr[10] & 0x07) + 1;
	Colors   = (1 << NewDepth);
	buffer  += (13 + (Colors * 3));

	// Wait for image separator
	while (buffer[0] != ',') switch (*buffer++)
		{
		case '!':
			// Overread extension block
			buffer++;
			while ((t = *buffer++) != 0)
			{
				buffer += t;
			}
			break; /* untested */

		default :
			// Error or end of GIF dataset */
			b3FreeTx();
			b3PrintF(B3LOG_NORMAL, "IMG GIF  # Parsing error:\n");
			B3_THROW(b3TxException, B3_TX_ERR_HEADER);
		}

	// Compute image resolution
	xk = xNewSize  = b3Endian::b3GetIntel16(&buffer[5]) - b3Endian::b3GetIntel16(&buffer[1]);
	yNewSize  = b3Endian::b3GetIntel16(&buffer[7]) - b3Endian::b3GetIntel16(&buffer[3]);
	if (!b3AllocTx(xNewSize, yNewSize, 8))
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG GIF  # Error allocating memory:\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}
	out = data;

	// Pointer to global color map
	pPtr += 13;
	for (i = 0; i < Colors; i++)
	{

		t  = ((b3_pkd_color) * pPtr++ << 16);
		t += ((b3_pkd_color) * pPtr++ <<  8);
		t += (b3_pkd_color) * pPtr++;
		palette[i] = t;
	}

	interlaced = ((buffer[9] & 0x40) == 0x40);
	status = 0;

	/* auf Local-Color-Map testen*/
	if (buffer[9] & 0x80)
	{
		depth   = (buffer[9] & 0x07) + 1;
		Colors  = (1 << depth);
		buffer += 10;
		for (i = 0; i < Colors; i++)
		{
			t  = ((unsigned long) * buffer++ << 16);
			t += ((unsigned long) * buffer++ <<  8);
			t += (unsigned long) * buffer++;
			palette[i] = t;
		}
	}
	else
	{
		buffer += 10;
	}

	/* Speicher für temporären Stack und Tabellen anfordern */
	size = *buffer++; // get code size
	if ((size < 2) || (9 < size))
	{
		b3Free(charstack);
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL, "IMG GIF  # Decompression format error:\n");
		B3_THROW(b3TxException, B3_TX_ERR_HEADER);
	}

	currsize    = size + 1;
	maxcode     = 1 << currsize;
	clearcode   = 1 << size;
	endcode     = clearcode + 1;
	codecnt     = newcodes = endcode + 1;
	oldcode     = fc = 0;
	sp          = charstack;
	decoder.bitsleft = decoder.availbytes = yk = 0;

	while ((c = decoder.b3GetNextGifCode(&buffer, currsize)) != endcode)
	{
		/* auf clearcode testen */
		if (c == clearcode)
		{
			currsize = size + 1;
			codecnt = newcodes;
			maxcode = 1 << currsize;

			while ((c = decoder.b3GetNextGifCode(&buffer, currsize)) == clearcode);

			if (c == endcode)
			{
				break;
			}

			if (c >= codecnt)
			{
				c = 0;
			}

			fc      = (b3_u08)c;
			oldcode = c;

			*sp++ = fc;
		}
		else
		{
			code = c;

			/* falls code 'out of Range', einfach den letzten nehmen ... */
			if (code >= codecnt)
			{
				code = oldcode;
				*sp++ = fc;
			}

			/* Pixelkette aus Code ermitteln */
			while (code >= newcodes)
			{
				*sp++ = suffix[code];		/* Pixel auf Stack legen */
				code = prefix[code];		/* Vorgänger */
			}
			*sp++ = (unsigned char) code;	/* erstes Pixel der Kette */

			/* logisch nächste Pixelkette in Tabelle eintragen */
			if (codecnt < maxcode)
			{
				fc = code;
				suffix[codecnt] = fc;
				prefix[codecnt++] = oldcode;
				oldcode = c;
			}

			/* Codelänge ggf. um ein Bit verlängern */
			if (codecnt >= maxcode)
			{
				if (currsize < 12)
				{
					maxcode <<= 1;
					currsize++;
				}
			}
		}

		if (interlaced)
		{
			while (sp > charstack)			/* Stack auslesen */
			{
				*out++ = *(--sp);
				if (--xk == 0)
				{
					xk = xSize;

					if ((yk += b3GifDecoder::m_GifNextRow[status]) >= ySize)
					{
						yk  = b3GifDecoder::m_GifFirstRow[status];
						out = data +
							b3GifDecoder::m_GifFirstRow[status++] * xSize;
					}
					else
					{
						out += (b3GifDecoder::m_GifNextRow[status] - 1) * xSize;
					}
				}
			}
		}
		else
		{
			while (sp > charstack)
			{
				*out++ = *(--sp); /* Stack auslesen */
			}
		}


	}

	FileType = FT_GIF;
	return B3_OK;
}
