/*
**
**      $Filename:      b3Disk.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing disk
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
**      Revision 1.12  2001/09/22 16:19:52  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.11  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.10  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.9  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.8  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.7  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.6  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.5  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.4  2001/08/09 15:27:34  sm
**      - Following shapes are newly supported now:
**        o disk
**        o cylinder
**        o cone
**        o ellipsoid
**        o torus
**        o triangles
**      - Done some makefile fixes
**      - Everything is Windozable
**
**      Revision 1.3  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.2  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.1  2001/08/06 15:26:00  sm
**      - Splitted shapes into their own files
**      - Some preparations for shapde drawing.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Disk::b3Disk(b3_u32 class_type) : b3Shape2(sizeof(b3Disk),class_type)
{
}

b3Disk::b3Disk(b3_u32 *src) : b3Shape2(src)
{
}

void b3Disk::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount   = SinCosSteps + SinCosSteps + 6;
}

void b3Disk::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_f64     sx,sy,b,a,h,start,end;
	b3_index   i;
	b3_count   iMax;

#ifdef BLZ3_USE_OPENGL
	Vector = (b3_vector *)glVertices;
	h = Limit.y2;
	b = Limit.y1;

	start = Limit.x1 * SinCosSteps;
	end   = Limit.x2 * SinCosSteps;
	i     = (b3_index)ceil(start);
	iMax  = (b3_count)floor(end);
	xSize = 0;
	ySize = 1;

	glVertexCount = 0;
	if (b > 0)
	{
		ySize++;
		// This is a ring formed disk
		if ((i - start) > Epsilon)
		{
			//compute front fractional disk ring
			a  = Limit.x1 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = m_Base.x + h * sx * m_Dir1.x + h * sy * m_Dir2.x;
			Vector->y = m_Base.y + h * sx * m_Dir1.y + h * sy * m_Dir2.y;
			Vector->z = m_Base.z + h * sx * m_Dir1.z + h * sy * m_Dir2.z;
			Vector++;

			Vector->x = m_Base.x + b * sx * m_Dir1.x + b * sy * m_Dir2.x;
			Vector->y = m_Base.y + b * sx * m_Dir1.y + b * sy * m_Dir2.y;
			Vector->z = m_Base.z + b * sx * m_Dir1.z + b * sy * m_Dir2.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		for (;i<=iMax;i++)
		{
			// compute ordered position of ring disk
			sx = h * Cos[i % SinCosSteps];
			sy = h * Sin[i % SinCosSteps];
			Vector->x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
			Vector->y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
			Vector->z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;
			Vector++;

			sx = b * Cos[i % SinCosSteps];
			sy = b * Sin[i % SinCosSteps];
			Vector->x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
			Vector->y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
			Vector->z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			// compute rest fractional ring disk
			a  = Limit.x2 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = m_Base.x + h * sx * m_Dir1.x + h * sy * m_Dir2.x;
			Vector->y = m_Base.y + h * sx * m_Dir1.y + h * sy * m_Dir2.y;
			Vector->z = m_Base.z + h * sx * m_Dir1.z + h * sy * m_Dir2.z;
			Vector++;

			Vector->x = m_Base.x + b * sx * m_Dir1.x + b * sy * m_Dir2.x;
			Vector->y = m_Base.y + b * sx * m_Dir1.y + b * sy * m_Dir2.y;
			Vector->z = m_Base.z + b * sx * m_Dir1.z + b * sy * m_Dir2.z;

			glVertexCount += 2;
			xSize++;
		}
	}
	else
	{
		// Position center first
		*Vector++ = m_Base;
		glVertexCount++;

		// First fractional disk part if any
		if ((i - start) > Epsilon)
		{
			a  = Limit.x1 * M_PI * 2;
			sx = h * cos(a);
			sy = h * sin(a);

			Vector->x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
			Vector->y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
			Vector->z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;
			Vector++;

			glVertexCount++;
			xSize++;
		}

		// ordered parts of disk
		for (;i<=iMax;i++)
		{
			sx = h * Cos[i % SinCosSteps];
			sy = h * Sin[i % SinCosSteps];
			Vector->x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
			Vector->y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
			Vector->z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;
			Vector++;

			glVertexCount++;
			xSize++;
		}

		// Last part as fracvtional disk
		if ((end - iMax) > Epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = h * cos(a);
			sy = h * sin(a);

			Vector->x = m_Base.x + sx * m_Dir1.x + sy * m_Dir2.x;
			Vector->y = m_Base.y + sx * m_Dir1.y + sy * m_Dir2.y;
			Vector->z = m_Base.z + sx * m_Dir1.z + sy * m_Dir2.z;

			glVertexCount++;
			xSize++;
		}
	}
#endif
}

void b3Disk::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_bool   EndLines = false;
	b3_index  i,Number = 0;
	b3_count  Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		EndLines = true;
		Overhead = -Overhead;
		Number   = 2;
	}
	Number += Overhead;
	if (Limit.y1 > 0) Number += Overhead;

	glGridCount = 0;
	glPolyCount = 0;
	b3RenderObject::b3Free(glGrids);
	b3RenderObject::b3Free(glPolygons);
	glGrids    = gPtr = (GLushort *)b3RenderObject::b3Alloc
		(Number * 2 * sizeof(GLushort));
	glPolygons = pPtr = (GLushort *)b3RenderObject::b3Alloc
		(Number * 3 * sizeof(GLushort));
	if ((gPtr == null) || (pPtr == null))
	{
		return;
	}

	if (Limit.y1 > 0)
	{
		b3_index pos;

		for (i = 0;i < Overhead;i++)
		{
			pos = i + i;
			*gPtr++ = pos;
			*gPtr++ = pos + 2;

			*gPtr++ = pos + 1;
			*gPtr++ = pos + 3;

			*pPtr++ = pos;
			*pPtr++ = pos + 2;
			*pPtr++ = pos + 1;

			*pPtr++ = pos + 3;
			*pPtr++ = pos + 1;
			*pPtr++ = pos + 2;

			glGridCount += 2;
			glPolyCount += 2;
		}
		if (EndLines)
		{
			*gPtr++ = 0;
			*gPtr++ = 1;

			*gPtr++ = Overhead + Overhead;
			*gPtr++ = Overhead + Overhead + 1;

			glGridCount += 2;
		}
	}
	else
	{
		for (i = 1;i <= Overhead;i++)
		{
			*gPtr++ = i;
			*gPtr++ = i + 1;

			*pPtr++ = i;
			*pPtr++ = i + 1;
			*pPtr++ = 0;

			glGridCount++;
			glPolyCount++;
		}
		if (EndLines)
		{
			*gPtr++ = 0;
			*gPtr++ = 1;

			*gPtr++ = 0;
			*gPtr++ = Overhead + 1;

			glGridCount += 2;
		}
	}
#endif
}
