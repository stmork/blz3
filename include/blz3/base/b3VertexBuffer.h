/*
**
**	$Filename:	b3VertexBuffer.h $ 
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

#ifndef B3_BASE_VERTEXBUFFER_H
#define B3_BASE_VERTEXBUFFER_H

#include "blz3/b3Config.h"
#include "blz3/base/b3RenderTypes.h"
#include "blz3/base/b3VectorBufferObjects.h"

#ifdef BLZ3_USE_OPENGL

/**
 * This enumeration defines the memory access rules for VBOs.
 */
enum b3_vbo_mapping
{
	B3_MAP_VBO_RW = GL_READ_WRITE_ARB,
	B3_MAP_VBO_R  = GL_READ_ONLY_ARB,
	B3_MAP_VBO_W  = GL_WRITE_ONLY_ARB
};

#else

/**
* This enumeration defines the memory access rules for VBOs.
*/
enum b3_vbo_mapping
{
	B3_MAP_VBO_RW,
	B3_MAP_VBO_R,
	B3_MAP_VBO_W
};

#endif

class b3RenderContext;

/**
 * This class defines the behaviour of VBOs.
 *
 * @see b3RenderObject
 */
class B3_PLUGIN b3VertexBuffer
{
protected:
	b3_count     glElementCount; //!< This is the amount of handled data elements.
	b3_bool      glBound;        //!< This flag signals if the data is mapped into the CPUs address space.
	b3_bool      glComputed;     //!< This flag signals if the data is up to date.
	b3_bool      glCustom;       //!< This flag signals application handled memory or API handled memory.

public:
	/**
	 * This constructor initializes the vertex buffer elements of this instance.
	 */
	inline       b3VertexBuffer()
	{
		glElementCount = 0;
		glBound        = false;
		glComputed     = false;
		glCustom       = false;
	}

	/**
	 * This method maps the vertex data for access into the CPUs address space.
	 * After that the vertex data is accessible.
	 *
	 * @param mode The access mode to the data.
	 */
	virtual void b3Map(b3_vbo_mapping mode = B3_MAP_VBO_RW);

	/**
	 * This method unmaps the data so that the data is not accessible to
	 * the CPU any more.
	 */
	virtual void b3Unmap();

	/**
	 * This method is called to handle application controlled data and send it
	 * to the OpenGL API.
	 */
	virtual void b3CustomData();

	/**
	 * This method allocates enough memory for the given amount of vertices.
	 *
	 * @param context The render context to use.
	 * @param amount  The new amount of vertices.
	 */
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count amount) = 0;

	/**
	 * This method frees the VBO memory.
	 */
	virtual void b3FreeVertexMemory() = 0;

	/**
	 * This method draws the vertex buffer object.
	 */
	virtual void b3Draw();

	/**
	 * This method returns the allocated amount of objects.
	 *
	 * @return The amount of VBOs.
	 */
	inline b3_count b3GetCount()
	{
		return glElementCount;
	}

	/**
	 * This method sets the amount of data elements. The call
	 * may be necessary if you set application specific custom
	 * data.
	 *
	 * @param count The new amount of data elements.
	 */
	inline void b3SetCount(b3_count count)
	{
		glElementCount = count;
	}

	/**
	 * This method returns true if no element is allocated.
	 *
	 * @return True if not element is allocated.
	 */
	inline b3_bool b3IsEmpty()
	{
		return glElementCount <= 0;
	}

	/**
	 * This sets the custom state of this buffer object. If the state
	 * should be true the application must provide the data memory.
	 * This could be a static index field.
	 *
	 * @param custom The custom state.
	 */
	inline void b3SetCustom(b3_bool custom = true)
	{
		glCustom = custom;
	}

	/**
	 * This method returns the custom state.
	 *
	 * @return True if thie instance uses custom data elements.
	 */
	inline b3_bool b3IsCustom()
	{
		return glCustom;
	}

	/**
	 * This method is called to signal recomputation of the data.
	 */
	inline void b3Recompute()
	{
		glComputed = false;
	}

	/**
	 * This method determines if the data is actual.
	 *
	 * @return True if the data is actual.
	 */
	inline b3_bool b3IsComputed()
	{
		return glComputed;
	}

	/**
	 * This method is called to signal that the recomputation of
	 * the data is complete.
	 */
	inline void b3Recomputed()
	{
		if (!glComputed)
		{
			b3CustomData();
		}
		glComputed = true;
	}
};

/**
 * This class handles vertex data as VBO.
 */
class B3_PLUGIN b3VertexElements : public b3VertexBuffer
{
protected:
	b3_gl_vertex *glVertex; //!< The pointer to the vertex data.

public:
	/**
	 * This constructor initializes the vertex data.
	 */
	inline b3VertexElements()
	{
		glVertex = null;
	}

	/**
	 * This method sets a pointer to custom vertex data.
	 *
	 * @param vertices The vertex data.
	 */
	inline void b3SetVertices(b3_gl_vertex *vertices)
	{
		glVertex = vertices;
	}

	/**
	 * This cast operator returns a pointer to the vertex data.
	 */
	inline operator b3_gl_vertex *()
	{
		return glVertex;
	}
};

/**
 * This class handles line index data as VBO.
 */
class B3_PLUGIN b3GridElements : public b3VertexBuffer
{
protected:
	b3_gl_line   *glGrids; //!< The pointer to the line index data.

public:
	/**
	 * This constructor initializes the line index data.
	 */
	inline b3GridElements()
	{
		glGrids = null;
	}

	/**
	 * This method sets a pointer to custom line index data.
	 *
	 * @param grids The line index data.
	 */
	inline void b3SetGrids(b3_gl_line *grids)
	{
		glGrids = grids;
	}

