/*
**
**	$Filename:	b3RenderContext.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Render context for rendering some objects
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
#include "blz3/base/b3Render.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3VectorBufferObjects.h"
#include "blz3/base/b3MultiSample.h"

#define LOCAL_VIEWER GL_TRUE

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_vector b3RenderContext::glSimpleLightPosition =
{
	1000.0f,-2500.0f,2000.0f
};

b3_vector b3RenderContext::glSimpleLightDirection =
{
	0,0,-1
};

#ifdef BLZ3_USE_OPENGL
b3_bool b3RenderContext::glUse = true;
#else
b3_bool b3RenderContext::glUse = false;
#endif

#ifdef BLZ3_USE_OPENGL

GLenum b3RenderContext::glLightNum[] =
{
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
	GL_LIGHT4,
	GL_LIGHT5,
	GL_LIGHT6,
	GL_LIGHT7
};

#define VALIDATE_LIGHT_NUM(num) (((num) >= 0) && (((size_t)num) < (sizeof(glLightNum) / sizeof(GLint))))

#endif

b3RenderContext::b3RenderContext()
{
	b3PrintF(B3LOG_FULL," b3RenderContext::b3RenderContext()\n");
	b3LightNum();
	glDrawCachedTextures = true;
	glSelectedObject     = null;
	glBgColor.b3Init(0.8f,0.8f,0.8f);
}

void b3RenderContext::b3Init(b3_bool double_buffered)
{
#ifdef BLZ3_USE_OPENGL
	const char *extensions = (const char *)glGetString(GL_EXTENSIONS);

	b3PrintF(B3LOG_FULL,  " b3RenderContext::b3Init()\n");
	b3PrintF(B3LOG_NORMAL,"OpenGL vendor:     %s\n",glGetString(GL_VENDOR));
	b3PrintF(B3LOG_NORMAL,"OpenGL renderer:   %s\n",glGetString(GL_RENDERER));
	b3PrintF(B3LOG_NORMAL,"OpenGL version:    %s\n",glGetString(GL_VERSION));
	b3PrintF(B3LOG_DEBUG, "OpenGL extensions: %s\n",extensions);

	if (strstr(extensions,"GL_ARB_vertex_program") != null)
	{
		b3PrintF(B3LOG_DEBUG,"Vertex shader low level support.\n");
	}

	if (strstr(extensions,"GL_ARB_fragment_program") != null)
	{
		b3PrintF(B3LOG_DEBUG,"Pixel shader low level support.\n");
	}

	if (strstr(extensions,"GL_ARB_vertex_shader") != null)
	{
		b3PrintF(B3LOG_DEBUG,"Vertex shader language support (nice).\n");
	}

	if (strstr(extensions,"GL_ARB_fragment_shader") != null)
	{
		b3PrintF(B3LOG_NORMAL,"Pixel shader language support (very fine).\n");
	}

	if (strstr(extensions,"GL_ARB_shading_language_100") != null)
	{
		b3PrintF(B3LOG_DEBUG,"Support for OpenGL shading language V1.00.\n");
	}

	b3VectorBufferObjects::b3Init(extensions);
	if (b3VectorBufferObjects::b3HasVBO())
	{
		b3PrintF(B3LOG_DEBUG, "Having vertex buffer objects.\n");
	}
	if (b3VectorBufferObjects::b3AllowVBO())
	{
		b3PrintF(B3LOG_DEBUG, "Allowing vertex buffer objects.\n");
	}

	b3MultiSample::b3Init(extensions);
	if (b3MultiSample::b3HasMS())
	{
		b3PrintF(B3LOG_DEBUG, "Having multisampling.\n");
		b3PrintF(B3LOG_NORMAL, "Multisampling: %s\n",b3MultiSample::b3IsEnabled() ? "enabled" : "disabled");
		b3MultiSample::b3Enable(true);
	}

	glDrawBuffer(double_buffered ? GL_BACK : GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_AUTO_NORMAL);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER,0.5);
	glEnableClientState(GL_INDEX_ARRAY);

	b3LightReset();
#endif
}

void b3RenderContext::b3StartDrawing()
{
#ifdef BLZ3_USE_OPENGL
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL," b3RenderContext::b3StartDrawing()\n");
#endif
	glClearColor(
		glBgColor[b3Color::R],
		glBgColor[b3Color::G],
		glBgColor[b3Color::B],
		1.0 - glBgColor[b3Color::A]);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#endif
}

void b3RenderContext::b3SetAntiAliasing(b3_bool enable)
{
#ifdef BLZ3_USE_OPENGL
	if (enable)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
		glLineWidth(1.2f);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
		glLineWidth(1.0f);
	}
#endif
}

/*************************************************************************
**                                                                      **
**                        View setup                                    **
**                                                                      **
*************************************************************************/

