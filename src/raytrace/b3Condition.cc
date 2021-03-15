/*
**
**	$Filename:	b3Condition.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing stencil mapping
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

const b3_u32 b3Condition::m_LogicOps[] =
{
	MODE_OR, MODE_NOT, MODE_AND, MODE_NAND
};

void b3Condition::b3Register()
{
	b3_u32  mode;
	b3_size i;

	b3PrintF(B3LOG_DEBUG, "Registering stencil classes...\n");
	for (i = 0; i < (sizeof(m_LogicOps) / sizeof(b3_u32)); i++)
	{
		mode = m_LogicOps[i];

		b3Item::b3Register(&b3CondRectangle::b3StaticInit,   &b3CondRectangle::b3StaticInit,  mode | COND_RECTANGLE);
		b3Item::b3Register(&b3CondCircle::b3StaticInit,      &b3CondCircle::b3StaticInit,     mode | COND_CIRCLE);
		b3Item::b3Register(&b3CondSegment::b3StaticInit,     &b3CondSegment::b3StaticInit,    mode | COND_SEGMENT);
		b3Item::b3Register(&b3CondTria::b3StaticInit,        &b3CondTria::b3StaticInit,       mode | COND_TRIANGLE);
		b3Item::b3Register(&b3CondPara::b3StaticInit,        &b3CondPara::b3StaticInit,       mode | COND_PARALLELOGRAM);
		b3Item::b3Register(&b3CondTexture::b3StaticInit,     &b3CondTexture::b3StaticInit,    mode | COND_TEXTURE);
		b3Item::b3Register(&b3CondWrapTexture::b3StaticInit, &b3CondWrapTexture::b3StaticInit, mode | COND_TEXTUREWRAP);
		b3Item::b3Register(&b3CondEllipse::b3StaticInit,     &b3CondEllipse::b3StaticInit,    mode | COND_ELLIPSE);
	}
}


b3Condition::b3Condition(b3_size class_size, b3_u32 class_type) : b3Item(class_size, class_type)
{
}

b3Condition::b3Condition(b3_u32 class_type) : b3Item(sizeof(b3Condition), class_type)
{
}

b3Condition::b3Condition(b3_u32 * src) : b3Item(src)
{
}

b3_bool b3Condition::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	return true;
}

b3_bool b3Condition::b3Conditionate(
	b3_bool input,
	b3_bool operation)
{
	switch (b3GetClassType() & MODE_MASK)
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

void b3Condition::b3CheckInnerBound(
	b3_stencil_limit * limit,
	b3_stencil_limit * object)
{
	if (object->x2 < object->x1)
	{
		B3_PSWAP((b3_u32 *)&object->x1, (b3_u32 *)&object->x2);
	}
	if (object->y2 < object->y1)
	{
		B3_PSWAP((b3_u32 *)&object->y1, (b3_u32 *)&object->y2);
	}

	if (object->x1 > limit->x1)
	{
		limit->x1 = object->x1;
	}
	if (object->y1 > limit->y1)
	{
		limit->y1 = object->y1;
	}
	if (object->x2 < limit->x2)
	{
		limit->x2 = object->x2;
	}
	if (object->y2 < limit->y2)
	{
		limit->y2 = object->y2;
	}
}

void b3Condition::b3CheckOuterBound(
	b3_stencil_limit * limit,
	b3_stencil_limit * object)
{
	if (object->x2 < object->x1)
	{
		B3_PSWAP((b3_u32 *)&object->x1, (b3_u32 *)&object->x2);
	}
	if (object->y2 < object->y1)
	{
		B3_PSWAP((b3_u32 *)&object->y1, (b3_u32 *)&object->y2);
	}

	if (object->x1 < limit->x1)
	{
		limit->x1 = object->x1;
	}
	if (object->y1 < limit->y1)
	{
		limit->y1 = object->y1;
	}
	if (object->x2 > limit->x2)
	{
		limit->x2 = object->x2;
	}
	if (object->y2 > limit->y2)
	{
		limit->y2 = object->y2;
	}
}

/*************************************************************************
**                                                                      **
**                        Rectangular stencil                           **
**                                                                      **
*************************************************************************/

b3CondRectangle::b3CondRectangle(b3_u32 class_type) : b3Condition(sizeof(b3CondRectangle), class_type)
{
	m_xStart = 0;
	m_yStart = 0;
	m_xEnd   = 1;
	m_yEnd   = 1;
	m_Flags  = RCF_ACTIVE;
}

b3CondRectangle::b3CondRectangle(b3_u32 * src) : b3Condition(src)
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

void b3CondRectangle::b3Write()
{
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xEnd);
	b3StoreFloat(m_yEnd);
	b3StoreInt(m_Flags);
}

