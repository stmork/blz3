/*
**
**	$Filename:	b3Render.cc $
**	$Release:	Dortmund 2001 - 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Rendering some objects
**
**	(C) Copyright 2001 - 2005  Steffen A. Mork
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
#include "blz3/base/b3Render.h"
#include "blz3/base/b3VertexBuffer.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Color.h"

#ifdef BLZ3_USE_OPENGL
#define noB3_DISPLAY_LIST
#define B3_DISPLAY_LIST_COUNT 1
#endif

#ifndef _DEBUG
#	define B3_MAX_TX_SIZE 128
#else
#	define B3_MAX_TX_SIZE   8
#	define no_VERBOSE
#endif

/*************************************************************************
**                                                                      **
**                        b3RenderObject implementation                 **
**                                                                      **
*************************************************************************/

b3RenderObject::b3RenderObject()
{
	b3RecomputeMaterial();

	glInit = false;

#ifdef BLZ3_USE_OPENGL
	glDisplayList     = 0;
	glTextureId       = 0;
	glTextureData     = nullptr;
	glTextureSize     = 0;
	glTextureSizeX    = 0;
	glTextureSizeY    = 0;
	glVertexElements  = nullptr;
	glGridElements    = nullptr;
	glPolygonElements = nullptr;
#endif
}

b3RenderObject::~b3RenderObject()
{
	if (glVertexElements != nullptr)
	{
		delete glVertexElements;
	}
	if (glGridElements != nullptr)
	{
		delete glGridElements;
	}
	if (glPolygonElements != nullptr)
	{
		delete glPolygonElements;
	}
	b3CreateTexture(nullptr, 0);
	b3DeleteDisplayList();
}

void b3RenderObject::b3DeleteDisplayList()
{
#ifdef B3_DISPLAY_LIST
	if (glDisplayList != 0)
	{
		glDeleteLists(glDisplayList, B3_DISPLAY_LIST_COUNT);
		glDisplayList = 0;
	}
#endif
}

void b3RenderObject::b3GetCount(
	b3RenderContext * context B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	vertCount = 0;
	gridCount = 0;
	polyCount = 0;
}

void b3RenderObject::b3AddCount(b3RenderContext * context)
{
	context->glVertexCount += glVertexElements->b3GetCount();
	context->glPolyCount   += glPolygonElements->b3GetCount();
	context->glGridCount   += glGridElements->b3GetCount();
}

/*************************************************************************
**                                                                      **
**                        Vertex/normal computation                     **
**                                                                      **
*************************************************************************/

void b3RenderObject::b3PrintMapping(const char * text, b3_vbo_mapping map_mode)
{
	const char * mapping_text;

	switch (map_mode)
	{
	case B3_MAP_VBO_R:
		mapping_text = "B3_MAP_VBO_R";
		break;

	case B3_MAP_VBO_W:
		mapping_text = "B3_MAP_VBO_W";
		break;

	case B3_MAP_VBO_RW:
		mapping_text = "B3_MAP_VBO_RW";
		break;

	default:
		mapping_text = "???";
		break;
	}
	b3PrintF(B3LOG_FULL, "      %s(%s)\n", text, mapping_text);
}

#ifdef VERBOSE
#define print_mapping(t,m) b3PrintMapping(t,m)
#else
#define print_mapping(t,m)
#endif

void b3RenderObject::b3MapIndices(b3_vbo_mapping map_mode)
{
	print_mapping(" b3MapIndices", map_mode);

	glGridElements->b3Map(map_mode);
	glPolygonElements->b3Map(map_mode);
}

void b3RenderObject::b3MapVertices(b3_vbo_mapping map_mode)
{
	print_mapping(" b3MapVertices", map_mode);

	B3_ASSERT(glVertexElements != null);
	glVertexElements->b3Map(map_mode);
}

void b3RenderObject::b3UnmapIndices()
{
	B3_ASSERT(glGridElements != null);
	glGridElements->b3Unmap();

	B3_ASSERT(glPolygonElements != null);
	glPolygonElements->b3Unmap();
}

void b3RenderObject::b3UnmapVertices()
{
	B3_ASSERT(glVertexElements != null);
	glVertexElements->b3Unmap();
}

void b3RenderObject::b3Recompute()
{
	if (glVertexElements != nullptr)
	{
		glVertexElements->b3Recompute();
	}
}

void b3RenderObject::b3RecomputeIndices()
{
	if (glGridElements != nullptr)
	{
		glGridElements->b3Recompute();
	}
	if (glPolygonElements != nullptr)
	{
		glPolygonElements->b3Recompute();
	}
}

