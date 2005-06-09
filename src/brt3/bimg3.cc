/*
**
**	$Filename:	TestImage.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Testing image loading and displaying
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/system/b3DisplayView.h"
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.21  2005/06/09 09:24:00  smork
**	- Added image conversion tool to installation.
**
**	Revision 1.20  2005/01/21 10:28:59  smork
**	- Corrected exe banner.
**	
**	Revision 1.19  2004/11/29 09:58:00  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.18  2004/08/22 09:39:26  sm
**	- Found TGA file as JPEG. Fixed.
**	- Some exception handling problems found in bimg3.
**	
**	Revision 1.17  2004/01/05 08:01:55  sm
**	- Added new year.
**	
**	Revision 1.16  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
**	Revision 1.15  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**	
**	Revision 1.14  2002/10/06 14:58:18  sm
**	- Done some finetuning on setup
**	- Output of GCC version under Un*x
**	
**	Revision 1.13  2002/09/01 15:33:36  sm
**	- Updated makefiles
**	
**	Revision 1.12  2002/08/28 15:31:00  sm
**	- Added some help output to render and bimg3.
**	
**	Revision 1.11  2002/08/11 11:22:33  sm
**	- Adjusted some includes concerning b3DisplayView.
**	
**	Revision 1.10  2002/07/20 10:49:34  sm
**	- Added custom light support (not finished yet)
**	- Added b3Light::b3IsActive() for compatibility.
**	- Added texture search path support like in brt3.
**	
**	Revision 1.9  2002/05/08 14:44:46  sm
**	- Insert Tag names
**	
**	Revision 1.8  2002/05/08 14:37:56  sm
**	- Output some information when not specifying data files.
**	
**	Revision 1.7  2001/12/23 10:58:38  sm
**	- Accelerated b3Display.
**	- Fixed YUV conversion.
**	- Accelerated ILBM access to image  pixel/row.
**	
**	Revision 1.6  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**	
**	Revision 1.5  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.4  2001/10/24 14:59:08  sm
**	- Some GIG bug fixes
**	- An image viewing bug fixed in bimg3
**	
**	Revision 1.3  2001/10/23 15:50:31  sm
**	- Now parsing PCX4 correctly
**	- Found TGA parsing bug.
**	- Correcting path following behaviour.
**	
**	Revision 1.2  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.1  2001/10/15 14:46:11  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
**
*/

static b3TxPool texture_pool;

class b3ImageRow : public b3Row
{
public:
	b3ImageRow(b3_res xSize) : b3Row(0,xSize)
	{
	}

	void b3SetRow(b3Tx *tx,b3_coord y)
	{
		m_y = y;
		tx->b3GetRow(m_buffer,y);
	}
};

static void display(b3Tx *tx)
{
	b3Display   *display;

	if (tx->b3IsLoaded())
	{
		b3PrintF(B3LOG_NORMAL,"%s: %dx%d\n",
			tx->b3Name(),tx->xSize,tx->ySize);
		display = new b3DisplayView(tx->xSize,tx->ySize,tx->b3Name());
		display->b3PutTx(tx);
		display->b3Wait();
		delete display;
	}
	else
	{
		 b3PrintF(B3LOG_NORMAL,"%s not displayed\n",
		 	tx->b3Name());
	}
}

static void load(const char *name)
{
	try
	{
		texture_pool.b3LoadTexture(name);
		b3PrintF(B3LOG_NORMAL,".");
	}
	catch(b3TxException &txe)
	{
		b3PrintF(B3LOG_NORMAL,"\nImage Exception of file %s!\n",name);
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"\nError of file %s!\n",name);
	}
}

int main(int argc,char *argv[])
{
	char                 *BLZ3_TEXTURES    = getenv("BLZ3_TEXTURES");
	char                 *BLZ3_PICTURES    = getenv("BLZ3_PICTURES");
	char                 *HOME             = getenv("HOME");
	b3Path                textures;
	b3Path                pictures;
	b3FileList   list;
	b3FileEntry *entry;
	b3Tx        *tx;
	int          i;

	if (argc > 1)
	{
		if (HOME != null)
		{
			b3Dir::b3LinkFileName(textures,HOME,"Blizzard/Textures");
			b3Dir::b3LinkFileName(pictures,HOME,"Blizzard/Pictures");
			texture_pool.b3AddPath(textures);
			if (BLZ3_TEXTURES != null)
			{
				texture_pool.b3AddPath(BLZ3_TEXTURES);
			}
			texture_pool.b3AddPath(pictures);
		}

		for (i = 1;i < argc;i++)
		{
			if (argv[i][0] == '-')
			{
				switch(argv[i][1])
				{
				case 'd' :
					b3Log::b3SetLevel(B3LOG_DEBUG);
					break;
				case 'f' :
					b3Log::b3SetLevel(B3LOG_FULL);
					break;
				}
			}
			else
			{
				switch(b3Dir::b3Exists(argv[i]))
				{
				case B3_TYPE_DIR:
					list.b3CreateList(argv[i]);
					for (entry = list.b3First();entry != null;entry = entry->Succ)
					{
						load(entry->b3Name());
					}
					break;

				case B3_TYPE_FILE:
				case B3_NOT_EXISTANT:
					load(argv[i]);
					break;

				default:
					break;
				}
			}
		}
		b3PrintF(B3LOG_NORMAL,"\n");

		B3_FOR_BASE(texture_pool.b3GetTxHead(),tx)
		{
			display(tx);
		}
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Simple image viewer\n");
		b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001, 2002, 2003, 2004, 2005\n");
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s {Image files}\n",argv[0]);
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
		b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
	}
	return EXIT_SUCCESS;
}
