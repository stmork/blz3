/*
**
**      $Filename:      b3CameraColume.cpp $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Drawing the cameras view volume
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

#include "b3CameraVolume.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.14  2004/11/21 14:56:56  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.13  2004/09/25 11:52:30  sm
**      - Smoke run test without VBOs successful
**
**      Revision 1.12.2.1  2004/11/20 11:37:15  sm
**      - Added Windows VBO support
**
**      Revision 1.12  2004/09/24 20:22:05  sm
**      - Some VBO adjustments.
**
**      Revision 1.11  2003/03/04 20:37:36  sm
**      - Introducing new b3Color which brings some
**        performance!
**
**      Revision 1.10  2003/02/26 16:36:16  sm
**      - Sorted drawing colors and added configuration support
**        to dialog.
**
**      Revision 1.9  2003/01/11 12:30:30  sm
**      - Some additional undo/redo actions
**
**      Revision 1.8  2002/08/16 14:00:02  sm
**      - Lines III adjustments to new vertex computation.
**
**      Revision 1.7  2002/08/01 15:02:56  sm
**      - Found texture missing bug when printing. There weren't any
**        selected textures inside an other OpenGL rendering context.
**        Now fixed!
**
**      Revision 1.6  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.5  2002/07/26 22:08:09  sm
**      - Some b3RenderObject derived classed didn't initialize
**        glTexCoord. It's time to use glInterleavedArrays() to
**        avoid such hazards.
**      - The nVidia driver seems to use only 16 bit for depth
**        buffer:-(
**
**      Revision 1.4  2001/12/01 17:48:42  sm
**      - Added raytraced image saving
**      - Added texture search path configuration
**      - Always drawing fulcrum and view volume. The
**        depth buffer problem persists
**
**      Revision 1.3  2001/11/03 16:24:16  sm
**      - Added scene property dialog
**      - Added raytrace view title
**      - Added raytrace abort on button press
**
**      Revision 1.2  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.1  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**
*/

/*************************************************************************
**                                                                      **
**                        View colume visualization                     **
**                                                                      **
*************************************************************************/

b3_gl_line b3CameraVolume::m_VolumeIndices[B3_CV_INDEX_COUNT] =
{
	{ 0, 1 }, { 1, 2 }, { 2, 3}, { 3, 0 },  // border
	{ 4, 5 },                               // view axis
	{ 4, 6 }, { 4, 7 }, { 4, 8}, { 4, 9 }   // camera edges
};

b3Color b3CameraVolume::m_GridColor(1.0f,0.1f,0.25f);

b3CameraVolume::b3CameraVolume()
{
}

void b3CameraVolume::b3Update(b3CameraPart *camera)
{
	m_Camera = camera;
	b3Recompute();
}

void b3CameraVolume::b3GetCount(b3RenderContext *ctx,
	b3_count &vertCount,
	b3_count &gridCount,
	b3_count &polyCount)
{
	vertCount = B3_CV_VERTEX_COUNT;
}

