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
**      Revision 1.6  2001/09/22 16:19:51  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.5  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
**      Revision 1.4  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.3  2001/08/05 19:51:56  sm
**      - Now having OpenGL software for Windows NT and created
**        new Lines III.
**
**      Revision 1.2  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
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
		b3Item::b3Register(&b3CondTria::b3Init,        &b3CondTria::b3Init,       mode | COND_TRIANGLE);
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

void b3Condition::b3ComputeBound(b3CondLimit *Limit)
{
}

b3_bool b3Condition::b3CheckStencil(b3_polar *polar)
{
	return true;
}

void b3Condition::b3CheckInnerBound(
	b3CondLimit *limit,
	b3CondLimit *object)
{
	if (object->x2 < object->x1) B3_PSWAP((b3_u32 *)&object->x1,(b3_u32 *)&object->x2);
	if (object->y2 < object->y1) B3_PSWAP((b3_u32 *)&object->y1,(b3_u32 *)&object->y2);

	if (object->x1 > limit->x1) limit->x1 = object->x1;
	if (object->y1 > limit->y1) limit->y1 = object->y1;
	if (object->x2 < limit->x2) limit->x2 = object->x2;
	if (object->y2 < limit->y2) limit->y2 = object->y2;
}

void b3Condition::b3CheckOuterBound(
	b3CondLimit *limit,
	b3CondLimit *object)
{
	if (object->x2 < object->x1) B3_PSWAP((b3_u32 *)&object->x1,(b3_u32 *)&object->x2);
	if (object->y2 < object->y1) B3_PSWAP((b3_u32 *)&object->y1,(b3_u32 *)&object->y2);

	if (object->x1 < limit->x1) limit->x1 = object->x1;
	if (object->y1 < limit->y1) limit->y1 = object->y1;
	if (object->x2 > limit->x2) limit->x2 = object->x2;
	if (object->y2 > limit->y2) limit->y2 = object->y2;
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
	if (B3_PARSE_INDEX_VALID)
	{
		Flags  = b3InitInt();
	}
	else
	{
		Flags  = RCF_ACTIVE;
	}
}

void b3CondRectangle::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		if (Flags & RCF_ACTIVE)
		{
			Bound.y1 = yStart;
			Bound.y2 = yEnd;
			if (xStart < xEnd)
			{
				Bound.x1 = xStart;
				Bound.x2 = xEnd;
			}
			else
			{
				Bound.x1 = -1;
				Bound.x2 =  1;
			}
		}
		else
		{
			Bound.x1 =
			Bound.y1 = -1;
			Bound.x2 =
			Bound.y2 =  1;
		}
		b3CheckOuterBound(Limit,&Bound);
		break;
	case MODE_AND:
		if (Flags & RCF_ACTIVE)
		{
			Bound.y1 = yStart;
			Bound.y2 = yEnd;
			if (xStart < xEnd)
			{
				Bound.x1 = xStart;
				Bound.x2 = xEnd;
			}
			else
			{
				Bound.x1 = -1;
				Bound.x2 =  1;
			}
		}
		else
		{
			Bound.x1 =
			Bound.y1 = -1;
			Bound.x2 =
			Bound.y2 =  1;
		}
		b3CheckInnerBound(Limit,&Bound);
		break;
	}
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

void b3CondCircle::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = xCenter - radius;
		Bound.y1 = yCenter - radius;
		Bound.x2 = xCenter + radius;
		Bound.y2 = yCenter + radius;
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = xCenter - radius;
		Bound.y1 = yCenter - radius;
		Bound.x2 = xCenter + radius;
		Bound.y2 = yCenter + radius;
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
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

void b3CondSegment::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = xCenter - radEnd;
		Bound.y1 = yCenter - radEnd;
		Bound.x2 = xCenter + radEnd;
		Bound.y2 = yCenter + radEnd;
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = xCenter - radEnd;
		Bound.y1 = yCenter - radEnd;
		Bound.x2 = xCenter + radEnd;
		Bound.y2 = yCenter + radEnd;
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}


b3Cond2::b3Cond2(b3_size class_size,b3_u32 class_type) : b3Condition(class_size, class_type)
{
}

