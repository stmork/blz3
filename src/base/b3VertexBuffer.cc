/*
**
**	$Filename:	b3VertexBuffer.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3VertexBuffer.h"
#include "blz3/system/b3Mem.h"

/*************************************************************************
**                                                                      **
**                        b3VertexBuffer implementation                 **
**                                                                      **
*************************************************************************/

void b3VertexBuffer::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);
	glBound = true;
}

void b3VertexBuffer::b3Unmap()
{
	B3_ASSERT(glBound);
	glBound = false;
}

void b3VertexBuffer::b3CustomData()
{
}

void b3VertexBuffer::b3Draw()
{
}

/*************************************************************************
**                                                                      **
**                        b3SimpleVertexElements implementation         **
**                                                                      **
*************************************************************************/

b3SimpleVertexElements::~b3SimpleVertexElements()
{
	b3FreeVertexMemory();
}

void b3SimpleVertexElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		b3FreeVertexMemory();

		if (new_amount > 0)
		{
			glElementCount = new_amount;
			glVertex = (b3_gl_vertex *)b3MemAccess::b3Alloc(glElementCount * sizeof(b3_gl_vertex));
		}
		glComputed = false;
	}
}

void b3SimpleVertexElements::b3FreeVertexMemory()
{
	if ((glVertex != null) && (!glCustom))
	{
		b3MemAccess::b3Free(glVertex);
		glVertex       = null;
		glElementCount = 0;
	}
}

/*************************************************************************
**                                                                      **
**                        b3SimpleGridElements implementation           **
**                                                                      **
*************************************************************************/

b3SimpleGridElements::~b3SimpleGridElements()
{
	b3FreeVertexMemory();
}

void b3SimpleGridElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		b3FreeVertexMemory();

		if (new_amount > 0)
		{
			glElementCount = new_amount;
			glGrids = (b3_gl_line *)b3MemAccess::b3Alloc(glElementCount * sizeof(b3_gl_line));
		}
		glComputed = false;
	}
}

void b3SimpleGridElements::b3FreeVertexMemory()
{
	if ((glGrids != null) && (!glCustom))
	{
		b3MemAccess::b3Free(glGrids);
		glGrids        = null;
		glElementCount = 0;
	}
}

/*************************************************************************
**                                                                      **
**                        b3SimplePolygonElements implementation        **
**                                                                      **
*************************************************************************/

b3SimplePolygonElements::~b3SimplePolygonElements()
{
	b3FreeVertexMemory();
}

void b3SimplePolygonElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		b3FreeVertexMemory();

		if (new_amount > 0)
		{
			glElementCount = new_amount;
			glPolygons = (b3_gl_polygon *)b3MemAccess::b3Alloc(glElementCount * sizeof(b3_gl_polygon));
		}
		glComputed = false;
	}
}

void b3SimplePolygonElements::b3FreeVertexMemory()
{
	if ((glPolygons != null) && (!glCustom))
	{
		b3MemAccess::b3Free(glPolygons);
		glPolygons     = null;
		glElementCount = 0;
	}
}

/*************************************************************************
**                                                                      **
**                        b3ArrayVertexElements implementation          **
**                                                                      **
*************************************************************************/

b3ArrayVertexElements::b3ArrayVertexElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_VERTEX_ARRAY);
#endif
}

void b3ArrayVertexElements::b3Draw()
{
	B3_ASSERT(glVertex != null);

#ifdef BLZ3_USE_OPENGL
	glInterleavedArrays(GL_T2F_N3F_V3F,0, glVertex);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3ArrayGridElements implementation            **
**                                                                      **
*************************************************************************/

b3ArrayGridElements::b3ArrayGridElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_INDEX_ARRAY);
#endif
}

void b3ArrayGridElements::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	glDrawElements(GL_LINES,glElementCount * 2,GL_UNSIGNED_INT,glGrids);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3ArrayPolygonElements implementation         **
**                                                                      **
*************************************************************************/

b3ArrayPolygonElements::b3ArrayPolygonElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_INDEX_ARRAY);
#endif
}

