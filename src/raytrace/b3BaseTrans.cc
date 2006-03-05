/*
**
**	$Filename:	b3BaseTrans.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Performing base transformation
**
**	(C) Copyright 2005  Steffen A. Mork
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 21:22:35  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2005/12/04 16:48:40  sm
**	- Converted some defines into inlined methods.
**	
**	Revision 1.7  2005/06/09 19:07:24  sm
**	- Corrected TIFF files
**	- Enabled SSE optimization
**	
**	Revision 1.6  2005/06/08 11:18:44  smork
**	- Some more optimization.
**	
**	Revision 1.5  2005/06/08 11:09:05  smork
**	- Base transformation optimized.
**	
**	Revision 1.4  2005/06/07 14:30:47  smork
**	- Beautifying.
**	
**	Revision 1.3  2005/06/06 19:56:22  sm
**	- Some optimizations.
**	
**	Revision 1.2  2005/06/01 12:28:55  smork
**	- Removed some floating point operations.
**	
**	Revision 1.1  2005/04/27 13:55:01  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        basis transformation class                    **
**                                                                      **
*************************************************************************/

b3_bool b3BaseTransformation::b3Prepare()
{
	b3_f64  denom;
	b3_bool is_zero_volume;

	denom = b3Matrix::b3Det3(&m_Dir1,&m_Dir2,&m_Dir3);
	is_zero_volume = denom == 0;
	if (!is_zero_volume)
	{
		m_Denom = 1.0 / denom;

#ifdef B3_SSE
		b3Vector::b3CrossProduct(&m_Dir2, &m_Dir3, &m_Normals[0]);
		b3Vector::b3CrossProduct(&m_Dir3, &m_Dir1, &m_Normals[1]);
		b3Vector::b3CrossProduct(&m_Dir1, &m_Dir2, &m_Normals[2]);

		b3Vector::b3Scale(&m_Normals[0], m_Denom);
		b3Vector::b3Scale(&m_Normals[1], m_Denom);
		b3Vector::b3Scale(&m_Normals[2], m_Denom);
#else
		m_Normals[0].x = (m_Dir2.y * m_Dir3.z - m_Dir2.z * m_Dir3.y) / denom;
		m_Normals[0].y = (m_Dir2.z * m_Dir3.x - m_Dir2.x * m_Dir3.z) / denom;
		m_Normals[0].z = (m_Dir2.x * m_Dir3.y - m_Dir2.y * m_Dir3.x) / denom;

		m_Normals[1].x = (m_Dir3.y * m_Dir1.z - m_Dir3.z * m_Dir1.y) / denom;
		m_Normals[1].y = (m_Dir3.z * m_Dir1.x - m_Dir3.x * m_Dir1.z) / denom;
		m_Normals[1].z = (m_Dir3.x * m_Dir1.y - m_Dir3.y * m_Dir1.x) / denom;

		m_Normals[2].x = (m_Dir1.y * m_Dir2.z - m_Dir1.z * m_Dir2.y) / denom;
		m_Normals[2].y = (m_Dir1.z * m_Dir2.x - m_Dir1.x * m_Dir2.z) / denom;
		m_Normals[2].z = (m_Dir1.x * m_Dir2.y - m_Dir1.y * m_Dir2.x) / denom;
#endif
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"A quadric has zero volume!\n");
		m_Denom = 0;
	}

	m_DirLen[0] = b3Vector::b3QuadLength(&m_Dir1);
	m_DirLen[1] = b3Vector::b3QuadLength(&m_Dir2);
	m_DirLen[2] = b3Vector::b3QuadLength(&m_Dir3);

	return !is_zero_volume;
}

void b3BaseTransformation::b3BaseTransform(
	const b3_line64 *in,
	      b3_line64 *out)
{
#ifdef B3_SSE2
	b3_vector64         pos;
	b3_f64 B3_ALIGN_16 *op = &out->pos.x;
	b3_f64 B3_ALIGN_16 *od = &out->dir.x;

	pos.x = in->pos.x - m_Base.x;
	pos.y = in->pos.y - m_Base.y;
	pos.z = in->pos.z - m_Base.z;

	for(b3_loop o = 0;o < 3;o++)
	{
		op[o] = b3Vector::b3SMul(&pos,     &m_Normals[o]);
		od[o] = b3Vector::b3SMul(&in->dir, &m_Normals[o]);
	}
#else
	b3_f64 xPos,yPos,zPos;
	b3_f64 xDir,yDir,zDir;

	xPos = in->pos.x - m_Base.x;
	yPos = in->pos.y - m_Base.y;
	zPos = in->pos.z - m_Base.z;
	xDir = in->dir.x;
	yDir = in->dir.y;
	zDir = in->dir.z;

	out->pos.x =
		xPos * m_Normals[0].x +
		yPos * m_Normals[0].y +
		zPos * m_Normals[0].z;
	out->pos.y =
		xPos * m_Normals[1].x +
		yPos * m_Normals[1].y +
		zPos * m_Normals[1].z;
	out->pos.z =
		xPos * m_Normals[2].x +
		yPos * m_Normals[2].y +
		zPos * m_Normals[2].z;

	out->dir.x =
		xDir * m_Normals[0].x +
		yDir * m_Normals[0].y +
		zDir * m_Normals[0].z;
	out->dir.y =
		xDir * m_Normals[1].x +
		yDir * m_Normals[1].y +
		zDir * m_Normals[1].z;
	out->dir.z =
		xDir * m_Normals[2].x +
		yDir * m_Normals[2].y +
		zDir * m_Normals[2].z;
#endif
}


void b3BaseTransformation::b3BaseTransform(
	const b3_vector *in,
	      b3_vector *out)
{
#ifdef B3_SSE2
	b3_vector64            pos;
	b3_f32    B3_ALIGN_16 *o = &out->x;

	pos.x = in->x - m_Base.x;
	pos.y = in->y - m_Base.y;
	pos.z = in->z - m_Base.z;

	for(b3_loop i = 0;i < 3;i++)
	{
		o[i] = b3Vector::b3SMul(&pos, &m_Normals[i]);
	}
#else
	b3_f64 xPos,yPos,zPos;

	xPos = in->x - m_Base.x;
	yPos = in->y - m_Base.y;
	zPos = in->z - m_Base.z;

	out->x =
		xPos * m_Normals[0].x +
		yPos * m_Normals[0].y +
		zPos * m_Normals[0].z;
	out->y =
		xPos * m_Normals[1].x +
		yPos * m_Normals[1].y +
		zPos * m_Normals[1].z;
	out->z =
		xPos * m_Normals[2].x +
		yPos * m_Normals[2].y +
		zPos * m_Normals[2].z;
#endif
}


void b3BaseTransformation::b3Project(
	const b3_vector *in,
	      b3_vector *out)
{
	b3_f64 xPos,yPos,zPos;

	xPos = in->x - m_Base.x;
	yPos = in->y - m_Base.y;
	zPos = in->z - m_Base.z;

	out->z =
		xPos * m_Normals[2].x +
		yPos * m_Normals[2].y +
		zPos * m_Normals[2].z;
	out->x = (
		xPos * m_Normals[0].x +
		yPos * m_Normals[0].y +
		zPos * m_Normals[0].z) / out->z;
	out->y = (
		xPos * m_Normals[1].x +
		yPos * m_Normals[1].y +
		zPos * m_Normals[1].z) / out->z;
}