b3Cond2::b3Cond2(b3_u32 class_type) : b3Condition(class_type)
{
}

b3Cond2::b3Cond2(b3_u32 *src) : b3Condition(src)
{
	xPos  = b3InitFloat();
	yPos  = b3InitFloat();
	xDir1 = b3InitFloat();
	yDir1 = b3InitFloat();
	xDir2 = b3InitFloat();
	yDir2 = b3InitFloat();
}


b3CondPara::b3CondPara(b3_u32 class_type) : b3Cond2(sizeof(b3CondPara), class_type)
{
}

b3CondPara::b3CondPara(b3_u32 *src) : b3Cond2(src)
{
}

void b3CondPara::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound,Aux;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = xPos + xDir1;
		Bound.y1 = yPos + yDir1;
		Bound.x2 = xPos + xDir2;
		Bound.y2 = yPos + yDir2;
		Aux.x1   = Bound.x1 + xDir2;
		Aux.y1   = Bound.y1 + yDir2;
		Aux.x2   = xPos;
		Aux.y2   = yPos;
		b3CheckOuterBound (&Bound,&Aux);
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = xPos + xDir1;
		Bound.y1 = yPos + yDir1;
		Bound.x2 = xPos + xDir2;
		Bound.y2 = yPos + yDir2;
		Aux.x1   = Bound.x1 + xDir2;
		Aux.y1   = Bound.y1 + yDir2;
		Aux.x2   = xPos;
		Aux.y2   = yPos;
		b3CheckOuterBound (&Bound,&Aux);
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}


b3CondTria::b3CondTria(b3_u32 class_type) : b3Cond2(sizeof(b3CondTria), class_type)
{
}

b3CondTria::b3CondTria(b3_u32 *src) : b3Cond2(src)
{
}

void b3CondTria::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound,Aux;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = xPos + xDir1;
		Bound.y1 = yPos + yDir1;
		Bound.x2 = xPos + xDir2;
		Bound.y2 = yPos + yDir2;
		Aux.x1   = Bound.x1;
		Aux.y1   = Bound.y1;
		Aux.x2   = xPos;
		Aux.y2   = yPos;
		b3CheckOuterBound (&Bound,&Aux);
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = xPos + xDir1;
		Bound.y1 = yPos + yDir1;
		Bound.x2 = xPos + xDir2;
		Bound.y2 = yPos + yDir2;
		Aux.x1   = Bound.x1;
		Aux.y1   = Bound.y1;
		Aux.x2   = xPos;
		Aux.y2   = yPos;
		b3CheckOuterBound (&Bound,&Aux);
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
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

void b3CondTexture::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1  =
		Bound.x2  = xStart;
		Bound.y1  =
		Bound.y2  = yStart;
		Bound.x2 += (xScale * xTimes);
		Bound.y2 += (yScale * yTimes);
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1  = Bound.x2 = xStart;
		Bound.y1  = Bound.y2 = yStart;
		Bound.x2 += (xScale * xTimes);
		Bound.y2 += (yScale * yTimes);
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
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

void b3CondWrapTexture::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.y1 = yStart;
		Bound.y2 = yEnd;
		if (xStart < xEnd)
		{
			Bound.x1 = xStart;
			Bound.x2 = xEnd;
		}
		else
		{
			Bound.x1 = -1;
			Bound.x2 =  1;
		}
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.y1 = yStart;
		Bound.y2 = yEnd;
		if (xStart < xEnd)
		{
			Bound.x1 = xStart;
			Bound.x2 = xEnd;
		}
		else
		{
			Bound.x1 = -1;
			Bound.x2 =  1;
		}
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
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

void b3CondEllipse::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;
	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = xCenter - xRadius * radEnd;
		Bound.y1 = yCenter - yRadius * radEnd;
		Bound.x2 = xCenter + xRadius * radEnd;
		Bound.y2 = yCenter + yRadius * radEnd;
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = xCenter - xRadius * radEnd;
		Bound.y1 = yCenter - yRadius * radEnd;
		Bound.x2 = xCenter + xRadius * radEnd;
		Bound.y2 = yCenter + yRadius * radEnd;
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}
