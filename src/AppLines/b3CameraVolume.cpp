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

static GLushort VolumeIndices[B3_CV_INDEX_COUNT * 2] =
{
	 0, 1,    1, 2,    2, 3,    3, 0,  // border
	 4, 5,                             // view axis
	 4, 6,    4, 7,    4, 8,    4, 9   // camera edges
};

b3CameraVolume::b3CameraVolume()
{
	glComputed = false;
}

void b3CameraVolume::b3Update(b3CameraPart *camera)
{
	b3_vector  view_dir;

	view_dir.x = camera->ViewPoint.x - camera->EyePoint.x;
	view_dir.y = camera->ViewPoint.y - camera->EyePoint.y;
	view_dir.z = camera->ViewPoint.z - camera->EyePoint.z;

	m_Vertices[0].x = camera->EyePoint.x + view_dir.x + camera->Width.x + camera->Height.x;
	m_Vertices[0].y = camera->EyePoint.y + view_dir.y + camera->Width.y + camera->Height.y;
	m_Vertices[0].z = camera->EyePoint.z + view_dir.z + camera->Width.z + camera->Height.z;

	m_Vertices[1].x = camera->EyePoint.x + view_dir.x - camera->Width.x + camera->Height.x;
	m_Vertices[1].y = camera->EyePoint.y + view_dir.y - camera->Width.y + camera->Height.y;
	m_Vertices[1].z = camera->EyePoint.z + view_dir.z - camera->Width.z + camera->Height.z;

	m_Vertices[2].x = camera->EyePoint.x + view_dir.x - camera->Width.x - camera->Height.x;
	m_Vertices[2].y = camera->EyePoint.y + view_dir.y - camera->Width.y - camera->Height.y;
	m_Vertices[2].z = camera->EyePoint.z + view_dir.z - camera->Width.z - camera->Height.z;

	m_Vertices[3].x = camera->EyePoint.x + view_dir.x + camera->Width.x - camera->Height.x;
	m_Vertices[3].y = camera->EyePoint.y + view_dir.y + camera->Width.y - camera->Height.y;
	m_Vertices[3].z = camera->EyePoint.z + view_dir.z + camera->Width.z - camera->Height.z;

	m_Vertices[4]   = camera->EyePoint;

	m_Vertices[5].x = camera->EyePoint.x + view_dir.x * 2;
	m_Vertices[5].y = camera->EyePoint.y + view_dir.y * 2;
	m_Vertices[5].z = camera->EyePoint.z + view_dir.z * 2;

	m_Vertices[6].x = camera->EyePoint.x + view_dir.x * 1.5 + camera->Width.x * 1.5 + camera->Height.x * 1.5;
	m_Vertices[6].y = camera->EyePoint.y + view_dir.y * 1.5 + camera->Width.y * 1.5 + camera->Height.y * 1.5;
	m_Vertices[6].z = camera->EyePoint.z + view_dir.z * 1.5 + camera->Width.z * 1.5 + camera->Height.z * 1.5;

	m_Vertices[7].x = camera->EyePoint.x + view_dir.x * 1.5 - camera->Width.x * 1.5 + camera->Height.x * 1.5;
	m_Vertices[7].y = camera->EyePoint.y + view_dir.y * 1.5 - camera->Width.y * 1.5 + camera->Height.y * 1.5;
	m_Vertices[7].z = camera->EyePoint.z + view_dir.z * 1.5 - camera->Width.z * 1.5 + camera->Height.z * 1.5;

	m_Vertices[8].x = camera->EyePoint.x + view_dir.x * 1.5 - camera->Width.x * 1.5 - camera->Height.x * 1.5;
	m_Vertices[8].y = camera->EyePoint.y + view_dir.y * 1.5 - camera->Width.y * 1.5 - camera->Height.y * 1.5;
	m_Vertices[8].z = camera->EyePoint.z + view_dir.z * 1.5 - camera->Width.z * 1.5 - camera->Height.z * 1.5;

	m_Vertices[9].x = camera->EyePoint.x + view_dir.x * 1.5 + camera->Width.x * 1.5 - camera->Height.x * 1.5;
	m_Vertices[9].y = camera->EyePoint.y + view_dir.y * 1.5 + camera->Width.y * 1.5 - camera->Height.y * 1.5;
	m_Vertices[9].z = camera->EyePoint.z + view_dir.z * 1.5 + camera->Width.z * 1.5 - camera->Height.z * 1.5;

	glComputed = false;
}

void b3CameraVolume::b3AllocVertices(b3RenderContext *cts)
{
	glVertices = (GLfloat *)m_Vertices;
	glNormals  = (GLfloat *)m_Normals;
	glGrids    = VolumeIndices;
	glPolygons = null;

	glVertexCount = B3_CV_VERTEX_COUNT;
	glGridCount   = B3_CV_INDEX_COUNT;
}

void b3CameraVolume::b3FreeVertices()
{
	glVertices = null;
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
