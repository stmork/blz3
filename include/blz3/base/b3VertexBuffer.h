/*
**
**	$Filename:	b3VertexBuffer.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Vertex buffer implementation
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_VERTEXBUFFER_H
#define B3_BASE_VERTEXBUFFER_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Mem.h"
#include "blz3/base/b3RenderTypes.h"
#include "blz3/base/b3VectorBufferObjects.h"

#ifdef BLZ3_USE_OPENGL

enum b3_vbo_mapping
{
	B3_MAP_VBO_RW = GL_READ_WRITE_ARB,
	B3_MAP_VBO_R  = GL_READ_ONLY_ARB,
	B3_MAP_VBO_W  = GL_WRITE_ONLY_ARB
};

#else

enum b3_vbo_mapping
{
	B3_MAP_VBO_RW,
	B3_MAP_VBO_R,
	B3_MAP_VBO_W
};

#endif

class b3RenderContext;

class B3_PLUGIN b3VertexBuffer : public b3Mem
{
protected:
	b3_count     glElementCount;
	b3_bool      glBound;
	b3_bool      glComputed;
	b3_bool      glCustom;

public:
	             b3VertexBuffer();

	virtual void b3Map(b3_vbo_mapping map_mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount) = 0;
	virtual void b3FreeVertexMemory() = 0;
	virtual void b3Draw();

	inline b3_count b3GetCount()
	{
		return glElementCount;
	}

	inline void b3SetCount(b3_count count)
	{
		glElementCount = count;
	}

	inline b3_bool b3IsEmpty()
	{
		return glElementCount <= 0;
	}

	inline void b3SetCustom(b3_bool custom = true)
	{
		glCustom = custom;
	}

	inline b3_bool b3IsCustom()
	{
		return glCustom;
	}

	inline void b3Recompute()
	{
		glComputed = false;
	}

	inline b3_bool b3IsComputed()
	{
		return glComputed;
	}

	inline void b3Recomputed()
	{
		glComputed = true;
	}
};

class B3_PLUGIN b3VertexElements : public b3VertexBuffer
{
protected:
	b3_gl_vertex *glVertex;

public:
	inline b3VertexElements()
	{
		glVertex = null;
	}

	inline void b3SetVertices(b3_gl_vertex *vertices)
	{
		glVertex = vertices;
	}

	inline operator b3_gl_vertex *()
	{
		return glVertex;
	}
};

class B3_PLUGIN b3GridElements : public b3VertexBuffer
{
protected:
	b3_gl_line   *glGrids;

public:
	inline b3GridElements()
	{
		glGrids = null;
	}

	inline void b3SetGrids(b3_gl_line *grids)
	{
		glGrids = grids;
	}

	inline operator b3_gl_line *()
	{
		return glGrids;
	}
};

class B3_PLUGIN b3PolygonElements : public b3VertexBuffer
{
protected:
	b3_gl_polygon  *glPolygons;

public:
	inline b3PolygonElements()
	{
		glPolygons = null;
	}

	inline void b3SetPolygons(b3_gl_polygon *polygons)
	{
		glPolygons = polygons;
	}

	inline operator b3_gl_polygon *()
	{
		return glPolygons;
	}
};

/*************************************************************************
**                                                                      **
**                        Simple buffer implementation                  **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3SimpleVertexElements : public b3VertexElements
{
public:
	             b3SimpleVertexElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount);
	virtual void b3FreeVertexMemory();
};

class B3_PLUGIN b3SimpleGridElements : public b3GridElements
{
public:
	             b3SimpleGridElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount);
	virtual void b3FreeVertexMemory();
};

class B3_PLUGIN b3SimplePolygonElements : public b3PolygonElements
{
public:
	             b3SimplePolygonElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount);
	virtual void b3FreeVertexMemory();
};

/*************************************************************************
**                                                                      **
**                        Standard OpenGL VertexArray implementation    **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3ArrayVertexElements : public b3SimpleVertexElements
{
public:
	             b3ArrayVertexElements();
	virtual void b3Draw();
};

class B3_PLUGIN b3ArrayGridElements : public b3SimpleGridElements
{
public:
	             b3ArrayGridElements();
	virtual void b3Draw();
};

class B3_PLUGIN b3ArrayPolygonElements : public b3SimplePolygonElements
{
public:
	             b3ArrayPolygonElements();
	virtual void b3Draw();
};

/*************************************************************************
**                                                                      **
**                        VBO implementation                            **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3VBO : protected b3VectorBufferObjects
{
protected:
#ifdef BLZ3_USE_OPENGL
	GLuint glVBO;
#endif

public:
	              b3VBO();
	virtual      ~b3VBO();
};

class B3_PLUGIN b3VboVertexElements : public b3VertexElements, protected b3VBO
{
public:
	             b3VboVertexElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount);
	virtual void b3FreeVertexMemory();
	virtual void b3Map(b3_vbo_mapping map_mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3Draw();
};

class B3_PLUGIN b3VboGridElements : public b3GridElements, protected b3VBO
{
public:
	             b3VboGridElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount);
	virtual void b3FreeVertexMemory();
	virtual void b3Map(b3_vbo_mapping map_mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3Draw();
};

class B3_PLUGIN b3VboPolygonElements : public b3PolygonElements, protected b3VBO
{
public:
	             b3VboPolygonElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount);
	virtual void b3FreeVertexMemory();
	virtual void b3Map(b3_vbo_mapping map_mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3Draw();
};

#endif
