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

#include "blz3/image/b3Tx.h"
#include "blz3/base/b3Endian.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2002/01/01 13:50:21  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**
**	Revision 1.6  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.5  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.4  2001/10/24 14:59:08  sm
**	- Some GIG bug fixes
**	- An image viewing bug fixed in bimg3
**	
**	Revision 1.3  2001/10/15 14:45:07  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
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

b3_result b3Tx::b3ParseGIF (b3_u08 *buffer)
{
	b3_index       i;
	b3_count       Colors;
	b3_size        currsize;
	b3_pkd_color   t;
	b3_u08        *pPtr = buffer;
	b3_u08        *out;
	b3_u08        *sp,*suffix,*charstack;
	b3_u08         fc;
	b3_u32        *prefix;
	b3_coord       xk,yk;
	b3_res         xNewSize,yNewSize,NewDepth;
	b3_u32         status,code, oldcode,clearcode;
	b3_u32         endcode,newcodes,maxcode,codecnt,c,size;
	b3_bool        interlaced;
	b3GifDecoder   decoder;

	b3PrintF(B3LOG_FULL,"IMG GIF  # b3ParseGIF(%s)\n",
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
			b3PrintF(B3LOG_NORMAL,"IMG GIF  # Parsing error:\n");
			b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
			b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
			throw new b3TxException(B3_TX_ERR_HEADER);
	}

	// Compute image resolution
	xk = xNewSize  = b3Endian::b3GetIntel16(&buffer[5]) - b3Endian::b3GetIntel16(&buffer[1]);
	     yNewSize  = b3Endian::b3GetIntel16(&buffer[7]) - b3Endian::b3GetIntel16(&buffer[3]);
	if (!b3AllocTx(xNewSize,yNewSize,8))
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG GIF  # Error allocating memory:\n");
		b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
		b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
		throw new b3TxException(B3_TX_MEMORY);
	}
	out = (b3_u08 *)data;

	// Pointer to global color map
	pPtr     += 13;
	for (i = 0;i < Colors; i++)
	{
		
		t  = ((b3_pkd_color) *pPtr++ << 16);
		t += ((b3_pkd_color) *pPtr++ <<  8);
		t +=  (b3_pkd_color) *pPtr++;
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
		for (i = 0;i < Colors; i++)
		{
			t  = ((unsigned long) *buffer++ << 16);
			t += ((unsigned long) *buffer++ <<  8);
			t +=  (unsigned long) *buffer++;
			palette[i] = t;
		}
	}
	else
	{
		buffer += 10;
	}

	/* Speicher für temporären Stack und Tabellen anfordern */
	charstack = (b3_u08 *)b3Alloc ((
		sizeof(b3_u08) +
		sizeof(b3_u08) +
		sizeof(b3_u32)) * 4096);
	if (charstack==null)
	{
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG GIF  # Error allocating memory:\n");
		b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
		b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
		throw new b3TxException(B3_TX_MEMORY);
	}
	suffix =           &charstack[4096]; // pixel byte
	prefix = (b3_u32 *)&charstack[8192]; // predecessor

	size = *buffer++; // get code size
	if ((size<2) || (9<size))
	{
		b3Free (charstack);
		b3FreeTx();
		b3PrintF(B3LOG_NORMAL,"IMG GIF  # Decompression format error:\n");
		b3PrintF(B3LOG_NORMAL,"           file: %s\n",__FILE__);
		b3PrintF(B3LOG_NORMAL,"           line: %d\n",__LINE__);
		throw new b3TxException(B3_TX_ERR_HEADER);
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
	FileType = FT_GIF;
	return B3_OK;
}
