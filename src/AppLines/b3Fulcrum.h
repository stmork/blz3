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
	b3_tnv_vertex m_Vertex[B3_FULCRUM_VERTEX_COUNT];

public:
	         b3Fulcrum();
	void     b3Update(b3_vector *fulcrum);
	void     b3AllocVertices(b3RenderContext *context);
	void     b3FreeVertices();
	void     b3Draw();

protected:
	void     b3GetGridColor(b3_color *color);
	void     b3ComputeVertices();
};

#endif
