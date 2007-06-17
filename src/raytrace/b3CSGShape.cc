/*
**
**	$Filename:	b3CSGShape.cc $
**	$Release:	Dortmund 2002 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing CSG shapes
**
**	(C) Copyright 2002  Steffen A. Mork
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

b3_csg_operation b3CSGShape::m_CSGMode[] =
{
	B3_CSG_UNION,
	B3_CSG_INTERSECT,
	B3_CSG_SUB
};

/*************************************************************************
**                                                                      **
**                        CSG shape base class                          **
**                                                                      **
*************************************************************************/
 
b3CSGShape::b3CSGShape(b3_size class_size,b3_u32 class_type) : b3Shape(class_size, class_type)
{
	m_Operation = B3_CSG_UNION;
}

b3CSGShape::b3CSGShape(b3_u32 class_type) : b3Shape(sizeof(b3CSGShape), class_type)
{
	m_Operation = B3_CSG_UNION;
}

b3CSGShape::b3CSGShape(b3_u32 *src) : b3Shape(src)
{
}

b3_size b3CSGShape::b3GetOperationIndex(b3_csg_operation mode)
{
	b3_size i;

	for (i = 0;i < (sizeof(m_CSGMode) / sizeof(b3_csg_operation));i++)
	{
		if (m_CSGMode[i] == mode)
		{
			return i;
		}
	}
	return 0;
}

void b3CSGShape::b3Operate(
	b3_shape_intervals *local,
	b3_bbox_intervals  *source,
	b3_bbox_intervals  *result)
{
	b3_csg_point      *Point,*PointA,*PointB;
	b3_count           aCount,bCount;
	b3_bool            stat,aStat,bStat,cStat;

	// pointer to result interval
	result->m_Count = 0;
	aStat = bStat = cStat = stat = false;

	// these are the intervals to compute (A and B)
	aCount = source->m_Count;
	PointA = source->m_x;
	bCount = local->m_Count;
	PointB = local->m_x;

	// while both intervals are not empty.
	while ((aCount != 0) || (bCount != 0))
	{
		// if A is empty read B
		if (aCount == 0)
		{
			Point = PointB++;
			bCount--;
			bStat = !bStat;
		}
		else
		{
			// if B is empty, read point from A.
			if (bCount == 0)
			{
				Point = PointA++;
				aCount--;
				aStat = !aStat;
			}
			else
			{
				// else read smallest point
				if (PointA->m_Q < PointB->m_Q)
				{
					Point = PointA++;
					aCount--;
					aStat = !aStat;
				}
				else
				{
					Point = PointB++;
					bCount--;
					bStat = !bStat;
				}
			}
		}

		// Do compute boolean operation!
		switch(m_Operation)
		{
		case B3_CSG_UNION:
			stat = (aStat || bStat);
			break;
		case B3_CSG_INTERSECT:
			stat = (aStat && bStat);
			break;
		case B3_CSG_SUB:
			stat = (aStat ? (!bStat) : false);
			break;
		}
		
		// Store only points which state (inside/outside)
		// are changed.
		if (stat != cStat)
		{
			// insert result
			result->m_x[result->m_Count++] = *Point;
			cStat = stat;
		}
	}
}

void b3CSGShape::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
}

b3_count b3CSGShape::b3GetMaxIntersections()
{
	return 0;
}

/*************************************************************************
**                                                                      **
**                        Base class for quadric CSG shapes             **
**                                                                      **
*************************************************************************/
 
b3CSGShape3::b3CSGShape3(b3_size class_size,b3_u32 class_type) : b3CSGShape(class_size, class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0,50);
}

b3CSGShape3::b3CSGShape3(b3_u32 class_type) : b3CSGShape(sizeof(b3CSGShape3), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0,50);
}

b3CSGShape3::b3CSGShape3(b3_u32 *src) : b3CSGShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);

	b3InitFloat(); // This is lSize
	b3InitFloat(); // This is Denom
	b3InitFloat(); // This is DirLen[0]
	b3InitFloat(); // This is DirLen[1]
	b3InitFloat(); // This is DirLen[2]

	b3InitInt();   // This Index
	m_Operation = (b3_csg_operation)b3InitInt();

	b3InitVector(); // This is BTLine.pos
	b3InitVector(); // This is BTLine.dir
	b3InitActivation();
}

void b3CSGShape3::b3StoreShape()
{
	for (b3_loop i = 0;i < 3;i++)
	{
		b3_vector normal;

		b3Vector::b3Init(&normal, &m_Normals[i]);
		b3StoreVector(&normal);
	}
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreVector(&m_Dir3);

	b3StoreInt(0); // This is lSize
	b3StoreFloat(m_Denom);
	b3StoreFloat(m_DirLen[0]);
	b3StoreFloat(m_DirLen[1]);
	b3StoreFloat(m_DirLen[2]);
	b3StoreInt(m_Index);
	b3StoreInt(m_Operation);
	b3StoreVector(); // This is BTLine.pos
	b3StoreVector(); // This is BTLine.dir
}

void b3CSGShape3::b3ComputeNormals(b3_bool normalize)
{
	b3ComputeQuadricNormals(normalize);
}

void b3CSGShape3::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3Matrix::b3VMul (transformation,&m_Base,&m_Base,true);
	b3Matrix::b3VMul (transformation,&m_Dir1,&m_Dir1,false);
	b3Matrix::b3VMul (transformation,&m_Dir2,&m_Dir2,false);
	b3Matrix::b3VMul (transformation,&m_Dir3,&m_Dir3,false);
	b3TransformVertices(transformation,is_affine);
}

void b3CSGShape3::b3SetupPicking(b3PickInfo *info)
{
	info->b3AddPickPoint(&m_Base,"b");
	info->b3AddPickDir(  &m_Base,&m_Dir1,"1");
	info->b3AddPickDir(  &m_Base,&m_Dir2,"2");
	info->b3AddPickDir(  &m_Base,&m_Dir3,"3");
}

b3_bool b3CSGShape3::b3Prepare(b3_preparation_info *prep_info)
{
	b3_bool result = false;

	if (b3ShapeBaseTransformation::b3Prepare())
	{
		result = b3Shape::b3Prepare(prep_info);
	}
	return result;
}

void b3CSGShape3::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_ANGLE;
	info->yInfo.unit   = B3_STENCIL_LENGTH;
	info->xInfo.factor = 360;
	info->yInfo.factor = b3Vector::b3Length(&m_Dir3);;
}

b3_count b3CSGShape3::b3GetMaxIntersections()
{
	return 2;
}
