/*
**
**      $Filename:      b3Fulcrum.cpp $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Drawing a fulcrum
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

#include "b3Fulcrum.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.3  2001/12/01 17:48:42  sm
**      - Added raytraced image saving
**      - Added texture search path configuration
**      - Always drawing fulcrum and view volume. The
**        depth buffer problem persists
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
**                        Fulcrum point for roating/scaling/orientating **
**                                                                      **
*************************************************************************/

static b3_vector fulcrum[B3_FULCRUM_VERTEX_COUNT] =
{
	{  10,  0,  0},			/* x-Achse */
	{ -10,  0,  0},
	{   0, 10,  0},			/* y-Achse */
	{   0,-10,  0},
	{   0,  0, 10},			/* z-Achse */
	{   0,  0,-10},
	{  14,  0, -2},			/* x Buchstabe */
	{  18,  0,  2},
	{  14,  0,  2},
	{  18,  0, -2},
	{   0, 14,  2},			/* y Buchstabe */
	{   0, 14,  0},
	{   0, 18,  0},
	{   0, 18,  2},
	{   0, 16, -2},
	{   0, 14, -2},
	{  -2,  0, 14},			/* z Buchstabe */
	{  -2,  0, 18},
	{   2,  0, 14},
	{   2,  0, 18}
};

static GLushort FulcrumIndices[B3_FULCRUM_INDEX_COUNT * 2] =
{
	 0, 1,    2, 3,    4, 5,                    // Achsen
	 6, 7,    8, 9,                             // x
	10,11,   11,12,   13,12,   12,14,   14,15,  // y
	17,19,   19,16,   16,18                     // z
};

b3Fulcrum::b3Fulcrum()
{
	m_Scale      = 1.0;
	m_Position.x = 0;
	m_Position.y = 0;
	m_Position.z = 0;
}

void b3Fulcrum::b3Update(b3_vector *fulcrum)
{
	m_Position = *fulcrum;
	b3Recompute();
}

void b3Fulcrum::b3AllocVertices(b3RenderContext *cts)
{
	glVertices = (GLfloat *)m_Vertices;
	glNormals  = (GLfloat *)m_Normals;
	glGrids    = FulcrumIndices;
	glPolygons = null;
}

void b3Fulcrum::b3FreeVertices()
{
	glVertices = null;
	glGrids    = null;
	glPolygons = null;
}

void b3Fulcrum::b3ComputeVertices()
{
	b3_index i;

	for (i = 0;i < B3_FULCRUM_VERTEX_COUNT;i++)
	{
		m_Vertices[i].x = fulcrum[i].x * m_Scale + m_Position.x;
		m_Vertices[i].y = fulcrum[i].y * m_Scale + m_Position.y;
		m_Vertices[i].z = fulcrum[i].z * m_Scale + m_Position.z;
	}
	glVertexCount = B3_FULCRUM_VERTEX_COUNT;
	glGridCount   = B3_FULCRUM_INDEX_COUNT;
}

void b3Fulcrum::b3GetGridColor(b3_color *color)
{
	color->r = 1.0f;
	color->g = 0.1f;
	color->b = 0.4f;
	color->a = 0.0f;
}

void b3Fulcrum::b3Draw()
{
	int func;

	glDisable(GL_DEPTH_TEST);
	glGetIntegerv(GL_DEPTH_FUNC,&func);
	glDepthFunc(GL_ALWAYS);
//	glDepthMask(GL_FALSE);
	b3RenderObject::b3Draw();
//	glDepthMask(GL_TRUE);
	glDepthFunc(func);
	glEnable(GL_DEPTH_TEST);
}
