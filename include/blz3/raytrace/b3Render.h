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

typedef struct b3_cond_limit
{
	b3_f32 x1,y1,x2,y2;
} b3CondLimit;

class b3RenderContext : b3Mem
{
	b3_count   subdiv;
	b3_f64     Sin[B3_MAX_RENDER_SUBDIV + 1];
	b3_f64     Cos[B3_MAX_RENDER_SUBDIV + 1];
	b3_vector *Between;
#ifdef BLZ3_USE_OPENGL
	GLushort  *CylinderIndices;
	GLushort  *ConeIndices;
#endif

public:
	           b3RenderContext(b3_count subdiv = 16);
	void       b3InitSubdiv(b3_count subdiv);
	b3_count   b3GetSubdiv();
	b3_f64    *b3GetCosTable();
	b3_f64    *b3GetSinTable();
	b3_vector *b3GetSplineAux();

#ifdef BLZ3_USE_OPENGL
	GLushort  *b3GetCylinderIndices();
	GLushort  *b3GetConeIndices();
#endif
};

class b3RenderObject : public b3Mem
{
protected:
	b3_count         SinCosSteps;
	b3_vector       *Between;
	b3_f64          *Cos;
	b3_f64          *Sin;

	b3_f64           Epsilon;

	b3CondLimit      Limit;
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
	virtual void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	virtual void     b3AllocVertices(b3RenderContext *context);
	virtual void     b3FreeVertices();
	virtual void     b3ComputeVertices();
	virtual void     b3ComputeIndices();
	        void     b3Draw();

protected:
	        b3_count b3GetIndexOverhead(b3_f64 xl,b3_f64 yl);
};

#endif
