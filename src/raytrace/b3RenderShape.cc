/*
**
**      $Filename:      b3Shape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing shapes
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
**      $Log$
**      Revision 1.18  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.17  2001/10/10 17:52:24  sm
**      - Texture loading (only reading into memory) running.
**      - Raytracing without OpenGL must be possible!
**
**      Revision 1.16  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.15  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.14  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
**      Revision 1.13  2001/08/20 19:35:08  sm
**      - Index correction introduced (This is a hack!)
**      - Some OpenGL cleanups
**
**      Revision 1.12  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.11  2001/08/17 19:28:54  sm
**      - Now able to draw every shape by lines or shaded.
**        It's great!
**
**      Revision 1.10  2001/08/17 14:08:34  sm
**      - Now trying to draw BSPline surfaces with own routines.
**
**      Revision 1.9  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.8  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.7  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
**
**      Revision 1.6  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.5  2001/08/14 13:34:40  sm
**      - Corredted aspect ratio handling when doing somethiing with
**        the view
**      - New application icon
**      - Minor GUI bug fixes.
**
**      Revision 1.4  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.3  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.2  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.1  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.10  2001/08/09 15:27:34  sm
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
**      Revision 1.9  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.8  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.7  2001/08/06 20:49:39  sm
**      - Some conflicts solved
**
**      Revision 1.6  2001/08/06 19:58:59  sm
**      - Drawing area - the first shape we can see with OpenGL
**
**      Revision 1.5  2001/08/06 16:35:35  sm
**      - Drawing first area
**
**      Revision 1.4  2001/08/06 15:26:00  sm
**      - Splitted shapes into their own files
**      - Some preparations for shapde drawing.
**
**      Revision 1.3  2001/07/08 12:30:06  sm
**      - New tool to remove nasty CR/LF from Windoze.
**      - Removing some nasty CR/LF with that new tool.
**
**      Revision 1.2  2001/07/01 16:31:52  sm
**      - Creating MSVC Projects
**      - Welcome to Windows 32
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

#ifdef BLZ3_USE_OPENGL
static GLushort box_grids[] =
{
	0,1,
	1,2,
	2,3,
	3,0,
	7,6,
	6,5,
	5,4,
	4,7,
	0,7,
	1,6,
	2,5,
	3,4
};

static GLushort box_polygons[] =
{
	 6, 7, 5, // top
	 4, 5, 7,
	 0, 1, 3, // bottom
	 2, 3, 1,
	13,12,10, // back
	11,10,12,
	 9, 8,14, // front
	15,14, 8,
	16,19,23, // right
	20,23,19,
	18,17,21, // left
	22,21,17
};
#endif

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderShapeContext::b3RenderShapeContext(b3_count new_subdiv)
{
#ifdef BLZ3_USE_OPENGL
	CylinderIndices  = null;
	CylinderPolygons = null;
	ConeIndices      = null;
	ConePolygons     = null;
#endif
	Between         = null;
	b3InitSubdiv(new_subdiv);
}

void b3RenderShapeContext::b3InitSubdiv(b3_count new_subdiv)
{
	b3_f64   aux;
	b3_index i;

	subdiv = (new_subdiv > B3_MAX_RENDER_SUBDIV ? B3_MAX_RENDER_SUBDIV : new_subdiv);
	if (subdiv < 8)
	{
		subdiv = 8;
	}

	for (i = 0;i <= subdiv;i++)
	{
		aux    = i * M_PI * 2 / subdiv;
		Sin[i] = sin(aux);
		Cos[i] = cos(aux);
	}

#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_index  a;

	if (CylinderIndices != null)
	{
		b3Free(CylinderIndices);
	}
	if (CylinderPolygons != null)
	{
		b3Free(CylinderPolygons);
	}
	if (ConeIndices != null)
	{
		b3Free(ConeIndices);
	}
	if (ConePolygons != null)
	{
		b3Free(ConePolygons);
	}

	CylinderIndices  = (GLushort *)b3Alloc
			((subdiv + 1) * 3 * 2 * sizeof(GLushort));
	CylinderPolygons = (GLushort *)b3Alloc
			((subdiv + 1) * 2 * 3 * sizeof(GLushort));
	if ((CylinderIndices != null) && (CylinderPolygons != null))
	{
		gPtr = CylinderIndices;
		pPtr = CylinderPolygons;
		a = 0;
		for (i = 0;i <= subdiv;i++)
		{
			// Concatinate vertices in this ascending order:
			//
			//   (c)
			// 1-----3
			// |
			// |(a)
			// |
			// 0-----2
			//   (b)

			// (a)
			*gPtr++ = a;
			*gPtr++ = a + 1;

			// (b)
			*gPtr++ = a;
			*gPtr++ = a + 2;

			// (c)
			*gPtr++ = a + 1;
			*gPtr++ = a + 3;

			*pPtr++ = a;
			*pPtr++ = a + 2;
			*pPtr++ = a + 1;

			*pPtr++ = a + 3;
			*pPtr++ = a + 1;
			*pPtr++ = a + 2;

			a += 2;
		}
	}

	ConeIndices  = (GLushort *)b3Alloc
		((subdiv + 1) * 2 * 2 * sizeof(GLushort));
	ConePolygons = (GLushort *)b3Alloc
		((subdiv + 1) * 1 * 3 * sizeof(GLushort));
	if ((ConeIndices != null) && (ConePolygons != null))
	{
		gPtr = ConeIndices;
		pPtr = ConePolygons;
		for (i = 0;i <= subdiv;i++)
		{
			*gPtr++ = 0;
			*gPtr++ = i+1;

			*gPtr++ = i+1;
			*gPtr++ = i+2;

			*pPtr++ = i+1;
			*pPtr++ = i+2;
			*pPtr++ =   0;
		}
	}
#endif
}

b3_count b3RenderShapeContext::b3GetSubdiv()
{
	return subdiv;
}

b3_f64 *b3RenderShapeContext::b3GetSinTable()
{
	return Sin;
}

b3_f64 *b3RenderShapeContext::b3GetCosTable()
{
	return Cos;
}

b3_vector *b3RenderShapeContext::b3GetSplineAux()
{
	b3_count factor;

	if (Between == null)
	{
		factor  = B3_MAX(B3_MAX_CONTROLS,B3_MAX_SUBDIV) + 1;
		Between = (b3_vector *)b3Alloc(factor * factor * sizeof(b3_vector));
	}
	return Between;
}

#ifdef BLZ3_USE_OPENGL
GLushort *b3RenderShapeContext::b3GetCylinderIndices()
{
	return CylinderIndices;
}

GLushort *b3RenderShapeContext::b3GetCylinderPolygons()
{
	return CylinderPolygons;
}

GLushort *b3RenderShapeContext::b3GetConeIndices()
{
	return ConeIndices;
}

GLushort *b3RenderShapeContext::b3GetConePolygons()
{
	return ConePolygons;
}
#endif


/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderShapeObject::b3RenderShapeObject()
{
	Epsilon     = 0.001;
	Between     = null;
	m_Activated = false;
}

b3_count b3RenderShapeObject::b3GetIndexOverhead (
	b3_f64 xLeft,
	b3_f64 yLeft)
{
	b3_count Overhead;
	b3_index xs,xe;
	b3_f64   x1,x2;

	if  (Limit.x1 < xLeft) Limit.x1 = xLeft;
	if  (Limit.y1 < yLeft) Limit.y1 = yLeft;
	x1 = Limit.x1 * SinCosSteps;
	x2 = Limit.x2 * SinCosSteps;
	xs = (b3_index)ceil(x1);
	xe = (b3_index)floor(x2);
	Overhead = xe - xs;
	if ((xs - x1) > Epsilon) Overhead++;
	if ((x2 - xe) > Epsilon) Overhead++;

	return ((xs > 0) || (xe < SinCosSteps)) ? -Overhead : Overhead;
}

void b3RenderShapeObject::b3Activate(b3_bool activate)
{
	m_Activated = activate;
}
											
b3_bool b3RenderShapeObject::b3IsActivated()
{
	return m_Activated;
}

b3_render_mode b3RenderShapeObject::b3GetRenderMode()
{
	return m_Activated ? B3_RENDER_FILLED : B3_RENDER_LINE;
}

void b3RenderShapeObject::b3GetGridColor(b3_color *color)
{
	color->r = 0.2f;
	color->g = 0.2f;
	color->b = 0.2f;
	color->a = 0.0f;
}

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderShape::b3RenderShape(b3_size class_size,b3_u32 class_type) : b3Shape(class_size, class_type)
{
}

b3RenderShape::b3RenderShape(b3_u32 class_type) : b3Shape(sizeof(b3RenderShape), class_type)
{
}

b3RenderShape::b3RenderShape(b3_u32 *src) : b3Shape(src)
{
}

#ifdef BLZ3_USE_OPENGL
b3_index b3RenderShape::b3FindVertex(GLushort vertex)
{
	b3_vector *point;
	b3_vector *ptr = (b3_vector *)glVertices;
	b3_index   i;

	point = &ptr[vertex];
	for (i = 0;i < glVertexCount;i++)
	{
		if (b3Vector::b3Distance(point,ptr) < Epsilon)
		{
			return i;
		}
		ptr++;
	}

	B3_ASSERT(false);
	return vertex;
}
#endif

void b3RenderShape::b3CorrectIndices()
{
#ifdef BLZ3_USE_OPENGL
	b3_index  i;
	GLushort *pPtr = glPolygons;

	for (i = 0;i < glPolyCount;i++)
	{
		pPtr[0] = b3FindVertex(pPtr[0]);
		pPtr[1] = b3FindVertex(pPtr[1]);
		pPtr[2] = b3FindVertex(pPtr[2]);

		pPtr += 3;
	}
#endif
}

void b3RenderShape::b3ComputeSphereVertices(
	b3_vector   &Base,
	b3_vector   &Dir)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_index   i,j;
	b3_count   Circles;
	b3_f64     cx,sx,cy,sy,a;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32     Rad;
	b3_vector  Aux,Dir1,Dir2,Dir3;

	Vector = (b3_vector *)glVertices;
	Aux    = Base;
	Dir1.x = Rad = b3Vector::b3Length (&Dir);
	Dir1.y = 0;
	Dir1.z = 0;
	Dir2.x = 0;
	Dir2.y = Rad;
	Dir2.z = 0;
	Dir3.x = 0;
	Dir3.y = 0;
	Dir3.z = Rad;
	Circles = (SinCosSteps >> 1) + 1;
	xSize	 = SinCosSteps + 1;
	ySize  = Circles;

	a = 2.0/SinCosSteps;
	for (j =0 ;j < Circles;j++)
	{
		LocalCos[j] = Rad = -cos(j * a * M_PI);
		LocalSin[j] = sqrt(1.0-Rad*Rad);
	}

	for (i = 0;i <= SinCosSteps;i++)
	{
		cx = Cos[i];
		sx = Sin[i];
		for (j=0;j<Circles;j++)
		{
			cy = LocalCos[j];
			sy = LocalSin[j];

			Vector->x = Base.x + cx * sy * Dir1.x + sx * sy * Dir2.x + cy * Dir3.x;
			Vector->y = Base.y + cx * sy * Dir1.y + sx * sy * Dir2.y + cy * Dir3.y;
			Vector->z = Base.z + cx * sy * Dir1.z + sx * sy * Dir2.z + cy * Dir3.z;
			Vector++;
		}
	}
	b3CorrectIndices();
	/*
		PrintF ("\n");
		PrintF ("Points: %3ld\n",Points);
		PrintF ("Circles:%3ld\n",Circles);
	*/
