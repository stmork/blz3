/*
**
**      $Filename:      b3Intersect.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Intersection routines
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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2001/10/02 16:01:58  sm
**	- Moving b3Polar into b3Ray but that's not right at all. The
**	  result must be placed there but a simple result from one
**	  intersection must be placed into a temp instance. The same
**	  must be done for surface normals as they result from using
**	  the b3Polar class.
**
**	Revision 1.6  2001/09/30 16:27:48  sm
**	- Raytracing with diffuse color without shading
**	- Sphere intersection fixed (now using normalized rays)
**	
**	Revision 1.5  2001/09/30 15:53:19  sm
**	- Removing nasty CR/LF
**	
**	Revision 1.4  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**	
**	Revision 1.3  2001/09/23 15:37:15  sm
**	- Introducing raytracing for Lines III. There is much work
**	  for a b3Display-CScrollView.
**	
**	Revision 1.2  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.1  2001/09/22 16:19:52  sm
**	- Adding basic shape intersection routines
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static b3_f64 epsilon = 0.001;

b3_f64 b3Shape::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3Area::b3Intersect(b3_ray *ray)
{
	b3_f64    Denom,aValue,bValue,lValue;
	b3_vector Dir,Product;
 
	if ((Denom =
		-m_Normal.x * ray->dir.x
		-m_Normal.y * ray->dir.y
		-m_Normal.z * ray->dir.z) == 0)
	{
		return -1;
	}

	Denom = 1 / Denom;
	if ((lValue=(
		m_Normal.x * (Dir.x = ray->pos.x - m_Base.x) +
		m_Normal.y * (Dir.y = ray->pos.y - m_Base.y) +
		m_Normal.z * (Dir.z = ray->pos.z - m_Base.z))
			* Denom) < epsilon)
	{
		return -1;
	}

	if (lValue > ray->Q)
	{
		return -1;
	}

	aValue = (
		m_Dir2.x * (Product.x = Dir.y * ray->dir.z - Dir.z * ray->dir.y) +
		m_Dir2.y * (Product.y = Dir.z * ray->dir.x - Dir.x * ray->dir.z) +
		m_Dir2.z * (Product.z = Dir.x * ray->dir.y - Dir.y * ray->dir.x))
			* Denom;
	if (fabs(aValue) > 1)
	{
		return -1;
	}

	bValue = (
		-m_Dir1.x * Product.x
		-m_Dir1.y * Product.y
		-m_Dir1.z * Product.z) * Denom;
	if (fabs(bValue) > 1)
	{
		return -1;
	}

	ray->polar.polar.x = ray->polar.object_polar.x = aValue;
	ray->polar.polar.y = ray->polar.object_polar.y = bValue;
	ray->polar.polar.z = ray->polar.object_polar.z = 0;

	return b3CheckStencil(&ray->polar) ? lValue : -1;
}

b3_f64 b3Disk::b3Intersect(b3_ray *ray)
{
	b3_f64     Denom,aValue,bValue,lValue;
	b3_vector  Dir,Product;
 
	if ((Denom =
		-m_Normal.x * ray->dir.x
		-m_Normal.y * ray->dir.y
		-m_Normal.z * ray->dir.z) == 0)
	{
		return -1;
	}
	Denom = 1 / Denom;

	// check distance
	if ((lValue = (
		m_Normal.x * (Dir.x = ray->pos.x - m_Base.x) +
		m_Normal.y * (Dir.y = ray->pos.y - m_Base.y) +
		m_Normal.z * (Dir.z = ray->pos.z - m_Base.z))
			* Denom) < epsilon)
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
	bValue = (
		-m_Dir1.x * Product.x
		-m_Dir1.y * Product.y
		-m_Dir1.z * Product.z) * Denom;
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
	ray->polar.polar.x	= b3RelAngleOfScalars(aValue,bValue);
	ray->polar.polar.y	= sqrt (Denom);
	ray->polar.polar.z	= 0;
	ray->polar.object_polar.x = aValue;
	ray->polar.object_polar.y = bValue;
	ray->polar.object_polar.z = 0;

	return b3CheckStencil(&ray->polar) ? lValue : -1;
}

b3_f64 b3Sphere::b3Intersect(b3_ray *ray)
{
	b3_f64    l1,Discriminant,p,l2;
	b3_f64    xDiff,yDiff,zDiff;
	b3_vector n,aux,pole;

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
	if (l1  < epsilon) l1 = -2;
	if (l2  < epsilon) l2 = -1;
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
		pole.x =  m_Dir.y * aux.z - m_Dir.z * aux.y;
		pole.y =  m_Dir.z * aux.x - m_Dir.x * aux.z;
		pole.z =  m_Dir.x * aux.y - m_Dir.y * aux.x;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		n.x = ray->pos.x + l1 * ray->dir.x - m_Base.x;
		n.y = ray->pos.y + l1 * ray->dir.y - m_Base.y;
		n.z = ray->pos.z + l1 * ray->dir.z - m_Base.z;

		p = acos(-b3AngleOfVectors(&n,&pole));
		ray->polar.polar.x = acos(b3AngleOfVectors (&m_Dir,&n) /
			sin(p)) * 0.5 / M_PI;
		ray->polar.polar.y = p * 2.0 / M_PI - 1.0;
		ray->polar.polar.z = 0;
		if (b3AngleOfVectors(&aux,&n) < 0)
		{
			ray->polar.polar.x = 1.0 - ray->polar.polar.x;
		}

		if (b3CheckStencil(&ray->polar))
		{
			p = sqrt(m_QuadRadius);
			ray->polar.object_polar.x = n.x / p;
			ray->polar.object_polar.y = n.y / p;
			ray->polar.object_polar.z = n.z / p;
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		n.x = ray->pos.x + l2 * ray->dir.x - m_Base.x;
		n.y = ray->pos.y + l2 * ray->dir.y - m_Base.y;
		n.z = ray->pos.z + l2 * ray->dir.z - m_Base.z;

		p = acos(-b3AngleOfVectors(&n,&pole));
		ray->polar.polar.x = acos(b3AngleOfVectors (&m_Dir,&n) /
			sin(p)) * 0.5 / M_PI;
		ray->polar.polar.y = p * 2.0 / M_PI - 1.0;
		ray->polar.polar.z = 0;
		if (b3AngleOfVectors(&aux,&n) < 0)
		{
			ray->polar.polar.x = 1.0 - ray->polar.polar.x;
		}

		if (b3CheckStencil(&ray->polar))
		{
			p = sqrt(m_QuadRadius);
			ray->polar.object_polar.x = n.x / p;
			ray->polar.object_polar.y = n.y / p;
			ray->polar.object_polar.z = n.z / p;
			return l2;
		}
	}

	return l2;
}

b3_f64 b3Cylinder::b3Intersect(b3_ray *ray)
{
	b3_dLine BTLine;
	b3_f64   l1,l2,z,Discriminant,a,p;

	b3BaseTrans (ray,&BTLine);
	a =  BTLine.dir.x * BTLine.dir.x + BTLine.dir.y * BTLine.dir.y;
	if (a == 0)
	{
		return -1;
	}
	a = 1 / a;

	p = (BTLine.dir.x * BTLine.pos.x + BTLine.dir.y * BTLine.pos.y) * a;
	if ((Discriminant =	p * p -
		(BTLine.pos.x * BTLine.pos.x +
		 BTLine.pos.y * BTLine.pos.y - 1) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -p - z;
	l2 = -p + z;

	if (l1 < epsilon)
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
	if (l2 < epsilon)
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
		ray->polar.object_polar.x = BTLine.pos.x + l1 * BTLine.dir.x;
		ray->polar.object_polar.y = BTLine.pos.y + l1 * BTLine.dir.y;
		ray->polar.object_polar.z = BTLine.pos.z + l1 * BTLine.dir.z;

		ray->polar.polar.x = b3RelAngleOfScalars(
			ray->polar.object_polar.x,
			ray->polar.object_polar.y);
		ray->polar.polar.y = ray->polar.object_polar.z;
		ray->polar.polar.z = 0;
		if (b3CheckStencil(&ray->polar))
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
		ray->polar.object_polar.x = BTLine.pos.x + l2 * BTLine.dir.x;
		ray->polar.object_polar.y = BTLine.pos.y + l2 * BTLine.dir.y;
		ray->polar.object_polar.z = BTLine.pos.z + l2 * BTLine.dir.z;

		ray->polar.polar.x	= b3RelAngleOfScalars(
			ray->polar.object_polar.x,
			ray->polar.object_polar.y);
		ray->polar.polar.y	= ray->polar.object_polar.z;
		ray->polar.polar.z	= 0;
		if (!b3CheckStencil(&ray->polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Cone::b3Intersect(b3_ray *ray)
{
	b3_dLine BTLine;
	b3_f64   l1,l2,z,Discriminant,a,p;

	b3BaseTrans (ray,&BTLine);
	a = BTLine.dir.x * BTLine.dir.x +
		BTLine.dir.y * BTLine.dir.y -
		BTLine.dir.z * BTLine.dir.z;
	if (a == 0)
	{
		return -1;
	}

	a = 1 / a;
	p = (
		BTLine.dir.x  *      BTLine.pos.x +
		BTLine.dir.y  *      BTLine.pos.y +
		BTLine.dir.z  * (1 - BTLine.pos.z)) * a;
	if ((Discriminant = p * p -
		(   BTLine.pos.x  *    BTLine.pos.x +
		    BTLine.pos.y  *    BTLine.pos.y -
		 (1-BTLine.pos.z) * (1-BTLine.pos.z)) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -p - z;
	l2 = -p + z;

	if (l1 < epsilon)
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
	if (l2 < epsilon)
	{
		l2 = -1;
	}
	else
	{
		z = BTLine.pos.z + l2 * BTLine.dir.z;
		if ((z < 0)||(z > 1))
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
		ray->polar.object_polar.x = BTLine.pos.x + l1 * BTLine.dir.x;
		ray->polar.object_polar.y = BTLine.pos.y + l1 * BTLine.dir.y;
		ray->polar.object_polar.z = BTLine.pos.z + l1 * BTLine.dir.z;

		ray->polar.polar.x	= b3RelAngleOfScalars(
			ray->polar.object_polar.x,
			ray->polar.object_polar.y);
		ray->polar.polar.y	= ray->polar.object_polar.z;
		ray->polar.polar.z	= 0;
		if (b3CheckStencil(&ray->polar))
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
		ray->polar.object_polar.x = BTLine.pos.x + l2 * BTLine.dir.x;
		ray->polar.object_polar.y = BTLine.pos.y + l2 * BTLine.dir.y;
		ray->polar.object_polar.z = BTLine.pos.z + l2 * BTLine.dir.z;
		ray->polar.polar.x	= b3RelAngleOfScalars(
			ray->polar.object_polar.x,
			ray->polar.object_polar.y);
		ray->polar.polar.y	= ray->polar.object_polar.z;
		ray->polar.polar.z	= 0;
		if (!b3CheckStencil(&ray->polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Ellipsoid::b3Intersect(b3_ray *ray)
{
	b3_dLine  BTLine;
	b3_f64    l1,l2,z,Discriminant,a,p;

	b3BaseTrans (ray,&BTLine);
	a = BTLine.dir.x * BTLine.dir.x +
		BTLine.dir.y * BTLine.dir.y +
		BTLine.dir.z * BTLine.dir.z;
	if (a == 0)
	{
		return -1;
	}

	a = 1 / a;
	p = (BTLine.dir.x * BTLine.pos.x +
		 BTLine.dir.y * BTLine.pos.y +
		 BTLine.dir.z * BTLine.pos.z) * a;
	if ((Discriminant = p * p -
		(BTLine.pos.x * BTLine.pos.x +
		 BTLine.pos.y * BTLine.pos.y +
		 BTLine.pos.z * BTLine.pos.z - 1) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -z - p;
	l2 =  z - p;

	if (l1  < epsilon) l1 = -2;
	if (l2  < epsilon) l2 = -1;
	if ((l1 < 0) && (l2 < 0)) return -3;

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		ray->polar.object_polar.x = BTLine.pos.x + l1 * BTLine.dir.x;
		ray->polar.object_polar.y = BTLine.pos.y + l1 * BTLine.dir.y;
		ray->polar.object_polar.z = BTLine.pos.z + l1 * BTLine.dir.z;

		ray->polar.polar.x	= b3RelAngleOfScalars(
			ray->polar.object_polar.x,
			ray->polar.object_polar.y);
		ray->polar.polar.y	= asin(ray->polar.object_polar.z) * 2.0 / M_PI;
		ray->polar.polar.z	= 0;
		if (b3CheckStencil(&ray->polar))
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
		ray->polar.object_polar.x = BTLine.pos.x + l2 * BTLine.dir.x;
		ray->polar.object_polar.y = BTLine.pos.y + l2 * BTLine.dir.y;
		ray->polar.object_polar.z = BTLine.pos.z + l2 * BTLine.dir.z;

		ray->polar.polar.x	= b3RelAngleOfScalars(
			ray->polar.object_polar.x,
			ray->polar.object_polar.y);
		ray->polar.polar.y	= asin(ray->polar.object_polar.z) * 2.0 / M_PI;
		ray->polar.polar.z	= 0;
		if (!b3CheckStencil(&ray->polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Box::b3Intersect(b3_ray *ray)
{
	b3_dLine   BTLine;
	b3_dVector BasePoint,EndPoint;
	b3_f64     l[2];
	b3_f64     x,y,z,m,l1,l2;
	b3_index   Index;
	b3_index   n[2];

	Index = n[0] = n[1] = 0;
	b3BaseTrans (ray,&BTLine);

	if ((BTLine.dir.x == 0) ||
		(BTLine.dir.y == 0) ||
		(BTLine.dir.z == 0))
	{
		return -1;
	}

	EndPoint.x = (BasePoint.x = -BTLine.pos.x) + 1;
	EndPoint.y = (BasePoint.y = -BTLine.pos.y) + 1;
	EndPoint.z = (BasePoint.z = -BTLine.pos.z) + 1;

	if ((m = BasePoint.x / BTLine.dir.x) >= epsilon)
	{
		y = m * BTLine.dir.y;
		z = m * BTLine.dir.z;
		if ((y >= BasePoint.y) &&
		    (y <= EndPoint.y)  &&
		    (z >= BasePoint.z) &&
			(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = 0;
			Index++;
		}
	}

	if ((m = EndPoint.x / BTLine.dir.x) >= epsilon)
	{
		y = m * BTLine.dir.y;
		z = m * BTLine.dir.z;
		if ((y >= BasePoint.y) &&
		    (y <= EndPoint.y)  &&
		    (z >= BasePoint.z) &&
		    (z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = 0;
			Index++;
		}
	}

	if ((m = BasePoint.y / BTLine.dir.y) >= epsilon)
	{
		x = m * BTLine.dir.x;
		z = m * BTLine.dir.z;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (z >= BasePoint.z) &&
		    (z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = 1;
			Index++;
		}
	}

	if ((m = EndPoint.y / BTLine.dir.y) >= epsilon)
	{
		x = m * BTLine.dir.x;
		z = m * BTLine.dir.z;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (z >= BasePoint.z) &&
		    (z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = 1;
			Index++;
		}
	}

	if ((m = BasePoint.z / BTLine.dir.z) >= epsilon)
	{
		y = m * BTLine.dir.y;
		x = m * BTLine.dir.x;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (y >= BasePoint.y) &&
		    (y <= EndPoint.y))
		{
			l[Index] = m;
			n[Index] = 2;
			Index++;
		}
	}

	if ((m = EndPoint.z / BTLine.dir.z) >= epsilon)
	{
		y = m * BTLine.dir.y;
		x = m * BTLine.dir.x;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (y >= BasePoint.y) &&
		    (y <= EndPoint.y))
		{
			l[Index] = m;
			n[Index] = 2;
			Index++;
		}
	}

	if (Index <= 0)
	{
		return -1;
	}
	if (Index == 2)
	{
		if ((l2 = l[1]) < (l1 = l[0]))
		{
			l1   = l2;
			n[0] = n[1];
		}
	}
	else
	{
		l1 = l[0];
	}
	if (l1 < epsilon)
	{
		return l1;
	}

	x = BTLine.pos.x + l1 * BTLine.dir.x;
	y = BTLine.pos.y + l1 * BTLine.dir.y;
	if (x < 0)               x = 0;
	else if (x >= 0.9999999) x = 0.9999999;
	if (y < 0)               y = 0;
	else if (y >= 0.9999999) y = 0.9999999;

	ray->polar.polar.x = ray->polar.object_polar.x = x;
	ray->polar.polar.y = ray->polar.object_polar.y = y;
	ray->polar.polar.z = ray->polar.object_polar.z = BTLine.pos.z + l1 * BTLine.dir.z;

	ray->normal = m_Normals[n[0]];
	return l1;
}

b3_f64 b3Torus::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3TriangleShape::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3SplineArea::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3SplineCylinder::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3SplineRing::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3SplineCurveShape::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3SplineRotShape::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3CSGSphere::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3CSGCylinder::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3CSGCone::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3CSGEllipsoid::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3CSGBox::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_f64 b3CSGTorus::b3Intersect(b3_ray *ray)
{
	return -1;
}

b3_bool b3BBox::b3Intersect(b3_ray *ray)
{
	b3_f64 start,end,t_near,t_far,m,pos;

	m       = 1.0 / ray->dir.x;
	pos     = (ray->pos.x  - m_DimBase.x);
	start   = (            - pos) * m;
	end     = (m_DimSize.x - pos) * m;
	if (start > end)
	{
		m     = start;
		start = end;
		end   = m;
	}

	m       = 1.0 / ray->dir.y;
	pos     = (ray->pos.y  - m_DimBase.y);
	t_near  = (            - pos) * m;
	t_far   = (m_DimSize.y - pos) * m;
	if (t_near > t_far)
	{
		m      = t_near;
		t_near = t_far;
		t_far  = m;
	}
	if (t_near > start) start = t_near;
	if (t_far  < end)   end   = t_far;
	if (start  > end) return false;

	// Is it OK?
	if (end    < 0)   return false;

	m       = 1.0 / ray->dir.z;
	pos     = (ray->pos.z  - m_DimBase.z);
	t_near  = (            - pos) * m;
	t_far   = (m_DimSize.z - pos) * m;
	if (t_near > t_far)
	{
		m      = t_near;
		t_near = t_far;
		t_far  = m;
	}
	if (t_near > start) start = t_near;
	if (t_far  < end)   end   = t_far;
	if (start  > end) return false;

	if (end   < 0)      return false;
	if (start > ray->Q) return false;
	return true;
}


b3Shape *b3Scene::b3Intersect(
	b3BBox      *BBox,
	b3_ray_info *ray)
{
	b3Base<b3Item> *Shapes;
	b3Base<b3Item> *BBoxes;
	b3Shape        *Shape,*BackShape = null,*Found;
	b3Item         *item;
	b3_f64          Result;

	while (BBox != null)
	{
		Shapes = BBox->b3GetShapeHead();
		BBoxes = BBox->b3GetBBoxHead();
		if (BBox->b3Intersect(ray))
		{
			if (BBoxes->First)
			{
				Found = b3Intersect ((b3BBox *)BBoxes->First,ray);
				if (Found != null)
				{
					BackShape = Found;
				}
			}

			if (Shapes->b3GetClass() == CLASS_SHAPE)
			{
				B3_FOR_BASE(Shapes,item)
				{
					Shape  = (b3Shape *)item;
					Result = Shape->b3Intersect(ray);
					if ((Result > 0) && (Result <= ray->Q))
					{
						BackShape  = Shape;
//						GlobalBBox = BBox;
						ray->Q     = Result;
					}
				} /* for shape*/
			}     /* if CLASS_SHAPE */


			// CLASS_SHAPE
#if 0
			if (Shapes->b3GetClass() == CLASS_CSG)
			{
				BackShape = BBox->b3IntersectCSG (BackShape,ray,Q);
			}
#endif
		}
		BBox = (b3BBox *)BBox->Succ;
	}
	return BackShape;
}

b3_bool b3Scene::b3Intersect(b3_ray_info *ray,b3_f64 max)
{
	ray->Q     = max;
	ray->shape = b3Intersect(b3GetFirstBBox(),ray);

	return ray->shape != null;
}
