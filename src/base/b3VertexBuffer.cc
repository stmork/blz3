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
**	Revision 1.3  2004/11/23 09:01:10  smork
**	- Bump revision
**	- Added non OpenGL compiling for VBOs.
**
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

b3SimpleVertexElements::b3SimpleVertexElements()
{
	glVertex = null;
}

void b3SimpleVertexElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
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

void b3SimpleVertexElements::b3FreeVertexMemory()
{
	b3Free(glVertex);
	glVertex       = null;
	glElementCount = 0;
}

/*************************************************************************
**                                                                      **
**                        b3SimpleGridElements implementation           **
**                                                                      **
*************************************************************************/

b3SimpleGridElements::b3SimpleGridElements()
{
	glGrids = null;
}

void b3SimpleGridElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
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

void b3SimpleGridElements::b3FreeVertexMemory()
{
	b3Free(glGrids);
	glGrids        = null;
	glElementCount = 0;
}

/*************************************************************************
**                                                                      **
**                        b3SimplePolygonElements implementation        **
**                                                                      **
*************************************************************************/

b3SimplePolygonElements::b3SimplePolygonElements()
{
	glPolygons = null;
}

void b3SimplePolygonElements::b3AllocVertexMemory(b3RenderContext *context,b3_count new_amount)
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

void b3SimplePolygonElements::b3FreeVertexMemory()
{
	b3Free(glPolygons);
	glPolygons     = null;
	glElementCount = 0;
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
**                        b3VboVertexBuffer implementation               **
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
**                        b3VertexElements implementation               **
**                                                                      **
*************************************************************************/

b3VboVertexElements::b3VboVertexElements()
{
	glVertex = null;
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
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,
				glElementCount * sizeof(b3_gl_vertex), NULL, GL_DYNAMIC_DRAW_ARB);
#endif
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

void b3VboVertexElements::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL  
	if (!glCustom)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
		glVertex = (b3_gl_vertex *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, map_mode);

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
		glBindBufferARB (GL_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
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
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,
			glElementCount * sizeof(b3_gl_vertex), glVertex, GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboVertexElements::b3Draw()
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, glVBO);
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
	glGrids = null;
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
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
				glElementCount * sizeof(b3_gl_line), NULL, GL_STATIC_DRAW_ARB);
#endif
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

void b3VboGridElements::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL  
	if ((!glCustom) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glGrids = (b3_gl_line *)glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, map_mode);

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
		glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
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
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
			glElementCount * sizeof(b3_gl_line), glGrids, GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboGridElements::b3Draw()
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
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
	glPolygons = null;
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
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
				glElementCount * sizeof(b3_gl_polygon), NULL, GL_STATIC_DRAW_ARB);
#endif
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

void b3VboPolygonElements::b3Map(b3_vbo_mapping map_mode)
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL  
	if ((!glCustom) && (glElementCount > 0))
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glPolygons = (b3_gl_polygon *)glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, map_mode);

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
		glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
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
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
			glElementCount * sizeof(b3_gl_polygon), glPolygons, GL_STATIC_DRAW_ARB);
#endif
	}
}

void b3VboPolygonElements::b3Draw()
{
	B3_ASSERT(!glBound);

#ifdef BLZ3_USE_OPENGL
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glVBO);
	glDrawElements(GL_TRIANGLES, glElementCount * 3,GL_UNSIGNED_INT,0);
#endif
}
