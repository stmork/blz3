/*
**
**      $Filename:      b3Normal.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Normal computation routines
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Sphere::b3Normal(b3_ray *ray)
{
	ray->normal.x = ray->ipoint.x - m_Base.x;
	ray->normal.y = ray->ipoint.y - m_Base.y;
	ray->normal.z = ray->ipoint.z - m_Base.z;
}

void b3Shape2::b3Normal(b3_ray *ray)
{
	ray->normal.x = m_Normal.x;
	ray->normal.y = m_Normal.y;
	ray->normal.z = m_Normal.z;
}

void b3Cylinder::b3Normal(b3_ray *ray)
{
	register double	x,y;

	x = m_DirLen[1] * ray->polar.m_ObjectPolar.x;
	y = m_DirLen[0] * ray->polar.m_ObjectPolar.y;

	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, x, y, &ray->normal);
}

void b3Cone::b3Normal(b3_ray *ray)
{
	b3_f32    x,y;
	b3_f32    z3q,n1q;
	b3_vector n1,z3;

	x     = ray->polar.m_ObjectPolar.x;
	y     = ray->polar.m_ObjectPolar.y;
	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, x, y, &z3);

	x    *= m_DirLen[1];
	y    *= m_DirLen[0];
	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, x, y, &n1);

	z3q = b3Vector::b3QuadLength(&z3);
	n1q = b3Vector::b3QuadLength(&n1);
	b3Vector::b3Scale(&n1, sqrt(z3q / n1q));

#if 1
	x = sqrt((n1.x * n1.x + n1.y * n1.y + n1.z * n1.z) / m_DirLen[2]);
#else
	// The ICC can't compile this correctly ?!?
	x = sqrt(n1q / m_DirLen[2]);
#endif
	y = 1 / x;

	b3Vector::b3LinearCombine(&m_Dir3, &n1, x, y, &ray->normal);
}

void b3Ellipsoid::b3Normal(b3_ray *ray)
{
	b3_f64	x,y,z;

	x = m_DirLen[1] * m_DirLen[2] * ray->polar.m_ObjectPolar.x;
	y = m_DirLen[0] * m_DirLen[2] * ray->polar.m_ObjectPolar.y;
	z = m_DirLen[0] * m_DirLen[1] * ray->polar.m_ObjectPolar.z;

	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, &m_Dir3, x, y, z, &ray->normal);
}

void b3Box::b3Normal(b3_ray *ray)
{
	ray->normal.x = m_Normals[ray->polar.m_NormalIndex].x;
	ray->normal.y = m_Normals[ray->polar.m_NormalIndex].y;
	ray->normal.z = m_Normals[ray->polar.m_NormalIndex].z;
}

void b3Torus::b3Normal(b3_ray *ray)
{
	b3_f64 Dist,x,y,z;

	x     = ray->polar.m_ObjectPolar.x;
	y     = ray->polar.m_ObjectPolar.y;
	z     = ray->polar.m_ObjectPolar.z;

	Dist  = sqrt(x * x + y * y);
	x    -= (m_aRad * x / Dist);
	y    -= (m_aRad * y / Dist);

	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, &m_Dir3, x, y, z, &ray->normal);
}

void b3TriangleShape::b3Normal(b3_ray *ray)
{
	b3_f64   a,b;
	b3_index P1,P2,P3;

	if (m_Flags & PHONG)
	{
		b3_vector dir1, dir2;

		a        = ray->aTriaValue;
		b        = ray->bTriaValue;
		P1       = m_Triangles[ray->TriaIndex].P1;
		P2       = m_Triangles[ray->TriaIndex].P2;
		P3       = m_Triangles[ray->TriaIndex].P3;
		b3Vector::b3Sub(&m_Vertices[P2].Normal, &m_Vertices[P1].Normal, &dir1);
		b3Vector::b3Sub(&m_Vertices[P3].Normal, &m_Vertices[P1].Normal, &dir2);
		b3Vector::b3LinearCombine(&m_Vertices[P1].Normal, &dir1, &dir2, a, b, &ray->normal);
	}
	else
	{
		ray->normal.x = m_Triangles[ray->TriaIndex].Normal.x;
		ray->normal.y = m_Triangles[ray->TriaIndex].Normal.y;
		ray->normal.z = m_Triangles[ray->TriaIndex].Normal.z;
	}
}

void b3CSGSphere::b3Normal(b3_ray *ray)
{
	ray->normal.x = ray->ipoint.x - m_Base.x;
	ray->normal.y = ray->ipoint.y - m_Base.y;
	ray->normal.z = ray->ipoint.z - m_Base.z;
}

void b3CSGCylinder::b3Normal(b3_ray *ray)
{
	b3_f64 x,y;

	switch(ray->polar.m_NormalIndex)
	{
	case B3_CSG_NORMAL:
		x = m_DirLen[1] * ray->polar.m_ObjectPolar.x;
		y = m_DirLen[0] * ray->polar.m_ObjectPolar.y;

		b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, x, y, &ray->normal);
		break;

	case B3_CSG_TOP:
	case B3_CSG_BOTTOM:
		ray->normal.x = m_Normals[2].x;
		ray->normal.y = m_Normals[2].y;
		ray->normal.z = m_Normals[2].z;
		break;

	default:
		B3_ASSERT(false);
		break;
	}
}

void b3CSGCone::b3Normal(b3_ray *ray)
{
	b3_f32    x,y,Factor,n1q;
	b3_vector n1,z3;

	switch(ray->polar.m_NormalIndex)
	{
	case B3_CSG_NORMAL:
		x     = ray->polar.m_ObjectPolar.x;
		y     = ray->polar.m_ObjectPolar.y;
		b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, x, y, &z3);

		x    *= m_DirLen[1];
		y    *= m_DirLen[0];
		b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, x, y, &n1);

		n1q = b3Vector::b3QuadLength(&n1);
		Factor = sqrt(b3Vector::b3QuadLength(&z3) / n1q);
		b3Vector::b3Scale(&n1, Factor);

		x = sqrt(n1q / m_DirLen[2]);
		y = 1 / x;
		b3Vector::b3LinearCombine(&m_Dir3, &n1, x, y, &ray->normal);
		break;

	case B3_CSG_BOTTOM:
		ray->normal.x = m_Normals[2].x;
		ray->normal.y = m_Normals[2].y;
		ray->normal.z = m_Normals[2].z;
		break;

	default:
		B3_ASSERT(false);
		break;
	}
}

void b3CSGEllipsoid::b3Normal(b3_ray *ray)
{
	b3_f64	x,y,z;

	x = m_DirLen[1] * m_DirLen[2] * ray->polar.m_ObjectPolar.x;
	y = m_DirLen[0] * m_DirLen[2] * ray->polar.m_ObjectPolar.y;
	z = m_DirLen[0] * m_DirLen[1] * ray->polar.m_ObjectPolar.z;

	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, &m_Dir3, x, y, z, &ray->normal);
}

void b3CSGBox::b3Normal(b3_ray *ray)
{
	ray->normal.x = m_Normals[ray->polar.m_NormalIndex].x;
	ray->normal.y = m_Normals[ray->polar.m_NormalIndex].y;
	ray->normal.z = m_Normals[ray->polar.m_NormalIndex].z;
}

void b3CSGTorus::b3Normal(b3_ray *ray)
{
	b3_f64 Dist,x,y,z;

	x     = ray->polar.m_ObjectPolar.x;
	y     = ray->polar.m_ObjectPolar.y;
	z     = ray->polar.m_ObjectPolar.z;

	Dist  = sqrt (x * x + y * y);
	x    -= (m_aRad * x / Dist);
	y    -= (m_aRad * y / Dist);

	b3Vector::b3LinearCombine(&m_Dir1, &m_Dir2, &m_Dir3, x, y, z, &ray->normal);
}
