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
  
#include "blz3/system/b3Display.h" 
#include "blz3/base/b3FileList.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

int main(int argc,char *argv[])
{
	b3FileList   list;
	b3FileEntry *entry;
	b3Tx        *tx;
	int          i;

	if (argc > 1)
	{
		for (i = 1;i < argc;i++)
		{
			switch(b3Dir::b3Exists(argv[i]))
			{
			case B3_TYPE_DIR:
				list.b3CreateList(argv[i]);
				for (entry = list.b3First();entry != null;entry = entry->Succ)
				{
					texture_pool.b3LoadTexture(entry->b3Name());
					b3PrintF(B3LOG_NORMAL,".");
				}
				break;

			case B3_TYPE_FILE:
				texture_pool.b3LoadTexture(argv[i]);
				b3PrintF(B3LOG_NORMAL,".");
				break;

			default:
				break;
			}
		}
		b3PrintF(B3LOG_NORMAL,"\n");

		B3_FOR_BASE(texture_pool.b3GetTxHead(),tx)
		{
			display(tx);
		}
	}
	return 0;
}