void b3RenderObject::b3SetupVertexMemory(b3RenderContext * context)
{
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### >b3RenderObject::b3SetupVertexMemory() this = %p\n", this);
#endif

	b3PreAlloc();
	b3AllocVertexMemory(context);

#ifdef VERBOSE
	b3MapVertices(B3_MAP_VBO_R);
	b3MapIndices(B3_MAP_VBO_R);

	b3_gl_vertex * glVertex   = *glVertexElements;
	b3_gl_line  *  glGrids    = *glGridElements;
	b3_gl_polygon * glPolygons = *glPolygonElements;

	b3PrintF(B3LOG_FULL, "       %5d vertices: %p - %s\n",
		glVertexElements->b3GetCount(), glVertex,
		glVertexElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL, "       %5d grids:    %p - %s\n",
		glGridElements->b3GetCount(), glGrids,
		glGridElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL, "       %5d polygons: %p - %s\n",
		glPolygonElements->b3GetCount(), glPolygons,
		glPolygonElements->b3IsCustom() ? "custom" : "buffer");

	b3UnmapVertices();
	b3UnmapIndices();

	b3PrintF(B3LOG_FULL, "##### <b3RenderObject::b3SetupVertexMemory() this = %p\n", this);
#endif
}

void b3RenderObject::b3PreAlloc()
{
	if (!glInit)
	{
		if (b3RenderContext::b3IsGL())
		{
			if (b3VectorBufferObjects::b3AllowVBO())
			{
#if 0
				glVertexElements  = new b3VboVertexElements();
				glGridElements    = new b3VboGridElements();
				glPolygonElements = new b3VboPolygonElements();
#else
				glVertexElements  = new b3VboStaticVertexElements();
				glGridElements    = new b3VboStaticGridElements();
				glPolygonElements = new b3VboStaticPolygonElements();
#endif
			}
			else
			{
				glVertexElements  = new b3ArrayVertexElements();
				glGridElements    = new b3ArrayGridElements();
				glPolygonElements = new b3ArrayPolygonElements();
			}
		}
		else
		{
			glVertexElements  = new b3SimpleVertexElements();
			glGridElements    = new b3SimpleGridElements();
			glPolygonElements = new b3SimplePolygonElements();
		}
		glInit = true;
	}
}

void b3RenderObject::b3AllocVertexMemory(b3RenderContext * context)
{
	b3_count new_vertCount = 0;
	b3_count new_gridCount = 0;
	b3_count new_polyCount = 0;

	b3GetCount(context, new_vertCount, new_gridCount, new_polyCount);

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "       b3GetCount(NV=%d, NG=%d, NP=%d)\n",
		new_vertCount, new_gridCount, new_polyCount);
	b3PrintF(B3LOG_FULL, "      >b3AllocVertexMemory( V=%d,  G=%d,  P=%d)\n",
		glVertexElements->b3GetCount(),
		glGridElements->b3GetCount(),
		glPolygonElements->b3GetCount());
#endif

	glVertexElements->b3AllocVertexMemory(context, new_vertCount);
	glVertexElements->b3SetCustom(new_vertCount <= 0);

	glGridElements->b3AllocVertexMemory(context, new_gridCount);
	glGridElements->b3SetCustom(new_gridCount <= 0);

	glPolygonElements->b3AllocVertexMemory(context, new_polyCount);
	glPolygonElements->b3SetCustom(new_polyCount <= 0);
}

void b3RenderObject::b3FreeVertexMemory()
{
	glVertexElements->b3FreeVertexMemory();
	glGridElements->b3FreeVertexMemory();
	glPolygonElements->b3FreeVertexMemory();
}

#define B3_UPDATE_INDICES  1
#define B3_UPDATE_VERTICES 2