#endif
 }

void b3RenderShape::b3ComputeCylinderVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     sx,sy,b,h,start,end;
	b3_index   i;
	b3_count   iMax;
	b3_vector  Bottom;

	Vector = (b3_vector *)glVertices;
	h      = Limit.y2 - Limit.y1;
	b      = Limit.y1;

	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;
	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = 2;
	glVertexCount = 0;

	if ((i - start) > Epsilon)
	{
		b = Limit.x1 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx = Cos[i % SinCosSteps];
		sy = Sin[i % SinCosSteps];

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		b = Limit.x2 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		xSize++;
	}

	b3CorrectIndices();
#endif
}

void b3RenderShape::b3ComputeCylinderIndices()
{
#ifdef BLZ3_USE_OPENGL
	b3_count Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		glGridCount = 1;
		Overhead = -Overhead;
	}
	else
	{
		glGridCount = 0;
	}
	glGridCount += Overhead * 3;
	glPolyCount  = Overhead * 2;
#endif
}

void b3RenderShape::b3ComputeConeVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     sx,sy,b,a,h,start,end;
	b3_index   i;
	b3_count   iMax;
	b3_vector  Bottom;

	Vector   = (b3_vector *)glVertices;

	h        = Limit.y2 - Limit.y1;
	b        = Limit.y1;
	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = 1;
	glVertexCount = 0;

	if (Limit.y2 < 1)
	{
		ySize++;
		if ((i - start) > Epsilon)
		{
			a = Limit.x1 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = Bottom.x + (1-b) * sx * Dir1.x + (1-b) * sy * Dir2.x;
			Vector->y = Bottom.y + (1-b) * sx * Dir1.y + (1-b) * sy * Dir2.y;
			Vector->z = Bottom.z + (1-b) * sx * Dir1.z + (1-b) * sy * Dir2.z;
			Vector++;

			Vector->x = Bottom.x + (1-h) * sx * Dir1.x + (1-h) * sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + (1-h) * sx * Dir1.y + (1-h) * sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + (1-h) * sx * Dir1.z + (1-h) * sy * Dir2.z + h * Dir3.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		for (;i <= iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * Cos[i % SinCosSteps];
			sy = (1-h) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = Bottom.x + (1-b) * sx * Dir1.x + (1-b) * sy * Dir2.x;
			Vector->y = Bottom.y + (1-b) * sx * Dir1.y + (1-b) * sy * Dir2.y;
			Vector->z = Bottom.z + (1-b) * sx * Dir1.z + (1-b) * sy * Dir2.z;
			Vector++;

			Vector->x = Bottom.x + (1-h) * sx * Dir1.x + (1-h) * sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + (1-h) * sx * Dir1.y + (1-h) * sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + (1-h) * sx * Dir1.z + (1-h) * sy * Dir2.z + h * Dir3.z;

			glVertexCount += 2;
			xSize++;
		}
	}
	else
	{
		Vector->x = Base.x + Dir3.x;
		Vector->y = Base.y + Dir3.y;
		Vector->z = Base.z + Dir3.z;
		Vector++;
		glVertexCount++;

		if ((i - start) > Epsilon)
		{
			a  = Limit.x1 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			glVertexCount++;
			xSize++;
		}

		for (;i <= iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			glVertexCount++;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;

			glVertexCount++;
			xSize++;
		}
	}
#endif
}

