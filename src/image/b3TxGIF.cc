/*
**
**	$Filename:	b3TxGIF.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Loading GIF images
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include <blz3/image/b3Tx.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/10/13 09:56:44  sm
**	- Minor corrections
**
**	Revision 1.1  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        GIF - decoder                                 **
**                                                                      **
*************************************************************************/

static const long firstrow[4] = {4,2,1,0};
static const long nextrow[4]  = {8,8,4,2};
static const long mask[13]    =
{
	0x000, 0x001, 0x003, 0x007, 0x00F,
	0x01F, 0x03F, 0x07F, 0x0FF, 0x1FF,
	0x3FF, 0x7FF, 0xFFF
};

class b3GifDecoder
{
public:
	b3_count bitsleft,availbytes,currbyte;

public:
	b3GifDecoder()
	{
		bitsleft   = 0;
		availbytes = 0;
		currbyte   = 0;
	}

	b3_count b3GetNextGifCode (b3_u08 **Data,b3_count currsize)		 			
	{
		long code;

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
		return (code & mask[currsize]);
	}
};

b3_tx_type b3Tx::b3ParseGIF (b3_u08 *buffer)
{
	b3_index       i;
	b3_count       Colors;
	b3_size        DataSize, currsize;
	b3_pkd_color   t;
	b3_u08        *out;
	b3_u08        *sp,*suffix,*charstack;
	b3_u08         fc;
	b3_u32        *prefix;
	b3_coord       xk,yk;
	b3_u32         status,code, oldcode,clearcode;
	b3_u32         endcode,newcodes,maxcode,codecnt,c,size;
	b3_bool        interlaced;
	b3GifDecoder   decoder;

	FileType          = FT_GIF;
	depth             = (buffer[10] & 0x07) + 1;
	Colors            = (1 << depth);

	// Allocate memory for palette
	palette = (b3_pkd_color *)b3Alloc (sizeof(b3_pkd_color) * 256);
	if (palette==null)
	{
		return B3_TX_UNDEFINED;
	}

	// Pointer to global color map
	buffer += 13;
	for (i = 0;i < Colors; i++)
	{
		
		t  = ((b3_pkd_color) *buffer++ << 16);
		t += ((b3_pkd_color) *buffer++ <<  8);
		t +=  (b3_pkd_color) *buffer++;
		palette[i] = t;
	}


		/* auf Image seperator warten */
	while (buffer[0] != ',') switch (*buffer++)
	{
		case '!':                /* GIF Extension Block überlesen */
			buffer++;
			while ((t = *buffer++) != 0)
			{
				buffer += t;
			}
			break; /* untested */

		default :				 /* Fehler oder Ende des GIF dataset */
			return B3_TX_UNDEFINED;
	}

	xk = xSize = (buffer[5] + (buffer[6]<<8)) - (buffer[1] + (buffer[2]<<8));
	     ySize = (buffer[7] + (buffer[8]<<8)) - (buffer[3] + (buffer[4]<<8));
	interlaced = ((buffer[9] & 0x40) == 0x40);
	status = 0;

	/* auf Local-Color-Map testen*/
	if (buffer[9] & 0x80)
	{
		depth = (buffer[9] & 0x07) + 1;
		Colors = (1 << depth);
		buffer += 10;
		for (i=0; i<Colors; i++)
		{
			t  = ((unsigned long) *buffer++ << 16);
			t += ((unsigned long) *buffer++ <<  8);
			t +=  (unsigned long) *buffer++;
			palette[i] = t;
		}
	}
	else buffer += 10;

    /* Speicher fuer entpacktes Bild */
	DataSize = xSize * ySize;
	out      = (b3_u08 *)b3Alloc(DataSize);
	data = (b3_u08 *)out;
	if (out==null)
	{
		return B3_TX_UNDEFINED;
	}

	/* Speicher für temporären Stack und Tabellen anfordern */
	charstack = (unsigned char *)b3Alloc (
		(sizeof(unsigned char) +
		 sizeof(unsigned char) +
		 sizeof(unsigned long)) * 4096);
	if (charstack==null)
	{
		return B3_TX_UNDEFINED;
	}
	suffix = (charstack + 4096);					/* Pixelbyte */
	prefix = (b3_u32 *)(suffix + 4096);		/* Vorgaenger */

	size = *buffer++;								/* Codesize holen */
	if ((size<2) || (9<size))
	{
		b3Free (charstack);
		return B3_TX_UNDEFINED;
	}

	currsize    = size + 1;
	maxcode     = 1 << currsize;
	clearcode   = 1 << size;
	endcode     = clearcode + 1;
	codecnt     = newcodes = endcode + 1;
	oldcode     = fc = 0;
	sp          = charstack;
	decoder.bitsleft =
	decoder.availbytes = yk = 0;

	while ((c = decoder.b3GetNextGifCode(&buffer,currsize)) != endcode)
	{
		/* auf clearcode testen */
		if (c == clearcode)
		{
			currsize = size + 1;
			codecnt = newcodes;
			maxcode = 1 << currsize;

			while ((c = decoder.b3GetNextGifCode(&buffer,currsize)) == clearcode);

			if (c == endcode)
				break;

			if (c >= codecnt)
				c = 0;

			fc = oldcode = c;

			*sp++ = c;
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
				if (--xk==0)
				{
					xk = xSize;

					if ((yk+=nextrow[status]) >= ySize)
					{
						yk  = firstrow[status];
						out = (unsigned char *)(data +
							firstrow[status++] * xSize);
					}
					else
					{
						out += (nextrow[status]-1) * xSize;
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

	b3Free(charstack);
	return B3_TX_VGA;
}