void b3RenderObject::b3Update()
{
	int mode = 0;

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### >b3RenderObject::b3Update() this = %p\n", this);
#endif
	if ((!glGridElements->b3IsComputed()) || (!glPolygonElements->b3IsComputed()))
	{
		mode |= B3_UPDATE_INDICES;
	}
	if (!glVertexElements->b3IsComputed())
	{
		mode |= B3_UPDATE_VERTICES;
	}

	switch (mode)
	{
	case B3_UPDATE_INDICES:
		b3MapIndices(B3_MAP_VBO_W);
		break;

	case B3_UPDATE_VERTICES :
		b3MapVertices(B3_MAP_VBO_RW);
		b3MapIndices(B3_MAP_VBO_R);
		break;

	case B3_UPDATE_INDICES | B3_UPDATE_VERTICES:
		b3MapVertices(B3_MAP_VBO_RW);
		b3MapIndices(B3_MAP_VBO_RW);
		break;
	}

	if (mode & B3_UPDATE_INDICES)
	{
#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "      >b3RenderObject::b3UpdateIndices()\n");

		b3_gl_line  *  glGrids    = *glGridElements;
		b3_gl_polygon * glPolygons = *glPolygonElements;

		b3PrintF(B3LOG_FULL, "       %5d grids:    %p - %s\n",
			glGridElements->b3GetCount(), glGrids,
			glGridElements->b3IsCustom() ? "custom" : "buffer");
		b3PrintF(B3LOG_FULL, "       %5d polygons: %p - %s\n",
			glPolygonElements->b3GetCount(), glPolygons,
			glPolygonElements->b3IsCustom() ? "custom" : "buffer");
#endif

		b3ComputeIndices();

		glGridElements->b3Recomputed();
		glPolygonElements->b3Recomputed();

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "      <b3RenderObject::b3UpdateIndices()\n");
#endif
	}

	if (mode & B3_UPDATE_VERTICES)
	{
#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "      >b3RenderObject::b3UpdateVertices()\n");

		b3_gl_vertex * glVertex   = *glVertexElements;

		b3PrintF(B3LOG_FULL, "       %5d vertices: %p - %s\n",
			glVertexElements->b3GetCount(), glVertex,
			glVertexElements->b3IsCustom() ? "custom" : "buffer");
#endif
		b3ComputeVertices();
		b3ComputeNormals();

		glVertexElements->b3Recomputed();

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "      <b3RenderObject::b3UpdateVertices()\n");
#endif
	}

	switch (mode)
	{
	case B3_UPDATE_VERTICES | B3_UPDATE_INDICES:
	case B3_UPDATE_VERTICES:
		b3UnmapVertices();
		B3_FALLTHROUGH;
	case B3_UPDATE_INDICES:
		b3UnmapIndices();
		break;
	}

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### <b3RenderObject::b3Update()\n");
#endif
}

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}

void b3RenderObject::b3ComputeNormals(b3_bool normalize)
{
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "        >b3RenderObject::b3ComputeNormals()\n");
#endif

	b3_gl_vector   normal;
	b3_gl_vector   xDir, yDir;
	b3_gl_vertex * glVertex      = *glVertexElements;
	b3_gl_polygon * glPolygons    = *glPolygonElements;
	b3_count       glVertexCount =  glVertexElements->b3GetCount();
	b3_count       glGridCount   =  glGridElements->b3GetCount();
	b3_count       glPolyCount   =  glPolygonElements->b3GetCount();
	b3_index       i, start, end, v1, v2, v3;

#ifdef VERBOSE
	b3_gl_line  *  glGrids    = *glGridElements;

	b3PrintF(B3LOG_FULL, "       %5d vertices: %p - %s\n",
		glVertexElements->b3GetCount(), glVertex,
		glVertexElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL, "       %5d grids:    %p - %s\n",
		glGridElements->b3GetCount(), glGrids,
		glGridElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL, "       %5d polygons: %p - %s\n",
		glPolygonElements->b3GetCount(), glPolygons,
		glPolygonElements->b3IsCustom() ? "custom" : "buffer");
#endif

	// Clear normals
	b3GetVertexRange(start, end);
	for (i = start; i < end; i++)
	{
		glVertex[i].n.x =
			glVertex[i].n.y =
				glVertex[i].n.z = 0;
	}

	// Collect normals
	for (i = 0; i < glPolyCount; i++)
	{
		v1 = glPolygons[i].a;
		v2 = glPolygons[i].b;
		v3 = glPolygons[i].c;

		// Do some semantic checks
		if ((v1 < start) || (v1 >= end) ||
			(v2 < start) || (v2 >= end) ||
			(v3 < start) || (v3 >= end))
		{
			b3PrintF(B3LOG_NORMAL, "###### %zd: %zd # %zd %zd %zd # %zd # (V: %zd  P: %zd  G: %zd) # (%s:l.%d)\n", i,
				start, v1, v2, v3, end, glVertexCount, glPolyCount, glGridCount, __FILE__, __LINE__);
		}
		else
		{
			b3Vector::b3Sub(&glVertex[v2].v, &glVertex[v1].v, &xDir);
			b3Vector::b3Sub(&glVertex[v3].v, &glVertex[v1].v, &yDir);
			b3Vector::b3CrossProduct(&xDir, &yDir, &normal);
			if (b3Vector::b3Normalize(&normal) > 1e-5)
			{
				b3Vector::b3Add(&normal, &glVertex[v1].n);
				b3Vector::b3Add(&normal, &glVertex[v2].n);
				b3Vector::b3Add(&normal, &glVertex[v3].n);
			}
		}
	}

	// Normalize
	if (normalize)
	{
		for (i = 0; i < glVertexCount; i++)
		{
			b3Vector::b3Normalize(&glVertex[i].n);
		}
	}
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "        <b3RenderObject::b3ComputeNormals()\n");
#endif
}

