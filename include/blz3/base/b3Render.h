/*
**
**	$Filename:	b3Render.h $ 
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
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

#ifndef B3_RAYTRACE_RENDER_H
#define B3_RAYTRACE_RENDER_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Color.h"
#include "blz3/system/b3Mem.h"
#include "blz3/image/b3Tx.h"

#define B3_MAX_RENDER_SUBDIV 48

typedef enum
{
	B3_RENDER_UNDIFINED = -1,
	B3_RENDER_NOTHING   =  0,
	B3_RENDER_LINE,
	B3_RENDER_FILLED
} b3_render_mode;

typedef enum
{
	B3_MATRIX_OBJECT,
	B3_MATRIX_PROJECTION
} b3_matrix_mode;

class b3RenderObject;

class b3RenderContext : protected b3Mem
{
	b3_index         glLightNum;
	b3RenderObject  *glSelectedObject;

public:
#ifdef BLZ3_USE_OPENGL
	GLint            glMaxVertices;
	GLint            glMaxIndices;
#endif

	b3_count         glVertexCount;
	b3_count         glPolyCount;
	b3_count         glGridCount;
	b3_count         glTextureSize;
	b3_bool          glUseSpotLight;
	b3_bool          glDrawCachedTextures;
	b3Color          glBgColor;

public:
	                 b3RenderContext();
	        void     b3Init();
	virtual void     b3StartDrawing();
	static  void     b3SetAmbient(b3Color &ambient);
	static  void     b3LightReset();
	        void     b3LightDefault();
			void     b3LightNum(b3_index light_num = 0);
	static  b3_bool  b3LightSet(b3_vector *pos,b3_vector *dir,b3_f64 spot,b3Color *diffuse = null,b3Color *ambient = null,b3Color *specular = null,b3_index light_num = 0);
			b3_bool  b3LightAdd(b3_vector *pos,b3_vector *dir,b3_f64 spot,b3Color *diffuse = null,b3Color *ambient = null,b3Color *specular = null);
			void     b3LightSpotEnable(b3_bool enable = true);

	static  b3_bool  b3GetMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);
	static  b3_bool  b3PutMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);

	inline b3RenderObject  *b3GetSelected()
	{
		return glSelectedObject;
	}

	inline void b3SetSelected(b3RenderObject *selected)
	{
		glSelectedObject = selected;
	}

#ifdef BLZ3_USE_OPENGL
	// Some inlines :-)
	static inline void b3ColorToGL(b3Color &src,GLfloat *dst)
	{
		*dst++ =       src[b3Color::R];
		*dst++ =       src[b3Color::G];
		*dst++ =       src[b3Color::B];
		*dst   = 1.0 - src[b3Color::A];
	}

	static inline void b3ColorToGL(b3Color &src,GLubyte *dst)
	{
		*dst++ = (GLubyte)(src[b3Color::R] * 255);
		*dst++ = (GLubyte)(src[b3Color::G] * 255);
		*dst++ = (GLubyte)(src[b3Color::B] * 255);
		*dst   = (GLubyte)(src[b3Color::A] * 255) ^ 0xff;
	}

	static inline void b3PkdColorToGL(b3_pkd_color input,GLubyte *buffer)
	{
		*buffer++ = (GLubyte)((input & 0x00ff0000) >> 16);
		*buffer++ = (GLubyte)((input & 0x0000ff00) >>  8);
		*buffer++ = (GLubyte)((input & 0x000000ff));
		*buffer   = (GLubyte)((input & 0xff000000) >> 24) ^ 0xff;
	}

	static inline void b3PkdColorToGL(b3_pkd_color input,GLfloat *dst)
	{
		*dst++ =       ((input & 0x00ff0000) >> 16) * 0.0039215686;
		*dst++ =       ((input & 0x0000ff00) >>  8) * 0.0039215686;
		*dst++ =       ((input & 0x000000ff))       * 0.0039215686;
		*dst   = 1.0 - ((input & 0xff000000) >> 24) * 0.0039215686;
	}

	static inline void b3VectorToGL(b3_vector *src,GLfloat *dst)
	{
		*dst++ = src->x;
		*dst++ = src->y;
		*dst++ = src->z;
		*dst   = 1;
	}

	static inline void b3Vector4dToGL(b3_vector4D *src,GLfloat *dst)
	{
		*dst++ = src->x;
		*dst++ = src->y;
		*dst++ = src->z;
		*dst   = src->w;
	}
#endif
};

/*
** for use with glInterleavedArrays(GL_T2F_N3F_V3F,0, b3_vertex *));
*/

