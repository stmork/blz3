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

class b3ImageRow : public b3Row
{
public:
	b3ImageRow(b3_res xSize) : b3Row(0,xSize)
	{
	}

	void b3SetRow(b3Tx *tx,b3_coord y)
	{
		this->y = y;
		tx->b3GetRow(buffer,y);
	}
};

static void display(b3Tx *tx)
{
	b3Display   *display;
	b3ImageRow  *row;
	b3_coord     y;

	if (tx->b3IsLoaded())
	{
		b3PrintF(B3LOG_NORMAL,"%s: %dx%d\n",
			tx->b3Name(),tx->xSize,tx->ySize);
		display = new b3Display(tx->xSize,tx->ySize,tx->b3Name());
		row     = new b3ImageRow(tx->xSize);
		for (y = 0;y < tx->ySize;y++)
		{
			row->b3SetRow(tx,y);
			display->b3PutRow(row);
		}
		delete row;
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