void b3RenderObject::b3GetVertexRange(b3_index & start, b3_index & end)
{
	start = 0;
	end   = glVertexElements->b3GetCount();
}

b3_bool b3RenderObject::b3ComputeBounds(b3_vector * lower, b3_vector * upper)
{
	b3Update();

	b3MapVertices(B3_MAP_VBO_R);

	b3_gl_vertex * glVertex = *glVertexElements;
	b3_bool       result = false;
	b3_index      i, start, end;

	if (glVertexElements->b3IsComputed() &&
		(glVertex != nullptr) &&
		(glVertexElements->b3GetCount() > 0))
	{
		b3GetVertexRange(start, end);
		for (i = start; i < end; i++)
		{
			b3Vector::b3AdjustBound(&glVertex[i].v, lower, upper);
		}
		result = true;
	}
	b3UnmapVertices();

	return result;
}

void b3RenderObject::b3TransformVertices(
	b3_matrix * transformation,
	b3_bool    is_affine)
{
	if (glVertexElements != nullptr)
	{
#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "        >b3RenderObject::b3TransformVertices(...)\n");
#endif

		b3MapVertices(B3_MAP_VBO_RW);

		b3_count      glVertexCount =  glVertexElements->b3GetCount();
		b3_gl_vertex * glVertex      = *glVertexElements;
		b3_count      i;

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "       %5d vertices: %p - %s\n",
			glVertexElements->b3GetCount(), glVertex,
			glVertexElements->b3IsCustom() ? "custom" : "buffer");
#endif

		if (glVertex != nullptr)
		{
			glVertexElements->b3Recompute();
			if (is_affine)
			{
				for (i = 0; i < glVertexCount; i++)
				{
					b3Vector::b3MatrixMul4D(transformation, (b3_vector *)&glVertex[i].v);
					b3Vector::b3MatrixMul3D(transformation, (b3_vector *)&glVertex[i].n);
				}
			}
			else
			{
				for (i = 0; i < glVertexCount; i++)
				{
					b3Vector::b3MatrixMul4D(transformation, (b3_vector *)&glVertex[i].v);
				}

				b3MapIndices(B3_MAP_VBO_R);
				b3ComputeNormals();
				b3UnmapIndices();
			}

			glVertexElements->b3Recomputed();
		}
		b3UnmapVertices();

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL, "        <b3RenderObject::b3TransformVertices()\n");
#endif
	}
}

/*************************************************************************
**                                                                      **
**                        Material definition                           **
**                                                                      **
*************************************************************************/

b3Tx    b3RenderObject::glTextureBuffer;
b3Mutex b3RenderObject::glTextureMutex;
b3Color b3RenderObject::m_GridColor(0.2f, 0.2f, 0.2f, 0.0f);
b3Color b3RenderObject::m_SelectedColor(1.0f, 0.1f, 0.25f, 0.0f);

void b3RenderObject::b3RecomputeMaterial()
{
#ifdef BLZ3_USE_OPENGL
	glMaterialComputed = false;
#endif
}

void b3RenderObject::b3GetDiffuseColor(b3Color & diffuse) const
{
	diffuse.b3Init(0.0f, 0.5f, 1.0f);
}

b3_f64 b3RenderObject::b3GetColors(
	b3Color & ambient,
	b3Color & diffuse,
	b3Color & specular) const
{
	ambient.b3Init(0.5, 0.5, 0.5);
	diffuse.b3Init(0.0, 0.5, 1.0);
	specular.b3Init(1.0, 1.0, 1.0);
	return 1.0;
}

b3_bool b3RenderObject::b3GetChess(
	B3_UNUSED b3Color & bColor,
	B3_UNUSED b3Color & wColor,
	B3_UNUSED b3_res & xRepeat,
	B3_UNUSED b3_res & yRepeat) const
{
	return false;
}

