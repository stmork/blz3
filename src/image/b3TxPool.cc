/*
**
**	$Filename:	b3TxPool.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture pool
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

#include "blz3/image/b3TxPool.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Endian.h"

#include "b3TxIFF.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2001/10/13 15:48:53  sm
**	- Minor image loading corrections
**
**	Revision 1.7  2001/10/13 15:35:32  sm
**	- Adding further image file format support.
**	
**	Revision 1.6  2001/10/13 09:20:49  sm
**	- Adding multi image file format support
**	
**	Revision 1.5  2001/10/11 16:06:33  sm
**	- Cleaning up b3BSpline with including isolated methods.
**	- Cleaning up endian conversion routines and collecting into
**	  b3Endian
**	- Cleaning up some datatypes for proper display in Together.
**	
**	Revision 1.4  2001/10/10 17:52:24  sm
**	- Texture loading (only reading into memory) running.
**	- Raytracing without OpenGL must be possible!
**	
**	Revision 1.3  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.2  2001/10/07 20:41:32  sm
**	- Updating MSVC project status
**	
**	Revision 1.1  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Texture maintainance                          **
**                                                                      **
*************************************************************************/

b3TxPool texture_pool;

b3TxPath::b3TxPath(const char *new_path) : b3Link<b3TxPath>(sizeof(b3TxPath))
{
	strcpy(path,new_path);
}

void b3TxPool::b3AddPath(const char *path)
{
	b3TxPath *path_item = new b3TxPath(path);

	b3PrintF(B3LOG_DEBUG,"Adding texture path \"%s\"\n",
		(const char *)*path_item);
	m_SearchPath.b3Append(path_item);
}

b3TxPool::b3TxPool()
{
	m_SearchPath.b3InitBase();
	m_Pool.b3InitBase();
}

b3TxPool::~b3TxPool()
{
	b3TxPath *path_item;

	while(path_item = m_SearchPath.First)
	{
		m_SearchPath.b3Remove(path_item);
		delete path_item;
	}
}

/*************************************************************************
**                                                                      **
**                        Parsing routines                              **
**                                                                      **
*************************************************************************/

