/*
**
**	$Filename:	b3CameraVolume.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Drawing a fulcrum
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef CAMERAVOLUME_H
#define CAMERAVOLUME_H

#include "blz3/raytrace/b3Special.h"
#include "blz3/base/b3Render.h"

#define B3_CV_VERTEX_COUNT 10
#define B3_CV_INDEX_COUNT   9

class b3CameraVolume : public b3RenderObject
{
	       b3_gl_vertex m_Vertex[B3_CV_VERTEX_COUNT];
	static b3_gl_line   m_VolumeIndices[B3_CV_INDEX_COUNT];

public:
	static b3Color m_GridColor;

public:
	         b3CameraVolume();
	void     b3Update(b3CameraPart *camera);
	void     b3AllocVertexMemory(b3RenderContext *context);
	void     b3FreeVertexMemory();
	void     b3Draw(b3RenderContext *context);

protected:
	inline void b3GetGridColor(b3Color &color)
	{
		color = m_GridColor;
	}
};

#endif