void b3CondRectangle::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound;

	switch (ClassType & MODE_MASK)
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
		b3CheckOuterBound(Limit, &Bound);
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
		b3CheckInnerBound(Limit, &Bound);
		break;
	}
}

b3_bool b3CondRectangle::b3CheckStencil(b3_polar * polar) const
{
	if ((m_Flags & RCF_ACTIVE) == 0)
	{
		return true;
	}

	if ((m_yStart <= polar->m_Polar.y) && (polar->m_Polar.y <= m_yEnd))
	{
		if (m_xStart <= m_xEnd)
		{
			return ((m_xStart <= polar->m_Polar.x) && (polar->m_Polar.x <= m_xEnd));
		}
		else
		{
			return (polar->m_Polar.x <= m_xEnd) || (polar->m_Polar.x >= m_xStart);
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

b3CondCircle::b3CondCircle(b3_u32 * src) :
	b3Condition(src)
{
	m_xCenter = b3InitFloat();
	m_yCenter = b3InitFloat();
	m_Radius  = b3InitFloat();
}

void b3CondCircle::b3Write()
{
	b3StoreFloat(m_xCenter);
	b3StoreFloat(m_yCenter);
	b3StoreFloat(m_Radius);
}

void b3CondCircle::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound;

	switch (ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xCenter - m_Radius;
		Bound.y1 = m_yCenter - m_Radius;
		Bound.x2 = m_xCenter + m_Radius;
		Bound.y2 = m_yCenter + m_Radius;
		b3CheckOuterBound(Limit, &Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xCenter - m_Radius;
		Bound.y1 = m_yCenter - m_Radius;
		Bound.x2 = m_xCenter + m_Radius;
		Bound.y2 = m_yCenter + m_Radius;
		b3CheckInnerBound(Limit, &Bound);
		break;
	}
}

b3_bool b3CondCircle::b3CheckStencil(b3_polar * polar) const
{
	b3_f64 x, y;

	x = polar->m_Polar.x - m_xCenter;
	y = polar->m_Polar.y - m_yCenter;
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

b3CondSegment::b3CondSegment(b3_u32 * src) : b3Condition(src)
{
	m_xCenter    = b3InitFloat();
	m_yCenter    = b3InitFloat();
	m_RadStart   = b3InitFloat();
	m_RadEnd     = b3InitFloat();
	m_AngleStart = b3InitFloat();
	m_AngleEnd   = b3InitFloat();
}

void b3CondSegment::b3Write()
{
	b3StoreFloat(m_xCenter);
	b3StoreFloat(m_yCenter);
	b3StoreFloat(m_RadStart);
	b3StoreFloat(m_RadEnd);
	b3StoreFloat(m_AngleStart);
	b3StoreFloat(m_AngleEnd);
}

void b3CondSegment::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound;

	switch (ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xCenter - m_RadEnd;
		Bound.y1 = m_yCenter - m_RadEnd;
		Bound.x2 = m_xCenter + m_RadEnd;
		Bound.y2 = m_yCenter + m_RadEnd;
		b3CheckOuterBound(Limit, &Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xCenter - m_RadEnd;
		Bound.y1 = m_yCenter - m_RadEnd;
		Bound.x2 = m_xCenter + m_RadEnd;
		Bound.y2 = m_yCenter + m_RadEnd;
		b3CheckInnerBound(Limit, &Bound);
		break;
	}
}

b3_bool b3CondSegment::b3CheckStencil(b3_polar * polar) const
{
	b3_f64 x, y, angle, Rad;

	x = polar->m_Polar.x - m_xCenter;
	y = polar->m_Polar.y - m_yCenter;
	Rad = sqrt(x * x + y * y);
	if ((Rad < m_RadStart) || (Rad > m_RadEnd))
	{
		return false;
	}

	angle = atan(y / x) * 180.0 / M_PI;
	if (x     < 0)
	{
		angle += 180;
	}
	if (angle < 0)
	{
		angle += 360;
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

b3Cond2::b3Cond2(b3_size class_size, b3_u32 class_type) : b3Condition(class_size, class_type)
{
}

b3Cond2::b3Cond2(b3_u32 class_type) : b3Condition(class_type)
{
}

b3Cond2::b3Cond2(b3_u32 * src) : b3Condition(src)
{
	m_xPos  = b3InitFloat();
	m_yPos  = b3InitFloat();
	m_xDir1 = b3InitFloat();
	m_yDir1 = b3InitFloat();
	m_xDir2 = b3InitFloat();
	m_yDir2 = b3InitFloat();
}

void b3Cond2::b3Write()
{
	b3StoreFloat(m_xPos);
	b3StoreFloat(m_yPos);
	b3StoreFloat(m_xDir1);
	b3StoreFloat(m_yDir1);
	b3StoreFloat(m_xDir2);
	b3StoreFloat(m_yDir2);
	b3StoreFloat(m_Denom);
}

b3_bool b3Cond2::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	m_Denom = m_xDir1 * m_yDir2 - m_yDir1 * m_xDir2;
	return true;
}

void b3Cond2::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound, Aux;

	switch (ClassType & MODE_MASK)
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
		b3CheckOuterBound(&Bound, &Aux);
		b3CheckOuterBound(Limit, &Bound);
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
		b3CheckOuterBound(&Bound, &Aux);
		b3CheckInnerBound(Limit, &Bound);
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

b3CondPara::b3CondPara(b3_u32 * src) : b3Cond2(src)
{
}

b3_bool b3CondPara::b3CheckStencil(b3_polar * polar) const
{
	b3_f64 Dx, Dy, a, b;

	Dx = polar->m_Polar.x - m_xPos;
	Dy = polar->m_Polar.y - m_yPos;

	a = (Dx * m_yDir2 - Dy * m_xDir2) / m_Denom;
	if ((a < 0) || (a > 1))
	{
		return (false);
	}

	b = (m_xDir1 * Dy - m_yDir1 * Dx) / m_Denom;
	if ((b < 0) || (b > 1))
	{
		return (false);
	}

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

b3CondTria::b3CondTria(b3_u32 * src) : b3Cond2(src)
{
}

b3_bool b3CondTria::b3CheckStencil(b3_polar * polar) const
{
	b3_f64 Dx, Dy, a, b;

	Dx = polar->m_Polar.x - m_xPos;
	Dy = polar->m_Polar.y - m_yPos;

	a = (Dx * m_yDir2 - Dy * m_xDir2) / m_Denom;
	if (a < 0)
	{
		return false;
	}

	b = (m_xDir1 * Dy - m_yDir1 * Dx) / m_Denom;
	if ((b < 0) || ((a + b) > 1))
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

b3CondTexture::b3CondTexture(b3_u32 * src) : b3Condition(src)
{
	m_Texture = (b3Tx *)b3InitNull();
	m_Flags   = b3InitInt();
	m_xStart  = b3InitFloat();
	m_yStart  = b3InitFloat();
	m_xScale  = b3InitFloat();
	m_yScale  = b3InitFloat();
	m_xTimes  = b3InitInt();
	m_yTimes  = b3InitInt();
	b3InitString(m_Name, B3_TEXSTRINGLEN);
}

void b3CondTexture::b3Write()
{
	b3StoreNull();
	b3StoreInt(m_Flags);
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xScale);
	b3StoreFloat(m_yScale);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	b3StoreString(m_Name, B3_TEXSTRINGLEN);
}

b3_bool b3CondTexture::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	return b3Scene::b3CheckTexture(&m_Texture, m_Name);
}

void b3CondTexture::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound;

	switch (ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1  =
			Bound.x2  = m_xStart;
		Bound.y1  =
			Bound.y2  = m_yStart;
		Bound.x2 += (m_xScale * m_xTimes);
		Bound.y2 += (m_yScale * m_yTimes);
		b3CheckOuterBound(Limit, &Bound);
		break;

	case MODE_AND:
		Bound.x1  = Bound.x2 = m_xStart;
		Bound.y1  = Bound.y2 = m_yStart;
		Bound.x2 += (m_xScale * m_xTimes);
		Bound.y2 += (m_yScale * m_yTimes);
		b3CheckInnerBound(Limit, &Bound);
		break;
	}
}

b3_bool b3CondTexture::b3CheckStencil(b3_polar * polar) const
{
	b3_f64   fx, fy;
	b3_coord x, y;

	fx = (polar->m_Polar.x - m_xStart) / m_xScale;
	if ((fx < 0) || (fx >= m_xTimes))
	{
		return false;
	}

	fy = (polar->m_Polar.y - m_yStart) / m_yScale;
	if ((fy < 0) || (fy >= m_yTimes))
	{
		return false;
	}

	x = (b3_coord)((fx - (b3_coord)fx) * m_Texture->xSize);
	y = (b3_coord)((fy - (b3_coord)fy) * m_Texture->ySize);

	return m_Texture->b3IsBackground(x, y);
}

/*************************************************************************
**                                                                      **
**                        Wrap around stencil mapping                   **
**                                                                      **
*************************************************************************/

b3CondWrapTexture::b3CondWrapTexture(b3_u32 class_type) : b3Condition(sizeof(b3CondWrapTexture), class_type)
{
}

b3CondWrapTexture::b3CondWrapTexture(b3_u32 * src) : b3Condition(src)
{
	m_Texture = (b3Tx *)b3InitNull();
	m_Flags   = b3InitInt();
	m_xStart  = b3InitFloat();
	m_yStart  = b3InitFloat();
	m_xEnd    = b3InitFloat();
	m_yEnd    = b3InitFloat();
	b3InitString(m_Name, B3_TEXSTRINGLEN);
}

void b3CondWrapTexture::b3Write()
{
	b3StoreNull();
	b3StoreInt(m_Flags);
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xEnd);
	b3StoreFloat(m_yEnd);
	b3StoreString(m_Name, B3_TEXSTRINGLEN);
}

b3_bool b3CondWrapTexture::b3Prepare(b3_preparation_info * prep_info B3_UNUSED)
{
	return b3Scene::b3CheckTexture(&m_Texture, m_Name);
}

void b3CondWrapTexture::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound;

	switch (ClassType & MODE_MASK)
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
		b3CheckOuterBound(Limit, &Bound);
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
		b3CheckInnerBound(Limit, &Bound);
		break;
	}
}

b3_bool b3CondWrapTexture::b3CheckStencil(b3_polar * polar) const
{
	b3_coord x, y;
	b3_f64   fx, fy, xEnd, xPolar;

	if ((polar->m_Polar.y >= m_yStart) && (polar->m_Polar.y <= m_yEnd))
	{
		xEnd	= m_xEnd;
		xPolar	= polar->m_Polar.x;
		fy = (polar->m_Polar.y - m_yStart) /
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
				if (xPolar < m_xStart)
				{
					xPolar++;
				}
				xEnd++;
				fx = (xPolar - m_xStart) / (xEnd - m_xStart);
				x = (b3_coord)(fx * m_Texture->xSize);
			}
			else
			{
				return (false);
			}
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
	else
	{
		return (false);
	}

	return m_Texture->b3IsBackground(x, y);
}

/*************************************************************************
**                                                                      **
**                        Ellipse stencil                               **
**                                                                      **
*************************************************************************/

b3CondEllipse::b3CondEllipse(b3_u32 class_type) : b3Condition(sizeof(b3CondEllipse), class_type)
{
}

b3CondEllipse::b3CondEllipse(b3_u32 * src) : b3Condition(src)
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

void b3CondEllipse::b3Write()
{
	b3StoreFloat(m_xCenter);
	b3StoreFloat(m_yCenter);
	b3StoreFloat(m_xRadius);
	b3StoreFloat(m_yRadius);
	b3StoreFloat(m_RadStart);
	b3StoreFloat(m_RadEnd);
	b3StoreFloat(m_AngleStart);
	b3StoreFloat(m_AngleEnd);
}

void b3CondEllipse::b3ComputeBound(b3_stencil_limit * Limit) const
{
	b3_stencil_limit Bound;

	switch (ClassType & MODE_MASK)
	{
	case MODE_OR:
		Bound.x1 = m_xCenter - m_xRadius * m_RadEnd;
		Bound.y1 = m_yCenter - m_yRadius * m_RadEnd;
		Bound.x2 = m_xCenter + m_xRadius * m_RadEnd;
		Bound.y2 = m_yCenter + m_yRadius * m_RadEnd;
		b3CheckOuterBound(Limit, &Bound);
		break;

	case MODE_AND:
		Bound.x1 = m_xCenter - m_xRadius * m_RadEnd;
		Bound.y1 = m_yCenter - m_yRadius * m_RadEnd;
		Bound.x2 = m_xCenter + m_xRadius * m_RadEnd;
		Bound.y2 = m_yCenter + m_yRadius * m_RadEnd;
		b3CheckInnerBound(Limit, &Bound);
		break;
	}
}

b3_bool b3CondEllipse::b3CheckStencil(b3_polar * polar) const
{
	b3_f64 x, y, angle, AngleEnd, Rad;

	x = (polar->m_Polar.x - m_xCenter) / m_xRadius;
	y = (polar->m_Polar.y - m_yCenter) / m_yRadius;
	Rad = sqrt(x * x + y * y);
	if ((Rad < m_RadStart) || (Rad > m_RadEnd))
	{
		return false;
	}

	angle = atan(y / x) * 180.0 / M_PI;
	if (x     < 0)
	{
		angle += 180;
	}
	if (angle < 0)
	{
		angle += 360;
	}

	AngleEnd = m_AngleEnd;
	if (m_AngleStart > AngleEnd)
	{
		if ((angle >= m_AngleStart) || (angle <= AngleEnd))
		{
			return (true);
		}
		return (false);
	}
	else
	{
		if ((angle <= m_AngleStart) || (angle >= AngleEnd))
		{
			return (false);
		}
		return (true);
	}
}