void b3RenderContext::b3ViewSet(const b3_render_view_info *info)
{
#ifdef BLZ3_USE_OPENGL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (info->perspective)
	{
		glFrustum(
			-info->width,   info->width,
			-info->height,  info->height,
			info->near_cp, info->far_cp);
	}
	else
	{
		glOrtho(
			-info->width,   info->width,
			-info->height,  info->height,
			info->near_cp, info->far_cp);
	}
	gluLookAt(
		info->eye.x,    info->eye.y,    info->eye.z,
		info->look.x,   info->look.y,   info->look.z,
		info->up.x,     info->up.y,     info->up.z);
	glTranslatef(
		info->offset.x, info->offset.y, info->offset.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif
}

/*************************************************************************
**                                                                      **
**                        Light setup                                   **
**                                                                      **
*************************************************************************/

#ifdef BLZ3_USE_OPENGL
void b3RenderContext::b3SetAmbient(const b3_pkd_color ambient)
{
	GLfloat gl_ambient[4];

	b3PrintF(B3LOG_FULL," b3RenderContext::b3SetAmbient(%08lx)\n",
			 ambient);

	b3PkdColorToGL(ambient,gl_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gl_ambient);
}

void b3RenderContext::b3SetAmbient(b3Color &ambient)
{
	GLfloat gl_ambient[4];

	b3PrintF(B3LOG_FULL," b3RenderContext::b3SetAmbient(%08lx)\n",
			 (b3_pkd_color)ambient);

	b3ColorToGL(ambient,gl_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gl_ambient);
}
#endif

void b3RenderContext::b3LightReset(b3_pkd_color ambient)
{
	b3PrintF(B3LOG_FULL," b3RenderContext::b3LightReset()\n");

#ifdef BLZ3_USE_OPENGL
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,      GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER , LOCAL_VIEWER);

#ifdef GL_LIGHT_MODEL_COLOR_CONTROL
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif

	// Disable all other lights
	b3SetAmbient(ambient);
	for (b3_size i = 0;i < (sizeof(glLightNum) / sizeof(GLint));i++)
	{
		glDisable(glLightNum[i]);
	}
#endif
}

void b3RenderContext::b3LightDefault(b3_pkd_color ambient)
{
	b3PrintF(B3LOG_FULL," b3RenderContext::b3LightDefault()\n");
	b3LightNum();
	b3LightReset(ambient);

#ifdef BLZ3_USE_OPENGL
	b3_render_light_info info;

	// Colors
	b3PkdColorToGL(B3_BLACK, info.gl_ambient);
	b3PkdColorToGL(B3_WHITE, info.gl_diffuse);
	b3PkdColorToGL(B3_WHITE, info.gl_specular);

	// Geometry
	b3VectorToDirectionalGL( &glSimpleLightPosition,  info.gl_position);
	b3VectorToGL(            &glSimpleLightDirection, info.gl_direction);

	// Spot
	info.gl_spot_exp    =  20;
	info.gl_spot_cutoff = 180;

	//Attenuation
	info.gl_Ac = 1;
	info.gl_Al = 0;
	info.gl_Aq = 0;

	b3LightSet(0, &info);
#endif
}

void b3RenderContext::b3LightNum(b3_index num)
{
	b3PrintF(B3LOG_FULL," b3RenderContext::b3LightNum(%d)\n",num);

#ifdef BLZ3_USE_OPENGL
	if (VALIDATE_LIGHT_NUM(num))
	{
		glLightCount = num;
	}
#endif
}

b3_bool b3RenderContext::b3LightAdd(const b3_render_light_info *info)
{
	b3_bool result = false;

#ifdef BLZ3_USE_OPENGL
	if (VALIDATE_LIGHT_NUM(glLightCount))
	{
		b3LightSet(glLightCount++,info);
		result = true;
	}
#endif

	return result;
}

