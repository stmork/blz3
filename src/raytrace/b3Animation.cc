/*
**
**      $Filename:      b3Animation.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing Animation
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
**      Revision 1.2  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3InitAnimation::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering animation elements...\n");
	b3Item::b3Register(&b3AnimElement::b3Init,&b3AnimElement::b3Init,ANIM_MOVE );
	b3Item::b3Register(&b3AnimElement::b3Init,&b3AnimElement::b3Init,ANIM_ROTATE );
	b3Item::b3Register(&b3AnimElement::b3Init,&b3AnimElement::b3Init,ANIM_SCALE );
}

b3AnimElement::b3AnimElement(b3_u32 class_type) : b3Item(sizeof (b3AnimElement),class_type)
{
}

b3AnimElement::b3AnimElement(b3_u32 *src) : b3Item(src)
{
	b3InitVector(&center);
	b3InitMatrix(&actual);
	b3InitMatrix(&neutralInverse);
	ratio      = b3InitFloat();
	empty      = b3InitInt();
	start      = b3InitFloat();
	end        = b3InitFloat();
	flags      = b3InitInt();
	trackIndex = b3InitInt();
	curveUse   = b3InitInt();

	// Init nurbs
	b3InitNurbs(&param,null,knots);
	for (b3_index i = 0;i < B3_MAX_KNOTS;i++)
	{
		knots[i] = b3InitFloat();
	}

	b3InitString(name,  B3_ANIMSTRINGLEN);
	b3InitString(object,B3_BOXSTRINGLEN);
}

void b3AnimElement::b3Write()
{
	b3StoreVector(&center);
	b3StoreMatrix(&actual);
	b3StoreMatrix(&neutralInverse);
	b3StoreFloat(ratio);
	b3StoreInt  (empty);
	b3StoreFloat(start);
	b3StoreFloat(end);
	b3StoreInt(flags);
	b3StoreInt(trackIndex);
	b3StoreInt(curveUse);

	// Init nurbs
	b3StoreNurbs(&param);
	for (b3_index i = 0;i < B3_MAX_KNOTS;i++)
	{
		b3StoreFloat(knots[i]);
	}

	b3StoreString(name,  B3_ANIMSTRINGLEN);
	b3StoreString(object,B3_BOXSTRINGLEN);
}
