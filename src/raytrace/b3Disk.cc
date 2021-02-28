/*
**
**	$Filename:	b3Disk.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing disk
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

b3Disk::b3Disk(b3_u32 class_type) : b3Shape2(sizeof(b3Disk), class_type)
{
}

b3Disk::b3Disk(b3_u32 * src) : b3Shape2(src)
{
}

void b3Disk::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	vertCount = b3ShapeRenderContext::m_SubDiv * 2 + 6;
	gridCount = b3ShapeRenderContext::m_SubDiv * 2 + 6;
	polyCount = b3ShapeRenderContext::m_SubDiv * 2 + 4;
}

void b3Disk::b3ComputeVertices()
{
	b3_gl_vertex * Vector      = *glVertexElements;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_f64        sx, sy, b, a, h, start, end;
	b3_index      i;
	b3_count      iMax;
	b3_count      vertex_count = 0;

	h = m_Limit.y2;
	b = m_Limit.y1;

	start = m_Limit.x1 * SinCosSteps;
	end   = m_Limit.x2 * SinCosSteps;
	i     = (b3_index)ceil(start);
	iMax  = (b3_count)floor(end);
	xSize = 0;
	ySize = 1;

	// This is a ring formed disk
	if ((i - start) > b3Scene::epsilon)
	{
		//compute front fractional disk ring
		a  = m_Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		Vector->t.s = 0;
		Vector->t.t = 0;
		Vector->v.x = m_Base.x + h * sx * m_Dir1.x + h * sy * m_Dir2.x;
		Vector->v.y = m_Base.y + h * sx * m_Dir1.y + h * sy * m_Dir2.y;
		Vector->v.z = m_Base.z + h * sx * m_Dir1.z + h * sy * m_Dir2.z;
		Vector++;

		Vector->t.s = 0;
		Vector->t.t = 1;
		Vector->v.x = m_Base.x + b * sx * m_Dir1.x + b * sy * m_Dir2.x;
		Vector->v.y = m_Base.y + b * sx * m_Dir1.y + b * sy * m_Dir2.y;
		Vector->v.z = m_Base.z + b * sx * m_Dir1.z + b * sy * m_Dir2.z;
		Vector++;

		vertex_count += 2;
		xSize++;
	}

	for (; i <= iMax; i++)
	{
		b3_f64 pos = (double)i / SinCosSteps;
		b3_f64 s   = pos / (m_Limit.x2 - m_Limit.x1) - m_Limit.x1;

		// compute ordered position of ring disk
		sx = h * b3ShapeRenderContext::m_Cos[i % SinCosSteps];
		sy = h * b3ShapeRenderContext::m_Sin[i % SinCosSteps];
		Vector->t.s = s;
		Vector->t.t = 0;
		Vector->v.x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
		Vector->v.y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
		Vector->v.z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;
		Vector++;

		sx = b * b3ShapeRenderContext::m_Cos[i % SinCosSteps];
		sy = b * b3ShapeRenderContext::m_Sin[i % SinCosSteps];
		Vector->t.s = s;
		Vector->t.t = 1;
		Vector->v.x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
		Vector->v.y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
		Vector->v.z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;
		Vector++;

		vertex_count += 2;
		xSize++;
	}

	if ((end - iMax) > b3Scene::epsilon)
	{
		// compute rest fractional ring disk
		a  = m_Limit.x2 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		Vector->t.s = 1;
		Vector->t.t = 0;
		Vector->v.x = m_Base.x + h * sx * m_Dir1.x + h * sy * m_Dir2.x;
		Vector->v.y = m_Base.y + h * sx * m_Dir1.y + h * sy * m_Dir2.y;
		Vector->v.z = m_Base.z + h * sx * m_Dir1.z + h * sy * m_Dir2.z;
		Vector++;

		Vector->t.s = 1;
		Vector->t.t = 1;
		Vector->v.x = m_Base.x + b * sx * m_Dir1.x + b * sy * m_Dir2.x;
		Vector->v.y = m_Base.y + b * sx * m_Dir1.y + b * sy * m_Dir2.y;
		Vector->v.z = m_Base.z + b * sx * m_Dir1.z + b * sy * m_Dir2.z;

		vertex_count += 2;
		xSize++;
	}

	glVertexElements->b3SetCount(vertex_count);
}

void b3Disk::b3ComputeIndices()
{
	b3_gl_line  *  gPtr;
	b3_gl_polygon * pPtr;
	b3_bool        EndLines = false;
	b3_index       i, pos;
	b3_count       Overhead;
	b3_count       grid_count = 0;
	b3_count       poly_count = 0;

	b3ComputeBound(&m_Limit);
	Overhead = b3GetIndexOverhead(0.0, 0.0);
	if (Overhead < 0)
	{
		EndLines = true;
		Overhead = -Overhead;
	}

	gPtr = *glGridElements;
	pPtr = *glPolygonElements;

	for (i = pos = 0; i < Overhead; i++)
	{
		B3_GL_LINIT(gPtr, pos,  pos + 2);
		B3_GL_LINIT(gPtr, pos + 1, pos + 3);

		B3_GL_PINIT(pPtr, pos,  pos + 2, pos + 1);
		B3_GL_PINIT(pPtr, pos + 3, pos + 1, pos + 2);

		grid_count += 2;
		poly_count += 2;
		pos += 2;
	}

	if (EndLines)
	{
		B3_GL_LINIT(gPtr, 0, 1);
		B3_GL_LINIT(gPtr, Overhead + Overhead, Overhead + Overhead + 1);

		grid_count += 2;
	}

	glGridElements->b3SetCount(grid_count);
	glPolygonElements->b3SetCount(poly_count);
}

void b3Disk::b3GetStencilBoundInfo(b3_stencil_bound * info)
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_ANGLE;
	info->yInfo.unit   = B3_STENCIL_LENGTH;
	info->xInfo.factor = 360;
	info->yInfo.factor = (b3Vector::b3Length(&m_Dir1) + b3Vector::b3Length(&m_Dir2)) * 0.5;
}
