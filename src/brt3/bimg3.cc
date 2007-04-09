/*
**
**	$Filename:	bimg3.cc $
**	$Release:	Dortmund 2001-2007 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Image loading and displaying
**
**      (C) Copyright 2001-2007  Steffen A. Mork
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

static b3TxPool texture_pool;

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/
  
class b3ImageRow : public b3Row
{
public:
	b3ImageRow(const b3_res xSize) : b3Row(0,xSize)
	{
	}

	void b3SetRow(b3Tx *tx, const b3_coord y)
	{
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
	char        *BLZ3_TEXTURES    = getenv("BLZ3_TEXTURES");
	char        *BLZ3_PICTURES    = getenv("BLZ3_PICTURES");
	char        *HOME             = getenv("HOME");
	b3Path       textures;
	b3Path       pictures;
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
			texture_pool.b3AddPath(BLZ3_TEXTURES);
			texture_pool.b3AddPath(pictures);
			texture_pool.b3AddPath(BLZ3_PICTURES);
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
		b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001-2007\n");
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s [-f][-d] {Image files}\n",argv[0]);
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"  -d        debug level output\n");
		b3PrintF(B3LOG_NORMAL,"  -f        verbose level output\n");
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n", __DATE__, __TIME__);
		b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
	}
	return EXIT_SUCCESS;
}