void b3ArrayPolygonElements::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	glDrawElements(GL_TRIANGLES, glElementCount * 3,GL_UNSIGNED_INT,glPolygons);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboVertexBuffer implementation              **
**                                                                      **
*************************************************************************/

b3VBO::b3VBO()
{
#ifdef BLZ3_USE_OPENGL
	glGenBuffersARB(1,&glVBO);
#endif
}

b3VBO::~b3VBO()
{
#ifdef BLZ3_USE_OPENGL
	glDeleteBuffersARB(1,&glVBO);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboVertexElements implementation            **
**                                                                      **
*************************************************************************/

b3VboVertexElements::b3VboVertexElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_VERTEX_ARRAY);
#endif
}

void b3VboVertexElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		glVertex       = null;
		glElementCount = new_amount;
		if (glElementCount > 0)
		{
#ifdef BLZ3_USE_OPENGL
			glBindBufferARB((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB((GLenum)GL_ARRAY_BUFFER_ARB,
							glElementCount * sizeof(b3_gl_vertex), NULL,
							(GLenum)GL_DYNAMIC_DRAW_ARB);
#endif
			glVertex = null;
		}
		glComputed = false;
	}
}

void b3VboVertexElements::b3FreeVertexMemory()
{
	glVertex       = null;
	glElementCount = 0;
}

