/*
**
**      $Filename:      b3Cylinder.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing cylinder
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.20  2004/12/11 18:39:44  sm
**      - Fixed modified object problem in Lines when returning
**        to scene editor.
**
**      Revision 1.19  2004/11/21 14:56:58  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.18.2.1  2004/11/19 19:38:43  sm
**      - OK. The arrays are drawing correctly and the ATi VBOs are drawing
**        something. The draw buffer seams to be defective. Now we should
**        look what nVIDIA is doing with my code.
**
**      Revision 1.18  2004/09/24 11:42:14  sm
**      - First VBO run under Linux.
**
**      Revision 1.17  2004/09/23 15:47:04  sm
**      - Splitted b3RenderContext into own file.
**      - Added vertex buffer object support which does not
**        run yet.
**
**      Revision 1.16  2004/07/02 19:28:03  sm
**      - Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**      - Recoupled b3Scene include from CApp*Doc header files to allow
**        faster compilation.
**      - Removed intersection counter completely because of a mysterious
**        destruction problem of b3Mutex.
**
**      Revision 1.15  2004/06/21 09:26:19  sm
**      - Changed rendering: The constant sin/cos tables are now directly
**        used from b3ShapeRenderContext.
**      - Memory allocation for polygons/grid removed from disks.
**
**      Revision 1.14  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.13  2002/08/16 11:40:38  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.12  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.11  2001/09/22 16:19:52  sm
**      - Adding basic shape intersection routines
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

b3Cylinder::b3Cylinder(b3_u32 class_type) : b3Shape3(sizeof(b3Cylinder),class_type)
{
}

b3Cylinder::b3Cylinder(b3_u32 *src) : b3Shape3(src)
{
}

void b3Cylinder::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount = SinCosSteps + SinCosSteps + 6;
}

void b3Cylinder::b3AllocVertexMemory(b3RenderContext *ctx)
{
	b3ShapeRenderContext *context = (b3ShapeRenderContext *)ctx;

	b3RenderObject::b3AllocVertexMemory(context);
	glGridElements->b3SetGrids(context->b3GetCylinderIndices());
	glPolygonElements->b3SetPolygons(context->b3GetCylinderPolygons());
}

void b3Cylinder::b3FreeVertexMemory()
{
	glVertexElements->b3FreeVertexMemory();
}

void b3Cylinder::b3ComputeVertices()
{
	b3ComputeCylinderVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3Cylinder::b3ComputeIndices()
{
	b3ComputeCylinderIndices();
}
