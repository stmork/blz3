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

static b3_gl_line VolumeIndices[B3_CV_INDEX_COUNT] =
{
	{ 0, 1 }, { 1, 2 }, { 2, 3}, { 3, 0 },  // border
	{ 4, 5 },                               // view axis
	{ 4, 6 }, { 4, 7 }, { 4, 8}, { 4, 9 }   // camera edges
};

b3CameraVolume::b3CameraVolume()
{
	glComputed = false;
	memset(m_Vertex,0,sizeof(m_Vertex));
}

void b3CameraVolume::b3Update(b3CameraPart *camera)
{
	b3_vector  view_dir;

	view_dir.x = camera->m_ViewPoint.x - camera->m_EyePoint.x;
	view_dir.y = camera->m_ViewPoint.y - camera->m_EyePoint.y;
	view_dir.z = camera->m_ViewPoint.z - camera->m_EyePoint.z;

	m_Vertex[0].v.x = camera->m_EyePoint.x + view_dir.x + camera->m_Width.x + camera->m_Height.x;
	m_Vertex[0].v.y = camera->m_EyePoint.y + view_dir.y + camera->m_Width.y + camera->m_Height.y;
	m_Vertex[0].v.z = camera->m_EyePoint.z + view_dir.z + camera->m_Width.z + camera->m_Height.z;

	m_Vertex[1].v.x = camera->m_EyePoint.x + view_dir.x - camera->m_Width.x + camera->m_Height.x;
	m_Vertex[1].v.y = camera->m_EyePoint.y + view_dir.y - camera->m_Width.y + camera->m_Height.y;
	m_Vertex[1].v.z = camera->m_EyePoint.z + view_dir.z - camera->m_Width.z + camera->m_Height.z;

	m_Vertex[2].v.x = camera->m_EyePoint.x + view_dir.x - camera->m_Width.x - camera->m_Height.x;
	m_Vertex[2].v.y = camera->m_EyePoint.y + view_dir.y - camera->m_Width.y - camera->m_Height.y;
	m_Vertex[2].v.z = camera->m_EyePoint.z + view_dir.z - camera->m_Width.z - camera->m_Height.z;

	m_Vertex[3].v.x = camera->m_EyePoint.x + view_dir.x + camera->m_Width.x - camera->m_Height.x;
	m_Vertex[3].v.y = camera->m_EyePoint.y + view_dir.y + camera->m_Width.y - camera->m_Height.y;
	m_Vertex[3].v.z = camera->m_EyePoint.z + view_dir.z + camera->m_Width.z - camera->m_Height.z;

	m_Vertex[4].v.x = camera->m_EyePoint.x;
	m_Vertex[4].v.y = camera->m_EyePoint.y;
	m_Vertex[4].v.z = camera->m_EyePoint.z;

	m_Vertex[5].v.x = camera->m_EyePoint.x + view_dir.x * 2;
	m_Vertex[5].v.y = camera->m_EyePoint.y + view_dir.y * 2;
	m_Vertex[5].v.z = camera->m_EyePoint.z + view_dir.z * 2;

	m_Vertex[6].v.x = camera->m_EyePoint.x + view_dir.x * 1.5 + camera->m_Width.x * 1.5 + camera->m_Height.x * 1.5;
	m_Vertex[6].v.y = camera->m_EyePoint.y + view_dir.y * 1.5 + camera->m_Width.y * 1.5 + camera->m_Height.y * 1.5;
	m_Vertex[6].v.z = camera->m_EyePoint.z + view_dir.z * 1.5 + camera->m_Width.z * 1.5 + camera->m_Height.z * 1.5;

	m_Vertex[7].v.x = camera->m_EyePoint.x + view_dir.x * 1.5 - camera->m_Width.x * 1.5 + camera->m_Height.x * 1.5;
	m_Vertex[7].v.y = camera->m_EyePoint.y + view_dir.y * 1.5 - camera->m_Width.y * 1.5 + camera->m_Height.y * 1.5;
	m_Vertex[7].v.z = camera->m_EyePoint.z + view_dir.z * 1.5 - camera->m_Width.z * 1.5 + camera->m_Height.z * 1.5;

	m_Vertex[8].v.x = camera->m_EyePoint.x + view_dir.x * 1.5 - camera->m_Width.x * 1.5 - camera->m_Height.x * 1.5;
	m_Vertex[8].v.y = camera->m_EyePoint.y + view_dir.y * 1.5 - camera->m_Width.y * 1.5 - camera->m_Height.y * 1.5;
	m_Vertex[8].v.z = camera->m_EyePoint.z + view_dir.z * 1.5 - camera->m_Width.z * 1.5 - camera->m_Height.z * 1.5;

	m_Vertex[9].v.x = camera->m_EyePoint.x + view_dir.x * 1.5 + camera->m_Width.x * 1.5 - camera->m_Height.x * 1.5;
	m_Vertex[9].v.y = camera->m_EyePoint.y + view_dir.y * 1.5 + camera->m_Width.y * 1.5 - camera->m_Height.y * 1.5;
	m_Vertex[9].v.z = camera->m_EyePoint.z + view_dir.z * 1.5 + camera->m_Width.z * 1.5 - camera->m_Height.z * 1.5;

	glComputed = false;
}

void b3CameraVolume::b3AllocVertices(b3RenderContext *cts)
{
	glVertex   = m_Vertex;
	glGrids    = VolumeIndices;
	glPolygons = null;

	glVertexCount = B3_CV_VERTEX_COUNT;
	glGridCount   = B3_CV_INDEX_COUNT;
}

void b3CameraVolume::b3FreeVertices()
{
	glVertex   = null;
	glGrids    = null;
	glPolygons = null;
}

void b3CameraVolume::b3GetGridColor(b3_color *color)
{
	color->r = 1.0f;
	color->g = 0.1f;
	color->b = 0.4f;
	color->a = 0.0f;
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
