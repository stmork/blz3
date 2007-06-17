/*
**
**	$Filename:	b3Fulcrum.h $
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

#ifndef FULCRUM_H
#define FULCRUM_H

#include "blz3/base/b3Render.h"

#define B3_FULCRUM_VERTEX_COUNT 20
#define B3_FULCRUM_INDEX_COUNT  13

class b3Fulcrum : public b3RenderObject
{
	       b3_f64        m_Scale;
	       b3_vector     m_Position;
	static b3_vector     m_FulcrumShape[B3_FULCRUM_VERTEX_COUNT];
	static b3_gl_line    m_FulcrumIndices[B3_FULCRUM_INDEX_COUNT];

public:
	static b3Color       m_GridColor;
 
public:
	         b3Fulcrum();
	void     b3Update(b3_vector *fulcrum);
	void     b3Draw(b3RenderContext *context);

protected:
	void     b3GetCount(b3RenderContext *ctx,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeIndices();
	void     b3ComputeVertices();

	inline void b3GetGridColor(b3Color &color)
	{
		color = m_GridColor;
	}
};

#endif
