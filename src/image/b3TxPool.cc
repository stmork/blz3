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

/*
#include <blizzard/system/config.h>
#include <blizzard/system/tdisplay.h>
#include <blizzard/system/bio.h>
#include <blizzard/btools.h>
#include <blizzard/bimage.h>

#include "tx_main.h"
#include "tx_easy.h"
#include "tx_gif.h"
#include "tx_iff.h"
#include "tx_img.h"
#include "tx_jpeg.h"
#include "tx_pcx.h"
#include "tx_tga.h"
#include "tx_tiff.h"
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**                        get data types in processor manner.           **
**                                                                      **
*************************************************************************/

b3_u16 b3TxPool::b3GetShort(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
	{
		Value =                (long)Pointer[1];
		Value = (Value << 8) | (long)Pointer[0];
	}
	else
	{
		Value =                (long)Pointer[0];
		Value = (Value << 8) | (long)Pointer[1];
	}
	return Value;
}

b3_u32 b3TxPool::b3GetLong (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
	{
		Value =                (b3_u32)Pointer[3];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[0];
	}
	else 
	{
		Value =                (b3_u32)Pointer[0];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[3];
	}
	return Value;
}

/*************************************************************************
**                                                                      **
**                  get values in Motorola(R) order                     **
**                                                                      **
*************************************************************************/

b3_u16 b3TxPool::b3GetMotShort(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	Value =                (b3_u16)Pointer[0];
	Value = (Value << 8) | (b3_u16)Pointer[1];

	return Value;
}

b3_u32 b3TxPool::b3GetMotLong (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	Value =                (b3_u32)Pointer[0];
	Value = (Value << 8) | (b3_u32)Pointer[1];
	Value = (Value << 8) | (b3_u32)Pointer[2];
	Value = (Value << 8) | (b3_u32)Pointer[3];

	return Value;
}

/*************************************************************************
**                                                                      **
**                    get values in Intel(R) order                      **
**                                                                      **
*************************************************************************/

b3_u16 b3TxPool::b3GetIntelShort(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	Value =                (b3_u16)Pointer[1];
	Value = (Value << 8) | (b3_u16)Pointer[0];

	return Value;
}

b3_u32 b3TxPool::b3GetIntelLong (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	Value =                (b3_u32)Pointer[3];
	Value = (Value << 8) | (b3_u32)Pointer[2];
	Value = (Value << 8) | (b3_u32)Pointer[1];
	Value = (Value << 8) | (b3_u32)Pointer[0];

	return Value;
}

/*************************************************************************
**                                                                      **
**                  Change endian type of data types                    **
**                                                                      **
*************************************************************************/

b3_size b3TxPool::b3ChangeWord (void *Ptr)
{
	b3_u16 *Pointer = (b3_u16 *)Ptr;
	b3_u16	Value;

	Value = Pointer[0];
	Pointer[0] = ((Value & 0xff00) >> 8) | ((Value & 0xff) << 8);
	return 2;
}

b3_size b3TxPool::b3ChangeLong (void *Ptr)
{
	b3_u32 *Pointer = (b3_u32 *)Ptr;
	b3_u32  Value;

	Value = Pointer[0];
	Pointer[0] =
		((Value & 0xff000000) >> 24) |
		((Value & 0x00ff0000) >>  8) |
		((Value & 0x0000ff00) <<  8) |
		((Value & 0x000000ff) << 24);
	return 4;
}

/*************************************************************************
**                                                                      **
**                        random functions                              **
**                                                                      **
*************************************************************************/

