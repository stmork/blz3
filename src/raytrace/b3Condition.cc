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
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.12  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**
**	Revision 1.11  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.10  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.9  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.8  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**
**	Revision 1.7  2001/10/05 20:30:45  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**
**	Revision 1.6  2001/09/22 16:19:51  sm
**	- Adding basic shape intersection routines
**
**	Revision 1.5  2001/09/01 15:54:54  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**
**	Revision 1.4  2001/08/07 16:54:26  sm
**	- Checking bounds on condition base for line drawing
**	- Some object reordering
**	- Bug fix for Mandel makefile
**
**	Revision 1.3  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**
**	Revision 1.2  2001/08/02 15:37:17  sm
**	- Now we are able to draw Blizzard Scenes with OpenGL.
**
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
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
	b3_u32  mode;
	b3_size i;

	b3PrintF(B3LOG_DEBUG,"Registering stencil classes...\n");
	for (i = 0;i < (sizeof(logic_ops) / sizeof(b3_u32));i++)
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

b3_bool b3Condition::b3Conditionate(
	b3_bool input,
	b3_bool operation)
{
	switch(b3GetClassType() & MODE_MASK)
	{
	case MODE_OR:
		return input ||  operation;

	case MODE_NOT:
		return input || (!operation);

	case MODE_AND:
		return input &&  operation;

	case MODE_NAND:
		return input && (!operation);
	}
	return input;
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

/*************************************************************************
**                                                                      **
**                        Rectangular stencil                           **
**                                                                      **
*************************************************************************/

b3CondRectangle::b3CondRectangle(b3_u32 class_type) : b3Condition(sizeof(b3CondRectangle), class_type)
{
}

b3CondRectangle::b3CondRectangle(b3_u32 *src) : b3Condition(src)
{
	m_xStart = b3InitFloat();
	m_yStart = b3InitFloat();
	m_xEnd   = b3InitFloat();
	m_yEnd   = b3InitFloat();
	if (B3_PARSE_INDEX_VALID)
	{
		m_Flags  = b3InitInt();
	}
	else
	{
		m_Flags  = RCF_ACTIVE;
	}
}

void b3CondRectangle::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		if (m_Flags & RCF_ACTIVE)
		{
			Bound.y1 = m_yStart;
			Bound.y2 = m_yEnd;
			if (m_xStart < m_xEnd)
			{
				Bound.x1 = m_xStart;
				Bound.x2 = m_xEnd;
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
		if (m_Flags & RCF_ACTIVE)
		{
			Bound.y1 = m_yStart;
			Bound.y2 = m_yEnd;
			if (m_xStart < m_xEnd)
			{
				Bound.x1 = m_xStart;
				Bound.x2 = m_xEnd;
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

b3_bool b3CondRectangle::b3CheckStencil(b3_polar *polar)
{
	if ((m_Flags & RCF_ACTIVE) == 0)
	{
		return true;
	}

	if ((m_yStart <= polar->polar.y) && (polar->polar.y <= m_yEnd))
	{
		if (m_xStart <= m_xEnd)
		{
			return ((m_xStart <= polar->polar.x) && (polar->polar.x <= m_xEnd));
		}
		else
		{
			return (polar->polar.x <= m_xEnd) || (polar->polar.x >= m_xStart);
		}
	}
	return false;
}

/*************************************************************************
**                                                                      **
**                        Circular stencil                              **
**                                                                      **
*************************************************************************/

b3CondCircle::b3CondCircle(b3_u32 class_type) :
	b3Condition(sizeof(b3CondCircle), class_type)
{
}

b3CondCircle::b3CondCircle(b3_u32 *src) :
	b3Condition(src)
{
	m_xCenter = b3InitFloat();
	m_yCenter = b3InitFloat();
	m_Radius  = b3InitFloat();
}

void b3CondCircle::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xCenter - m_Radius;
		Bound.y1 = m_yCenter - m_Radius;
		Bound.x2 = m_xCenter + m_Radius;
		Bound.y2 = m_yCenter + m_Radius;
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xCenter - m_Radius;
		Bound.y1 = m_yCenter - m_Radius;
		Bound.x2 = m_xCenter + m_Radius;
		Bound.y2 = m_yCenter + m_Radius;
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}

b3_bool b3CondCircle::b3CheckStencil(b3_polar *polar)
{
	b3_f64 x,y;

	x = polar->polar.x - m_xCenter;
	y = polar->polar.y - m_yCenter;
	return ((x * x + y * y) < (m_Radius * m_Radius));
}

/*************************************************************************
**                                                                      **
**                        Segment stencil                               **
**                                                                      **
*************************************************************************/

b3CondSegment::b3CondSegment(b3_u32 class_type) : b3Condition(sizeof(b3CondSegment), class_type)
{
}

b3CondSegment::b3CondSegment(b3_u32 *src) : b3Condition(src)
{
	m_xCenter    = b3InitFloat();
	m_yCenter    = b3InitFloat();
	m_RadStart   = b3InitFloat();
	m_RadEnd     = b3InitFloat();
	m_AngleStart = b3InitFloat();
	m_AngleEnd   = b3InitFloat();
}

void b3CondSegment::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xCenter - m_RadEnd;
		Bound.y1 = m_yCenter - m_RadEnd;
		Bound.x2 = m_xCenter + m_RadEnd;
		Bound.y2 = m_yCenter + m_RadEnd;
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xCenter - m_RadEnd;
		Bound.y1 = m_yCenter - m_RadEnd;
		Bound.x2 = m_xCenter + m_RadEnd;
		Bound.y2 = m_yCenter + m_RadEnd;
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}

b3_bool b3CondSegment::b3CheckStencil(b3_polar *polar)
{
	b3_f64 x,y,angle,Rad;

	x = polar->polar.x - m_xCenter;
	y = polar->polar.y - m_yCenter;
	Rad = sqrt(x * x + y * y);
	if ((Rad < m_RadStart)||(Rad > m_RadEnd))
	{
		return false;
	}
 
	if (x==0)
	{
		if (y<0)	angle = 270;
		else		angle =  90;
	}
	else
	{
		angle = atan (y/x) * 180.0 / M_PI;
		if (x     < 0) angle += 180;
		if (angle < 0) angle += 360;
	}

	if (m_AngleStart < m_AngleEnd)
	{
		return ((m_AngleStart <= angle) && (angle <= m_AngleEnd));
	}
	else
	{
		return ((angle >= m_AngleStart) || (angle <= m_AngleEnd));
	}
}


/*************************************************************************
**                                                                      **
**                        Base class for parallelogramme and            **
**                        triangular stencil                            **
**                                                                      **
*************************************************************************/

b3Cond2::b3Cond2(b3_size class_size,b3_u32 class_type) : b3Condition(class_size, class_type)
{
}

b3Cond2::b3Cond2(b3_u32 class_type) : b3Condition(class_type)
{
}

b3Cond2::b3Cond2(b3_u32 *src) : b3Condition(src)
{
	m_xPos  = b3InitFloat();
	m_yPos  = b3InitFloat();
	m_xDir1 = b3InitFloat();
	m_yDir1 = b3InitFloat();
	m_xDir2 = b3InitFloat();
	m_yDir2 = b3InitFloat();
	m_Denom = m_xDir1 * m_yDir2 - m_yDir1 * m_xDir2;
}

void b3Cond2::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound,Aux;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xPos   + m_xDir1;
		Bound.y1 = m_yPos   + m_yDir1;
		Bound.x2 = m_xPos   + m_xDir2;
		Bound.y2 = m_yPos   + m_yDir2;
		Aux.x1   = Bound.x1 + m_xDir2;
		Aux.y1   = Bound.y1 + m_yDir2;
		Aux.x2   = m_xPos;
		Aux.y2   = m_yPos;
		b3CheckOuterBound (&Bound,&Aux);
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xPos   + m_xDir1;
		Bound.y1 = m_yPos   + m_yDir1;
		Bound.x2 = m_xPos   + m_xDir2;
		Bound.y2 = m_yPos   + m_yDir2;
		Aux.x1   = Bound.x1 + m_xDir2;
		Aux.y1   = Bound.y1 + m_yDir2;
		Aux.x2   = m_xPos;
		Aux.y2   = m_yPos;
		b3CheckOuterBound (&Bound,&Aux);
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}

/*************************************************************************
**                                                                      **
**                        Parallelogramme stencil                       **
**                                                                      **
*************************************************************************/

b3CondPara::b3CondPara(b3_u32 class_type) : b3Cond2(sizeof(b3CondPara), class_type)
{
}

b3CondPara::b3CondPara(b3_u32 *src) : b3Cond2(src)
{
}

b3_bool b3CondPara::b3CheckStencil(b3_polar *polar)
{
	b3_f64 Dx,Dy,a,b;

	Dx = polar->polar.x - m_xPos;
	Dy = polar->polar.y - m_yPos;
	if (m_Denom == 0)
	{
		return false;
	}

	a = (Dx * m_yDir2 - Dy * m_xDir2) / m_Denom;
	if ((a < 0) || (a > 1)) return (false);

	b = (m_xDir1 * Dy - m_yDir1 * Dx) / m_Denom;
	if ((b < 0) || (b > 1)) return (false);
	return true;
}

/*************************************************************************
**                                                                      **
**                        Triangular stencil                            **
**                                                                      **
*************************************************************************/

b3CondTria::b3CondTria(b3_u32 class_type) : b3Cond2(sizeof(b3CondTria), class_type)
{
}

b3CondTria::b3CondTria(b3_u32 *src) : b3Cond2(src)
{
}

b3_bool b3CondTria::b3CheckStencil(b3_polar *polar)
{
	b3_f64 Dx,Dy,a,b;

	Dx = polar->polar.x - m_xPos;
	Dy = polar->polar.y - m_yPos;
	if (m_Denom==0)
	{
		return false;
	}

	a = (Dx * m_yDir2 - Dy * m_xDir2) / m_Denom;
	if (a < 0)
	{
		return false;
	}

	b = (m_xDir1 * Dy - m_yDir1 * Dx) / m_Denom;
	if ((b < 0)||((a + b) > 1))
	{
		return false;
	}

	return true;
}

/*************************************************************************
**                                                                      **
**                        Stencil mapping                               **
**                                                                      **
*************************************************************************/

b3CondTexture::b3CondTexture(b3_u32 class_type) : b3Condition(sizeof(b3CondTexture), class_type)
{
}

b3CondTexture::b3CondTexture(b3_u32 *src) : b3Condition(src)
{
	b3InitNull();
	m_Flags   = b3InitInt();
	m_xStart  = b3InitFloat();
	m_yStart  = b3InitFloat();
	m_xScale  = b3InitFloat();
	m_yScale  = b3InitFloat();
	m_xTimes  = b3InitInt();
	m_yTimes  = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
	m_Texture = texture_pool.b3LoadTexture(m_Name);
}

void b3CondTexture::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1  =
		Bound.x2  = m_xStart;
		Bound.y1  =
		Bound.y2  = m_yStart;
		Bound.x2 += (m_xScale * m_xTimes);
		Bound.y2 += (m_yScale * m_yTimes);
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1  = Bound.x2 = m_xStart;
		Bound.y1  = Bound.y2 = m_yStart;
		Bound.x2 += (m_xScale * m_xTimes);
		Bound.y2 += (m_yScale * m_yTimes);
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}

b3_bool b3CondTexture::b3CheckStencil(b3_polar *polar)
{
	b3_f64   fx,fy;
	b3_coord x,y;

	fx = (polar->polar.x - m_xStart) / m_xScale;
	if ((fx < 0) || (fx >= m_xTimes))
	{
		return false;
	}

	fy = (polar->polar.y - m_yStart) / m_yScale;
	if ((fy < 0) || (fy >= m_yTimes))
	{
		return false;
	}

	x = (b3_coord)((fx - (b3_coord)fx) * m_Texture->xSize);
	y = (b3_coord)((fy - (b3_coord)fy) * m_Texture->ySize);

	return m_Texture->b3IsBackground(x,y);
}

/*************************************************************************
**                                                                      **
**                        Wrap around stencil mapping                   **
**                                                                      **
*************************************************************************/

b3CondWrapTexture::b3CondWrapTexture(b3_u32 class_type) : b3Condition(sizeof(b3CondWrapTexture), class_type)
{
}

b3CondWrapTexture::b3CondWrapTexture(b3_u32 *src) : b3Condition(src)
{
	b3InitNull();
	m_Flags   = b3InitInt();
	m_xStart  = b3InitFloat();
	m_yStart  = b3InitFloat();
	m_xEnd    = b3InitFloat();
	m_yEnd    = b3InitFloat();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
	m_Texture = texture_pool.b3LoadTexture(m_Name);
}

void b3CondWrapTexture::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.y1 = m_yStart;
		Bound.y2 = m_yEnd;
		if (m_xStart < m_xEnd)
		{
			Bound.x1 = m_xStart;
			Bound.x2 = m_xEnd;
		}
		else
		{
			Bound.x1 = -1;
			Bound.x2 =  1;
		}
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.y1 = m_yStart;
		Bound.y2 = m_yEnd;
		if (m_xStart < m_xEnd)
		{
			Bound.x1 = m_xStart;
			Bound.x2 = m_xEnd;
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

b3_bool b3CondWrapTexture::b3CheckStencil(b3_polar *polar)
{
	b3_coord x,y;
	b3_f64   fx,fy,xEnd,xPolar;

	if ((polar->polar.y >= m_yStart) && (polar->polar.y <= m_yEnd))
	{
		xEnd	= m_xEnd;
		xPolar	= polar->polar.x;
		fy = (polar->polar.y - m_yStart) /
			(m_yEnd - m_yStart);
		if ((fy < 0) || (fy > 1))
		{
			return false;
		}
		y = (b3_coord)(fy * m_Texture->ySize);
		if (m_xStart > xEnd)
		{
			if ((xPolar >= m_xStart) || (xPolar <= xEnd))
			{
				if (xPolar < 0)
				{
					return false;
				}
				if (xPolar < m_xStart) xPolar++;
				xEnd++;
				fx = (xPolar - m_xStart) / (xEnd - m_xStart);
				x = (b3_coord)(fx * m_Texture->xSize);
			}
			else return (false);
		}
		else
		{
			if ((xPolar <= m_xStart) || (xPolar >= xEnd))
			{
				return false;
			}
			fx = (xPolar - m_xStart) / (xEnd  - m_xStart);
			x = (long)(fx * m_Texture->xSize);
		}
	}
	else return (false);

	return m_Texture->b3IsBackground(x,y);
}

/*************************************************************************
**                                                                      **
**                        Ellipse stencil                               **
**                                                                      **
*************************************************************************/

b3CondEllipse::b3CondEllipse(b3_u32 class_type) : b3Condition(sizeof(b3CondEllipse), class_type)
{
}

b3CondEllipse::b3CondEllipse(b3_u32 *src) : b3Condition(src)
{
	m_xCenter    = b3InitFloat();
	m_yCenter    = b3InitFloat();
	m_xRadius    = b3InitFloat();
	m_yRadius    = b3InitFloat();
	m_RadStart   = b3InitFloat();
	m_RadEnd     = b3InitFloat();
	m_AngleStart = b3InitFloat();
	m_AngleEnd   = b3InitFloat();
}

void b3CondEllipse::b3ComputeBound(b3CondLimit *Limit)
{
	b3CondLimit Bound;

	switch(ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xCenter - m_xRadius * m_RadEnd;
		Bound.y1 = m_yCenter - m_yRadius * m_RadEnd;
		Bound.x2 = m_xCenter + m_xRadius * m_RadEnd;
		Bound.y2 = m_yCenter + m_yRadius * m_RadEnd;
		b3CheckOuterBound (Limit,&Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xCenter - m_xRadius * m_RadEnd;
		Bound.y1 = m_yCenter - m_yRadius * m_RadEnd;
		Bound.x2 = m_xCenter + m_xRadius * m_RadEnd;
		Bound.y2 = m_yCenter + m_yRadius * m_RadEnd;
		b3CheckInnerBound (Limit,&Bound);
		break;
	}
}

b3_bool b3CondEllipse::b3CheckStencil(b3_polar *polar)
{
	b3_f64 x,y,angle,AngleEnd,Rad;

	x = (polar->polar.x - m_xCenter) / m_xRadius;
	y = (polar->polar.y - m_yCenter) / m_yRadius;
	Rad = sqrt(x * x + y * y);
	if ((Rad < m_RadStart) || (Rad > m_RadEnd))
	{
		return false;
	}
 
	if (x==0)
	{
		if (y<0) angle = 270;
		else     angle =  90;
	}
	else
	{
		angle = atan (y/x) * 180.0 / M_PI;
		if (x     < 0) angle += 180;
		if (angle < 0) angle += 360;
	}
	AngleEnd = m_AngleEnd;
	if (m_AngleStart > AngleEnd)
	{
		if ((angle >= m_AngleStart)||(angle <= AngleEnd))
			return (true);
		return (false);
	}
	else
	{
		if ((angle <= m_AngleStart)||(angle >= AngleEnd))
			return (false);
		return (true);
	}
}
