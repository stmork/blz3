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

#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.30  2004/07/02 19:28:03  sm
**      - Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**      - Recoupled b3Scene include from CApp*Doc header files to allow
**        faster compilation.
**      - Removed intersection counter completely because of a mysterious
**        destruction problem of b3Mutex.
**
**      Revision 1.29  2004/06/21 18:10:53  sm
**      - Fixed disk drawing problem. The Problem occured in
**        conjunction with optimizing code with the Visual C++
**        compiler. Should I install Visual.Net?
**
**      Revision 1.28  2004/06/21 09:26:19  sm
**      - Changed rendering: The constant sin/cos tables are now directly
**        used from b3ShapeRenderContext.
**      - Memory allocation for polygons/grid removed from disks.
**
**      Revision 1.27  2004/06/20 15:33:10  sm
**      - Update material when edited.
**
**      Revision 1.26  2004/05/10 15:12:09  sm
**      - Unified condition legends for conditions and
**        texture materials.
**      - Added wrap texture material dialog.
**
**      Revision 1.25  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.24  2003/02/22 17:21:34  sm
**      - Changed some global variables into static class members:
**        o b3Scene::epsilon
**        o b3Scene::m_TexturePool et. al.
**        o b3SplineTemplate<class VECTOR>::bspline_errno
**
**      Revision 1.23  2002/08/16 11:40:38  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.22  2002/08/15 13:56:43  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.21  2002/08/09 13:20:19  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.20  2002/07/29 12:32:56  sm
**      - Full disk draws textures correctly now
**      - Windows selects the correct pixel format for
**        the nVidia driver.
**      - Some problems concerning first drawing and lighting
**        aren't fixed, yet. This seems to be a nVidia problem
**
**      Revision 1.19  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.18  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.17  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.16  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.15  2002/02/27 20:14:52  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.14  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.13  2001/10/20 16:14:59  sm
**      - Some runtime environment cleanups. The CPU count is determined
**        only once.
**      - Introduced preparing routines for raytring to shapes.
**      - Found 5% performance loss: No problem, this was eaten by
**        bug fxing of the rotation spline shapes. (Phuu!)
**      - The next job is to implement different row sampler. Then we
**        should implemented the base set of the Blizzard II raytracer.
**
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
	vertCount = b3ShapeRenderContext::m_SubDiv * 2 + 6;
	gridCount = b3ShapeRenderContext::m_SubDiv * 2 + 6;
	polyCount = b3ShapeRenderContext::m_SubDiv * 2 + 4;
}

void b3Disk::b3ComputeVertices()
{
	b3_gl_vertex *Vector = glVertex;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_f64        sx,sy,b,a,h,start,end;
	b3_index      i;
	b3_count      iMax;

	h = m_Limit.y2;
	b = m_Limit.y1;

	start = m_Limit.x1 * SinCosSteps;
	end   = m_Limit.x2 * SinCosSteps;
	i     = (b3_index)ceil(start);
	iMax  = (b3_count)floor(end);
	xSize = 0;
	ySize = 1;

	glVertexCount = 0;

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

		glVertexCount += 2;
		xSize++;
	}

	for (;i<=iMax;i++)
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

		glVertexCount += 2;
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

		glVertexCount += 2;
		xSize++;
	}
}

void b3Disk::b3ComputeIndices()
{
	b3_gl_line    *gPtr;
	b3_gl_polygon *pPtr;
	b3_bool        EndLines = false;
	b3_index       i,pos;
	b3_count       Overhead;

	b3ComputeBound(&m_Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		EndLines = true;
		Overhead = -Overhead;
	}

	glGridCount = 0;
	glPolyCount = 0;

	gPtr = glGrids;
	pPtr = glPolygons;

	for (i = pos = 0;i < Overhead;i++)
	{
		B3_GL_LINIT(gPtr,pos,  pos+2);
		B3_GL_LINIT(gPtr,pos+1,pos+3);

		B3_GL_PINIT(pPtr,pos,  pos+2,pos+1);
		B3_GL_PINIT(pPtr,pos+3,pos+1,pos+2);

		glGridCount += 2;
		glPolyCount += 2;
		pos += 2;
	}
	if (EndLines)
	{
		B3_GL_LINIT(gPtr,0,1);
		B3_GL_LINIT(gPtr,Overhead + Overhead,Overhead + Overhead + 1);

		glGridCount += 2;
	}
}

void b3Disk::b3GetStencilBoundInfo(b3_stencil_bound *info)
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
