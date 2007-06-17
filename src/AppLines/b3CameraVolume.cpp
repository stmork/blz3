/*
**
**	$Filename:	b3CameraColume.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Drawing the cameras view volume
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

#include "AppLinesInclude.h"
#include "b3CameraVolume.h"

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
	m_Camera = null;
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
