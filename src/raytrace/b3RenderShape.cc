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
**      Revision 1.32  2002/03/10 21:14:40  sm
**      - Fixed rotation shapes with custom subdivision for rotation.
**
**      Revision 1.31  2002/03/10 20:34:18  sm
**      - Cleaned up and tested CB3ShapeDialgo derivates:
**        o Ordered meaning of methods
**        o Made registry entries of stencil creation unique for
**          each shape.
**        o Fixed some bugs.
**
**      Revision 1.30  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.29  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.28  2002/02/27 20:14:52  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.27  2002/02/20 20:23:57  sm
**      - Some type cleanups done.
**
**      Revision 1.26  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.25  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.24  2002/02/17 21:25:07  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.23  2002/01/10 17:31:11  sm
**      - Some minor GUI updates.
**      - b3BBox::b3Transform() changes m_Matrix member.
**      - Added image selection with image preview.
**
**      Revision 1.22  2002/01/06 21:38:18  sm
**      - Nasty Un CR/LF
**      - b3Dir fix. Not tested, yet!
**      - make count/make count_all update
**
**      Revision 1.21  2002/01/06 16:30:47  sm
**      - Added Load/Save/Replace object
**      - Enhanced "New world"
**      - Added some non static methods to b3Dir (Un*x untested, yet!)
**      - Fixed missing sphere/ellipsoid south pole triangles
**      - Fixed Spline cylinder/ring triangle indexing
**
**      Revision 1.20  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.19  2001/10/20 16:14:59  sm
**      - Some runtime environment cleanups. The CPU count is determined
**        only once.
**      - Introduced preparing routines for raytring to shapes.
**      - Found 5% performance loss: No problem, this was eaten by
**        bug fxing of the rotation spline shapes. (Phuu!)
**      - The next job is to implement different row sampler. Then we
**        should implemented the base set of the Blizzard II raytracer.
**
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

b3ShapeRenderContext::b3ShapeRenderContext(b3_count new_subdiv)
{
#ifdef BLZ3_USE_OPENGL
	m_CylinderIndices  = null;
	m_CylinderPolygons = null;
	m_ConeIndices      = null;
	m_ConePolygons     = null;
#endif
	m_Between          = null;
	b3InitSubdiv(new_subdiv);
}

