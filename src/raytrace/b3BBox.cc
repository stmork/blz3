/*
**
**      $Filename:      b3BBox.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing bounding boxes
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

#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.1  2001/07/01 12:24:59  sm
**      Initial revision
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3InitBBox::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering bounding boxes...\n");
	b3Item::b3Register(&b3BBox::b3Init,&b3BBox::b3Init,CLASS_BBOX,true);
}
                                                                                                                     
b3BBox::b3BBox(b3_u32 class_type) : b3Item(sizeof(b3BBox),class_type)
{
}

b3BBox::b3BBox(b3_u32 *src) : b3Item(src)
{
	Type = b3InitInt();
	b3InitVector(&Base);
	b3InitVector(&Size);
	b3InitNOP();
	b3InitMatrix(&Matrix);
	b3InitString(BoxName,B3_BOXSTRINGLEN);
	b3InitString(BoxURL, B3_BOXSTRINGLEN);
}

void b3BBox::b3Dump(b3_count level)
{
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL,"Object %s\n",BoxName);
}
