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

class b3RenderContext : protected b3Mem
{
	b3_index         glLightNum;

public:
	b3_count         glVertexCount;
	b3_count         glPolyCount;
	b3_count         glGridCount;
	b3_color         glBGColor;
	b3_bool          glUseSpotLight;

public:
	                 b3RenderContext();
	static  void     b3Init();
	virtual void     b3StartDrawing();
	        void     b3SetBGColor(b3_f64 r,b3_f64 g,b3_f64 b);
			void     b3SetBGColor(b3_color *color);
	static  void     b3SetAmbient(b3_color *ambient);
	static  void     b3LightReset();
	        void     b3LightDefault();
			void     b3LightNum(b3_index light_num = 0);
	static  b3_bool  b3LightSet(b3_vector *pos,b3_vector *dir,b3_f64 spot,b3_color *diffuse = null,b3_color *ambient = null,b3_color *specular = null,b3_index light_num = 0);
			b3_bool  b3LightAdd(b3_vector *pos,b3_vector *dir,b3_f64 spot,b3_color *diffuse = null,b3_color *ambient = null,b3_color *specular = null);
			void     b3LightSpotEnable(b3_bool enable = true);

	static  b3_bool  b3GetMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);
	static  b3_bool  b3PutMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);

#ifdef BLZ3_USE_OPENGL
	// Some inlines :-)
	static inline void b3ColorToGL(b3_color *src,GLfloat *dst)
	{
		*dst++ =       src->r;
		*dst++ =       src->g;
		*dst++ =       src->b;
		*dst   = 1.0 - src->a;
	}

	static inline void b3ColorToGL(b3_color *src,GLubyte *dst)
	{
		*dst++ = (GLubyte)(src->r * 255);
		*dst++ = (GLubyte)(src->g * 255);
		*dst++ = (GLubyte)(src->b * 255);
		*dst   = (GLubyte)(src->a * 255) ^ 0xff;
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

#ifdef BLZ3_USE_OPENGL
/*
** for use with glInterleavedArrays(GL_T2F_N3F_V3F,0, b3_vertex *));
*/
struct b3_tnv_vertex
{
	struct
	{
		GLfloat s,t;
	}         t;
	b3_vector n;
	b3_vector v;
};
#endif

class b3RenderObject : public b3Mem
{
protected:
	b3_count         glVertexCount;
	b3_count         glGridCount;
	b3_count         glPolyCount;
#ifdef BLZ3_USE_OPENGL
	b3_bool          glComputed;
	b3_tnv_vertex   *glVertex;
	GLushort        *glGrids;
	GLushort        *glPolygons;

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

protected:

	                        b3RenderObject();
	virtual                ~b3RenderObject();
	        void            b3Update();
	        void            b3UpdateMaterial();
public:
	        void            b3AddCount(b3RenderContext *context);
	virtual void            b3AllocVertices(b3RenderContext *context);
	virtual void            b3FreeVertices();
	virtual void            b3Draw();
	        void            b3Recompute();
	        b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);

protected:
	virtual void            b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	virtual void            b3GetVertexRange(b3_index &start,b3_index &end);
	virtual void            b3ComputeVertices();
	virtual void            b3ComputeIndices();
	virtual void            b3ComputeNormals(b3_bool normalize=true);
	        void            b3RecomputeMaterial();

	// Some render overridables
	virtual b3_render_mode  b3GetRenderMode();
	virtual void            b3GetGridColor(b3_color *color);
	virtual void            b3GetDiffuseColor(b3_color *diffuse);
	virtual b3_f64          b3GetColors(b3_color *ambient,b3_color *diffuse,b3_color *specular);
	virtual b3_bool         b3GetChess(b3_color *bColor,b3_color *wColor,b3_res &xRepeat,b3_res &yRepeat);
	virtual b3Tx           *b3GetTexture(b3_f64 &xTrans,b3_f64 &yTrans,b3_f64 &xScale,b3_f64 &yScale);
	virtual b3_bool         b3GetImage(b3Tx *image);
	        void            b3TransformVertices(b3_matrix *transformation);

private:
	        void            b3CreateTexture(b3RenderContext *context,b3_res xSize = 128,b3_res ySize = 0);
	        void            b3CreateChess(  b3RenderContext *context,b3_color *bColor,b3_color *wColor);
	        void            b3CopyTexture(  b3RenderContext *context,b3Tx *image);
	        void            b3CreateImage(  b3RenderContext *context,b3Tx *image);
};

#endif