	/**
	 * This cast operator returns a pointer to the line index data.
	 */
	inline operator b3_gl_line *()
	{
		return glGrids;
	}
};

/**
 * This class handles polygon index data as VBO.
 */
class B3_PLUGIN b3PolygonElements : public b3VertexBuffer
{
protected:
	b3_gl_polygon  *glPolygons; //!< The pointer to the polygon index data.

public:
	/**
	 * This constructor initializes the polygon index data.
	 */
	inline b3PolygonElements()
	{
		glPolygons = null;
	}

	/**
	 * This method sets a pointer to custom polygon index data.
	 *
	 * @param polygons The polygon index data.
	 */
	inline void b3SetPolygons(b3_gl_polygon *polygons)
	{
		glPolygons = polygons;
	}

	/**
	 * This cast operator returns a pointer to the polygon index data.
	 */
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

/**
 * This class handles vertex data without drawing them in any rendering API.
 */
class B3_PLUGIN b3SimpleVertexElements : public b3VertexElements
{
public:
	/**
	 * This destructor deinitializes this instance.
	 */
	virtual      ~b3SimpleVertexElements();
	virtual void  b3AllocVertexMemory(b3RenderContext *context,b3_count amount);
	virtual void  b3FreeVertexMemory();
};

/**
 * This class handles line index vertex data without drawing them in any rendering API.
 */
class B3_PLUGIN b3SimpleGridElements : public b3GridElements
{
public:
	/**
	 * This destructor deinitializes this instance.
	 */
	virtual      ~b3SimpleGridElements();
	virtual void  b3AllocVertexMemory(b3RenderContext *context,b3_count amount);
	virtual void  b3FreeVertexMemory();
};

/**
 * This class handles polygon index vertex data without drawing them in any rendering API.
 */
class B3_PLUGIN b3SimplePolygonElements : public b3PolygonElements
{
public:
	/**
	 * This destructor deinitializes this instance.
	 */
	virtual      ~b3SimplePolygonElements();
	virtual void  b3AllocVertexMemory(b3RenderContext *context,b3_count amount);
	virtual void  b3FreeVertexMemory();
};

/*************************************************************************
**                                                                      **
**                        Standard OpenGL VertexArray implementation    **
**                                                                      **
*************************************************************************/

/**
 * This class handles vertex data by using simple vertex array drawing methods of the OpenGL API.
 */
class B3_PLUGIN b3ArrayVertexElements : public b3SimpleVertexElements
{
public:
	b3ArrayVertexElements();
	virtual void b3Draw();
};

/**
 * This class handles line index data by using simple vertex array drawing methods of the OpenGL API.
 */
class B3_PLUGIN b3ArrayGridElements : public b3SimpleGridElements
{
public:
	b3ArrayGridElements();
	virtual void b3Draw();
};

/**
 * This class handles polygon index data by using simple vertex array drawing methods of the OpenGL API.
 */
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

/**
 * This class simply contains a VBO reference id.
 */
class B3_PLUGIN b3VBO : protected b3VectorBufferObjects
{
protected:
#ifdef BLZ3_USE_OPENGL
	GLuint glVBO; //!< The VBO id.
#endif

public:
	/**
	 * This constructor allocates an OpenGL VBO.
	 */
	b3VBO();

	/**
	 * This destructor frees the OpenGL VBO.
	 */
	virtual      ~b3VBO();
};

/*************************************************************************
**                                                                      **
**                        VBO dynamic version                           **
**                                                                      **
*************************************************************************/

/**
 * This class handles vertex data by using the VBO functionality of OpenGL.
 */
class B3_PLUGIN b3VboVertexElements : public b3VertexElements, protected b3VBO
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3VboVertexElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count amount);
	virtual void b3FreeVertexMemory();
	virtual void b3Map(b3_vbo_mapping mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3Draw();
};

/**
 * This class handles line index data by using the VBO functionality of OpenGL.
 */
class B3_PLUGIN b3VboGridElements : public b3GridElements, protected b3VBO
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3VboGridElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count amount);
	virtual void b3FreeVertexMemory();
	virtual void b3Map(b3_vbo_mapping mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3Draw();
};

/**
 * This class handles polygon index data by using the VBO functionality of OpenGL in dynamic mapping mode.
 */
class B3_PLUGIN b3VboPolygonElements : public b3PolygonElements, protected b3VBO
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3VboPolygonElements();
	virtual void b3AllocVertexMemory(b3RenderContext *context,b3_count amount);
	virtual void b3FreeVertexMemory();
	virtual void b3Map(b3_vbo_mapping mode = B3_MAP_VBO_RW);
	virtual void b3Unmap();
	virtual void b3CustomData();
	virtual void b3Draw();
};

/*************************************************************************
**                                                                      **
**                        VBO static version                            **
**                                                                      **
*************************************************************************/

/**
 * This class handles vertex data by using the VBO functionality of OpenGL in static mapping mode.
 */
class B3_PLUGIN b3VboStaticVertexElements : public b3SimpleVertexElements, protected b3VBO
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3VboStaticVertexElements();
	virtual void b3CustomData();
	virtual void b3Draw();
};

/**
 * This class handles line index data by using the VBO functionality of OpenGL in static mapping mode.
 */
class B3_PLUGIN b3VboStaticGridElements : public b3SimpleGridElements, protected b3VBO
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3VboStaticGridElements();
	virtual void b3CustomData();
	virtual void b3Draw();
};

/**
 * This class handles polygon index data by using the VBO functionality of OpenGL in static mapping mode.
 */
class B3_PLUGIN b3VboStaticPolygonElements : public b3SimplePolygonElements, protected b3VBO
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3VboStaticPolygonElements();
	virtual void b3CustomData();
	virtual void b3Draw();
};

#endif
