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

public:
	                 b3RenderContext();
	static  void     b3Init();
	virtual void     b3StartDrawing();
	        void     b3SetBGColor(b3_f64 r,b3_f64 g,b3_f64 b);
			void     b3SetBGColor(b3_color *color);
	static  void     b3SetAmbient(b3_color *ambient);
	static  void     b3LightReset();
			void     b3LightNum(b3_index light_num = 0);
	static  void     b3LightSet(b3_vector *pos,b3_color *diffuse = null,b3_color *ambient = null,b3_color *specular = null,b3_index light_num = 0);
			void     b3LightAdd(b3_vector *pos,b3_color *diffuse = null,b3_color *ambient = null,b3_color *specular = null);

	static  b3_bool  b3GetMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);
	static  b3_bool  b3PutMatrix(b3_matrix_mode matrix_mode,b3_matrix *matrix);

#ifdef BLZ3_USE_OPENGL
	// Some inlines :-)
	static inline void b3ColorToGL(b3_color *src,GLfloat *dst)
	{
		dst[0] =       src->r;
		dst[1] =       src->g;
		dst[2] =       src->b;
		dst[3] = 1.0 - src->a;
	}

	static inline void b3VectorToGL(b3_vector *src,GLfloat *dst)
	{
		dst[0] = src->x;
		dst[1] = src->y;
		dst[2] = src->z;
		dst[3] = 1;
	}

	static inline void b3Vector4dToGL(b3_vector4D *src,GLfloat *dst)
	{
		dst[0] = src->x;
		dst[1] = src->y;
		dst[2] = src->z;
		dst[3] = src->w;
	}

	static inline void b3PkdColorToGL(b3_pkd_color input,GLubyte *buffer)
	{
		*buffer++ = (GLubyte)((input & 0x00ff0000) >> 16);
		*buffer++ = (GLubyte)((input & 0x0000ff00) >>  8);
		*buffer++ = (GLubyte)((input & 0x000000ff));
		*buffer++ = (GLubyte)((input & 0xff000000) >> 24) ^ 0xff;
	}
#endif
};

class b3RenderObject : public b3Mem
{
protected:
	b3_count         glVertexCount;
	b3_count         glGridCount;
	b3_count         glPolyCount;
#ifdef BLZ3_USE_OPENGL
	b3_bool          glComputed;
	GLfloat         *glVertices;
	GLfloat         *glNormals;
	GLfloat         *glTexCoord;
	GLushort        *glGrids;
	GLushort        *glPolygons;

	// Some texture values
	GLuint           glTextureId;
	GLubyte         *glTextureData;
	b3_res           glTextureSize;
	b3_res           glTextureRepeatX;
	b3_res           glTextureRepeatY;
#endif

protected:

	                        b3RenderObject();
	virtual                ~b3RenderObject();
	        void            b3Update();
	        void            b3UpdateTexture();
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

	// Some render overridables
	virtual b3_render_mode  b3GetRenderMode();
	virtual void            b3GetGridColor(b3_color *color);
	virtual void            b3GetDiffuseColor(b3_color *diffuse);
	virtual b3_f64          b3GetColors(b3_color *ambient,b3_color *diffuse,b3_color *specular);
	virtual b3_bool         b3GetChess(b3_color *bColor,b3_color *wColor,b3_res &xRepeat,b3_res &yRepeat);
	virtual b3_bool         b3GetImage(b3Tx *image);
	        void            b3TransformVertices(b3_matrix *transformation);

private:
	        void            b3CreateTexture(b3RenderContext *context,b3_res size = 128);
	        void            b3CreateChess(b3RenderContext *context,b3_color *bColor,b3_color *wColor);
	        void            b3CreateImage(b3RenderContext *context,b3Tx *image);
};

#endif
