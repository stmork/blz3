/*
**
**	$Filename:	b3VertexBuffer.cc $ 
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3VertexBuffer.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/11/21 14:56:58  sm
**	- Merged VBO development into main trunk.
**
**	Revision 1.1.2.5  2004/11/21 10:17:32  sm
**	- We have to map the object before getting the pointer. Then the
**	  bounding boxes can be computed correctly to setup the far and
**	  near clipping plane correctly. When mapping correctly the
**	  transformation can occur correctly which plays the ananimation
**	  in a way we expect ;-)
**	  ** That's it **
**	
**	Revision 1.1.2.4  2004/11/20 13:24:17  sm
**	- Searching for VBO bug.
**	
**	Revision 1.1.2.3  2004/11/20 13:22:09  sm
**	- Searching VBO bug.
**	
**	Revision 1.1.2.2  2004/11/20 11:37:15  sm
**	- Added Windows VBO support
**	
**	Revision 1.1.2.1  2004/11/20 05:50:57  sm
**	- Added VBO implementation of different array/index objects.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3VertexBuffer implementation                 **
**                                                                      **
*************************************************************************/

b3VertexBuffer::b3VertexBuffer()
{
	glElementCount = 0;
	glBound        = false;
	glComputed     = false;
	glCustom       = false;
}

void b3VertexBuffer::b3Map(GLenum map_mode)
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

/*************************************************************************
**                                                                      **
**                        b3VertexElements implementation               **
**                                                                      **
*************************************************************************/

b3ArrayVertexElements::b3ArrayVertexElements()
{
	glVertex = null;
	glEnableClientState(GL_VERTEX_ARRAY);
}

void b3ArrayVertexElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		b3Free(glVertex);
		glVertex       = null;
		glElementCount = new_amount;

		if (glElementCount > 0)
		{
			glVertex = (b3_gl_vertex *)b3Alloc(glElementCount * sizeof(b3_gl_vertex));
		}
		glComputed = false;
	}
}

void b3ArrayVertexElements::b3FreeVertexMemory()
{
	b3Free(glVertex);
	glVertex       = null;
	glElementCount = 0;
}

void b3ArrayVertexElements::b3Draw()
{
	B3_ASSERT(glVertex != null);

	glInterleavedArrays(GL_T2F_N3F_V3F,0, glVertex);
}

/*************************************************************************
**                                                                      **
**                        b3GridElements implementation                 **
**                                                                      **
*************************************************************************/

b3ArrayGridElements::b3ArrayGridElements()
{
	glGrids = null;
	glEnableClientState(GL_INDEX_ARRAY);
}

void b3ArrayGridElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		b3Free(glGrids);
		glGrids        = null;
		glElementCount = new_amount;

		if (glElementCount > 0)
		{
			glGrids = (b3_gl_line *)b3Alloc(glElementCount * sizeof(b3_gl_line));
		}
		glComputed = false;
	}
}

void b3ArrayGridElements::b3FreeVertexMemory()
{
	b3Free(glGrids);
	glGrids        = null;
	glElementCount = 0;
}

void b3ArrayGridElements::b3Draw()
{
	glDrawElements(GL_LINES,glElementCount * 2,GL_UNSIGNED_INT,glGrids);
}

/*************************************************************************
**                                                                      **
**                        b3PolygonElements implementation              **
**                                                                      **
*************************************************************************/

b3ArrayPolygonElements::b3ArrayPolygonElements()
{
	glPolygons = null;
	glEnableClientState(GL_INDEX_ARRAY);
}

void b3ArrayPolygonElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		b3Free(glPolygons);
		glPolygons     = null;
		glElementCount = new_amount;

		if (glElementCount > 0)
		{
			glPolygons = (b3_gl_polygon *)b3Alloc(glElementCount * sizeof(b3_gl_polygon));
		}
		glComputed = false;
	}
}

void b3ArrayPolygonElements::b3FreeVertexMemory()
{
	b3Free(glPolygons);
	glPolygons     = null;
	glElementCount = 0;
}

void b3ArrayPolygonElements::b3Draw()
{
	glDrawElements(GL_TRIANGLES, glElementCount * 3,GL_UNSIGNED_INT,glPolygons);
}

/*************************************************************************
**                                                                      **
**                        b3VboVertexBuffer implementation               **
**                                                                      **
*************************************************************************/

b3VBO::b3VBO()
{
	glGenBuffersARB(1,&glVBO);
}

b3VBO::~b3VBO()
{
	glDeleteBuffersARB(1,&glVBO);
}

/*************************************************************************
**                                                                      **
**                        b3VertexElements implementation               **
**                                                                      **
*************************************************************************/

b3VboVertexElements::b3VboVertexElements()
{
	glVertex = null;
	glEnableClientState(GL_VERTEX_ARRAY);
}

