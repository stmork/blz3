/*
**
**      $Filename:      b3RenderContext.cc $
**      $Release:       Dortmund 2004 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Render context for rendering some objects
**
**      (C) Copyright 2004  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Render.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Color.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2004/09/24 13:45:36  sm
**	- Extracted OpenGL extension vector buffer objects into own files.
**	- Some cleanup for Lines.
**
**	Revision 1.5  2004/09/24 11:42:14  sm
**	- First VBO run under Linux.
**	
**	Revision 1.4  2004/09/23 21:27:38  sm
**	- VBOs still don't work.
**	
**	Revision 1.3  2004/09/23 20:02:25  sm
**	- Introduced VBOs on Windows - with success!
**	
**	Revision 1.2  2004/09/23 16:05:28  sm
**	- Some BLZ3_USE_OPENGL caveats removed.
**	
**	Revision 1.1  2004/09/23 15:47:04  sm
**	- Splitted b3RenderContext into own file.
**	- Added vertex buffer object support which does not
**	  run yet.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static b3_vector light0_position =
{
	1000.0f,-2500.0f,2000.0f
};

#ifdef BLZ3_USE_OPENGL
static b3Color world_ambient(  0.25f,0.25f,0.25f);
static b3Color light0_ambient( 0.25f,0.25f,0.25f);
static b3Color light0_diffuse( 0.8f, 0.8f, 0.8f);
static b3Color light0_specular(1.0f, 1.0f, 1.0f);

static GLenum light_num[] =
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

#define VALIDATE_LIGHT_NUM(num) (((num) >= 0) && (((size_t)num) < (sizeof(light_num) / sizeof(GLint))))

#endif

b3RenderContext::b3RenderContext()
{
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3RenderContext()\n");
	b3LightNum();
	b3LightSpotEnable(false);
	glDrawCachedTextures = true;
	glSelectedObject     = null;
	glBgColor.b3Init(0.8f,0.8f,0.8f);
}

void b3RenderContext::b3Init()
{
#ifdef BLZ3_USE_OPENGL
	const char *extensions = (const char *)glGetString(GL_EXTENSIONS);

	b3PrintF(B3LOG_FULL,  "b3RenderContext::b3Init()\n");
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

	glDrawBuffer(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_COLOR_MATERIAL);
//	GLboolean bVal;
//	glGetBooleanv(GL_MULTISAMPLE,&bVal);
//	b3PrintF(B3LOG_FULL, "Multisample: %d, %d\n",bVal,glIsEnabled(GL_MULTISAMPLE));
//	glDisable(GL_MULTISAMPLE);
//	glDisable(GL_SAMPLE_BUFFERS_EXT);
//	glDisable(GL_SAMPLES_EXT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_AUTO_NORMAL);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER,0.5);

	// Enable light
	b3LightReset();
	b3LightSet(&light0_position,null,1.0);
#endif
}

void b3RenderContext::b3SetAmbient(b3Color &ambient)
{
#ifdef BLZ3_USE_OPENGL
	GLfloat gl_ambient[4];

	b3PrintF(B3LOG_FULL,"b3RenderContext::b3SetAmbient(%08lx)\n",
		(b3_pkd_color)ambient);
	b3ColorToGL(ambient,gl_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gl_ambient);
#endif
}

void b3RenderContext::b3LightReset()
{
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightReset()\n");

#ifdef BLZ3_USE_OPENGL
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,      GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER , GL_TRUE);
#ifdef GL_LIGHT_MODEL_COLOR_CONTROL
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif

	// Disable all other lights
	b3SetAmbient(world_ambient);
	for (b3_size i = 0;i < (sizeof(light_num) / sizeof(GLint));i++)
	{
		glDisable(light_num[i]);
	}
#endif
}

void b3RenderContext::b3LightDefault()
{
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightDefault()\n");

	b3LightNum();
	b3LightReset();
	b3LightSet(&light0_position,null,0.0);
}

void b3RenderContext::b3LightSpotEnable(b3_bool enable)
{
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightSpotEnable(%s)\n",
		enable ? "true" : "false");
	glUseSpotLight = enable;
}

void b3RenderContext::b3LightNum(b3_index num)
{
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightNum(%d)\n",num);

#ifdef BLZ3_USE_OPENGL
	if (VALIDATE_LIGHT_NUM(num))
	{
		glLightNum = num;
	}
#endif
}

b3_bool b3RenderContext::b3LightAdd(
	b3_vector *b3_position,
	b3_vector *b3_direction,
	b3_f64     spot_exp,
	b3Color   *b3_diffuse,
	b3Color   *b3_ambient,
	b3Color   *b3_specular)
{
	b3_bool  result = false;

	b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightAdd(%d)\n",
		glLightNum);
	result = b3LightSet(
		b3_position,glUseSpotLight ? b3_direction : null,
		spot_exp,
		b3_diffuse,b3_ambient,b3_specular,glLightNum++);

	return result;
}

b3_bool b3RenderContext::b3LightSet(
	b3_vector *b3_position,
	b3_vector *b3_direction,
	b3_f64     spot_exp,
	b3Color   *b3_diffuse,
	b3Color   *b3_ambient,
	b3Color   *b3_specular,
	b3_index   num)
{
	b3_bool result = false;
#ifdef BLZ3_USE_OPENGL
	GLfloat gl_position[4];
	GLfloat gl_direction[4];
	GLfloat gl_ambient[4];
	GLfloat gl_diffuse[4];
	GLfloat gl_specular[4];
	GLenum  light;

	if (VALIDATE_LIGHT_NUM(num))
	{
		light = light_num[num];

		b3VectorToGL(b3_position,gl_position);

		b3ColorToGL(b3_ambient  != null ? *b3_ambient  : light0_ambient, gl_ambient);
		b3ColorToGL(b3_diffuse  != null ? *b3_diffuse  : light0_diffuse, gl_diffuse);
		b3ColorToGL(b3_specular != null ? *b3_specular : light0_specular,gl_specular);

		glEnable( light);

		glLightfv(light,GL_AMBIENT, gl_ambient);
		glLightfv(light,GL_DIFFUSE, gl_diffuse);
		glLightfv(light,GL_SPECULAR,gl_specular);
		glLightfv(light,GL_POSITION,gl_position);

		if (b3_direction != null)
		{
			b3VectorToGL(b3_direction,gl_direction);
			glLightfv(light,GL_SPOT_DIRECTION,gl_direction);
			glLightf (light,GL_SPOT_EXPONENT, spot_exp);
			glLightf (light,GL_SPOT_CUTOFF,   90.0);
		}
		else
		{
			glLightf (light,GL_SPOT_EXPONENT,   0.0);
			glLightf (light,GL_SPOT_CUTOFF,   180.0);
		}

		// Influence on ambient light
		glLightf (light,GL_CONSTANT_ATTENUATION,  1.0);
		glLightf (light,GL_LINEAR_ATTENUATION,    0.0);
		glLightf (light,GL_QUADRATIC_ATTENUATION, 0.0);
		result = true;
	}
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightSet(%d) = %s\n",
		num,result ? "true" : "false");
#ifdef _DEBUG
	if (result)
	{
		b3PrintF(B3LOG_FULL,"b3RenderContext::b3LightSet() # Light %d: %3.2f %3.2f %3.2f\n",
			light - GL_LIGHT0,gl_position[0],gl_position[1],gl_position[2]);
	}
#endif
#endif
	return result;
}


void b3RenderContext::b3StartDrawing()
{
#ifdef BLZ3_USE_OPENGL
	b3PrintF(B3LOG_FULL,"b3RenderContext::b3StartDrawing()\n");

	glClearColor(
		glBgColor[b3Color::R],
		glBgColor[b3Color::G],
		glBgColor[b3Color::B],
		1.0 - glBgColor[b3Color::A]);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#endif
}

b3_bool b3RenderContext::b3GetMatrix(
	b3_matrix_mode  mode,
	b3_matrix      *matrix)
{
#ifdef BLZ3_USE_OPENGL
	b3_bool result = false;
	GLfloat values[16];

	switch(mode)
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
	b3_matrix_mode  mode,
	b3_matrix      *matrix)
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

	switch(mode)
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