void b3ShapeRenderContext::b3InitSubdiv(b3_count new_subdiv)
{
	b3_f64   aux;
	b3_index i;

	m_SubDiv = (new_subdiv > B3_MAX_RENDER_SUBDIV ? B3_MAX_RENDER_SUBDIV : new_subdiv);
	if (m_SubDiv < 8)
	{
		m_SubDiv = 8;
	}

	for (i = 0;i <= m_SubDiv;i++)
	{
		aux    = i * M_PI * 2 / m_SubDiv;
		m_Sin[i] = sin(aux);
		m_Cos[i] = cos(aux);
	}

#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_index  a;

	b3Free(m_CylinderIndices);
	b3Free(m_CylinderPolygons);
	b3Free(m_ConeIndices);
	b3Free(m_ConePolygons);

	m_CylinderIndices  = (GLushort *)b3Alloc
			((m_SubDiv + 1) * 3 * 2 * sizeof(GLushort));
	m_CylinderPolygons = (GLushort *)b3Alloc
			((m_SubDiv + 1) * 2 * 3 * sizeof(GLushort));
	if ((m_CylinderIndices != null) && (m_CylinderPolygons != null))
	{
		gPtr = m_CylinderIndices;
		pPtr = m_CylinderPolygons;
		a = 0;
		for (i = 0;i <= m_SubDiv;i++)
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
	else
	{
		throw new b3WorldException(B3_WORLD_MEMORY);
	}

	m_ConeIndices  = (GLushort *)b3Alloc
		((m_SubDiv + 1) * 2 * 2 * sizeof(GLushort));
	m_ConePolygons = (GLushort *)b3Alloc
		((m_SubDiv + 1) * 1 * 3 * sizeof(GLushort));
	if ((m_ConeIndices != null) && (m_ConePolygons != null))
	{
		gPtr = m_ConeIndices;
		pPtr = m_ConePolygons;
		for (i = 0;i <= m_SubDiv;i++)
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
	else
	{
		throw new b3WorldException(B3_WORLD_MEMORY);
	}
#endif
}

b3_count b3ShapeRenderContext::b3GetSubdiv()
{
	return m_SubDiv;
}

b3_f64 *b3ShapeRenderContext::b3GetSinTable()
{
	return m_Sin;
}

b3_f64 *b3ShapeRenderContext::b3GetCosTable()
{
	return m_Cos;
}

b3_vector *b3ShapeRenderContext::b3GetSplineAux()
{
	b3_count factor;

	if (m_Between == null)
	{
		factor  = B3_MAX(B3_MAX_CONTROLS,B3_MAX_SUBDIV) + 1;
		m_Between = (b3_vector *)b3Alloc(factor * factor * sizeof(b3_vector));
		if (m_Between == null)
		{
			throw new b3WorldException(B3_WORLD_MEMORY);
		}
	}
	return m_Between;
}

#ifdef BLZ3_USE_OPENGL
GLushort *b3ShapeRenderContext::b3GetCylinderIndices()
{
	return m_CylinderIndices;
}

GLushort *b3ShapeRenderContext::b3GetCylinderPolygons()
{
	return m_CylinderPolygons;
}

GLushort *b3ShapeRenderContext::b3GetConeIndices()
{
	return m_ConeIndices;
}

GLushort *b3ShapeRenderContext::b3GetConePolygons()
{
	return m_ConePolygons;
}
#endif


/*************************************************************************
**                                                                      **
**                        b3ShapeRenderObject Implementation            **
**                                                                      **
*************************************************************************/

b3ShapeRenderObject::b3ShapeRenderObject(b3_size class_size,b3_u32 class_type) :
	b3Shape(class_size, class_type)
{
	Between = null;
}

b3ShapeRenderObject::b3ShapeRenderObject(b3_u32 class_type) :
	b3Shape(sizeof(b3ShapeRenderObject), class_type)
{
	Between = null;
}

b3ShapeRenderObject::b3ShapeRenderObject(b3_u32 *src) :
	b3Shape(src)
{
	Between = null;
}

void b3ShapeRenderObject::b3ComputeBound(b3_stencil_limit *limit)
{
	b3Item      *item;
	b3Condition *cond;

	limit->x1 = -1;
	limit->y1 = -1;
	limit->x2 =  1;
	limit->y2 =  1;

	B3_FOR_BASE(b3GetConditionHead(),item)
	{
		cond = (b3Condition *)item;
		cond->b3ComputeBound(limit);
	}
}

b3_count b3ShapeRenderObject::b3GetIndexOverhead (
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
	if ((xs - x1) > epsilon) Overhead++;
	if ((x2 - xe) > epsilon) Overhead++;

	return ((xs > 0) || (xe < SinCosSteps)) ? -Overhead : Overhead;
}

void b3ShapeRenderObject::b3GetDiffuseColor(b3_color *color)
{
	b3Item     *item;
	b3Material *material;
	b3_color    ambient,specular;
	b3_polar    polar;

	color->r = 0.1f;
	color->g = 0.5f;
	color->b = 1.0f;
	color->a = 0.0f;

	B3_FOR_BASE(b3GetMaterialHead(),item)
	{
		material = (b3Material *)item;
		if (material->b3GetColors(&polar,color,&ambient,&specular))
		{
			return;
		}
	}
}

b3_render_mode b3ShapeRenderObject::b3GetRenderMode()
{
	return b3IsActive() ? B3_RENDER_FILLED : B3_RENDER_LINE;
}

void b3ShapeRenderObject::b3GetGridColor(b3_color *color)
{
	color->r = 0.2f;
	color->g = 0.2f;
	color->b = 0.2f;
	color->a = 0.0f;
}

/*************************************************************************
**                                                                      **
**                        Searching for equal indices                   **
**                                                                      **
*************************************************************************/

#ifdef BLZ3_USE_OPENGL
b3_index b3ShapeRenderObject::b3FindVertex(GLushort vertex)
{
	b3_vector *point;
	b3_vector *ptr = (b3_vector *)glVertices;
	b3_index   i;

	point = &ptr[vertex];
	for (i = 0;i < glVertexCount;i++)
	{
		if (b3Vector::b3Distance(point,ptr) < epsilon)
		{
			return i;
		}
		ptr++;
	}

	B3_ASSERT(false);
	return vertex;
}
#endif

void b3ShapeRenderObject::b3CorrectIndices()
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

/*************************************************************************
**                                                                      **
**                        Sphere computation                            **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeSphereVertices(
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

/*************************************************************************
**                                                                      **
**                        Cylinder computation                          **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeCylinderVertices(
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

	if ((i - start) > epsilon)
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

	if ((end - iMax) > epsilon)
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

void b3ShapeRenderObject::b3ComputeCylinderIndices()
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

/*************************************************************************
**                                                                      **
**                        Cone computation                              **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeConeVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     sx,sy,b,h,d,a,start,end;
	b3_index   i;
	b3_count   iMax;
	b3_vector  Bottom;

	Vector   = (b3_vector *)glVertices;

	d        = Limit.y2 - Limit.y1;
	b        = Limit.y1;
	h        = Limit.y2;
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
		if ((i - start) > epsilon)
		{
			a = Limit.x1 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * cos(a);
			sy = (1-h) * sin(a);
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;
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
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		if ((end - iMax) > epsilon)
		{
			a  = Limit.x2 * M_PI * 2;

			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * cos(a);
			sy = (1-h) * sin(a);
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;

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

		if ((i - start) > epsilon)
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

		if ((end - iMax) > epsilon)
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

void b3ShapeRenderObject::b3ComputeConeIndices()
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

/*************************************************************************
**                                                                      **
**                        Ellipsoid computation                         **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeEllipsoidVertices(
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
	if ((i - start) > epsilon)	/* underflow */
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
	if ((end - iMax) > epsilon)	/* Overflow */
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

	if ((i - start) > epsilon)
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

	if ((end - iMax) > epsilon)
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

void b3ShapeRenderObject::b3ComputeEllipsoidIndices()
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
	if ((ys - y1) > epsilon) Heights++;
	if ((y2 - ye) > epsilon) Heights++;

	Widths = Heights - 1;
	if ((SinCosSteps * 0.5 - y2) > epsilon) Widths++;
	if (                     y1  > epsilon) Widths++;

	if (EndLine) Number = (Widths + Heights + 1) * Overhead + Heights;
	else         Number = (Widths + Heights + 1) * Overhead;

	// Realloc buffers
	b3RenderObject::b3Free(glGrids);
	b3RenderObject::b3Free(glPolygons);
	glGrids    = gPtr = (GLushort *)b3RenderObject::b3Alloc
		(Number * 2 * sizeof(GLushort));
	glPolygons = pPtr = (GLushort *)b3RenderObject::b3Alloc
		(Number * 3 * sizeof(GLushort));
	if ((gPtr == null) || (pPtr == null))
	{
		throw new b3WorldException(B3_WORLD_MEMORY);
	}

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			// This marks a longitude
			*gPtr++ = s + j;
			*gPtr++ = s + j + 1;
		}
		glGridCount += Heights;

		if (y1 <= epsilon)
		{
			// NOTE: j = 0 substitution
			*pPtr++ = s + Heights + 2;
			*pPtr++ = s + Heights + 1;
			*pPtr++ = s + 1;
			glPolyCount++;

			j = 1;
		}
		else
		{
			j = 0;
		}
		while(j < Heights)
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

		if ((SinCosSteps * 0.5 - y2) > epsilon)
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

/*************************************************************************
**                                                                      **
**                        Box computation                               **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeBoxVertices(
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

void b3ShapeRenderObject::b3ComputeBoxIndices()
{
#ifdef BLZ3_USE_OPENGL
	glGrids    = box_grids;
	glPolygons = box_polygons;
#endif
}

/*************************************************************************
**                                                                      **
**                        Torus computation                             **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeTorusVertices(
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
	if ((i - start) > epsilon)	/* underflow */
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
	if ((end - iMax) > epsilon)	/* Overflow */
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

	if ((i - start) > epsilon)
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

	if ((end - iMax) > epsilon)
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

void b3ShapeRenderObject::b3ComputeTorusIndices()
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
	if ((ys - y1) > epsilon) Heights++;
	if ((y2 - ye) > epsilon) Heights++;
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
		throw new b3WorldException(B3_WORLD_MEMORY);
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