void b3RenderContext::b3LightSet(
	const b3_index              num,
	const b3_render_light_info *info)
{
#ifdef BLZ3_USE_OPENGL
	GLenum light = glLightNum[num];

	b3PrintF(B3LOG_FULL," b3RenderContext::b3LightSet(...)\n");

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER , LOCAL_VIEWER);
	glEnable( light);

	// Colors
	glLightfv(light,GL_AMBIENT, info->gl_ambient);
	glLightfv(light,GL_DIFFUSE, info->gl_diffuse);
	glLightfv(light,GL_SPECULAR,info->gl_specular);

	// Geometry
	glLightfv(light,GL_POSITION,      info->gl_position);
	glLightfv(light,GL_SPOT_DIRECTION,info->gl_direction);

	// Spot values
	glLightf (light,GL_SPOT_EXPONENT, info->gl_spot_exp);
	glLightf (light,GL_SPOT_CUTOFF,   info->gl_spot_cutoff);

	// Attenuation
	glLightf (light,GL_CONSTANT_ATTENUATION,  info->gl_Ac);
	glLightf (light,GL_LINEAR_ATTENUATION,    info->gl_Al);
	glLightf (light,GL_QUADRATIC_ATTENUATION, info->gl_Aq);

	b3PrintF(B3LOG_FULL," A: %06x D: %06x S: %06x\n",
			 b3GLToPkdColor(info->gl_ambient),
			 b3GLToPkdColor(info->gl_diffuse),
			 b3GLToPkdColor(info->gl_specular));
	b3PrintF(B3LOG_FULL," P: %1.3f %1.3f %1.3f  D: %1.3f %1.3f %1.3f\n",
			 info->gl_position[0],  info->gl_position[1],  info->gl_position[2],
			 info->gl_direction[0], info->gl_direction[1], info->gl_direction[2]);
	b3PrintF(B3LOG_FULL," SE: %1.4f   SC: %1.4f\n",
			 info->gl_spot_exp, info->gl_spot_cutoff);
	b3PrintF(B3LOG_FULL," Ac: %1.4f   Al: %1.4f   Aq: %1.4f\n",
			 info->gl_Ac, info->gl_Al, info->gl_Aq);
#endif
}

/*************************************************************************
**                                                                      **
**                        Some helper                                   **
**                                                                      **
*************************************************************************/

b3_bool b3RenderContext::b3GetMatrix(
	b3_matrix_mode  mode,
	b3_matrix      *matrix)
{
#ifdef BLZ3_USE_OPENGL
	b3_bool result = false;
	GLfloat values[16];

	switch (mode)
	{
	case B3_MATRIX_OBJECT:
		glGetFloatv(GL_MODELVIEW_MATRIX,values);
		result = true;
		break;

	case B3_MATRIX_PROJECTION:
		glGetFloatv(GL_PROJECTION_MATRIX,values);
		result = true;
		break;

	default:
		result = false;
		break;
	}
	if (result)
	{
		matrix->m11 = values[ 0];
		matrix->m12 = values[ 4];
		matrix->m13 = values[ 8];
		matrix->m14 = values[12];

		matrix->m21 = values[ 1];
		matrix->m22 = values[ 5];
		matrix->m23 = values[ 9];
		matrix->m24 = values[13];

		matrix->m31 = values[ 2];
		matrix->m32 = values[ 6];
		matrix->m33 = values[10];
		matrix->m34 = values[14];

		matrix->m41 = values[ 3];
		matrix->m42 = values[ 7];
		matrix->m43 = values[11];
		matrix->m44 = values[15];
	}
	return result;
#else
	return false;
#endif
}

b3_bool b3RenderContext::b3PutMatrix(
	const b3_matrix_mode  mode,
	const b3_matrix      *matrix)
{
#ifdef BLZ3_USE_OPENGL
	b3_bool result = false;
	GLfloat values[16];

	values[ 0] = matrix->m11;
	values[ 1] = matrix->m21;
	values[ 2] = matrix->m31;
	values[ 3] = matrix->m41;

	values[ 4] = matrix->m12;
	values[ 5] = matrix->m22;
	values[ 6] = matrix->m32;
	values[ 7] = matrix->m42;

	values[ 8] = matrix->m13;
	values[ 9] = matrix->m23;
	values[10] = matrix->m33;
	values[11] = matrix->m43;

	values[12] = matrix->m14;
	values[13] = matrix->m24;
	values[14] = matrix->m34;
	values[15] = matrix->m44;

	switch (mode)
	{
	case B3_MATRIX_OBJECT:
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(values);
		result = true;
		break;

	case B3_MATRIX_PROJECTION:
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(values);
		result = true;
		break;

	default:
		result = false;
		break;
	}
	return result;
#else
	return false;
#endif
}