struct b3_gl_vertex
{
	b3_gl_texture t;
	b3_gl_vector  n;
	b3_gl_vector  v;
};

struct b3_gl_line
{
#ifdef BLZ3_USE_OPENGL
	GLushort a,b;
#else
	b3_u16   a,b;
#endif
};

struct b3_gl_polygon
{
#ifdef BLZ3_USE_OPENGL
	GLushort a,b,c;
#else
	b3_u16   a,b,c;
#endif
};

#define B3_GL_LINIT(l,ai,bi)    { (l)->a = (ai); (l)->b = (bi); (l)++; }
#define B3_GL_PINIT(p,ai,bi,ci) { (p)->a = (ai); (p)->b = (bi); (p)->c = (ci); (p)++; }

class b3RenderObject : public b3Mem
{
protected:
	b3_count         glVertexCount;
	b3_count         glGridCount;
	b3_count         glPolyCount;
	b3_bool          glComputed;
	b3_gl_vertex    *glVertex;
	b3_gl_line      *glGrids;
	b3_gl_polygon   *glPolygons;

#ifdef BLZ3_USE_OPENGL
	GLuint           glDisplayList;

	// Some material values
	b3_bool          glMaterialComputed;
	GLfloat          glAmbient[4];
	GLfloat          glDiffuse[4];
	GLfloat          glSpecular[4];
	GLfloat          glShininess;
	
	// Some texture values
	GLuint           glTextureId;
	GLubyte         *glTextureData;
	b3_res           glTextureSize;
	b3_res           glTextureSizeX;
	b3_res           glTextureSizeY;
	b3_f64           glTextureTransX;
	b3_f64           glTextureTransY;
	b3_f64           glTextureScaleX;
	b3_f64           glTextureScaleY;
#endif

public:
	static b3Color   m_GridColor;
	static b3Color   m_SelectedColor;

protected:

	                        b3RenderObject();
	virtual                ~b3RenderObject();
public:
	        void            b3AddCount(b3RenderContext *context);
	virtual void            b3AllocVertices(b3RenderContext *context);
	virtual void            b3FreeVertices();
	virtual void            b3Draw(b3RenderContext *context);
	        void            b3Recompute();
	        void            b3Update();
	        void            b3UpdateMaterial();
	        b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);
	        void            b3RecomputeMaterial();

protected:
	virtual void            b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	virtual void            b3GetVertexRange(b3_index &start,b3_index &end);
	virtual void            b3ComputeVertices();
	virtual void            b3ComputeIndices();
	virtual void            b3ComputeNormals(b3_bool normalize=true);

	// Some render overridables
	virtual inline b3_render_mode  b3GetRenderMode()
	{
		return B3_RENDER_LINE;
	}

	virtual inline void b3GetGridColor(b3Color &color)
	{
		color = m_GridColor;
	}

	virtual inline void b3GetSelectedColor(b3Color &color)
	{
		color = m_SelectedColor;
	}

	virtual void            b3GetDiffuseColor(b3Color &diffuse);

	virtual b3_f64          b3GetColors(b3Color &ambient,b3Color &diffuse,b3Color &specular);
	virtual b3_bool         b3GetChess(b3Color &bColor,b3Color &wColor,b3_res &xRepeat,b3_res &yRepeat);
	virtual b3Tx           *b3GetTexture(b3_f64 &xTrans,b3_f64 &yTrans,b3_f64 &xScale,b3_f64 &yScale);
	virtual b3_bool         b3GetImage(b3Tx *image);
	        void            b3TransformVertices(b3_matrix *transformation,b3_bool is_affine);

private:
	        void            b3DefineTexture();
	        void            b3CreateTexture(b3RenderContext *context,b3_res xSize = 128,b3_res ySize = 0);
	        void            b3CreateChess(  b3RenderContext *context,b3Color &bColor,b3Color &wColor);
	        void            b3CopyTexture(  b3RenderContext *context,b3Tx *image);
	        void            b3CreateImage(  b3RenderContext *context,b3Tx *image);
	        void            b3DeleteDisplayList();

	        void            b3CheckGeometry(b3RenderContext *context,b3_render_mode render_mode);
	        void            b3DrawGeometry(b3RenderContext *context,b3_render_mode render_mode);

			// Selecting material and drawing geometry in different modes
	        void            b3SelectMaterialForLineDrawing(b3RenderContext *context);
	        void            b3SelectMaterialForFilledDrawing(b3RenderContext *context);
	        void            b3DrawLinedGeometry(b3RenderContext *context);
	        void            b3DrawFilledGeometry(b3RenderContext *context);
};

#endif