b3Tx * b3RenderObject::b3GetTexture(
	B3_UNUSED b3_f64 & xTrans,
	B3_UNUSED b3_f64 & yTrans,
	B3_UNUSED b3_f64 & xScale,
	B3_UNUSED b3_f64 & yScale) const
{
	return nullptr;
}

b3_bool b3RenderObject::b3GetImage(B3_UNUSED b3Tx * image) const
{
	return false;
}

void b3RenderObject::b3DefineTexture()
{
#ifdef BLZ3_USE_OPENGL
	GLfloat blend[4];

	b3RenderContext::b3PkdColorToGL(B3_TRANSPARENT | B3_WHITE, blend);

	// Set texture parameter
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE,  GL_BLEND);
	glTexEnvfv(GL_TEXTURE_2D, GL_TEXTURE_ENV_COLOR, blend);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,    GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,    GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA, glTextureSizeX, glTextureSizeY,
		0, GL_RGBA, GL_UNSIGNED_BYTE, glTextureData);
#endif
}

void b3RenderObject::b3UpdateMaterial()
{
#ifdef BLZ3_USE_OPENGL
	if (!glMaterialComputed)
	{
		b3Tx  *  tx;
		b3Color  black, white;
		b3Color  ambient, diffuse, specular;
		b3_res   xRep, yRep;

		b3DeleteDisplayList();

		glShininess = b3GetColors(ambient, diffuse, specular);
		glTextureTransX = 0;
		glTextureTransY = 0;
		if (b3GetChess(black, white, xRep, yRep))
		{
			glTextureScaleX = 0.5 * xRep;
			glTextureScaleY = 0.5 * yRep;
			b3CreateChess(nullptr, black, white);
		}
		else
		{
			b3_f64 xScale = 1;
			b3_f64 yScale = 1;

			tx = b3GetTexture(glTextureTransX, glTextureTransY, xScale, yScale);
			if ((tx != nullptr) && (tx->b3IsLoaded()))
			{
				glTextureScaleX = 1.0 / xScale;
				glTextureScaleY = 1.0 / yScale;
				b3CopyTexture(nullptr, tx);
			}
			else
			{
				glTextureScaleX = 1;
				glTextureScaleY = 1;

				// CRITICAL SECTION
				{
					b3CriticalSection lock(glTextureMutex);

					if (!glTextureBuffer.b3IsLoaded())
					{
						b3_res max  = B3_MAX_TX_SIZE;

						glTextureBuffer.b3AllocTx(max, max, 24);
					}
				}

				// CRITICAL SECTION
				{
					b3CriticalSection lock(glTextureMutex);

					if (b3GetImage(&glTextureBuffer))
					{
						b3CreateImage(nullptr, &glTextureBuffer);
					}
					else
					{
						// Free memory
						b3CreateTexture(nullptr, 0);
					}
				}
			}
		}

		if (glTextureSize > 0)
		{
			glBindTexture(GL_TEXTURE_2D, glTextureId);
			b3DefineTexture();

			// Set material parameter
			b3RenderContext::b3PkdColorToGL(B3_WHITE, glAmbient);
			b3RenderContext::b3PkdColorToGL(B3_WHITE, glDiffuse);
			b3RenderContext::b3PkdColorToGL(B3_WHITE, glSpecular);
		}
		else
		{
#if 0
			ambient = specular = diffuse;
#else
			ambient = diffuse;
			specular = B3_WHITE;
#endif

			b3RenderContext::b3ColorToGL(ambient, glAmbient);
			b3RenderContext::b3ColorToGL(diffuse, glDiffuse);
			b3RenderContext::b3ColorToGL(specular, glSpecular);
		}
		glMaterialComputed = true;
	}
#endif
}

