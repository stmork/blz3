/*
**
**	$Filename:	b3Render.h $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Rendering class definitions
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_BASE_RENDER_H
#define B3_BASE_RENDER_H

#include "blz3/b3Config.h"
#include <blz3/base/b3ConstantMap.h>
#include "blz3/base/b3Color.h"
#include "blz3/base/b3RenderTypes.h"
#include "blz3/base/b3VectorBufferObjects.h"
#include "blz3/base/b3VertexBuffer.h"
#include "blz3/base/b3World.h"
#include "blz3/system/b3Mem.h"
#include "blz3/image/b3Tx.h"

class b3RenderObject;

/**
 * This structure holds information about a view perspective.
 */
struct b3_render_view_info
{
	bool      perspective;         //!< A flag whether the perspective is projective.
	b3_f64    near_cp;             //!< The near clipping plane.
	b3_f64    far_cp;              //!< The far clipping plane.
	b3_f64    width;               //!< The viewport width.
	b3_f64    height;              //!< The viewport height.
	b3_vector eye;                 //!< The eye point of the camera.
	b3_vector look;                //!< The view poin of the camera.
	b3_vector up;                  //!< The up vector of the camera perspective.
	b3_vector offset;
};

/**
 * This structure holds information about an OpenGL light source.
 */
struct b3_render_light_info
{
#ifdef BLZ3_USE_OPENGL
	GLfloat gl_position[4];  //!< The light position.
	GLfloat gl_direction[4]; //!< The spot light direction.
	GLfloat gl_ambient[4];   //!< The ambient color.
	GLfloat gl_diffuse[4];   //!< The diffuse color.
	GLfloat gl_specular[4];  //!< The specular color.

	// Spot values
	GLfloat gl_spot_exp;     //!< The light exponent.
	GLfloat gl_spot_cutoff;  //!< The cut off angle for spot light.

	// Attenuation
	GLfloat gl_Ac;           //!< The constant attenuation.
	GLfloat gl_Al;           //!< The linear attenuation.
	GLfloat gl_Aq;           //!< The quadratic attenuation.
#endif
};

/**
 * This class provides a basic context for OpenGL rendering.
 */
class b3RenderContext : protected b3Mem
{
	b3_index             glLightCount;
	b3RenderObject   *   glSelectedObject = nullptr;

	static b3_vector     glSimpleLightPosition;
	static b3_vector     glSimpleLightDirection;
	static bool          glUse;

#ifdef BLZ3_USE_OPENGL
	static GLenum        glLightNum[];
#endif

public:
	b3_count             glVertexCount; //!< The overall vertex count
	b3_count             glPolyCount;   //!< The overall triangle count.
	b3_count             glGridCount;   //!< The overall line count.
	b3_count             glTextureSize; //!< The maximal texture resolution.
	bool                 glDrawCachedTextures; //!< A flag which specifies if the OpenGL textures should be cached.
	b3Color              glBgColor;     //!< The background color.

public:
	/**
	 * The constructor initializes this instance and initializes the OpenGL
	 * background color.
	 */
	b3RenderContext();

	/**
	 * This method defines whether OpenGL vertex computation should be used.
	 *
	 * @param use The flag whether to use OpenGL vertex initialization.
	 */
	static inline void b3UseGL(bool use = true)
	{
		glUse = use;
	}

	/**
	 * This method returns a flag whether to use OpenGL vertex initialization.
	 *
	 * @return The OpenGL vertex initialization flag.
	 */
	static inline bool    b3IsGL()
	{
		return glUse;
	}

	/**
	 * This method dumps some version information and initializes OpenGL
	 * for Blizzard III rendering purposes.
	 *
	 * @param doublebuffered A flag if the render context (GLX or WGL) is double buffered.
	 */
	static  void     b3Init(bool doublebuffered = true);

	/**
	 * This method marks the beginning of render object drawing.
	 */
	virtual void     b3StartDrawing();

	/**
	 * This method sets the anti aliasing drawing mode for wire frames.
	 *
	 * @param enable True if anti aliasing should be used.
	 */
	static  void     b3SetAntiAliasing(const bool enable = false);

	/**
	 * This method initializes the projection. The model view matrix
	 * is also loaded with the identity matrix.
	 *
	 * @param info The specified projection.
	 * @see b3_render_view_info
	 */
	static  void     b3ViewSet(const b3_render_view_info * info);

	/**
	 * This method resets the lighting with the specified color as ambient color
	 * using the b3SetAmbient() method. All lights sources are disabled.
	 *
	 * @param color The ambient color.
	 */
	static  void     b3LightReset(const b3_pkd_color color = B3_DARK_GREY);

