/*
**
**      $Filename:      b3Condition.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing stencil mapping
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

static b3_u32 logic_ops[] =
{
	MODE_OR, MODE_NOT, MODE_AND, MODE_NAND
};

void b3InitCondition::b3Init()
{
	b3_u32 mode;

	b3PrintF(B3LOG_DEBUG,"Registering stencil classes...\n");
	for (int i = 0;i < (sizeof(logic_ops) / sizeof(b3_u32));i++)
	{
		mode = logic_ops[i];

		b3Item::b3Register(&b3CondRectangle::b3Init,   &b3CondRectangle::b3Init,  mode | COND_RECTANGLE);
		b3Item::b3Register(&b3CondCircle::b3Init,      &b3CondCircle::b3Init,     mode | COND_CIRCLE);
		b3Item::b3Register(&b3CondSegment::b3Init,     &b3CondSegment::b3Init,    mode | COND_SEGMENT);
		b3Item::b3Register(&b3CondPara::b3Init,        &b3CondPara::b3Init,       mode | COND_TRIANGLE);
		b3Item::b3Register(&b3CondPara::b3Init,        &b3CondPara::b3Init,       mode | COND_PARALLELOGRAM);
		b3Item::b3Register(&b3CondTexture::b3Init,     &b3CondTexture::b3Init,    mode | COND_TEXTURE);
		b3Item::b3Register(&b3CondWrapTexture::b3Init, &b3CondWrapTexture::b3Init,mode | COND_TEXTUREWRAP);
		b3Item::b3Register(&b3CondEllipse::b3Init,     &b3CondEllipse::b3Init,    mode | COND_ELLIPSE);
	}
}


b3Condition::b3Condition(b3_size class_size,b3_u32 class_type) : b3Item(class_size, class_type)
{
}

b3Condition::b3Condition(b3_u32 class_type) : b3Item(sizeof(b3Condition), class_type)
{
}

b3Condition::b3Condition(b3_u32 *src) : b3Item(src)
{
}


b3CondRectangle::b3CondRectangle(b3_u32 class_type) : b3Condition(sizeof(b3CondRectangle), class_type)
{
}

b3CondRectangle::b3CondRectangle(b3_u32 *src) : b3Condition(src)
{
	xStart = b3InitFloat();
	yStart = b3InitFloat();
	xEnd   = b3InitFloat();
	yEnd   = b3InitFloat();
	Flags  = b3InitInt();
}


b3CondCircle::b3CondCircle(b3_u32 class_type) : b3Condition(sizeof(b3CondCircle), class_type)
{
}

b3CondCircle::b3CondCircle(b3_u32 *src) : b3Condition(src)
{
	xCenter = b3InitFloat();
	yCenter = b3InitFloat();
	radius  = b3InitFloat();
}


b3CondSegment::b3CondSegment(b3_u32 class_type) : b3Condition(sizeof(b3CondSegment), class_type)
{
}

b3CondSegment::b3CondSegment(b3_u32 *src) : b3Condition(src)
{
	xCenter    = b3InitFloat();
	yCenter    = b3InitFloat();
	radStart   = b3InitFloat();
	radEnd     = b3InitFloat();
	angleStart = b3InitFloat();
	angleEnd   = b3InitFloat();
}


b3CondPara::b3CondPara(b3_u32 class_type) : b3Condition(sizeof(b3CondPara), class_type)
{
}

b3CondPara::b3CondPara(b3_u32 *src) : b3Condition(src)
{
	xPos  = b3InitFloat();
	yPos  = b3InitFloat();
	xDir1 = b3InitFloat();
	yDir1 = b3InitFloat();
	xDir2 = b3InitFloat();
	yDir2 = b3InitFloat();
	denom = xDir1 * yDir2 - yDir1 * xDir2;
}


b3CondTexture::b3CondTexture(b3_u32 class_type) : b3Condition(sizeof(b3CondTexture), class_type)
{
}

b3CondTexture::b3CondTexture(b3_u32 *src) : b3Condition(src)
{
	Texture = (b3Tx *)b3InitNull();
	Flags   = b3InitInt();
	xStart  = b3InitFloat();
	yStart  = b3InitFloat();
	xScale  = b3InitFloat();
	yScale  = b3InitFloat();
	xTimes  = b3InitInt();
	yTimes  = b3InitInt();
	b3InitString(Name,B3_TEXSTRINGLEN);
}


b3CondWrapTexture::b3CondWrapTexture(b3_u32 class_type) : b3Condition(sizeof(b3CondWrapTexture), class_type)
{
}

b3CondWrapTexture::b3CondWrapTexture(b3_u32 *src) : b3Condition(src)
{
	Texture = (b3Tx *)b3InitNull();
	Flags   = b3InitInt();
	xStart  = b3InitFloat();
	yStart  = b3InitFloat();
	xEnd    = b3InitFloat();
	yEnd    = b3InitFloat();
	b3InitString(Name,B3_TEXSTRINGLEN);
}


b3CondEllipse::b3CondEllipse(b3_u32 class_type) : b3Condition(sizeof(b3CondEllipse), class_type)
{
}

b3CondEllipse::b3CondEllipse(b3_u32 *src) : b3Condition(src)
{
	xCenter    = b3InitFloat();
	yCenter    = b3InitFloat();
	xRadius    = b3InitFloat();
	yRadius    = b3InitFloat();
	radStart   = b3InitFloat();
	radEnd     = b3InitFloat();
	angleStart = b3InitFloat();
	angleEnd   = b3InitFloat();
}
