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

typedef enum
{
	B3_RENDER_NOTHING,
	B3_RENDER_LINE,
	B3_RENDER_FILLED
} b3_render_mode;

typedef enum
{
	B3_MATRIX_OBJECT,
	B3_MATRIX_PROJECTION
} b3_matrix_mode;

class b3RenderContext : protected b3Mem
{
public:
	                 b3RenderContext();
	virtual void     b3Init();
	virtual void     b3StartDrawing();

	static  b3_bool  b3GetMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);
	static  b3_bool  b3PutMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);
};

class b3RenderObject : public b3Mem
{
protected:
	b3_count         glVertexCount;
	b3_count         glGridCount;
	b3_count         glPolyCount;
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
	        void            b3Recompute();
	        void            b3Update();
public:
	virtual void            b3AllocVertices(b3RenderContext *context);
	virtual void            b3FreeVertices();
	virtual void            b3Draw();
	        b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);

protected:
	virtual void            b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	virtual void            b3GetVertexRange(b3_index &start,b3_index &end);
	virtual void            b3ComputeVertices();
	virtual void            b3ComputeIndices();
	virtual void            b3ComputeNormals(b3_bool normalize=true);
	virtual b3_render_mode  b3GetRenderMode();
	virtual void            b3GetGridColor(b3_color *color);
	virtual void            b3GetDiffuseColor(b3_color *color);
};

#endif
