/*
**
**	$Filename:	b3PickInfo.h $ 
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setup pick points depending on shapes
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_PICKINFO_H
#define B3_RAYTRACE_PICKINFO_H

#include "blz3/base/b3Array.h"
#include "blz3/base/b3Pick.h"
#include "blz3/base/b3Render.h"

class b3Shape;

enum b3_pick_mode
{
	B3_PICK_UNABLE,
	B3_PICK_ALL,
	B3_PICK_HORIZONTAL,
	B3_PICK_VERTICAL
};

class b3PickInfo : public b3PickBase, public b3RenderObject
{
	b3Array<b3_gl_vertex> m_Vertices;
	b3Array<b3_gl_line>   m_Grid;

protected:
	b3Shape         *m_Shape;

public:
	b3_pick_mode     m_AllowedMode;
	static b3Color   m_GridColor;

public:
	                 b3PickInfo();
	virtual         ~b3PickInfo();
	        b3_bool  b3SetShape(b3Shape *shape);
	        void     b3Modified();

	virtual void     b3AddPickPoint(b3_vector *point,const char *title = null) = 0;
	virtual void     b3AddPickDir(b3_vector *point,b3_vector *dir,const char *title = null) = 0;
	        void     b3AddVertex(b3_vector *point);
	        void     b3AddLine(b3_index a,b3_index b);

protected:
	        void     b3AllocVertices(b3RenderContext *context);
	        void     b3FreeVertices();
	        void     b3ComputeVertices();
	        void     b3ComputeIndices();
	        void     b3GetGridColor(b3Color &color);
};

#endif
