/*
**
**	$Filename:	b3Render.h $ 
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Rendering class definitions
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_RENDER_H
#define B3_RAYTRACE_RENDER_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Mem.h"

#define B3_MAX_RENDER_SUBDIV 48

class b3RenderContext : protected b3Mem
{
public:
	                 b3RenderContext();
	virtual void     b3StartDrawing();
};

class b3RenderObject : public b3Mem
{
protected:
	b3_count         VertexCount;
	b3_count         GridCount;
	b3_count         PolyCount;
#ifdef BLZ3_USE_OPENGL
	GLfloat         *glVertices;
	GLfloat         *glNormals;
	GLushort        *glGrids;
	GLushort        *glPolygons;
	b3_bool          glComputed;
#endif

protected:

	                 b3RenderObject();
	                ~b3RenderObject();
public:
	virtual void     b3AllocVertices(b3RenderContext *context);
	virtual void     b3FreeVertices();
	virtual void     b3Draw();
	        b3_bool  b3ComputeBounds(b3_vector *lower,b3_vector *upper);

protected:
	virtual void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	virtual void     b3ComputeVertices();
	virtual void     b3ComputeIndices();
	virtual void     b3ComputeNormals(b3_bool normalize=true);
	virtual b3_bool  b3IsSolid();
	virtual void     b3GetGridColor(b3_color *color);
	virtual void     b3GetDiffuseColor(b3_color *color);
};

#endif