void b3CameraVolume::b3ComputeVertices()
{
	b3_vector     view_dir;
	b3_gl_vertex *vertex = *glVertexElements;

	B3_ASSERT(m_Camera != null);

	view_dir.x = m_Camera->m_ViewPoint.x - m_Camera->m_EyePoint.x;
	view_dir.y = m_Camera->m_ViewPoint.y - m_Camera->m_EyePoint.y;
	view_dir.z = m_Camera->m_ViewPoint.z - m_Camera->m_EyePoint.z;

	vertex[0].v.x = m_Camera->m_EyePoint.x + view_dir.x + m_Camera->m_Width.x + m_Camera->m_Height.x;
	vertex[0].v.y = m_Camera->m_EyePoint.y + view_dir.y + m_Camera->m_Width.y + m_Camera->m_Height.y;
	vertex[0].v.z = m_Camera->m_EyePoint.z + view_dir.z + m_Camera->m_Width.z + m_Camera->m_Height.z;

	vertex[1].v.x = m_Camera->m_EyePoint.x + view_dir.x - m_Camera->m_Width.x + m_Camera->m_Height.x;
	vertex[1].v.y = m_Camera->m_EyePoint.y + view_dir.y - m_Camera->m_Width.y + m_Camera->m_Height.y;
	vertex[1].v.z = m_Camera->m_EyePoint.z + view_dir.z - m_Camera->m_Width.z + m_Camera->m_Height.z;
														          				        
	vertex[2].v.x = m_Camera->m_EyePoint.x + view_dir.x - m_Camera->m_Width.x - m_Camera->m_Height.x;
	vertex[2].v.y = m_Camera->m_EyePoint.y + view_dir.y - m_Camera->m_Width.y - m_Camera->m_Height.y;
	vertex[2].v.z = m_Camera->m_EyePoint.z + view_dir.z - m_Camera->m_Width.z - m_Camera->m_Height.z;
														          				        
	vertex[3].v.x = m_Camera->m_EyePoint.x + view_dir.x + m_Camera->m_Width.x - m_Camera->m_Height.x;
	vertex[3].v.y = m_Camera->m_EyePoint.y + view_dir.y + m_Camera->m_Width.y - m_Camera->m_Height.y;
	vertex[3].v.z = m_Camera->m_EyePoint.z + view_dir.z + m_Camera->m_Width.z - m_Camera->m_Height.z;
	 
	vertex[4].v.x = m_Camera->m_EyePoint.x;
	vertex[4].v.y = m_Camera->m_EyePoint.y;
	vertex[4].v.z = m_Camera->m_EyePoint.z;
	 				        
	vertex[5].v.x = m_Camera->m_EyePoint.x + view_dir.x * 2;
	vertex[5].v.y = m_Camera->m_EyePoint.y + view_dir.y * 2;
	vertex[5].v.z = m_Camera->m_EyePoint.z + view_dir.z * 2;
					        
	vertex[6].v.x = m_Camera->m_EyePoint.x + view_dir.x * 1.5 + m_Camera->m_Width.x * 1.5 + m_Camera->m_Height.x * 1.5;
	vertex[6].v.y = m_Camera->m_EyePoint.y + view_dir.y * 1.5 + m_Camera->m_Width.y * 1.5 + m_Camera->m_Height.y * 1.5;
	vertex[6].v.z = m_Camera->m_EyePoint.z + view_dir.z * 1.5 + m_Camera->m_Width.z * 1.5 + m_Camera->m_Height.z * 1.5;
	 				        									          					          
	vertex[7].v.x = m_Camera->m_EyePoint.x + view_dir.x * 1.5 - m_Camera->m_Width.x * 1.5 + m_Camera->m_Height.x * 1.5;
	vertex[7].v.y = m_Camera->m_EyePoint.y + view_dir.y * 1.5 - m_Camera->m_Width.y * 1.5 + m_Camera->m_Height.y * 1.5;
	vertex[7].v.z = m_Camera->m_EyePoint.z + view_dir.z * 1.5 - m_Camera->m_Width.z * 1.5 + m_Camera->m_Height.z * 1.5;
	 														          					          
	vertex[8].v.x = m_Camera->m_EyePoint.x + view_dir.x * 1.5 - m_Camera->m_Width.x * 1.5 - m_Camera->m_Height.x * 1.5;
	vertex[8].v.y = m_Camera->m_EyePoint.y + view_dir.y * 1.5 - m_Camera->m_Width.y * 1.5 - m_Camera->m_Height.y * 1.5;
	vertex[8].v.z = m_Camera->m_EyePoint.z + view_dir.z * 1.5 - m_Camera->m_Width.z * 1.5 - m_Camera->m_Height.z * 1.5;
					        									          					          
	vertex[9].v.x = m_Camera->m_EyePoint.x + view_dir.x * 1.5 + m_Camera->m_Width.x * 1.5 - m_Camera->m_Height.x * 1.5;
	vertex[9].v.y = m_Camera->m_EyePoint.y + view_dir.y * 1.5 + m_Camera->m_Width.y * 1.5 - m_Camera->m_Height.y * 1.5;
	vertex[9].v.z = m_Camera->m_EyePoint.z + view_dir.z * 1.5 + m_Camera->m_Width.z * 1.5 - m_Camera->m_Height.z * 1.5;
}

void b3CameraVolume::b3ComputeIndices()
{
	glGridElements->b3SetGrids(m_VolumeIndices);
	glGridElements->b3SetCount(B3_CV_INDEX_COUNT);
	glPolygonElements->b3SetPolygons(null);
	glPolygonElements->b3SetCount(0);
}

void b3CameraVolume::b3Draw(b3RenderContext *context)
{
	int func;

	glDisable(GL_DEPTH_TEST);
	glGetIntegerv(GL_DEPTH_FUNC,&func);
	glDepthFunc(GL_ALWAYS);
//	glDepthMask(GL_FALSE);
	b3RenderObject::b3Draw(context);
//	glDepthMask(GL_TRUE);
	glDepthFunc(func);
	glEnable(GL_DEPTH_TEST);
}
