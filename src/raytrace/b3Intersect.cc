/*
**
**	$Filename:	b3Intersect.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Intersection routines
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
#include "blz3/base/b3Cubic.h"

/*************************************************************************
**                                                                      **
**                        Normal shape intersections                    **
**                                                                      **
*************************************************************************/

b3_f64 b3Area::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_f64    Denom, aValue, bValue, lValue, result = -1;
	b3_vector Dir, Product;

	Denom = -1.0 / (
			m_Normal.x * ray->dir.x +
			m_Normal.y * ray->dir.y +
			m_Normal.z * ray->dir.z);
	if (((lValue = (
						m_Normal.x * (Dir.x = ray->pos.x - m_Base.x) +
						m_Normal.y * (Dir.y = ray->pos.y - m_Base.y) +
						m_Normal.z * (Dir.z = ray->pos.z - m_Base.z))
					* Denom) >= b3Scene::epsilon) && (lValue < ray->Q))
	{
		aValue = (
				m_Dir2.x * (Product.x = Dir.y * ray->dir.z - Dir.z * ray->dir.y) +
				m_Dir2.y * (Product.y = Dir.z * ray->dir.x - Dir.x * ray->dir.z) +
				m_Dir2.z * (Product.z = Dir.x * ray->dir.y - Dir.y * ray->dir.x))
			* Denom;
		if (fabs(aValue) <= 1)
		{
			bValue = -b3Vector::b3SMul(&m_Dir1, &Product) * Denom;
			if (fabs(bValue) <= 1)
			{
				polar->m_Polar.x = polar->m_ObjectPolar.x = aValue;
				polar->m_Polar.y = polar->m_ObjectPolar.y = bValue;
				polar->m_Polar.z = polar->m_ObjectPolar.z = 0;

				if (b3CheckStencil(polar))
				{
					result = lValue;
				}
			}
		}
	}
	return result;
}

b3_f64 b3Disk::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_f64     Denom, aValue, bValue, lValue;
	b3_vector  Dir, Product;

	Denom = -1.0 / (
			m_Normal.x * ray->dir.x +
			m_Normal.y * ray->dir.y +
			m_Normal.z * ray->dir.z);
	if ((lValue = (
					m_Normal.x * (Dir.x = ray->pos.x - m_Base.x) +
					m_Normal.y * (Dir.y = ray->pos.y - m_Base.y) +
					m_Normal.z * (Dir.z = ray->pos.z - m_Base.z))
				* Denom) < b3Scene::epsilon)
	{
		return -1;
	}
	if (lValue > ray->Q)
	{
		return -1;
	}

	// check x axis
	aValue = (
			m_Dir2.x * (Product.x = Dir.y * ray->dir.z - Dir.z * ray->dir.y) +
			m_Dir2.y * (Product.y = Dir.z * ray->dir.x - Dir.x * ray->dir.z) +
			m_Dir2.z * (Product.z = Dir.x * ray->dir.y - Dir.y * ray->dir.x))
		* Denom;
	if (fabs(aValue) > 1)
	{
		return -1;
	}

	// check y axis
	bValue = -b3Vector::b3SMul(&m_Dir1, &Product) * Denom;
	if (fabs(bValue) > 1)
	{
		return -1;
	}

	// check radius
	Denom = aValue * aValue + bValue * bValue;
	if (Denom > 1)
	{
		return -1;
	}

	// compute surface coordinates
	polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(aValue, bValue);
	polar->m_Polar.y	= sqrt(Denom);
	polar->m_Polar.z	= 0;
	polar->m_ObjectPolar.x = aValue;
	polar->m_ObjectPolar.y = bValue;
	polar->m_ObjectPolar.z = 0;

	return b3CheckStencil(polar) ? lValue : -1;
}

b3_f64 b3Sphere::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_f64    l1, Discriminant, p, l2;
	b3_f64    xDiff, yDiff, zDiff;
	b3_vector n, pole, aux;

	p = ray->dir.x * (xDiff = ray->pos.x - m_Base.x) +
		ray->dir.y * (yDiff = ray->pos.y - m_Base.y) +
		ray->dir.z * (zDiff = ray->pos.z - m_Base.z);
	if ((Discriminant = p * p + m_QuadRadius -
				xDiff * xDiff -
				yDiff * yDiff -
				zDiff * zDiff) < 0)
	{
		return -1;
	}

	Discriminant = sqrt(Discriminant);

	// l1 <= l2, always!
	l1 = -p - Discriminant;
	l2 = -p + Discriminant;

	// check against limit
	if (l1  < b3Scene::epsilon)
	{
		l1 = -2;
	}
	if (l2  < b3Scene::epsilon)
	{
		l2 = -1;
	}
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	// now compute "north pole" - radius is point on equator
	if ((m_Dir.x == 0) && (m_Dir.y == 0))
	{
		// special case: point on equator is north pole
		aux.x   = m_Dir.z;
		aux.y   = 0;
		aux.z   = 0;
		pole.x  = 0;
		pole.y  = m_Dir.z;
		pole.z  = 0;
	}
	else
	{
		aux.x  = -m_Dir.y;
		aux.y  =  m_Dir.x;
		aux.z  =  0;
		b3Vector::b3CrossProduct(&m_Dir, &aux, &pole);
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		b3Vector::b3LinearCombine(ray, l1, &m_Base, &n);

		p = acos(-b3Vector::b3AngleOfVectors(&n, &pole));
		polar->m_Polar.x = b3Math::b3Acos(b3Vector::b3AngleOfVectors(&m_Dir, &n) /
				sin(p)) * 0.5 / M_PI;
		polar->m_Polar.y = p * 2.0 / M_PI - 1.0;
		polar->m_Polar.z = 0;
		if (b3Vector::b3AngleOfVectors(&aux, &n) < 0)
		{
			polar->m_Polar.x = 1.0 - polar->m_Polar.x;
		}

		if (b3CheckStencil(polar))
		{
			p = 1.0 / sqrt(m_QuadRadius);
			polar->m_ObjectPolar.x = n.x * p;
			polar->m_ObjectPolar.y = n.y * p;
			polar->m_ObjectPolar.z = n.z * p;
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		b3Vector::b3LinearCombine(ray, l2, &m_Base, &n);

		p = acos(-b3Vector::b3AngleOfVectors(&n, &pole));
		polar->m_Polar.x = acos(b3Vector::b3AngleOfVectors(&m_Dir, &n) /
				sin(p)) * 0.5 / M_PI;
		polar->m_Polar.y = p * 2.0 / M_PI - 1.0;
		polar->m_Polar.z = 0;
		if (b3Vector::b3AngleOfVectors(&aux, &n) < 0)
		{
			polar->m_Polar.x = 1.0 - polar->m_Polar.x;
		}

		if (b3CheckStencil(polar))
		{
			p = 1.0 / sqrt(m_QuadRadius);
			polar->m_ObjectPolar.x = n.x * p;
			polar->m_ObjectPolar.y = n.y * p;
			polar->m_ObjectPolar.z = n.z * p;
			return l2;
		}
	}

	return l2;
}