void b3RenderShape::b3ComputeConeIndices()
{
#ifdef BLZ3_USE_OPENGL
	b3_count Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		glGridCount = 1;
		Overhead  = -Overhead;
	}
	else
	{
		glGridCount = 0;
	}
	if (Limit.y2 < 1)
	{
		glGrids    = GridsCyl;
		glPolygons = PolysCyl;
		glGridCount += Overhead * 3;
		glPolyCount  = Overhead * 2;
	}
	else
	{
		glGrids    = GridsCone;
		glPolygons = PolysCone;
		glGridCount += Overhead * 2;
		glPolyCount  = Overhead;
	}
#endif
}

void b3RenderShape::b3ComputeEllipsoidVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_index   i,j;
	b3_count   iMax,Circles = 0;
	b3_f64     RadX,RadY,sx,sy;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32     start,end,a;

	Vector = (b3_vector *)glVertices;
	start  = (Limit.y1 + 1) * SinCosSteps * 0.25;
	end    = (Limit.y2 + 1) * SinCosSteps * 0.25;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > Epsilon)	/* underflow */
	{
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 4.0 / SinCosSteps;
	for (j=i;j<=iMax;j++)		/* integers */
	{
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > Epsilon)	/* Overflow */
	{
		LocalSin[Circles] = Limit.y2;
		Circles++;
	}

	for (j=0;j<Circles;j++)		/* Calculate Values */
	{
		LocalSin[j] = a = sin(LocalSin[j] * M_PI * 0.5);
		LocalCos[j] = sqrt(1.0-a*a);
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = Circles;
	glVertexCount = 0;

	if ((i - start) > Epsilon)
	{
		a  = Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx = Cos[i];
		sy = Sin[i];

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		a  = Limit.x2 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	b3CorrectIndices();
#endif
}

void b3RenderShape::b3ComputeEllipsoidIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_bool   EndLine = false;
	b3_index  i,j,Number,s,ys,ye;
	b3_count  Heights,Widths,Overhead;
	b3_f64    y1,y2;

	glGridCount = 0;
	glPolyCount = 0;
	b3ComputeBound(&Limit);
	Overhead  = b3GetIndexOverhead (0.0,-1.0);
	if (Overhead < 0)
	{
		EndLine  = true;
		Overhead = -Overhead;
	}

	y1 = (Limit.y1 + 1) * SinCosSteps * 0.25;
	y2 = (Limit.y2 + 1) * SinCosSteps * 0.25;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	Heights = ye - ys;
	if ((ys - y1) > Epsilon) Heights++;
	if ((y2 - ye) > Epsilon) Heights++;

	Widths = Heights - 1;
	if ((SinCosSteps * 0.5 - y2) > Epsilon) Widths++;
	if (                     y1  > Epsilon) Widths++;

	if (EndLine) Number = (Widths + Heights + 1) * Overhead + Heights;
	else         Number = (Widths + Heights + 1) * Overhead;
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

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + 1;
		}
		glGridCount += Heights;

		if (y1 <= Epsilon) j = 1;
		else               j = 0;
		while (j < Heights)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + Heights + 1;

			glGridCount++;

			*pPtr++ = s + j;
			*pPtr++ = s + j + 1;
			*pPtr++ = s + j + Heights + 1;

			*pPtr++ = s + j + Heights + 2;
			*pPtr++ = s + j + Heights + 1;
			*pPtr++ = s + j + 1;

			glPolyCount += 2;
			j++;
		}
		if ((SinCosSteps * 0.5 - y2) > Epsilon)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + Heights + 1;
			glGridCount++;
			j++;
		}
		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + 1;
		}
		glGridCount += Heights;
	}

	B3_ASSERT(glGridCount <= Number);