void b3RenderObject::b3CreateTexture(
	b3RenderContext * context B3_UNUSED,
	b3_res            xSize,
	b3_res            ySize)
{
#ifdef BLZ3_USE_OPENGL
	b3_res size;

	if (ySize == 0)
	{
		ySize = xSize;
	}
	size = xSize * ySize;

	try
	{
		if (size != glTextureSize)
		{
			glGetError();
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL, "b3RenderObject::b3CreateTexture(...,%4d,%4d) # %5d previous: %5d\n",
				xSize, ySize, size, glTextureSize);
#endif
			if (size != 0)
			{
				void  * ptr = b3MemAccess::b3Alloc(size * 4);
				GLenum  error;

				if (ptr == nullptr)
				{
					B3_THROW(b3TxException, B3_TX_MEMORY);
				}
				if (glTextureData != nullptr)
				{
					b3MemAccess::b3Free(glTextureData);
				}
				glTextureData  = (GLubyte *)ptr;
				glTextureSize  =  size;
				glTextureSizeX = xSize;
				glTextureSizeY = ySize;
#ifdef VERBOSE
				b3PrintF(B3LOG_FULL, "   Allocated texture memory of %d pixel\n", size);
#endif

				if (glTextureId == 0)
				{
					glGenTextures(1, &glTextureId);
					error = glGetError();

					if (error != GL_NO_ERROR)
					{
						b3PrintF(B3LOG_NORMAL, "  glGetError() = %d\n", error);
						B3_THROW(b3TxException, B3_TX_MEMORY);
					}

					glBindTexture(GL_TEXTURE_2D, glTextureId);
					B3_ASSERT(glIsTexture(glTextureId));
#ifdef VERBOSE
					b3PrintF(B3LOG_FULL, "   Allocated texture id %d\n", glTextureId);
#endif
				}
			}
			else
			{
				if (glTextureId != 0)
				{
#ifdef VERBOSE
					b3PrintF(B3LOG_FULL, "   Freeing texture id %d\n", glTextureId);
#endif
					glDeleteTextures(1, &glTextureId);
					glTextureId = 0;
				}
#ifdef VERBOSE
				b3PrintF(B3LOG_FULL, "   Freeing texture data\n");
#endif
				if (glTextureData != nullptr)
				{
					b3MemAccess::b3Free(glTextureData);
				}
				glTextureData  = nullptr;
				glTextureSize  = 0;
				glTextureSizeX = 0;
				glTextureSizeY = 0;
			}
		}
		else
		{
			B3_ASSERT(((glTextureSize != 0) && (glTextureId != 0)) ||
				((glTextureSize == 0) && (glTextureId == 0)));
		}
	}
	catch (...)
	{
		// Restore to defined and unallocated state
		if (glTextureData != nullptr)
		{
			b3MemAccess::b3Free(glTextureData);
		}
		if (glTextureId != 0)
		{
			glDeleteTextures(1, &glTextureId);
		}
		glTextureData  = nullptr;
		glTextureId    = 0;
		glTextureSize  = 0;
		glTextureSizeX = 0;
		glTextureSizeY = 0;
		throw;
	}
#endif
}

void b3RenderObject::b3CreateChess(
	b3RenderContext * context B3_UNUSED,
	b3Color     &     bColor,
	b3Color     &     wColor)
{
#ifdef BLZ3_USE_OPENGL
	b3CreateTexture(nullptr, 2);

	b3RenderContext::b3ColorToGL(wColor, &glTextureData[ 0]);
	b3RenderContext::b3ColorToGL(bColor, &glTextureData[ 4]);
	b3RenderContext::b3ColorToGL(bColor, &glTextureData[ 8]);
	b3RenderContext::b3ColorToGL(wColor, &glTextureData[12]);
#endif
}

void b3RenderObject::b3CopyTexture(
	b3RenderContext * context,
	b3Tx      *      input)
{
#ifdef BLZ3_USE_OPENGL
	b3Tx          scale;
	b3_pkd_color * lPtr;
	b3_res        xMax = B3_MAX_TX_SIZE;
	b3_res        yMax = B3_MAX_TX_SIZE;
	b3_res        size;
	b3_coord      i = 0;

	// Limit size
	B3_ASSERT(input != null);
	while (xMax > input->xSize)
	{
		xMax /= 2;
	}
	while (yMax > input->ySize)
	{
		yMax /= 2;
	}
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "b3RenderObject::b3CopyTexture(...) # xSize: %4d ySize: %4d # xMax: %4d yMax: %4d\n",
		input->xSize, input->ySize, xMax, yMax);
#endif
	scale.b3AllocTx(xMax, yMax, 24);
	scale.b3Scale(input);
	b3CreateTexture(context, xMax, yMax);

	lPtr = scale.b3GetTrueColorData();
	size = xMax * yMax;
	for (i = 0; i < size; i++)
	{
		b3RenderContext::b3PkdColorToGL(*lPtr++, &glTextureData[i << 2]);
	}
#endif
}

void b3RenderObject::b3CreateImage(
	b3RenderContext * context,
	b3Tx      *      input)
{
#ifdef BLZ3_USE_OPENGL
	b3_pkd_color * lPtr = input->b3GetTrueColorData();
	b3_coord      size, i = 0;

	B3_ASSERT(lPtr != null);
	size = input->xSize * input->ySize;
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "b3RenderObject::b3CreateImage(...) # size: %4d\n", size);
#endif
	b3CreateTexture(context, input->xSize, input->ySize);
	for (i = 0; i < size; i++)
	{
		b3RenderContext::b3PkdColorToGL(*lPtr++, &glTextureData[i << 2]);
	}