void b3VboVertexElements::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	if (!glCustom)
	{
		glBindBufferARB((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
		glVertex = (b3_gl_vertex *)glMapBufferARB(
					   (GLenum)GL_ARRAY_BUFFER_ARB,
					   (GLenum)map_mode);

		B3_ASSERT(glVertex != null);
	}
#endif

	glBound = true;
}

void b3VboVertexElements::b3Unmap()
{
	B3_ASSERT(glBound);

#ifdef BLZ3_USE_OPENGL
	if ((!glCustom) && (glVBO != 0) && (glVertex != null))
	{
		glBindBufferARB ((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB((GLenum)GL_ARRAY_BUFFER_ARB);
		glVertex = null;
	}
#endif

	glBound = false;
}

void b3VboVertexElements::b3CustomData()
{
	if ((glCustom) && (glVertex != null) && (glElementCount > 0))
	{
#ifdef BLZ3_USE_OPENGL
		glBindBufferARB((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB((GLenum)GL_ARRAY_BUFFER_ARB,
						glElementCount * sizeof(b3_gl_vertex), glVertex,
						(GLenum)GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboVertexElements::b3Draw()
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	glBindBufferARB((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
	glInterleavedArrays(GL_T2F_N3F_V3F, 0, 0);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboGridElements implementation              **
**                                                                      **
*************************************************************************/

b3VboGridElements::b3VboGridElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_INDEX_ARRAY);
#endif
}

void b3VboGridElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		glGrids        = null;
		glElementCount = new_amount;
		if (glElementCount > 0)
		{
#ifdef BLZ3_USE_OPENGL
			glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
							glElementCount * sizeof(b3_gl_line), NULL,
							(GLenum)GL_DYNAMIC_DRAW_ARB);
#endif
			glGrids = null;
		}
		glComputed = false;
	}
}

void b3VboGridElements::b3FreeVertexMemory()
{
	glGrids        = null;
	glElementCount = 0;
}

void b3VboGridElements::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	if ((!glCustom) && (glElementCount > 0))
	{
		glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glGrids = (b3_gl_line *)glMapBufferARB(
					  (GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
					  (GLenum)map_mode);

		B3_ASSERT(glGrids != null);
	}
#endif

	glBound = true;
}

void b3VboGridElements::b3Unmap()
{
	B3_ASSERT(glBound);

#ifdef BLZ3_USE_OPENGL
	if ((!glCustom) && (glVBO != 0) && (glGrids != null))
	{
		glBindBufferARB ((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB);
		glGrids = null;
	}
#endif

	glBound = false;
}

void b3VboGridElements::b3CustomData()
{
	if ((glCustom) && (glGrids != null) && (glElementCount > 0))
	{
#ifdef BLZ3_USE_OPENGL
		glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
						glElementCount * sizeof(b3_gl_line), glGrids,
						(GLenum)GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboGridElements::b3Draw()
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_LINES,glElementCount * 2,GL_UNSIGNED_INT, 0);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboPolygonElements implementation           **
**                                                                      **
*************************************************************************/

b3VboPolygonElements::b3VboPolygonElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_INDEX_ARRAY);
#endif
}

void b3VboPolygonElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		glPolygons     = null;
		glElementCount = new_amount;
		if (glElementCount > 0)
		{
#ifdef BLZ3_USE_OPENGL
			glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
							glElementCount * sizeof(b3_gl_polygon), NULL,
							(GLenum)GL_DYNAMIC_DRAW_ARB);
#endif
			glPolygons = null;
		}
		glComputed = false;
	}
}

void b3VboPolygonElements::b3FreeVertexMemory()
{
	glPolygons     = null;
	glElementCount = 0;
}

void b3VboPolygonElements::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	if ((!glCustom) && (glElementCount > 0))
	{
		glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glPolygons = (b3_gl_polygon *)glMapBufferARB(
						 (GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
						 (GLenum)map_mode);

		B3_ASSERT(glPolygons != null);
	}
#endif

	glBound = true;
}

void b3VboPolygonElements::b3Unmap()
{
	B3_ASSERT(glBound);

#ifdef BLZ3_USE_OPENGL
	if ((!glCustom) && (glVBO != 0) && (glPolygons != null))
	{
		glBindBufferARB ((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB);
		glPolygons = null;
	}
#endif

	glBound = false;
}

void b3VboPolygonElements::b3CustomData()
{
	if ((glCustom) && (glPolygons != null) && (glElementCount > 0))
	{
#ifdef BLZ3_USE_OPENGL
		glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
						glElementCount * sizeof(b3_gl_polygon), glPolygons,
						(GLenum)GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboPolygonElements::b3Draw()
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_TRIANGLES, glElementCount * 3,GL_UNSIGNED_INT,0);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboStaticVertexElements implementation      **
**                                                                      **
*************************************************************************/

b3VboStaticVertexElements::b3VboStaticVertexElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_VERTEX_ARRAY);
#endif
}

void b3VboStaticVertexElements::b3CustomData()
{
	if ((glVertex != null) && (glElementCount > 0))
	{
#ifdef BLZ3_USE_OPENGL
		glBindBufferARB((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB((GLenum)GL_ARRAY_BUFFER_ARB,
						glElementCount * sizeof(b3_gl_vertex), glVertex,
						(GLenum)GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboStaticVertexElements::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	glBindBufferARB((GLenum)GL_ARRAY_BUFFER_ARB, glVBO);
	glInterleavedArrays(GL_T2F_N3F_V3F, 0, 0);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboStaticGridElements implementation        **
**                                                                      **
*************************************************************************/

b3VboStaticGridElements::b3VboStaticGridElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_INDEX_ARRAY);
#endif
}

void b3VboStaticGridElements::b3CustomData()
{
	if ((glGrids != null) && (glElementCount > 0))
	{
#ifdef BLZ3_USE_OPENGL
		glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
						glElementCount * sizeof(b3_gl_line), glGrids,
						(GLenum)GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboStaticGridElements::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_LINES,glElementCount * 2,GL_UNSIGNED_INT, 0);
#endif
}

/*************************************************************************
**                                                                      **
**                        b3VboStaticPolygonElements implementation     **
**                                                                      **
*************************************************************************/

b3VboStaticPolygonElements::b3VboStaticPolygonElements()
{
#ifdef BLZ3_USE_OPENGL
	glEnableClientState(GL_INDEX_ARRAY);
#endif
}

void b3VboStaticPolygonElements::b3CustomData()
{
	if ((glPolygons != null) && (glElementCount > 0))
	{
#ifdef BLZ3_USE_OPENGL
		glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB,
						glElementCount * sizeof(b3_gl_polygon), glPolygons,
						(GLenum)GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboStaticPolygonElements::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	glBindBufferARB((GLenum)GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_TRIANGLES, glElementCount * 3,GL_UNSIGNED_INT,0);
#endif
}
