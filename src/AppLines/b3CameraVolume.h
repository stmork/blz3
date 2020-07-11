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
	static b3_gl_line    m_VolumeIndices[B3_CV_INDEX_COUNT];
	b3CameraPart * m_Camera;

public:
	static b3Color       m_GridColor;

public:
	b3CameraVolume();
	void     b3Update(b3CameraPart * camera);
	void     b3GetCount(b3RenderContext * ctx, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
	void     b3Draw(b3RenderContext * context);

protected:
	inline void b3GetGridColor(b3Color & color)
	{
		color = m_GridColor;
	}
};

#endif