#endif
}

/*************************************************************************
**                                                                      **
**                        Rendering (the central part)                  **
**                                                                      **
*************************************************************************/

#ifdef BLZ3_USE_OPENGL
#ifdef B3_DISPLAY_LIST

void b3RenderObject::b3Draw(b3RenderContext * context)
{
	b3_render_mode render_mode = b3GetRenderMode();

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### >b3RenderObject::b3Draw() this = %p\n", this);
#endif

	b3Update();
	if ((!glMaterialComputed) || (glDisplayList == 0))
	{
		b3UpdateMaterial();
		glDisplayList = glGenLists(B3_DISPLAY_LIST_COUNT);

		glNewList(glDisplayList, GL_COMPILE);
		b3SelectMaterialForFilledDrawing(context);
		glEndList();
	}

#ifdef _DEBUG
	b3CheckGeometry(context, render_mode);
#endif

	switch (render_mode)
	{
	case B3_RENDER_LINE:
		if (glGridElements->b3GetCount() > 0)
		{
			b3SelectMaterialForLineDrawing(context);
			b3DrawLinedGeometry(context);
		}
		break;

	case B3_RENDER_FILLED:
		if (glPolygonElements->b3GetCount() > 0)
		{
			glCallList(glDisplayList);
			b3DrawFilledGeometry(context);
		}
		break;

	default:
		// Do nothing!
		break;
	}

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### <b3RenderObject::b3Draw()\n");
#endif
}

#else

void b3RenderObject::b3Draw(b3RenderContext * context)
{
	b3_render_mode render_mode = b3GetRenderMode();

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### >b3RenderObject::b3Draw() this = %p\n", this);
#endif

	b3Update();

	if (!glMaterialComputed)
	{
		b3UpdateMaterial();
	}

#ifdef _DEBUG
	b3CheckGeometry(context, render_mode);
#endif

	b3DrawGeometry(context, render_mode);

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "##### <b3RenderObject::b3Draw()\n");
#endif
}

#endif
#else

// No drawing without OpenGL
void b3RenderObject::b3Draw(b3RenderContext * context)
{
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "#####  b3RenderObject::b3Draw()\n");
#endif
	b3Update();
}

#endif

void b3RenderObject::b3CheckGeometry(
	B3_UNUSED const b3RenderContext * context,
	const b3_render_mode    render_mode)
{
	B3_ASSERT(glVertexElements->b3IsComputed());
	B3_ASSERT(glGridElements->b3IsComputed());
	B3_ASSERT(glPolygonElements->b3IsComputed());

	b3MapVertices(B3_MAP_VBO_R);
	b3MapIndices(B3_MAP_VBO_R);

	b3_gl_vertex * glVertex   = *glVertexElements;
#ifdef _DEBUG
	b3_gl_line  *  glGrids    = *glGridElements;
	b3_gl_polygon * glPolygons = *glPolygonElements;
	b3_index       i;
#endif

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "       %5d vertices: %p - %s\n",
		glVertexElements->b3GetCount(), glVertex,
		glVertexElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL, "       %5d grids:    %p - %s\n",
		glGridElements->b3GetCount(), glGrids,
		glGridElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL, "       %5d polygons: %p - %s\n",
		glPolygonElements->b3GetCount(), glPolygons,
		glPolygonElements->b3IsCustom() ? "custom" : "buffer");
