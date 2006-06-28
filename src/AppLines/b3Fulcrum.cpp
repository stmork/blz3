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

#include "AppLinesInclude.h"
#include "b3Fulcrum.h"

/*************************************************************************
**                                                                      **
**                        Fulcrum point for roating/scaling/orientating **
**                                                                      **
*************************************************************************/

b3_vector b3Fulcrum::m_FulcrumShape[B3_FULCRUM_VERTEX_COUNT] =
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

b3_gl_line b3Fulcrum::m_FulcrumIndices[B3_FULCRUM_INDEX_COUNT] =
{
	{  0, 1 },   {  2, 3 },   {  4, 5 },                            // Achsen
	{  6, 7 },   {  8, 9 },                                         // x
	{ 10,11 },   { 11,12 },   { 13,12 },   { 12,14 },   { 14,15 },  // y
	{ 17,19 },   { 19,16 },   { 16,18 }                             // z
};

b3Color b3Fulcrum::m_GridColor(1.0f,0.1f,0.25f);

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
	if (glVertexElements != null)
	{
		b3Recompute();
	}
}

void b3Fulcrum::b3GetCount(b3RenderContext *ctx,
	b3_count &vertCount,
	b3_count &gridCount,
	b3_count &polyCount)
{
	vertCount = B3_FULCRUM_VERTEX_COUNT;
}

void b3Fulcrum::b3ComputeVertices()
{
	b3_index i;
	b3_gl_vertex *vertex = *glVertexElements;

	for (i = 0;i < B3_FULCRUM_VERTEX_COUNT;i++)
	{
		vertex[i].v.x = m_FulcrumShape[i].x * m_Scale + m_Position.x;
		vertex[i].v.y = m_FulcrumShape[i].y * m_Scale + m_Position.y;
		vertex[i].v.z = m_FulcrumShape[i].z * m_Scale + m_Position.z;
	}
}

void b3Fulcrum::b3ComputeIndices()
{
	glGridElements->b3SetGrids(m_FulcrumIndices);
	glGridElements->b3SetCount(B3_FULCRUM_INDEX_COUNT);

	glPolygonElements->b3SetPolygons(null);
	glPolygonElements->b3SetCount(0);
}

void b3Fulcrum::b3Draw(b3RenderContext *context)
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
