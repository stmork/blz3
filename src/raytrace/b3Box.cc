/*
**
**      $Filename:      b3Box.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing box
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
#include "blz3/raytrace/b3BBox.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.24  2004/11/21 14:56:58  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.23  2004/09/25 10:48:47  sm
**      - Setting to static fixed size arrays.
**
**      Revision 1.22.2.1  2004/11/19 19:38:43  sm
**      - OK. The arrays are drawing correctly and the ATi VBOs are drawing
**        something. The draw buffer seams to be defective. Now we should
**        look what nVIDIA is doing with my code.
**
**      Revision 1.22  2004/09/24 11:42:14  sm
**      - First VBO run under Linux.
**
**      Revision 1.21  2004/09/23 15:47:04  sm
**      - Splitted b3RenderContext into own file.
**      - Added vertex buffer object support which does not
**        run yet.
**
**      Revision 1.20  2004/05/10 15:12:09  sm
**      - Unified condition legends for conditions and
**        texture materials.
**      - Added wrap texture material dialog.
**
**      Revision 1.19  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.18  2003/08/31 10:44:07  sm
**      - Further buffer overflow avoidments.
**
**      Revision 1.17  2003/02/22 15:17:18  sm
**      - Added support for selected shapes in object modeller
**      - Glued b3Shape and b3ShapeRenderObject. There was no
**        distinct access method in use.
**      - Made some b3Shape methods inline and/or static which
**        saves some memory.
**
**      Revision 1.16  2002/08/16 11:40:38  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.15  2002/08/10 14:36:31  sm
**      - Some shapes had cleared the vertex array whenever the
**        b3AllocVertices() method were called. Without calling
**        b3Recomute() the shapes disoccured.
**      - Some methods moved as static methods into the
**        b3Mem class.
**
**      Revision 1.14  2002/07/31 07:30:44  sm
**      - New normal computation. Textures are rendered correctly and
**        quadrics are shaded correctly. Spheres and doughnuts have
**        got their own more simple computation.
**
**      Revision 1.13  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.12  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.11  2002/07/21 17:02:36  sm
**      - Finished advanced color mix support (correct Phong/Mork shading)
**      - Added first texture mapping support. Further development on
**        Windows now...
**
**      Revision 1.10  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.9  2002/02/27 20:14:51  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.8  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.7  2001/09/22 16:19:51  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.6  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.5  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.4  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.3  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.2  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.1  2001/08/06 15:25:59  sm
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

b3Box::b3Box(b3_u32 class_type) : b3Shape3(sizeof(b3Box),class_type)
{
}

b3Box::b3Box(b3_u32 *src) : b3Shape3(src)
{
}

void b3BBox::b3SetName(const char *name)
{
	b3Item::b3SetString(m_BoxName,sizeof(m_BoxName),name);
}

void b3Box::b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount)
{
	vertCount = 24;
}

void b3Box::b3ComputeVertices()
{
	b3ComputeBoxVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3Box::b3ComputeNormals(b3_bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3Box::b3ComputeIndices()
{
	glGridElements->b3SetCount(12);
	glPolygonElements->b3SetCount(12);

	b3ComputeBoxIndices();
}

void b3Box::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_LENGTH;
	info->yInfo.unit   = B3_STENCIL_LENGTH;
	info->xInfo.factor = b3Vector::b3Length(&m_Dir1);
	info->yInfo.factor = b3Vector::b3Length(&m_Dir2);
}