	/**
	 * This method resets the lighting with the specified color as ambient color
	 * using the b3SetAmbient() method. All lights sources except the first one
	 * are disabled.
	 *
	 * @param color The ambient color.
	 */
	void     b3LightDefault(const b3_pkd_color color = B3_DARK_GREY);

	/**
	 * This method verifies the given light index if the index can be handled by OpenGL.
	 * All futher b3LightAdd() method calls use this new light index. Illegal light
	 * indices are ignored.
	 *
	 * @param index The light index to verify.
	 */
	void b3LightNum(const b3_index index = 0);

	/**
	 * This method adds a light with the specified light information structure. If
	 * no more lights are available no light is set. The light information itself
	 * is converted into OpenGL calls via the b3LightSet() method.
	 *
	 * @param info The light information.
	 * @return True if the light was added.
	 * @see b3_render_light_info
	 */
	bool    b3LightAdd(const b3_render_light_info * info);

	/**
	 * This method converts the information from the given information structure
	 * into a sequence of OpenGL API calls.
	 *
	 * @param index The light index.
	 * @param info The light information structure.
	 * @see b3_render_light_info
	 */
	static void b3LightSet(const b3_index index, const b3_render_light_info * info);

	/**
	 * This method sets the given matrix into the specified OpenGL matrix mode.
	 *
	 * @param mode The matrix mode to use.
	 * @param matrix The matrix itself.
	 * @return True on success.
	 * @see b3_matrix_mode
	 */
	static  bool     b3GetMatrix(const b3_matrix_mode mode, b3_matrix * matrix);

	/**
	 * This method retrieves the current selected matrix in the specified OpenGL matrix mode.
	 *
	 * @param mode The used matrix mode.
	 * @param matrix The matrix to retrieve.
	 * @return True on success.
	 */
	static  bool     b3PutMatrix(const b3_matrix_mode mode, const b3_matrix * matrix);

	/**
	 * This method returns the actual selected render object.
	 *
	 * @return The selected object.
	 */
	inline b3RenderObject * b3GetSelected() const
	{
		return glSelectedObject;
	}

	/**
	 * This method sets a new render object selection.
	 *
	 * @param selected The new selected render object.
	 */
	inline void b3SetSelected(b3RenderObject * selected)
	{
		glSelectedObject = selected;
	}

#ifdef BLZ3_USE_OPENGL

	/**
	 * This method converts a b3Color instance into OpenGL floats.
	 *
	 * @param src The b3Color instance to convert.
	 * @param dst The resulting OpenGL floats.
	 */
	static inline void b3ColorToGL(const b3Color & src, GLfloat * dst)
	{
		const b3_color color = src;

		*dst++ =       color.r;
		*dst++ =       color.g;
		*dst++ =       color.b;
		*dst   = 1.0 - color.a;
	}

	/**
	 * This method converts a b3Color instance into OpenGL unsigned bytes.
	 *
	 * @param src The b3Color instance to convert.
	 * @param dst The resulting OpenGL unsigned bytes.
	 */
	static inline void b3ColorToGL(const b3Color & src, GLubyte * dst)
	{
		const b3_color color = src * 255.0f;

		*dst++ = GLubyte(color.r);
		*dst++ = GLubyte(color.g);
		*dst++ = GLubyte(color.b);
		*dst   = GLubyte(color.a) ^ 0xff;
	}

	/**
	 * This method converts a b3_pkd_color into OpenGL floats.
	 *
	 * @param input The b3_pkd_color to convert.
	 * @param dst The resulting OpenGL floats.
	 */
	static inline void b3PkdColorToGL(const b3_pkd_color input, GLfloat * dst)
	{
		const b3_color color = b3Color(input);

		*dst++ =       color.r;
		*dst++ =       color.g;
		*dst++ =       color.b;
		*dst   = 1.0 - color.a;
	}

	/**
	 * This method converts a b3_pkd_color into OpenGL unsigned bytes.
	 *
	 * @param input The b3_pkd_color to convert.
	 * @param buffer The resulting OpenGL unsigned bytes.
	 */
	static inline void b3PkdColorToGL(const b3_pkd_color input, GLubyte * buffer)
	{
		*buffer++ = (GLubyte)((input & 0x00ff0000) >> 16);
		*buffer++ = (GLubyte)((input & 0x0000ff00) >>  8);
		*buffer++ = (GLubyte)((input & 0x000000ff));
		*buffer   = (GLubyte)((input & 0xff000000) >> 24) ^ 0xff;
	}