b3_f64 b3Cylinder::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_line64 BTLine;
	b3_f64    l1, l2, z, Discriminant, a, p;

	b3BaseTransform(ray, &BTLine);
	a = 1.0 / (BTLine.dir.x * BTLine.dir.x + BTLine.dir.y * BTLine.dir.y);

	p = (
			BTLine.dir.x * BTLine.pos.x +
			BTLine.dir.y * BTLine.pos.y) * a;
	if ((Discriminant =	p * p - (
					BTLine.pos.x * BTLine.pos.x +
					BTLine.pos.y * BTLine.pos.y - 1) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -p - z;
	l2 = -p + z;

	if (l1 < b3Scene::epsilon)
	{
		l1 = -2;
	}
	else
	{
		z = BTLine.pos.z + l1 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l1 = -2;
		}
	}
	if (l2 < b3Scene::epsilon)
	{
		l2 = -1;
	}
	else
	{
		z = BTLine.pos.z + l2 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l2 = -1;
		}
	}

	// Not found any intersection
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		b3Vector::b3LinearCombine(&BTLine, l1, &polar->m_ObjectPolar);

		polar->m_Polar.x = b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y = polar->m_ObjectPolar.z;
		polar->m_Polar.z = 0;
		if (b3CheckStencil(polar))
		{
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		b3Vector::b3LinearCombine(&BTLine, l2, &polar->m_ObjectPolar);

		polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y	= polar->m_ObjectPolar.z;
		polar->m_Polar.z	= 0;
		if (!b3CheckStencil(polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Cone::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_line64 BTLine;
	b3_f64    l1, l2, z, Discriminant, a, p;

	b3BaseTransform(ray, &BTLine);
	a = 1.0 / (
			BTLine.dir.x * BTLine.dir.x +
			BTLine.dir.y * BTLine.dir.y -
			BTLine.dir.z * BTLine.dir.z);

	p = (
			BTLine.dir.x  *      BTLine.pos.x +
			BTLine.dir.y  *      BTLine.pos.y +
			BTLine.dir.z  * (1 - BTLine.pos.z)) * a;
	if ((Discriminant = p * p - (
					BTLine.pos.x  *    BTLine.pos.x +
					BTLine.pos.y  *    BTLine.pos.y -
					(1 - BTLine.pos.z) * (1 - BTLine.pos.z)) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -p - z;
	l2 = -p + z;


	if (l1 < b3Scene::epsilon)
	{
		l1 = -2;
	}
	else
	{
		z = BTLine.pos.z + l1 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l1 = -2;
		}
	}
	if (l2 < b3Scene::epsilon)
	{
		l2 = -1;
	}
	else
	{
		z = BTLine.pos.z + l2 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l2 = -1;
		}
	}
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		b3Vector::b3LinearCombine(&BTLine, l1, &polar->m_ObjectPolar);

		polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y	= polar->m_ObjectPolar.z;
		polar->m_Polar.z	= 0;
		if (b3CheckStencil(polar))
		{
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}

		b3Vector::b3LinearCombine(&BTLine, l2, &polar->m_ObjectPolar);
		polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y	= polar->m_ObjectPolar.z;
		polar->m_Polar.z	= 0;
		if (!b3CheckStencil(polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Ellipsoid::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_line64  BTLine;
	b3_f64     l1, l2, z, Discriminant, a, p;

	b3BaseTransform(ray, &BTLine);
	a = 1.0 / b3Vector::b3QuadLength(&BTLine.dir);

	p = b3Vector::b3SMul(&BTLine.dir, &BTLine.pos) * a;
	if ((Discriminant = p * p - (b3Vector::b3QuadLength(&BTLine.pos) - 1) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -z - p;
	l2 =  z - p;

	if (l1  < b3Scene::epsilon)
	{
		l1 = -2;
	}
	if (l2  < b3Scene::epsilon)
	{
		l2 = -1;
	}
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		b3Vector::b3LinearCombine(&BTLine, l1, &polar->m_ObjectPolar);

		polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y	= asin(polar->m_ObjectPolar.z) * 2.0 / M_PI;
		polar->m_Polar.z	= 0;
		if (b3CheckStencil(polar))
		{
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		b3Vector::b3LinearCombine(&BTLine, l2, &polar->m_ObjectPolar);

		polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y	= asin(polar->m_ObjectPolar.z) * 2.0 / M_PI;
		polar->m_Polar.z	= 0;
		if (!b3CheckStencil(polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Box::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_line64    BTLine;
	b3_vector64  BasePoint;
	b3_vector64  EndPoint;
	b3_f64       l[6];
	b3_f64       x, y, z, m, l1;
	b3_index     Index = 0;
	b3_index     n[6];

	n[0] = n[1] = 0;
	b3BaseTransform(ray, &BTLine);

	EndPoint.x = (BasePoint.x = -BTLine.pos.x) + 1;
	EndPoint.y = (BasePoint.y = -BTLine.pos.y) + 1;
	EndPoint.z = (BasePoint.z = -BTLine.pos.z) + 1;

	if (BTLine.dir.x != 0)
	{
		if ((m = BasePoint.x / BTLine.dir.x) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			z = m * BTLine.dir.z;
			if ((y >= BasePoint.y) && (y <= EndPoint.y)  &&
				(z >= BasePoint.z) && (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 0;
				Index++;
			}
		}

		if ((m = EndPoint.x / BTLine.dir.x) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			z = m * BTLine.dir.z;
			if ((y >= BasePoint.y) && (y <= EndPoint.y)  &&
				(z >= BasePoint.z) && (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 0;
				Index++;
			}
		}
	}

	if (BTLine.dir.y != 0)
	{
		if ((m = BasePoint.y / BTLine.dir.y) >= b3Scene::epsilon)
		{
			x = m * BTLine.dir.x;
			z = m * BTLine.dir.z;
			if ((x >= BasePoint.x) && (x <= EndPoint.x)  &&
				(z >= BasePoint.z) && (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 1;
				Index++;
			}
		}

		if ((m = EndPoint.y / BTLine.dir.y) >= b3Scene::epsilon)
		{
			x = m * BTLine.dir.x;
			z = m * BTLine.dir.z;
			if ((x >= BasePoint.x) && (x <= EndPoint.x)  &&
				(z >= BasePoint.z) && (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 1;
				Index++;
			}
		}
	}

	if (BTLine.dir.z != 0)
	{
		if ((m = BasePoint.z / BTLine.dir.z) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			x = m * BTLine.dir.x;
			if ((x >= BasePoint.x) && (x <= EndPoint.x)  &&
				(y >= BasePoint.y) && (y <= EndPoint.y))
			{
				l[Index] = m;
				n[Index] = 2;
				Index++;
			}
		}

		if ((m = EndPoint.z / BTLine.dir.z) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			x = m * BTLine.dir.x;
			if ((x >= BasePoint.x) && (x <= EndPoint.x)  &&
				(y >= BasePoint.y) && (y <= EndPoint.y))
			{
				l[Index] = m;
				n[Index] = 2;
				Index++;
			}
		}
	}

	switch (Index)
	{
	case 0:
		// No result
		return -1;

	case 1:
		// One result
		l1 = l[0];
		break;

	case 2:
		// Tow results
		if (l[1] < l[0])
		{
			l1   = l[1];
			n[0] = n[1];
		}
		else
		{
			l1 = l[0];
		}
		break;

	default:
		// Shouldn't be!
		b3PrintF(B3LOG_FULL, "Index = %d\n", Index);
		B3_ASSERT(false);
		return -1;
	}

	x = BTLine.pos.x + l1 * BTLine.dir.x;
	y = BTLine.pos.y + l1 * BTLine.dir.y;
	if (x < 0)
	{
		x = 0;
	}
	else if (x >= 0.9999999)
	{
		x = 0.9999999;
	}
	if (y < 0)
	{
		y = 0;
	}
	else if (y >= 0.9999999)
	{
		y = 0.9999999;
	}

	polar->m_Polar.x = polar->m_ObjectPolar.x = x;
	polar->m_Polar.y = polar->m_ObjectPolar.y = y;
	polar->m_Polar.z = polar->m_ObjectPolar.z = BTLine.pos.z + l1 * BTLine.dir.z;
	polar->m_NormalIndex = n[0];

	return l1;
}

b3_f64 b3Torus::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_loop   NumOfX, i, k;
	b3_f64    Val1, Val2, pQuad, dQuad, pdQuad;
	b3_f64    xp, yp;
	b3_f64    Coeff[5], x[4];
	b3_line64 BTLine;

	b3BaseTransform(ray, &BTLine);
	pQuad	= BTLine.pos.z * BTLine.pos.z;
	dQuad	= BTLine.dir.z * BTLine.dir.z;
	pdQuad	= BTLine.pos.z * BTLine.dir.z;
	Val1	= BTLine.pos.x * BTLine.pos.x +
		BTLine.pos.y * BTLine.pos.y + pQuad - m_aQuad - m_bQuad;
	Val2	= BTLine.pos.x * BTLine.dir.x +
		BTLine.pos.y * BTLine.dir.y + pdQuad;

	Coeff[4]	= 1;
	Coeff[3]	= 4 *  Val2;
	Coeff[2]	= 2 * (Val1        + 2 * Val2    * Val2 + 2 * m_aQuad * dQuad);
	Coeff[1]	= 4 * (Val1 * Val2 + 2 * m_aQuad * pdQuad);
	Coeff[0]	=      Val1 * Val1 + 4 * m_aQuad * (pQuad - m_bQuad);

	if ((NumOfX = b3Cubic::b3SolveOrd4(Coeff, x)) == 0)
	{
		return -1;
	}
	for (i = 0; i < NumOfX;)
	{
		if ((x[i] > b3Scene::epsilon) && (x[i] < ray->Q))
		{
			i++;
		}
		else
		{
			x[i] = x[--NumOfX];
		}
	}

	for (i = 0; i < NumOfX; i++)
	{
		Val1 = x[i];
		for (k = NumOfX - 1; k > i; k--)
		{
			if (x[k] < Val1)
			{
				Val2 = x[k];
				x[k] = Val1;
				Val1 = Val2;
			}
		}
		b3Vector::b3LinearCombine(&BTLine, Val1, &polar->m_ObjectPolar);
		xp = polar->m_ObjectPolar.x;
		yp = polar->m_ObjectPolar.y;

		Val2 = m_aRad - m_aQuad / sqrt(xp * xp + yp * yp);
		polar->m_Polar.x = b3Math::b3RelAngleOfScalars(
				polar->m_ObjectPolar.x,
				polar->m_ObjectPolar.y);
		polar->m_Polar.y = b3Math::b3RelAngleOfScalars(
				Val2,
				polar->m_ObjectPolar.z);
		polar->m_Polar.z = 0;

		if (b3CheckStencil(polar))
		{
			return Val1;
		}
	}
	return -1;
}

#define INCREMENT(x)  ((x) >= 0 ? 1 : -1)
#define FSWAP(a,b,m)  { m = a; a = b; b = m; }

#define _TEST

b3_f64 b3TriangleShape::b3IntersectTriangleList(
	b3_ray  *  ray,
	b3_polar * polar,
	b3_index   TriaField)
{
	b3_index        Index, i, max;
	b3_index    *   buffer;
	b3_triainfo  *  infos, *info;
	b3_res          dxSize;
	b3_vector32     aux, product;
	b3_vector32     dir, pos;
	b3_f64          denominator;
	b3_f64          aValue, bValue;
	b3_f64          e = b3Scene::epsilon;
	b3_f64          OldValue = -1, lValue;

	xSize   = m_xSize;
	ySize   = m_ySize;
	dxSize   = m_xSize << 1;

	pos.x    = ray->pos.x;
	pos.y    = ray->pos.y;
	pos.z    = ray->pos.z;

	dir.x    = ray->dir.x;
	dir.y    = ray->dir.y;
	dir.z    = ray->dir.z;

	B3_ASSERT(TriaField < m_GridCount);
	infos  = m_TriaInfos.b3GetBuffer();
	buffer = m_GridList[TriaField].b3GetBuffer();
	max    = m_GridList[TriaField].b3GetCount();
	for (i = 0; i < max; i++)
	{
		Index    = buffer[i];
		info     = &infos[Index];

		denominator = -1.0 / b3Vector::b3SMul(&info->Normal, &ray->dir);
		{
			b3Vector::b3Sub(&pos, &info->base, &aux);
			lValue = denominator * b3Vector::b3SMul(&info->Normal, &aux);
			if ((lValue >= e) && (lValue < ray->Q))
			{
				b3Vector::b3CrossProduct(&aux, &dir, &product);
				if ((aValue = b3Vector::b3SMul(&info->dir2, &product) * denominator) >= 0)
				{
					bValue = -denominator * b3Vector::b3SMul(&info->dir1, &product);
					if ((bValue >= 0) && ((aValue + bValue) <= 1))
					{
						if (Index & 1)
						{
							polar->m_Polar.x =
								((((Index % dxSize) >> 1) + 1) - aValue) / xSize;
							polar->m_Polar.y =
								((Index / dxSize        + 1) - bValue) / ySize;
						}
						else
						{
							polar->m_Polar.x =
								(((Index % dxSize) >> 1) + aValue) / xSize;
							polar->m_Polar.y =
								(Index / dxSize        + bValue) / ySize;
						}
						polar->m_Polar.z      = 0;
						polar->m_ObjectPolar = polar->m_Polar;
						if (b3CheckStencil(polar))
						{
							ray->Q = OldValue = lValue;
							ray->TriaIndex    = Index;
							ray->aTriaValue   = aValue;
							ray->bTriaValue   = bValue;
						}
					}
				}
			}
		}
	}
	return OldValue;
}

b3_f64 b3TriangleShape::b3Intersect(b3_ray * ray, b3_polar * polar)
{
	b3_index     gx, gy, gz;
	b3_index     sx, sy, sz, index;
	b3_f64       start, end, tn, tf, m, result = -1;
	b3_vector64  pos;
	b3_vector64  d;
	b3_vector64  denom;
	b3_vector64  dmax;
#ifdef _DEBUG
	b3_count     GridMax;
#endif

	B3_ASSERT(m_GridList != null);

	if (m_GridSize <= 1)
	{
		return b3IntersectTriangleList(ray, polar, 0);
	}
#ifdef _DEBUG
	GridMax = m_GridSize * m_GridSize * m_GridSize;
#endif

	denom.x = 1.0 / ray->dir.x;
	pos.x   = (ray->pos.x - m_Base.x);
	start   = (- pos.x) * denom.x;
	end     = (m_GridSize * m_Size.x - pos.x) * denom.x;
	if (start > end)
	{
		FSWAP(start, end, m);
	}

	denom.y = 1.0 / ray->dir.y;
	pos.y   = (ray->pos.y - m_Base.y);
	tn      = (- pos.y) * denom.y;
	tf      = (m_GridSize * m_Size.y - pos.y) * denom.y;
	if (tn    > tf)
	{
		FSWAP(tn, tf, m);
	}
	if (tn    > start)
	{
		start = tn;
	}
	if (tf    < end)
	{
		end   = tf;
	}
	if (start > end)
	{
		return result;
	}

	denom.z = 1.0 / ray->dir.z;
	pos.z   = (ray->pos.z - m_Base.z);
	tn      = (- pos.z) * denom.z;
	tf      = (m_GridSize * m_Size.z - pos.z) * denom.z;
	if (tn    > tf)
	{
		FSWAP(tn, tf, m);
	}
	if (tn    > start)
	{
		start = tn;
	}
	if (tf    < end)
	{
		end   = tf;
	}
	if (start > end)
	{
		return result;
	}

	if ((end   < 0) || (start > ray->Q))
	{
		return result;
	}

	if (start < 0)
	{
		start = 0;
	}
	end -= b3Scene::epsilon;
	if (ray->Q < end)
	{
		end = ray->Q;
	}

	// start position
	pos.x = (pos.x + start * ray->dir.x) / m_Size.x;
	pos.y = (pos.y + start * ray->dir.y) / m_Size.y;
	pos.z = (pos.z + start * ray->dir.z) / m_Size.z;

	// start indizes
	gx    = (b3_index)pos.x;
	gy    = (b3_index)pos.y;
	gz    = (b3_index)pos.z;

	// compute deltas
	dmax.x = fabs(m_Size.x * denom.x);
	dmax.y = fabs(m_Size.y * denom.y);
	dmax.z = fabs(m_Size.z * denom.z);
	d.x    = (pos.x - gx);
	d.y    = (pos.y - gy);
	d.z    = (pos.z - gz);


	// correct start point
	if (d.x > b3Scene::epsilon)
	{
		if (ray->dir.x >= 0)
		{
			d.x  = (1.0 - d.x) * dmax.x;
		}
		else
		{
			d.x *= dmax.x;
		}
	}
	else
	{
		d.x = dmax.x;
		if (gx == m_GridSize)
		{
			gx--;
		}
	}
	if (d.y > b3Scene::epsilon)
	{
		if (ray->dir.y >= 0)
		{
			d.y  = (1.0 - d.y) * dmax.y;
		}
		else
		{
			d.y *= dmax.y;
		}
	}
	else
	{
		d.y = dmax.y;
		if (gy == m_GridSize)
		{
			gy--;
		}
	}
	if (d.z > b3Scene::epsilon)
	{
		if (ray->dir.z >= 0)
		{
			d.z  = (1.0 - d.z) * dmax.z;
		}
		else
		{
			d.z *= dmax.z;
		}
	}
	else
	{
		d.z = dmax.z;
		if (gz == m_GridSize)
		{
			gz--;
		}
	}


	// compute increments
	sx  = INCREMENT(ray->dir.x);
	sy  = INCREMENT(ray->dir.y);
	sz  = INCREMENT(ray->dir.z);

	do
	{
		index = GRID_INDEX(gx, gy, gz, m_GridSize);
#ifdef _DEBUG
		if (index >= GridMax)
		{
			b3PrintF(B3LOG_NORMAL, "index error: (%2ld,%2ld,%2ld) = %ld\n",
				gx, gy, gz, index);
		}
		else
#endif
		{
			if (m_GridList[index].b3GetCount() > 0)
			{
				m = b3IntersectTriangleList(ray, polar, index);
				if (m > 0)
				{
					result = m;  /* ray->Q is modified by IntersectTriangleList */
					if (ray->Q < end)
					{
						end = ray->Q;
					}
				}
			}
			if (start >= end)
			{
				return result;
			}
		}

		if ((d.x <= d.y) && (d.x <= d.z))
		{
			gx    +=     sx;
			if ((gx < 0) || (gx >= m_GridSize))
			{
				return result;
			}
			start +=    d.x;
			d.y   -=    d.x;
			d.z   -=    d.x;
			d.x    = dmax.x;
		}
		else if ((d.y <= d.x) && (d.y <= d.z))
		{
			gy    +=     sy;
			if ((gy < 0) || (gy >= m_GridSize))
			{
				return result;
			}
			start +=    d.y;
			d.x   -=    d.y;
			d.z   -=    d.y;
			d.y    = dmax.y;
		}
		else if ((d.z <= d.x) && (d.z <= d.y))
		{
			gz    +=     sz;
			if ((gz < 0) || (gz >= m_GridSize))
			{
				return result;
			}
			start +=    d.z;
			d.x   -=    d.z;
			d.y   -=    d.z;
			d.z    = dmax.z;
		}
		else
		{
			B3_BEEP;
		}
	}
	while (true);

	return result;
}

/*************************************************************************
**                                                                      **
**                        CSG intersections                             **
**                                                                      **
*************************************************************************/

b3_bool b3CSGShape::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	return false;
}

b3_bool b3CSGSphere::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	b3_f64 Discriminant, p;
	b3_f64 xDiff, yDiff, zDiff;

	p = ray->dir.x * (xDiff = ray->pos.x - m_Base.x) +
		ray->dir.y * (yDiff = ray->pos.y - m_Base.y) +
		ray->dir.z * (zDiff = ray->pos.z - m_Base.z);
	if ((Discriminant = p * p + m_QuadRadius -
				xDiff * xDiff -
				yDiff * yDiff -
				zDiff * zDiff) >= 0)
	{
		Discriminant = sqrt(Discriminant);

		interval->m_x[0].m_Q     = -p - Discriminant;
		interval->m_x[0].m_Shape =  this;
		interval->m_x[1].m_Q     = -p + Discriminant;
		interval->m_x[1].m_Shape =  this;
		interval->m_Count        = 2;
	}
	else
	{
		interval->m_Count = 0;
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGCylinder::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	double l1, l2, z, Discriminant, a, p, x, y;
	bool   check;

	interval->m_Count = 0;

	b3BaseTransform(ray, BTLine);

	// Compute normal cylinder intersection
	a = 1.0 / (
			BTLine->dir.x * BTLine->dir.x +
			BTLine->dir.y * BTLine->dir.y);

	p = (BTLine->dir.x * BTLine->pos.x +
			BTLine->dir.y * BTLine->pos.y) * a;
	if ((Discriminant = p * p - (
					BTLine->pos.x * BTLine->pos.x +
					BTLine->pos.y * BTLine->pos.y - 1) * a) >= 0)
	{
		z  = sqrt(Discriminant);
		l1 = -p - z;
		l2 = -p + z;

		// check near intersection
		z = BTLine->pos.z + l1 * BTLine->dir.z;
		if (z < 0)
		{
			l1 =  -BTLine->pos.z      / BTLine->dir.z;
			x  =   BTLine->pos.x + l1 * BTLine->dir.x;
			y  =   BTLine->pos.y + l1 * BTLine->dir.y;
			check = (x * x + y * y) <= 1;
			interval->m_x[0].m_Index = B3_CSG_BOTTOM;
		}
		else
		{
			if (z > 1)
			{
				l1 = (-BTLine->pos.z + 1) / BTLine->dir.z;
				x  =   BTLine->pos.x + l1 * BTLine->dir.x;
				y  =   BTLine->pos.y + l1 * BTLine->dir.y;
				check = (x * x + y * y) <= 1;
				interval->m_x[0].m_Index = B3_CSG_TOP;
			}
			else
			{
				check = true;
				interval->m_x[0].m_Index = B3_CSG_NORMAL;
			}
		}
		if (check)
		{
			interval->m_x[0].m_Q       = l1;
			interval->m_x[0].m_Shape   = this;
			interval->m_x[0].m_BTLine = BTLine;
			interval->m_Count++;
		}

		// check far intersection
		z = BTLine->pos.z + l2 * BTLine->dir.z;
		if (z < 0)
		{
			l2 =  -BTLine->pos.z      / BTLine->dir.z;
			x  =   BTLine->pos.x + l2 * BTLine->dir.x;
			y  =   BTLine->pos.y + l2 * BTLine->dir.y;
			check = (x * x + y * y) <= 1;
			interval->m_x[interval->m_Count].m_Index = B3_CSG_BOTTOM;	/* Zylinderboden */
		}
		else
		{
			if (z > 1)
			{
				l2 = (-BTLine->pos.z + 1) / BTLine->dir.z;
				x  =   BTLine->pos.x + l2 * BTLine->dir.x;
				y  =   BTLine->pos.y + l2 * BTLine->dir.y;
				check = (x * x + y * y) <= 1;
				interval->m_x[interval->m_Count].m_Index = B3_CSG_TOP;	/* Zylinderdeckel */
			}
			else
			{
				check = true;
				interval->m_x[interval->m_Count].m_Index = B3_CSG_NORMAL;	/* Zylindermantel */
			}
		}
		if (check)
		{
			interval->m_x[interval->m_Count].m_Q       = l2;
			interval->m_x[interval->m_Count].m_Shape   = this;
			interval->m_x[interval->m_Count].m_BTLine = BTLine;
			interval->m_Count++;
		}
		B3_ASSERT(interval->m_Count != 1);
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGCone::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	b3_f64     l1, l2, z1, z2, Discriminant, a, p, x, y;
	b3_index   Index = 0;

	interval->m_Count = 0;
	b3BaseTransform(ray, BTLine);
	a = 1.0 / (
			BTLine->dir.x * BTLine->dir.x +
			BTLine->dir.y * BTLine->dir.y -
			BTLine->dir.z * BTLine->dir.z);

	p = (
			BTLine->dir.x  *      BTLine->pos.x +
			BTLine->dir.y  *      BTLine->pos.y +
			BTLine->dir.z  * (1 - BTLine->pos.z)) * a;
	if ((Discriminant = p * p - (
					BTLine->pos.x  *      BTLine->pos.x +
					BTLine->pos.y  *      BTLine->pos.y -
					(1 - BTLine->pos.z) * (1 - BTLine->pos.z)) * a) >= 0)
	{
		Discriminant = sqrt(Discriminant);
		z1           = BTLine->pos.z + (l1 = -p - Discriminant) * BTLine->dir.z;
		z2           = BTLine->pos.z + (l2 = -p + Discriminant) * BTLine->dir.z;

		if ((z1 >= 0) && (z1 <= 1))
		{
			interval->m_x[Index].m_Q      = l1;
			interval->m_x[Index].m_Shape  = this;
			interval->m_x[Index].m_BTLine = BTLine;
			interval->m_x[Index].m_Index  = B3_CSG_NORMAL;
			Index++;
		}

		if ((z2 >= 0) && (z2 <= 1))
		{
			interval->m_x[Index].m_Q      = l2;
			interval->m_x[Index].m_Shape  = this;
			interval->m_x[Index].m_BTLine = BTLine;
			interval->m_x[Index].m_Index  = B3_CSG_NORMAL;
			Index++;
		}

		if (Index < 2)
		{
			l1 = -BTLine->pos.z /      BTLine->dir.z;
			x  =  BTLine->pos.x + l1 * BTLine->dir.x;
			y  =  BTLine->pos.y + l1 * BTLine->dir.y;
			if ((x * x + y * y) <= 1)
			{
				if (l1 < interval->m_x[0].m_Q)
				{
					interval->m_x[1] = interval->m_x[0];
					interval->m_x[0].m_Q      = l1;
					interval->m_x[0].m_Index  = B3_CSG_BOTTOM;
				}
				else
				{
					interval->m_x[1].m_Q      = l1;
					interval->m_x[1].m_Index  = B3_CSG_BOTTOM;
					interval->m_x[1].m_Shape  = this;
					interval->m_x[1].m_BTLine = BTLine;
				}

				Index++;
			}
		}
		interval->m_Count = Index;
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGEllipsoid::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	b3_f64  z, Discriminant, a, p;

	interval->m_Count = 0;
	b3BaseTransform(ray, BTLine);
	a = 1.0 / b3Vector::b3QuadLength(&BTLine->dir);

	p = b3Vector::b3SMul(&BTLine->dir, &BTLine->pos) * a;
	if ((Discriminant = p * p - (b3Vector::b3QuadLength(&BTLine->pos) - 1) * a) >= 0)
	{
		z  = sqrt(Discriminant);

		interval->m_x[0].m_Q      = -z - p;
		interval->m_x[0].m_Shape  = this;
		interval->m_x[0].m_BTLine = BTLine;
		interval->m_x[1].m_Q      =  z - p;
		interval->m_x[1].m_Shape  = this;
		interval->m_x[1].m_BTLine = BTLine;
		interval->m_Count         = 2;
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGBox::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	b3_vector64   BasePoint, EndPoint;
	b3_f64        l[2];
	b3_csg_index  n[2];
	b3_f64        x, y, z, m;
	b3_index      Index = 0;

	interval->m_Count = 0;
	b3BaseTransform(ray, BTLine);

	EndPoint.x = (BasePoint.x = -BTLine->pos.x) + 1;
	EndPoint.y = (BasePoint.y = -BTLine->pos.y) + 1;
	EndPoint.z = (BasePoint.z = -BTLine->pos.z) + 1;

	if (BTLine->dir.x != 0)
	{
		m = BasePoint.x / BTLine->dir.x;
		y = m * BTLine->dir.y;
		z = m * BTLine->dir.z;
		if ((y >= BasePoint.y) &&
			(y <= EndPoint.y)  &&
			(z >= BasePoint.z) &&
			(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_SIDE;
			Index++;
		}

		m = EndPoint.x / BTLine->dir.x;
		y = m * BTLine->dir.y;
		z = m * BTLine->dir.z;
		if ((y >= BasePoint.y) &&
			(y <= EndPoint.y)  &&
			(z >= BasePoint.z) &&
			(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_SIDE;
			Index++;
		}
	}

	if (BTLine->dir.y != 0)
	{
		m = BasePoint.y / BTLine->dir.y;
		x = m * BTLine->dir.x;
		z = m * BTLine->dir.z;
		if ((x >= BasePoint.x) &&
			(x <= EndPoint.x)  &&
			(z >= BasePoint.z) &&
			(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_FRONT;
			Index++;
		}

		m = EndPoint.y / BTLine->dir.y;
		x = m * BTLine->dir.x;
		z = m * BTLine->dir.z;
		if ((x >= BasePoint.x) &&
			(x <= EndPoint.x)  &&
			(z >= BasePoint.z) &&
			(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_FRONT;
			Index++;
		}
	}

	if (BTLine->dir.z != 0)
	{
		m = BasePoint.z / BTLine->dir.z;
		y = m * BTLine->dir.y;
		x = m * BTLine->dir.x;
		if ((x >= BasePoint.x) &&
			(x <= EndPoint.x)  &&
			(y >= BasePoint.y) &&
			(y <= EndPoint.y))
		{
			l[Index] = m;
			n[Index] = B3_CSG_NORMAL;
			Index++;
		}

		m = EndPoint.z / BTLine->dir.z;
		y = m * BTLine->dir.y;
		x = m * BTLine->dir.x;
		if ((x >= BasePoint.x) &&
			(x <= EndPoint.x)  &&
			(y >= BasePoint.y) &&
			(y <= EndPoint.y))
		{
			l[Index] = m;
			n[Index] = B3_CSG_NORMAL;
			Index++;
		}
	}

	B3_ASSERT(Index <= 2);
	if (Index == 2)
	{
		interval->m_Count = Index;

		// Index knows where lower one is...
		Index = (l[0] < l[1] ? 0 : 1);

		// Lower one
		interval->m_x[0].m_Q      = l[Index];
		interval->m_x[0].m_Shape  = this;
		interval->m_x[0].m_BTLine = BTLine;
		interval->m_x[0].m_Index  = n[Index];

		// Higher one
		Index ^= 1;
		interval->m_x[1].m_Q      = l[Index];
		interval->m_x[1].m_Shape  = this;
		interval->m_x[1].m_BTLine = BTLine;
		interval->m_x[1].m_Index  = n[Index];
	}

	return interval->m_Count > 0;
}

b3_bool b3CSGTorus::b3Intersect(
	b3_ray       *      ray,
	b3_shape_intervals * interval,
	b3_line64     *     BTLine)
{
	b3_loop       NumOfX;
	b3_index      i, k, t = 0;
	b3_f64        Val1, Val2, pQuad, dQuad, pdQuad;
	b3_f64        Coeff[5], x[4];

	interval->m_Count = 0;
	b3BaseTransform(ray, BTLine);
	pQuad  = BTLine->pos.z * BTLine->pos.z;
	dQuad  = BTLine->dir.z * BTLine->dir.z;
	pdQuad = BTLine->pos.z * BTLine->dir.z;
	Val1   =
		BTLine->pos.x * BTLine->pos.x +
		BTLine->pos.y * BTLine->pos.y + pQuad - m_aQuad - m_bQuad;
	Val2   =
		BTLine->pos.x * BTLine->dir.x +
		BTLine->pos.y * BTLine->dir.y + pdQuad;

	Coeff[4] = 1;
	Coeff[3] = 4 *  Val2;
	Coeff[2] = 2 * (Val1        + 2 * Val2  * Val2 + 2 * m_aQuad * dQuad);
	Coeff[1] = 4 * (Val1 * Val2 + 2 * m_aQuad * pdQuad);
	Coeff[0] =      Val1 * Val1 + 4 * m_aQuad * (pQuad - m_bQuad);

	NumOfX = b3Cubic::b3SolveOrd4(Coeff, x);
	if ((NumOfX == 2) || (NumOfX == 4))
	{
		// Insert sorted
		for (i = 0; i < NumOfX; i++)
		{
			// Simple bubble sort - with NumOfX <= 4
			// it's much faster.
			for (k = i; k < NumOfX; k++)
			{
				if (x[k] <= x[i])
				{
					t = i;
				}
			}

			interval->m_x[i].m_Q      = x[t];
			interval->m_x[i].m_Shape  = this;
			interval->m_x[i].m_BTLine = BTLine;

			x[t] = x[i];
		}
		interval->m_Count = NumOfX;
	}

	return interval->m_Count > 0;
}

/*************************************************************************
**                                                                      **
**                        BBox intersection                             **
**                                                                      **
*************************************************************************/

b3_bool b3BBox::b3Intersect(b3_ray * ray, b3_bool check_visibility)
{
	b3_f64 start, end, t_near, t_far, m, pos;

#if 1
	if ((check_visibility) && (m_Visibility == B3_BBOX_INVISIBLE))
	{
		// Early exit
		return false;
	}
#endif

	m       = 1.0 / ray->dir.x;
	pos     = (ray->pos.x  - m_DimBase.x);
	start   = (- pos) * m;
	end     = (m_DimSize.x - pos) * m;
	if (start > end)
	{
		m     = start;
		start = end;
		end   = m;
	}

	m       = 1.0 / ray->dir.y;
	pos     = (ray->pos.y  - m_DimBase.y);
	t_near  = (- pos) * m;
	t_far   = (m_DimSize.y - pos) * m;
	if (t_near > t_far)
	{
		m      = t_near;
		t_near = t_far;
		t_far  = m;
	}
	if (t_near > start)
	{
		start = t_near;
	}
	if (t_far  < end)
	{
		end   = t_far;
	}
	if (start  > end)
	{
		return false;
	}

	// Is it OK?
	if (end    < 0)
	{
		return false;
	}

	m       = 1.0 / ray->dir.z;
	pos     = (ray->pos.z  - m_DimBase.z);
	t_near  = (- pos) * m;
	t_far   = (m_DimSize.z - pos) * m;
	if (t_near > t_far)
	{
		m      = t_near;
		t_near = t_far;
		t_far  = m;
	}
	if (t_near > start)
	{
		start = t_near;
	}
	if (t_far  < end)
	{
		end   = t_far;
	}

	return (start <= end) && (end >= 0) && (start <= ray->Q);
}

b3CSGShape * b3BBox::b3IntersectCSG(b3_ray * ray)
{
	b3Item       *      item;
	b3CSGShape     *    shape;
	b3_line64           lines[B3_MAX_CSG_SHAPES_PER_BBOX]; // This is a hack! To be fixed!
	b3_shape_intervals  local;
	b3_bbox_intervals   intervals[2];
	b3_bbox_intervals * result = intervals;
	b3_csg_point    *   point;
	b3_index            t = 0, index = 0;

	intervals[0].m_Count = 0;
	intervals[1].m_Count = 0;
	B3_FOR_BASE(b3GetShapeHead(), item)
	{
		result    = &intervals[index];
		index    ^= 1;
		shape     = (b3CSGShape *)item;
		shape->b3Intersect(ray, &local, &lines[t++]);
		shape->b3Operate(&local, &intervals[index], result);
	}

	point = result->m_x;
	for (t = 0; t < result->m_Count; t++)
	{
		if ((point->m_Q >= b3Scene::epsilon) && (point->m_Q <= ray->Q))
		{
			ray->Q = point->m_Q;
			shape  = point->m_Shape;
			shape->b3InverseMap(ray, point);
			return shape;
		}
		point++;
	}
	return null;
}

b3Shape * b3Scene::b3Intersect(
	b3BBox * BBox,
	b3_ray * ray,
	b3_bool  check_visibility)
{
	b3Base<b3Item> * Shapes;
	b3Base<b3Item> * BBoxes;
	b3SimpleShape * Shape;
	b3Shape    *    ResultShape = null, *aux;
	b3Item     *    item;
	b3_polar        polar;
	b3_f64          Result;

	while (BBox != null)
	{
		if (BBox->b3Intersect(ray, check_visibility))
		{
			//Check recursively
			BBoxes = BBox->b3GetBBoxHead();
			if (BBoxes->First)
			{
				aux = b3Intersect((b3BBox *)BBoxes->First, ray, check_visibility);
				if (aux != null)
				{
					ResultShape = aux;
				}
			}

			// Check shapes inside this object
			Shapes = BBox->b3GetShapeHead();
			switch (Shapes->b3GetClass())
			{
			case CLASS_SHAPE:
				B3_FOR_BASE(Shapes, item)
				{
					Shape  = (b3SimpleShape *)item;
					Result = Shape->b3Intersect(ray, &polar);
					if ((Result > 0) && (Result <= ray->Q))
					{

						ResultShape = Shape;
						ray->bbox   = BBox;
						ray->Q      = Result;

						*((b3_polar *)&ray->polar) = polar;
					}
				}
				break;

			case CLASS_CSG:
				aux = BBox->b3IntersectCSG(ray);
				if (aux != null)
				{
					ResultShape = aux;
					ray->bbox   = BBox;
				}
				break;

			default:
				// We don't know any other kind of shapes, yet.
				break;
			}
		}
		BBox = (b3BBox *)BBox->Succ;
	}
	return ResultShape;
}

b3Shape * b3Scene::b3IsObscured(
	b3BBox * BBox,
	b3_ray * ray)
{
	b3Base<b3Item> * Shapes;
	b3Base<b3Item> * BBoxes;
	b3SimpleShape * Shape;
	b3Shape    *    ResultShape;
	b3Item     *    item;
	b3_polar        polar;
	b3_f64          Result;

	while (BBox != null)
	{
		if (BBox->b3Intersect(ray, false))
		{
			Shapes = BBox->b3GetShapeHead();
			switch (Shapes->b3GetClass())
			{
			case CLASS_SHAPE:
				B3_FOR_BASE(Shapes, item)
				{
					Shape  = (b3SimpleShape *)item;
					Result = Shape->b3Intersect(ray, &polar);
					if ((Result > 0) && (Result <= ray->Q))
					{
						return Shape;
					}
				}
				break;

			case CLASS_CSG:
				ResultShape = BBox->b3IntersectCSG(ray);
				if (ResultShape != null)
				{
					ray->shape = ResultShape;
					ray->bbox  = BBox;
					return ResultShape;
				}
				break;

			default:
				// We don't know any other kind of shapes, yet.
				break;
			}

			// Check recursively
			BBoxes = BBox->b3GetBBoxHead();
			if (BBoxes->First != null)
			{
				ResultShape = b3IsObscured((b3BBox *)BBoxes->First, ray);
				if (ResultShape != null)
				{
					return ResultShape;
				}
			}

		}
		BBox = (b3BBox *)BBox->Succ;
	}
	return null;
}

/*************************************************************************
**                                                                      **
**                        Find bboxes by ray                            **
**                                                                      **
*************************************************************************/

void b3BBox::b3CollectBBoxes(b3_ray * ray, b3Array<b3BBox *> * array)
{
	b3Item     *    item;
	b3Base<b3Item> * Shapes;
	b3SimpleShape * shape;
	b3BBox     *    bbox;
	b3_polar        polar;

	if (b3Intersect(ray, false))
	{
		// Is any shape hit?
		Shapes = b3GetShapeHead();
		switch (Shapes->b3GetClass())
		{
		case CLASS_SHAPE:
			B3_FOR_BASE(Shapes, item)
			{
				shape = (b3SimpleShape *)item;
				if (shape->b3Intersect(ray, &polar) > 0)
				{
					array->b3Add(this);

					// Leave loop
					break;
				}
			}
			break;

		case CLASS_CSG:
			if (b3IntersectCSG(ray) != null)
			{
				array->b3Add(this);
			}
			break;
		}

		// Collect sub bboxes
		B3_FOR_BASE(b3GetBBoxHead(), item)
		{
			bbox = (b3BBox *)item;
			bbox->b3CollectBBoxes(ray, array);
		}
	}
}

void b3Scene::b3CollectBBoxes(
	b3_line64     *    line,
	b3Array<b3BBox *> * array,
	b3_f64             max)
{
	b3Item * item;
	b3BBox * bbox;
	b3_ray  ray;

	array->b3Clear();
	ray.pos = line->pos;
	ray.dir = line->dir;
	ray.Q   = max;
	B3_FOR_BASE(b3GetBBoxHead(), item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(&ray, array);
	}
}

/*************************************************************************
**                                                                      **
**                        Find bboxes by area                           **
**                                                                      **
*************************************************************************/

void b3BBox::b3CollectBBoxes(
	b3_vector     *    lower,
	b3_vector     *    upper,
	b3Array<b3BBox *> * array)
{
	b3Item  *  item;
	b3BBox  *  bbox;
	b3_vector  lLower, lUpper;

	// Compute local dimensions
	lLower = m_DimBase;
	b3Vector::b3Add(&m_DimBase, &m_DimSize, &lUpper);

	// Are we completely inside the defined search area?
	// And do we contain any shape?
	if ((lower->x <= lLower.x) && (lUpper.x <= upper->x) &&
		(lower->y <= lLower.y) && (lUpper.y <= upper->y) &&
		(lower->z <= lLower.z) && (lUpper.z <= upper->z) &&
		(b3GetShapeHead()->First != null))
	{
		// Yes!
		array->b3Add(this);
	}

	// Search for sub bboxes
	B3_FOR_BASE(b3GetBBoxHead(), item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(lower, upper, array);
	}
}

void b3Scene::b3CollectBBoxes(
	b3_vector     *    lower,
	b3_vector     *    upper,
	b3Array<b3BBox *> * array)
{
	b3Item * item;
	b3BBox * bbox;

	array->b3Clear();
	B3_FOR_BASE(b3GetBBoxHead(), item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(lower, upper, array);
	}
}