b3_tx_type b3Tx::b3ParseTexture (b3_u08 *buffer,b3_size buffer_size)
{
	b3_pkd_color *LongData;
	HeaderTIFF   *TIFF;
	HeaderSGI    *HeaderSGI;
	b3_offset     pos;
	b3_coord      x,y;
	b3_s32        ppm_type;
	b3_index      i;

	b3FreeTx();
	LongData = (b3_pkd_color *)buffer;
	if (buffer_size < 4)
	{
		return type = B3_TX_UNDEFINED;
	}

	// schon mal irgend ein IFF
	if (b3Endian::b3GetMot32(LongData) == IFF_FORM)
	{
		switch (b3Endian::b3GetMot32(&LongData[2]))
		{
			case IFF_ILBM : return type = b3ParseIFF_ILBM(buffer,buffer_size);
			case IFF_RGB8 :	return type = b3ParseIFF_RGB8(buffer,buffer_size);
			case IFF_RGBN : return type = b3ParseIFF_RGB4(buffer,buffer_size);
			case IFF_YUVN : return type = b3ParseIFF_YUVN(buffer,buffer_size);
			default :		return type = B3_TX_UNDEFINED;
		}
	}


	// JPEG
	for (i = 0;i < (b3_index)B3_MIN(256,buffer_size - 2);i++)
	{
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xd8) && (buffer[i+2] == 0xff))
		{
			if (strncmp ((const char *)&buffer[i+6],"JFIF",4) == 0)
			{
				return type = b3ParseJPEG(&buffer[i],buffer_size - i);
			}
		}
	}


	// GIF
	if (strncmp((const char *)buffer,"GIF",3) == 0)
	{
		return type = b3ParseGIF (buffer);
	}


	// TIFF
	TIFF = (HeaderTIFF *)buffer;
	if ((TIFF->TypeCPU == B3_BIG_ENDIAN) || (TIFF->TypeCPU == B3_LITTLE_ENDIAN))
	{
#ifndef USE_TIFFLIB_LOAD
		if (TIFF->VersionTIFF == 0x2a00)
		{
			ChangeTIFF (TIFF);
		}
		if (TIFF->VersionTIFF == 0x002a)
		{
			return type = b3ParseTIFF(TIFF,buffer_size);
		}
#else
		b3LoadTIFF(b3Name(),buffer,buffer_size);
		return type;
#endif
	}


	// PPM6
	pos = 0;
	i   = sscanf((const char *)buffer,"P%ld %ld %ld %*d%n",&ppm_type,&x,&y,&pos);
	b3PrintF (B3LOG_FULL,"PxM (%ld): (%ld,%ld - %ld) %d\n",ppm_type,x,y,i,pos);
	if (i >= 2)
	{
		switch (ppm_type)
		{
			case 4 :
				if ((b3_size)(((x + 7) >> 3) * y + pos) <= buffer_size)
				{
					pos = buffer_size - ((x + 7) >> 3) * y;
					FileType = FT_PBM;
					return type = b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;

			case 5 :
				if ((b3_size)(x * y + pos) <= buffer_size)
				{
					pos = buffer_size - x * y;
					FileType = FT_PGM;
					return type = b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;
				
			case 6 :
				if ((b3_size)(x * y * 3 + pos) <= buffer_size)
				{
					pos = buffer_size - 3 * x * y;
					FileType = FT_PPM;
					return type = b3ParseRAW (&buffer[pos],x,y,ppm_type);
				}
				break;

			case 1 : /* ASCII pendants */
			case 2 :
			case 3 :
			default :
				FileType = FT_ERR_UNSUPP;
				break;
		}
	}


	// BMP
	if ((buffer[0] == 'B') &&
	    (buffer[1] == 'M') &&
	    (b3Endian::b3GetIntel32(&buffer[2]) == buffer_size))
	{
		return type = b3ParseBMP(buffer);
	}


	// MTV
	if (sscanf((const char *)buffer,"%ld %ld",&x,&y) == 2)
	{
		for (pos = 0;(pos < 100) && (buffer[pos] != 10);pos++);
		if (buffer[pos++] == 10)
		{
			if ((b3_size)(x * y * 3 + pos) == buffer_size)
			{
				FileType = FT_MTV;
				return type = b3ParseRAW (&buffer[pos],x,y,6);
			}
		}
	}


	// BMF
	x = b3Endian::b3GetIntel16 (&buffer[2]);
	y = b3Endian::b3GetIntel16 (&buffer[4]);
	switch (buffer[6])
	{
		case 2 :
			i = 1;
			break;
		case 4 :
			i = 3;
			break;
		default :
			i = 0;
			break;
	}
	if ((b3_size)(x * y * i + 16) == buffer_size)
	{
		return type = b3ParseBMF(buffer,buffer_size);
	}
	
	// SGI RLE
	HeaderSGI = (struct HeaderSGI *)buffer;
	if ((HeaderSGI->imagic == IMAGIC1) || (HeaderSGI->imagic == IMAGIC2))
	{
		return type = b3ParseSGI(buffer);
	}


	// Targa
	if ((buffer[2] == 2) || (buffer[2] == 10))
	{
		if ((b3Endian::b3Get32(&buffer[4])  ==  0) &&
		    (b3Endian::b3Get32(&buffer[8]) == 0))
		{
			if ((buffer[16]==32) || (buffer[16]==24))
			{
				return type = b3ParseTGA(buffer);
			}
		}
	}


	// PCX
	if ((buffer[2] == 1) && (buffer[0] == 0x0a))
	{
		switch (buffer[3])			/* Bits pro Pixel */
		{
			case 8  : return type = b3ParsePCX8(buffer);
			case 1 	: return type = b3ParsePCX4(buffer);
			default : return type = B3_TX_UNDEFINED;
		}
	}


	// really unknown
	return type = B3_TX_UNDEFINED;
}

void b3TxPool::b3ReloadTexture (b3Tx *Texture,const char *Name) /* 30.12.94 */
{
	b3TxPath *path;
	b3File    TextureFile;
	b3Path    FullName;
	b3_u08   *Data = null;
	b3_size   FileSize;

	if ((Name != null) && (strlen(Name) > 0))
	{
		try
		{
			strcpy(FullName,Name);
			b3PrintF(B3LOG_FULL,"Trying \"%s\"...\n",(char *)FullName);
			Data = TextureFile.b3ReadBuffer(FullName,FileSize);
		}
		catch(...)
		{
			B3_FOR_BASE(&m_SearchPath,path)
			{
				try
				{
					b3Path::b3LinkFileName(
						(char *)FullName,
						(const char *)*path,Name);
					b3PrintF(B3LOG_FULL,"Trying \"%s\"...\n",(const char *)FullName);
					Data = TextureFile.b3ReadBuffer(FullName,FileSize);

					// OK! Texture loaded -> leave search loop!
					break;
				}
				catch(b3FileException *f)
				{
					// An exception we expected.
					b3PrintF(B3LOG_FULL,"Error code: %d\n",f->b3GetError());
				}
			}
		}
	}
	else
	{
		Name = "<unnamed>";
	}

	// Check result of texture load
	if (Data == null)
	{
		Texture->b3Name(Name);
		b3PrintF (B3LOG_DEBUG,"\"%s\" not available!\n",Texture->b3Name());
		return;
	}

	Texture->b3Name(FullName);
	Texture->b3ParseTexture(Data,FileSize);
	b3PrintF(B3LOG_DEBUG,"Texture \"%s\" loaded. [%p,%d bytes]\n",
		Texture->b3Name(),Data,FileSize);
}

b3Tx *b3TxPool::b3LoadTexture (const char *Name) /* 06.12.92 */
{
	b3Tx       *Texture;
	const char *txName;
	b3_size     txLen,nameLen,diff;

	// find existing texture
	B3_FOR_BASE(&m_Pool,Texture)
	{
		txName  = Texture->b3Name();
		txLen   = strlen(txName);
		nameLen = strlen(Name);
		diff    = txLen - nameLen;
		if (strcmp(&txName[diff >= 0 ? diff : 0],Name) == 0)
		{
			if (Texture->b3GetData() == null)
			{
#if 0
				b3ReloadTexture (Texture);
			}
			else
			{
#endif
				b3PrintF (B3LOG_DEBUG,"%s: found.\n",Name);
			}
			return Texture;
		}
	}

	// OK, create new texture
	Texture = new b3Tx();

	// load data and insert in internal list
	b3ReloadTexture (Texture,Name);
	m_Pool.b3Append(Texture);
	return Texture;
}