	/**
	 * This method converts OpenGL floats into a b3_pkd_color.
	 *
	 * @param color The OpenGL floats to convert.
	 * @return The resulting b3_pkd_color.
	 */
	static inline b3_pkd_color b3GLToPkdColor(const GLfloat * color)
	{
		return b3Color(color[0], color[1], color[2]);
	}

	/**
	 * This method converts a three component b3_vector into four OpenGL floats.
	 * The homogenous coordinate w is set to 1.
	 *
	 * @param src The b3_vector instance.
	 * @param dst The resulting OpenGL floats.
	 */
	static inline void b3VectorToGL(const b3_vector * src, GLfloat * dst)
	{
		*dst++ = src->x;
		*dst++ = src->y;
		*dst++ = src->z;
		*dst   = 1;
	}

	/**
	 * This method converts a four component b3_vector4D into four OpenGL floats.
	 *
	 * @param src The b3_vector4D instance.
	 * @param dst The resulting OpenGL floats.
	 */
	static inline void b3VectorToGL(const b3_vector4D * src, GLfloat * dst)
	{
		*dst++ = src->x;
		*dst++ = src->y;
		*dst++ = src->z;
		*dst   = src->w;
	}

	/**
	 * This method copies a b3_vector into OpenGL floats for a direction.
	 * The direction is normalized for OpenGL.
	 *
	 * @param src The b3_vector direction instance.
	 * @param dst The resulting normalized OpenGL floats representing a direction.
	 */
	static inline void b3VectorToDirectionalGL(const b3_vector * src, GLfloat * dst)
	{
		const b3_f32 x   = src->x;
		const b3_f32 y   = src->y;
		const b3_f32 z   = src->z;
		const b3_f32 len = sqrt(x * x + y * y + z * z);

		*dst++ = x / len;
		*dst++ = y / len;
		*dst++ = z / len;
		*dst   = 0;
	}

private:
	static  void     b3SetAmbient(const b3_pkd_color  ambient);
	static  void     b3SetAmbient(b3Color   &   ambient);
#endif
};

/**
 * This class represents one render object.  A render object contains one
 * array of vertices. One array of indices for wire frame drawing (grid
 * drawing) and one array of indices for solid shading (triangle drawing).
 */
class B3_PLUGIN b3RenderObject
{
	static const b3ConstantMap<b3_vbo_mapping> glVboMap;

	static b3Tx         glTextureBuffer;
	static b3Mutex      glTextureMutex;

	bool                glMaterialComputed;
	bool                glInit;

protected:
	b3VertexElements  * glVertexElements  = nullptr;  //!< The vertex data.
	b3GridElements   *  glGridElements    = nullptr;  //!< The line index data.
	b3PolygonElements * glPolygonElements = nullptr;  //!< The triangle index data.

#ifdef BLZ3_USE_OPENGL
	GLuint              glDisplayList;            //!< The display list for defining material properties.

	// Some material values
	GLfloat             glAmbient[4];             //!< The ambient material color.
	GLfloat             glDiffuse[4];             //!< The diffuse material color.
	GLfloat             glSpecular[4];            //!< The specular material color.
	GLfloat             glShininess;              //!< The specular exponent.

	// Some texture values
	GLuint              glTextureId;              //!< The OpenGL texture id.
	GLubyte      *      glTextureData = nullptr;  //!< The bitmap data of the texture.
	b3_res              glTextureSize;            //!< The bitmap buffer size.
	b3_res              glTextureSizeX;           //!< The x resolution of the texture.
	b3_res              glTextureSizeY;           //!< The y resolution of the texture.
	b3_f64              glTextureTransX;          //!< The texture x translation.
	b3_f64              glTextureTransY;          //!< The texture y translation.
	b3_f64              glTextureScaleX;          //!< The x scaling factor of the texture.
	b3_f64              glTextureScaleY;          //!< The y scaling factor of the texture.
#endif

public:
	static b3Color     m_GridColor;        //!< The grid color.
	static b3Color     m_SelectedColor;    //!< The grid color in case this object is selected.

protected:
	/**
	 * This constructor initializes this render object.
	 */
	b3RenderObject();

	/**
	 * The destructor frees all allocated VBOs, display lists and texture references.
	 */
	virtual ~b3RenderObject();

public:
	/**
	 * This method adds the amount of vector elements to the overall counter
	 * stored in the render context. This is only for statistical purposes.
	 *
	 * @param context The render context containg the counter.
	 */
	void b3AddCount(b3RenderContext * context);