#endif

	if (glVertex != nullptr)
	{
		switch (render_mode)
		{
		case B3_RENDER_LINE:
#ifdef _DEBUG
			// This loop collects access vialoations
			// prior calling OpenGL routines. This
			// makes it possible to catch to faulty
			// index data. The access simply compute
			// the length of the lines to be drawn.
			for (i = 0; i < glGridElements->b3GetCount(); i++)
			{
				b3_vector aPoint, bPoint;
				b3_index  a, b;

				a = glGrids[i].a;
				aPoint.x = glVertex[a].v.x;
				aPoint.y = glVertex[a].v.y;
				aPoint.z = glVertex[a].v.z;

				b = glGrids[i].b;
				bPoint.x = glVertex[b].v.x;
				bPoint.y = glVertex[b].v.y;
				bPoint.z = glVertex[b].v.z;
			}
#endif
			break;

		case B3_RENDER_FILLED:
#ifdef _DEBUG
			for (i = 0; i < glPolygonElements->b3GetCount(); i++)
			{
				b3_vector aPoint, bPoint, cPoint;
				b3_index  a, b, c;

				a = glPolygons[i].a;
				aPoint.x = glVertex[a].v.x;
				aPoint.y = glVertex[a].v.y;
				aPoint.z = glVertex[a].v.z;

				b = glPolygons[i].b;
				bPoint.x = glVertex[b].v.x;
				bPoint.y = glVertex[b].v.y;
				bPoint.z = glVertex[b].v.z;

				c = glPolygons[i].c;
				cPoint.x = glVertex[c].v.x;
				cPoint.y = glVertex[c].v.y;
				cPoint.z = glVertex[c].v.z;
			}
#endif
			break;

		case B3_RENDER_NOTHING:
			// Nothing to do...
			break;

		default:
			b3PrintF(B3LOG_NORMAL, "Illegal render mode selected.\n");
			break;
		}
	}
	b3UnmapVertices();
	b3UnmapIndices();
}

void b3RenderObject::b3DrawGeometry(
	b3RenderContext    *   context,
	const b3_render_mode   render_mode)
{
	switch (render_mode)
	{
	case B3_RENDER_LINE:
		if (glGridElements->b3GetCount() > 0)
		{
			b3SelectMaterialForLineDrawing(context);
			b3DrawLinedGeometry(context);
		}
		break;

	case B3_RENDER_FILLED:
		if (glPolygonElements->b3GetCount() > 0)
		{
			b3SelectMaterialForFilledDrawing(context);
			b3DrawFilledGeometry(context);
		}
		break;

	default:
		// Do nothing!
		break;
	}
}

void b3RenderObject::b3SelectMaterialForLineDrawing(b3RenderContext * context)
{
#ifdef BLZ3_USE_OPENGL
	b3Color         diffuse;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	if (context->b3GetSelected() == this)
	{
		b3GetSelectedColor(diffuse);
	}
	else
	{
		b3GetGridColor(diffuse);
	}
	glColor3f(diffuse[b3Color::R], diffuse[b3Color::G], diffuse[b3Color::B]);
#endif
}

void b3RenderObject::b3SelectMaterialForFilledDrawing(const b3RenderContext * context)
{
#ifdef BLZ3_USE_OPENGL
	glEnable(GL_LIGHTING);
	if (glTextureSize > 0)
	{
		if (context->glDrawCachedTextures)
		{
			B3_ASSERT(glIsTexture(glTextureId));
			glBindTexture(GL_TEXTURE_2D, glTextureId);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			b3DefineTexture();
		}
		glEnable(GL_TEXTURE_2D);

		// Set repitition of chess fields by scaling texture
		// coordinates.
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(glTextureTransX, glTextureTransY, 0.0);
		glScalef(glTextureScaleX, glTextureScaleY, 1.0);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	// Set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  glAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  glDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, glShininess);
#endif
}

#ifndef _DEBUG

void b3RenderObject::b3DrawLinedGeometry(B3_UNUSED const b3RenderContext * context)
{
	glVertexElements->b3Draw();
	glGridElements->b3Draw();
}

void b3RenderObject::b3DrawFilledGeometry(B3_UNUSED const b3RenderContext * context)
{
	glVertexElements->b3Draw();
	glPolygonElements->b3Draw();
}

#else

void b3RenderObject::b3DrawLinedGeometry(const b3RenderContext * context)
{
#ifdef BLZ3_USE_OPENGL
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "       b3Draw lined:  %d vertices, %d lines\n",
		glVertexElements->b3GetCount(),
		glGridElements->b3GetCount());
#endif

	GLenum error = glGetError();
	glVertexElements->b3Draw();
	error = glGetError();
	if (error == GL_NO_ERROR)
	{
		glGridElements->b3Draw();
		error = glGetError();
	}
#endif
}

void b3RenderObject::b3DrawFilledGeometry(const b3RenderContext * context)
{
#ifdef BLZ3_USE_OPENGL
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL, "       b3Draw filled: %d vertices, %d polygons\n",
		glVertexElements->b3GetCount(),
		glPolygonElements->b3GetCount());
#endif

	GLenum error = glGetError();
	glVertexElements->b3Draw();

	error = glGetError();
	if (error == GL_NO_ERROR)
	{
		glPolygonElements->b3Draw();
		error = glGetError();
	}
#endif
}

#endif