#endif
}

void b3RenderShape::b3ComputeBoxVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_vector  Aux;
	b3_index   i;

	Vector = (b3_vector *)glVertices;

	*Vector = Aux = Base;

	Vector[1].x = (Aux.x += Dir1.x);
	Vector[1].y = (Aux.y += Dir1.y);
	Vector[1].z = (Aux.z += Dir1.z);

	Vector[2].x = (Aux.x += Dir2.x);
	Vector[2].y = (Aux.y += Dir2.y);
	Vector[2].z = (Aux.z += Dir2.z);

	Vector[3].x = (Aux.x -= Dir1.x);
	Vector[3].y = (Aux.y -= Dir1.y);
	Vector[3].z = (Aux.z -= Dir1.z);

	Vector[4].x = (Aux.x += Dir3.x);
	Vector[4].y = (Aux.y += Dir3.y);
	Vector[4].z = (Aux.z += Dir3.z);

	Vector[5].x = (Aux.x += Dir1.x);
	Vector[5].y = (Aux.y += Dir1.y);
	Vector[5].z = (Aux.z += Dir1.z);

	Vector[6].x = (Aux.x -= Dir2.x);
	Vector[6].y = (Aux.y -= Dir2.y);
	Vector[6].z = (Aux.z -= Dir2.z);

	Vector[7].x = (Aux.x -= Dir1.x);
	Vector[7].y = (Aux.y -= Dir1.y);
	Vector[7].z = (Aux.z -= Dir1.z);

	xSize = ySize = 1;

	for (i = 0;i < 8;i++)
	{
		Vector[i + 8] = Vector[i + 16] = Vector[i];
	}