	/**
	 * This method call initializes the vertex buffer objects for this render
	 * object. It calls:
	 *
	 * -# b3PreAlloc()
	 * -# b3AllocVertexMemory() and here in the default implementation:
	 *    -# b3GetCount()
	 *    -# b3VertexBuffer::b3AllocVertexMemory()  of these buffers:
	 *       - vertices
	 *       - grid indices
	 *       - triangle indices
	 *
	 * @param context The render context to use.
	 * @see b3VectorBufferObjects
	 * @see b3VertexBuffer
	 */
	virtual void            b3SetupVertexMemory(b3RenderContext * context);

	/**
	 * This method frees the memory of the participating VBOs.
	 *
	 * @see b3VectorBufferObjects
	 */
	virtual void            b3FreeVertexMemory();

	/**
	 * This method draws this render object in the defined state using
	 * the given render context.
	 *
	 * @param context The render context to use.
	 */
	virtual void b3Draw(b3RenderContext * context);

	/**
	 * This call forces this instance to recompute the geometry. It means
	 * that the vertices must be recomputed via the b3Update()
	 * method call using the b3ComputeVertices() method.
	 * The b3Update() method call is also done on b3Draw().
	 */
	void b3Recompute();

	/**
	 * This call forces this instance to recompute the line and triangle indices. It means
	 * that the indices must be recomputed via the b3Update()
	 * method call using the b3ComputeIndices() method.
	 * The b3Update() method call is also done on b3Draw().
	 */
	void b3RecomputeIndices();

	/**
	 * This call forces this instance to recompute the material. It means
	 * that the material properties must be recomputed via the b3UpdateMaterial()
	 * method call. The b3UpdateMaterial() method call is also done on b3Draw().
	 */
	void b3RecomputeMaterial();

	/**
	 * This method updates the geometry if necessary. After updating the
	 * recomputation is done.
	 */
	void b3Update();

	/**
	 * This method updates the material properties if necessary.
	 */
	void b3UpdateMaterial();