long b3TxPool::b3ParseTexture (
	b3Tx    *Texture,
	b3_u08  *Data,
	b3_size  Size)
{
#if 0
	long              *LongData;
	struct HeaderTIFF *TIFF;
	struct HeaderSGI  *HeaderSGI;
	long               x,y,type,i;
	b3_offset          pos;

	Texture->b3FreeTx();
	LongData = (long *)Data;
	if (Size < 4)
	{
		return 0;
	}

	// schon mal irgend ein IFF
	if (b3GetMotLong(LongData) == IFF_FORM)
	{
		switch (GetMotLong(&LongData[2]))
		{
			case IFF_ILBM : return ParseIFF_ILBM(Texture,Data,Size);
			case IFF_RGB8 :	return ParseIFF_RGB8(Texture,Data,Size);
			case IFF_RGBN : return ParseIFF_RGBN(Texture,Data,Size);
			case IFF_YUVN : return ParseIFF_YUVN(Texture,Data,Size);
			default :		return 0;
		}
	}


	// JPEG
	for (i = 0;i < BMIN(256,Size - 2);i++)
	{
		if ((Data[i] == 0xff) && (Data[i+1] == 0xd8) && (Data[i+2] == 0xff))
		{
			if (strncmp (&Data[i+6],"JFIF",4) == 0)
			{
				return ParseJPEG(Texture,&Data[i],Size-i);
			}
		}
	}

	// GIF
	if (strncmp (Data,"GIF",3) == 0) return ParseGIF (Texture,Data);


	// TIFF
	TIFF = (struct HeaderTIFF *)Data;
	if ((TIFF->TypeCPU == 0x4d4d)||(TIFF->TypeCPU == 0x4949))
	{
#ifndef USE_TIFFLIB_LOAD
		if (TIFF->VersionTIFF == 0x2a00) ChangeTIFF (TIFF);
		if (TIFF->VersionTIFF == 0x002a) return ParseTIFF(Texture,TIFF,Size);
#else
		return ParseTIFF(Texture,TIFF,Size);
#endif
	}


	// PPM6
	pos = 0;
	i   = sscanf(Data,"P%ld %ld %ld %*d%n",&type,&x,&y,&pos);
/*	if (DBUG(2)) PrintF ("PxM (%ld): (%ld,%ld - %ld) %d\n",type,x,y,i,pos); */
	if (i >= 2)
	{
		switch (type)
		{
			case 4 :
				if ((((x + 7) >> 3) * y + pos) <= Size)
				{
					pos = Size - ((x + 7) >> 3) * y;
					Texture->FileType = FT_PBM;
					return ParseRAW (Texture,&Data[pos],x,y,type);
				}
				break;

			case 5 :
				if ((x * y + pos) <= Size)
				{
					pos = Size - x * y;
					Texture->FileType = FT_PGM;
					return ParseRAW (Texture,&Data[pos],x,y,type);
				}
				break;
				
			case 6 :
				if ((x * y * 3 + pos) <= Size)
				{
					pos = Size - 3 * x * y;
					Texture->FileType = FT_PPM;
					return ParseRAW (Texture,&Data[pos],x,y,type);
				}
				break;

			case 1 : /* ASCII pendants */
			case 2 :
			case 3 :
			default :
				Texture->FileType = FT_ERR_UNSUPP;
				break;
		}
	}


	// BMP
	if ((Data[0] == 'B') &&
	    (Data[1] == 'M') &&
	    (GetIntelLong(&Data[2]) == Size))
	{
		return ParseBMP(Texture,Data);
	}


		/* MTV */
	if (sscanf(Data,"%ld %ld",&x,&y) == 2)
	{
		for (pos = 0;(pos < 100) && (Data[pos] != 10);pos++);
		if (Data[pos++] == 10)
		{
			if ((x * y * 3 + pos) == Size)
			{
				Texture->FileType = FT_MTV;
				return ParseRAW (Texture,&Data[pos],x,y,6);
			}
		}
	}


	// BMF
	x = GetIntelShort (&Data[2]);
	y = GetIntelShort (&Data[4]);
	switch (Data[6])
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
	if ((x * y * i + 16) == Size) return ParseBMF(Texture,Data,Size);
	
	// SGI RLE
	HeaderSGI = (struct HeaderSGI *)Data;
	if ((HeaderSGI->imagic == IMAGIC1) || (HeaderSGI->imagic == IMAGIC2))
	{
		return ParseSGI (Texture,Data,Size);
	}


	// Targa
	if ((Data[2] == 2) || (Data[2] == 10))
		if ((GetLong (&Data[4])  ==  0) && (GetLong(&Data[8]) == 0))
			if ((Data[16]==32) || (Data[16]==24)) return ParseTGA (Texture,Data);


	// PCX
	if ((Data[2] == 1) && (Data[0] == 0x0a))
	{
		switch (Data[3])			/* Bits pro Pixel */
		{
			case 8  : return ParsePCX8(Texture,Data);
			case 1 	: return ParsePCX4(Texture,Data);
			default : return 0; 	
		}
	}

#endif
	// really unknown
	return 0;
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
	else
	{
		Texture->b3Name(FullName);
		b3PrintF(B3LOG_DEBUG,"Texture \"%s\" loaded. [%p,%d bytes]\n",
			Texture->b3Name(),Data,FileSize);
	}

#if 0
/*
PrintF ("%s (%ld)\n%s (%ld)\n%s (%ld)\n",
	Texture->Name,strlen(Texture->Name),
	FullName1,    strlen(FullName1),
	FullName2,    strlen(FullName2));
*/

	TextureFile = BOpen (Texture->Name,B_READ);
	if (TextureFile == null) TextureFile = BOpen (FullName1,B_READ);
	if (TextureFile == null) TextureFile = BOpen (FullName2,B_READ);
	if (TextureFile == null)
	{
		if (DBUG(1)) PrintF ("%s: not available!\n",Texture->Name);
		Texture->FileType = FT_ERR_OPEN;
	}
	else
	{
		FileSize = BSize (TextureFile);
		if (FileSize < 1)
		{
			Texture->FileType = FT_ERR_UNCOMPL;
			if (DBUG(1)) PrintF ("%s: empty file!\n",Texture->Name);
		}
		else
		{
			Data = (char *)AllocTextureMem (Texture,FileSize);
			if (Data==null)
			{
				Texture->FileType = FT_ERR_MEM;
				if (DBUG(1)) PrintF ("%s: not enough memory!!\n",Texture->Name);
			}
			else
			{
				if (BRead(TextureFile,Data,FileSize)==FileSize)
				{
					if ((Texture->Type = ParseTexture(Texture,Data,FileSize))
						== 0)
					{
						FreePartMem (&TextureMem,Texture->Data);
						FreePartMem (&TextureMem,Texture->Palette);
						Texture->Data = null;
						if (DBUG(1)) PrintF ("%s: ",Texture->Name);
						switch (Texture->FileType)
						{
							case FT_ERR_UNSUPP :
								if (DBUG(1)) PrintF ("unsupported data type!\n");
								break;
							case FT_ERR_PACKING :
								if (DBUG(1)) PrintF ("unsupported packing algorithm!\n");
								break;
							case FT_ERR_HEADER :
								if (DBUG(1)) PrintF ("unknown header!\n");
								break;
							case FT_ERR_MEM :
								if (DBUG(1)) PrintF ("insufficient memory!\n");
								break;
							case FT_UNKNOWN :
								if (DBUG(1)) PrintF ("unknown file format!\n");
								break;
							default :
								if (DBUG(1)) PrintF ("not loaded (%ld)!\n",Texture->FileType);
								break;
						}
					}
					else
					{
						if (DBUG(1)) PrintF ("%s: loaded.\n",Texture->Name);
						Texture->LastAccess = AccessPos;
					}
				}
				else if (DBUG(1)) PrintF ("%s: not completely loaded!\n",Texture->Name);
				FreePartMem (&TextureMem,Data);
			}
		}
		BClose (TextureFile);
	}
#endif
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