void b3VboVertexElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		glVertex       = null;
		glElementCount = new_amount;

		if (glElementCount > 0)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,
				glElementCount * sizeof(b3_gl_vertex), NULL, GL_DYNAMIC_DRAW_ARB);
			glVertex = null;
		}
		glComputed = false;
	}
}

void b3VboVertexElements::b3FreeVertexMemory()
{
	b3Free(glVertex);
	glVertex       = null;
	glElementCount = 0;
}

void b3VboVertexElements::b3Map(GLenum map_mode)
{
	B3_ASSERT(!glBound);
	if (!glCustom)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
		glVertex = (b3_gl_vertex *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, map_mode);
		B3_ASSERT(glVertex != null);
	}
	glBound = true;
}

void b3VboVertexElements::b3Unmap()
{
	B3_ASSERT(glBound);

	if ((!glCustom) && (glVBO != 0) && (glVertex != null))
	{
		glBindBufferARB (GL_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		glVertex = null;
	}
	glBound = false;
}

void b3VboVertexElements::b3CustomData()
{
	if ((glCustom) && (glVertex != null) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,
			glElementCount * sizeof(b3_gl_vertex), glVertex, GL_STATIC_DRAW_ARB);
	}
}

void b3VboVertexElements::b3Draw()
{
	B3_ASSERT(!glBound);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
	glInterleavedArrays(GL_T2F_N3F_V3F, 0, 0);
}

/*************************************************************************
**                                                                      **
**                        b3VboGridElements implementation              **
**                                                                      **
*************************************************************************/

b3VboGridElements::b3VboGridElements()
{
	glGrids = null;
	glEnableClientState(GL_INDEX_ARRAY);
}

void b3VboGridElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		glGrids        = null;
		glElementCount = new_amount;

		if (glElementCount > 0)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
				glElementCount * sizeof(b3_gl_line), NULL, GL_STATIC_DRAW_ARB);
				glGrids = null;
		}
		glComputed = false;
	}
}

void b3VboGridElements::b3FreeVertexMemory()
{
	b3Free(glGrids);
	glGrids        = null;
	glElementCount = 0;
}

void b3VboGridElements::b3Map(GLenum map_mode)
{
	B3_ASSERT(!glBound);
	if ((!glCustom) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glGrids = (b3_gl_line *)glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, map_mode);
		B3_ASSERT(glGrids != null);
	}
	glBound = true;
}

void b3VboGridElements::b3Unmap()
{
	B3_ASSERT(glBound);
	if ((!glCustom) && (glVBO != 0) && (glGrids != null))
	{
		glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
		glGrids = null;
	}
	glBound = false;
}

void b3VboGridElements::b3CustomData()
{
	if ((glCustom) && (glGrids != null) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
			glElementCount * sizeof(b3_gl_line), glGrids, GL_STATIC_DRAW_ARB);
	}
}

void b3VboGridElements::b3Draw()
{
	B3_ASSERT(!glBound);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_LINES,glElementCount * 2,GL_UNSIGNED_INT, 0);
}

/*************************************************************************
**                                                                      **
**                        b3VboPolygonElements implementation           **
**                                                                      **
*************************************************************************/

b3VboPolygonElements::b3VboPolygonElements()
{
	glPolygons = null;
	glEnableClientState(GL_INDEX_ARRAY);
}

void b3VboPolygonElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
{
	if (glElementCount != new_amount)
	{
		glPolygons     = null;
		glElementCount = new_amount;

		if (glElementCount > 0)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
				glElementCount * sizeof(b3_gl_polygon), NULL, GL_STATIC_DRAW_ARB);
			glPolygons = null;
		}
		glComputed = false;
	}
}

void b3VboPolygonElements::b3FreeVertexMemory()
{
	b3Free(glPolygons);
	glPolygons     = null;
	glElementCount = 0;
}

void b3VboPolygonElements::b3Map(GLenum map_mode)
{
	B3_ASSERT(!glBound);
	if ((!glCustom) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glPolygons = (b3_gl_polygon *)glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, map_mode);
		B3_ASSERT(glPolygons != null);
	}
	glBound = true;
}

void b3VboPolygonElements::b3Unmap()
{
	B3_ASSERT(glBound);
	if ((!glCustom) && (glVBO != 0) && (glPolygons != null))
	{
		glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
		glPolygons = null;
	}
	glBound = false;
}

void b3VboPolygonElements::b3CustomData()
{
	if ((glCustom) && (glPolygons != null) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
			glElementCount * sizeof(b3_gl_polygon), glPolygons, GL_STATIC_DRAW_ARB);
	}
}

void b3VboPolygonElements::b3Draw()
{
	B3_ASSERT(!glBound);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_TRIANGLES, glElementCount * 3,GL_UNSIGNED_INT,0);
}