#endif
}

void b3RenderShape::b3ComputeBoxIndices()
{
#ifdef BLZ3_USE_OPENGL
	glGrids    = box_grids;
	glPolygons = box_polygons;
#endif
}

void b3RenderShape::b3ComputeTorusVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3,
	b3_f64       aRad,
	b3_f64       bRad)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     RadX,RadY,sx,sy,start,end,a;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_index   i,j;
	b3_count   iMax,Circles=0;
	b3_vector  Aux;

	Vector = (b3_vector *)glVertices;
	start  = Limit.y1 * SinCosSteps;
	end    = Limit.y2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > Epsilon)	/* underflow */
	{
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 1.0 / SinCosSteps;
	for (j = i;j <= iMax;j++)		/* integers */
	{
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > Epsilon)	/* Overflow */
	{
		LocalSin[Circles] = Limit.y2;
		Circles++;
	}
	xSize = 0;
	ySize = Circles;
	glVertexCount = 0;

	for (j=0;j<Circles;j++)		/* Calculate Values */
	{
		LocalCos[j] = cos(LocalSin[j] * M_PI * 2) * bRad;
		LocalSin[j] = sin(LocalSin[j] * M_PI * 2) * bRad;
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);

	if ((i - start) > Epsilon)
	{
		a     = Limit.x1 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx    = Cos[i];
		sy    = Sin[i];
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		a     = Limit.x2 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	b3CorrectIndices();
#endif
}

void b3RenderShape::b3ComputeTorusIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_bool   EndLine = false,EndCol = false;
	b3_index  i,j,Number,s,ys,ye;
	b3_count  Heights,Widths,Overhead;
	b3_f64    y1,y2;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		EndLine  =  true;
		Overhead = -Overhead;
	}

	y1 = Limit.y1 * SinCosSteps;
	y2 = Limit.y2 * SinCosSteps;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	Heights = ye - ys;
	if ((ys - y1) > Epsilon) Heights++;
	if ((y2 - ye) > Epsilon) Heights++;
	if ((ys > 0) || (ye < SinCosSteps))
	{
		EndCol = true;
	}

	Widths = Heights;
	if (EndCol) Widths++;

	Number = (Widths + Heights + 1) * Overhead;
	if (EndLine) Number += Heights;

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

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + 1;

			*pPtr++ = s + j;
			*pPtr++ = s + j + 1;
			*pPtr++ = s + j + Heights + 1;

			*pPtr++ = s + j + Heights + 2;
			*pPtr++ = s + j + Heights + 1;
			*pPtr++ = s + j + 1;
		}
		glGridCount += Heights;

		for (j = 0;j < Widths;j++)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + Heights + 1;

			glPolyCount += 2;
		}
		glGridCount += Widths;

		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			*gPtr++ = s + j;
			*gPtr++ = s + j + 1;
		}
		glGridCount += Heights;
	}
#endif
}