	/**
	 * This method adjusts the given bounding box with the vertices found in this
	 * render object.
	 *
	 * @param lower The lower bounding box corner.
	 * @param upper The upper bounding box corner.
	 * @return True on success.
	 */
	bool            b3ComputeBounds(b3_vector * lower, b3_vector * upper);

protected:
	/**
	 * The implementation of this method computes the VBO element counts. If any count is
	 * initialized to zero a custom initialization is assumed.
	 *
	 * @param context The render context.
	 * @param vertCount The computed vertex count.
	 * @param gridCount The computed line indexing count.
	 * @param polyCount The computed polygon indexing count.
	 */
	virtual void     b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount);

	/**
	 * This method returns the used vertex range. The default implementation
	 * returns a range between 0 and the number of allocated vertices. The first
	 * range from [0, start[ is the range for grid vertices. The range from
	 * [stert, end [ is the range for triangle vertices.
	 *
	 * @param start The start index including.
	 * @param end The end index excluding.
	 */
	virtual void     b3GetVertexRange(b3_index & start, b3_index & end);

	/**
	 * This method sets up the b3VectorBuffer object implementation as needed.
	 */
	void b3PreAlloc();

	/**
	 * The default implementation asks for the needed element counts via the
	 * b3GetCount() method call and then calls the b3VertexBuffer::b3AllocVertexMemory()
	 * method vor each of the three possible VBO instances.
	 *
	 * @param context The used render context.
	 * @see b3VectorBufferObjects
	 * @see b3VertexBuffer
	 */
	virtual void            b3AllocVertexMemory(b3RenderContext * context);

	/**
	 * The implementation of this method computes the vertices. The vertex buffer is
	 * already mapped into CPU address space for use. You can access the buffer by
	 * using using following code snippet:
	\verbatim
	b3_gl_vertex *Vector = *glVertexElements;
	\endverbatim
	 */
	virtual void            b3ComputeVertices();

	/**
	 * The implementation of this method computes the line indices and the triangle indices.
	 * The index buffers are already mapped into CPU address space for use. You can acces
	 * the buffers using following code snippet:
	\verbatim
	   b3_gl_line    *gPtr = *glGridElements;
	   b3_gl_polygon *pPtr = *glPolygonElements;
	\endverbatim
	   */
	virtual void            b3ComputeIndices();

	/**
	 * This method computes the normals based on the already computed triangle indices and the
	 * computed vertices. A flag controls if the computed normals should be normalized.
	 *
	 * @param normalize The flag if the computed normals shoud be normalized.
	 */
	virtual void            b3ComputeNormals(bool normalize = true);

	/**
	 * The implementation of this method computes the render mode depending
	 * on the internal state of this render object. There are three
	 * possible render modes:
	 * - B3_RENDER_NOTHING renders nothing.
	 * - B3_RENDER_LINE renders in wire frame mode.
	 * - B3_RENDER_SOLID renders with gouraud shaded triangles.
	 *
	 * @return The actual render mode.
	 */
	virtual b3_render_mode  b3GetRenderMode() const
	{
		return B3_RENDER_LINE;
	}

	/**
	 * This method retrieves the wire frame color.
	 *
	 * @param color The resulting color.
	 */
	virtual void b3GetGridColor(b3Color & color) const
	{
		color = m_GridColor;
	}

	/**
	 * This method returns the color in case of a selected object.
	 *
	 * @param color The resulting color.
	 */
	virtual void b3GetSelectedColor(b3Color & color) const
	{
		color = m_SelectedColor;
	}

	/**
	 * The imlementation of this method retrieves the object color.
	 *
	 * @param diffuse The resulting color.
	 */
	virtual void b3GetDiffuseColor(b3Color & diffuse) const;

	/**
	 * The imlementation of this method retrieves the object color components.
	 *
	 * @param ambient The resulting ambient color.
	 * @param diffuse The resulting diffuse color.
	 * @param specular The resulting specular color.
	 * @return The resulting specular exponent.
	 */
	virtual b3_f64 b3GetColors(b3Color & ambient, b3Color & diffuse, b3Color & specular) const;

	/**
	 * If this method returns true the referenced values are initialized by the implementation of
	 * this method with chess values. The default implementation simply returns false indicating
	 * no chess material.
	 *
	 * @param bColor The black color.
	 * @param wColor The white color.
	 * @param xRepeat The repitition count in x direction.
	 * @param yRepeat The repitition count in y direction.
	 * @return True if the object has a chess material.
	 */
	virtual bool    b3GetChess(b3Color & bColor, b3Color & wColor, b3_res & xRepeat, b3_res & yRepeat) const;

	/**
	 * If the implementation of this method can simply return its texture this method returns this
	 * texture with the translation and scaling values initialized.
	 *
	 * @param xTrans texture translation in x direction.
	 * @param yTrans texture translation in y direction.
	 * @param xScale texture translation in x direction.
	 * @param yScale texture translation in y direction.
	 * @return The texture to use.
	 */
	virtual b3Tx * b3GetTexture(b3_f64 & xTrans, b3_f64 & yTrans, b3_f64 & xScale, b3_f64 & yScale) const;

	/**
	 * If the render object is not a normal material nor a chess board nor a simple texture material the
	 * material must be sampled into the given image.
	 *
	 * @param image The image to which the surface colors are sampled in.
	 * @return True if sampling was OK.
	 */
	virtual bool    b3GetImage(b3Tx * image) const;

	/**
	 * This method transforms the vertex data with the given transformation.
	 *
	 * @param transformation The transformation matrix.
	 * @param isAffine If false the normals must be recomputed.
	 */
	void            b3TransformVertices(b3_matrix * transformation, bool isAffine);

private:
	void            b3DefineTexture();
	void            b3CreateTexture(b3RenderContext * context, b3_res xSize = 128, b3_res ySize = 0);
	void            b3CreateChess(b3RenderContext * context, b3Color & bColor, b3Color & wColor);
	void            b3CopyTexture(b3RenderContext * context, b3Tx * image);
	void            b3CreateImage(b3RenderContext * context, b3Tx * image);
	void            b3DeleteDisplayList();

	void            b3CheckGeometry(const b3RenderContext * context, const b3_render_mode render_mode);
	void            b3DrawGeometry(b3RenderContext * context, const b3_render_mode render_mode);

	// Selecting material and drawing geometry in different modes
	void            b3SelectMaterialForLineDrawing(b3RenderContext * context);
	void            b3SelectMaterialForFilledDrawing(const b3RenderContext * context);
	void            b3DrawLinedGeometry(const b3RenderContext * context);
	void            b3DrawFilledGeometry(const b3RenderContext * context);

	void            b3MapIndices(const b3_vbo_mapping map_mode = B3_MAP_VBO_RW);
	void            b3MapVertices(const b3_vbo_mapping map_mode = B3_MAP_VBO_RW);
	void            b3UnmapIndices();
	void            b3UnmapVertices();
	static  void    b3PrintMapping(const char * text, b3_vbo_mapping map_mode);
};

#endif
