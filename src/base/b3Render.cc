
/*
**
**      $Filename:      b3Render.cc $
**      $Release:       Dortmund 2001, 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Rendering some objects
**
**      (C) Copyright 2001, 2002  Steffen A. Mork
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
#include "blz3/base/b3Spline.h"
#include "blz3/base/b3Color.h"

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.66  2004/03/07 12:41:23  sm
**      - Print shader support.
**
**      Revision 1.65  2003/03/04 20:37:37  sm
**      - Introducing new b3Color which brings some
**        performance!
**
**      Revision 1.64  2003/02/26 16:36:16  sm
**      - Sorted drawing colors and added configuration support
**        to dialog.
**
**      Revision 1.63  2003/02/24 19:18:06  sm
**      - spline control grid drawing completed.
**
**      Revision 1.62  2003/02/22 15:17:18  sm
**      - Added support for selected shapes in object modeller
**      - Glued b3Shape and b3ShapeRenderObject. There was no
**        distinct access method in use.
**      - Made some b3Shape methods inline and/or static which
**        saves some memory.
**
**      Revision 1.61  2003/01/26 19:45:39  sm
**      - OpenGL drawing problem of Caligari imported objects fixed.
**
**      Revision 1.60  2003/01/26 14:11:50  sm
**      - COB support integrated into Lines III
**
**      Revision 1.59  2002/12/31 15:11:03  sm
**      - Fixed bound checking.
**      - Added a vector test module.
**
**      Revision 1.58  2002/12/27 12:55:38  sm
**      - Trying to optimize vectorization for ICC.
**
**      Revision 1.57  2002/08/24 13:22:02  sm
**      - Extensive debugging on threading code done!
**        o Cleaned up POSIX threads
**        o Made safe thread handling available in raytracing code
**        o b3PrepareInfo instantiates threads only once.
**      - Added new thread options to gcc: "-D_REENTRAND -pthread"
**        which I only can assume what they are doing;-)
**      - Time window in motion blur moved from [-0.5,0.5] to [0,1]
**        and corrected upper time limit.
**
**      Revision 1.56  2002/08/16 13:20:13  sm
**      - Removed some unused methods.
**      - Allocation bug found in brt3 - the Un*x version of the
**        Blizzard III raytracer: It's necessary to use b3ShapeRenderContext
**        rather than b3renderContext which doesn't initialize subdivision
**        for shapes.
**
**      Revision 1.55  2002/08/16 11:40:38  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.54  2002/08/15 13:56:43  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.53  2002/08/10 14:36:31  sm
**      - Some shapes had cleared the vertex array whenever the
**        b3AllocVertices() method were called. Without calling
**        b3Recomute() the shapes disoccured.
**      - Some methods moved as static methods into the
**        b3Mem class.
**
**      Revision 1.52  2002/08/07 12:38:43  sm
**      - Modified exception definition. Exceptions are identified with
**        a three character code to unify error codes. This is necessary
**        to convert error codes into error messages inside applications.
**      - Added some additional b3Hash methods.
**      - Added -Wall compiler option to all C++ files.
**      - Removed some compiler warnings.
**
**      Revision 1.51  2002/08/05 17:46:41  sm
**      - Some merges...
**
**      Revision 1.50  2002/08/05 16:04:55  sm
**      - Found first texture init bug. This wasn't an OpenGL bug. This
**        couldn't be because every implementation had got the same
**        bug. The static aux image for creating textures wasn't initialized
**        at the right time.
**      - Version handling introduced: The version number is extracted
**        from the version resource now.
**      - The b3Tx::b3AllocTx() method uses b3Realloc() for better
**        memory usage.
**      - Some b3World messages removed.
**      - The 0x7fff class is registered into the b3ItemRegister now. This
**        prevents printing a warning when this class isn't found. Due to
**        the fact that *every* Blizzard data contains this class every
**        data read put out this warning.
**
**      Revision 1.49  2002/08/04 13:24:55  sm
**      - Found transformation bug: Normals have to be treated as
**        direction vectors, aren't them?
**      - b3PrepareInfo::m_PrepareProc initialized not only in
**        debug mode.
**
**      Revision 1.48  2002/08/03 18:05:10  sm
**      - Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**      - Moved b3PrepareInfo into b3Scene class as member. This
**        saves memory allocation calls and is an investment into
**        faster Lines III object transformation.
**
**      Revision 1.47  2002/08/01 15:02:56  sm
**      - Found texture missing bug when printing. There weren't any
**        selected textures inside an other OpenGL rendering context.
**        Now fixed!
**
**      Revision 1.46  2002/07/31 13:46:02  sm
**      - I have to spend more brain into correct shading of quadrics...
**
**      Revision 1.45  2002/07/31 11:57:11  sm
**      - The nVidia OpenGL init bug fixed by using following work
**        around: The wglMakeCurrent() method is invoked on
**        every OnPaint(). This is configurable depending on the
**        hostname.
**
**      Revision 1.44  2002/07/31 07:30:44  sm
**      - New normal computation. Textures are rendered correctly and
**        quadrics are shaded correctly. Spheres and doughnuts have
**        got their own more simple computation.
**
**      Revision 1.43  2002/07/29 14:48:11  sm
**      - Circled shapes like cylinder, doughnuts etc. draw
**        textures correctly but renders shading a little bit
**        wrong at seam.
**      - Added support for multiple lights. This should be
**        configurable inside a scene (via b3ModellerInfo?)
**
**      Revision 1.42  2002/07/29 12:32:56  sm
**      - Full disk draws textures correctly now
**      - Windows selects the correct pixel format for
**        the nVidia driver.
**      - Some problems concerning first drawing and lighting
**        aren't fixed, yet. This seems to be a nVidia problem
**
**      Revision 1.41  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.40  2002/07/26 22:08:09  sm
**      - Some b3RenderObject derived classed didn't initialize
**        glTexCoord. It's time to use glInterleavedArrays() to
**        avoid such hazards.
**      - The nVidia driver seems to use only 16 bit for depth
**        buffer:-(
**
**      Revision 1.39  2002/07/26 10:22:37  sm
**      - Some minor fixes
**      - Texturing simply runs under Windows :-)
**
**      Revision 1.38  2002/07/26 09:13:33  sm
**      - Found alpha problem: the Linux OpenGL renderer didn't use the
**        b3RenderContext::b3Init() method! Now everything function very well:-)
**      - The Un*x OpenGL renderer has got a key press interface now.
**      - Corrected spot lights
**      - Textures needn't to be square any more (some less memory usage)
**
**      Revision 1.37  2002/07/25 19:06:21  sm
**      - Why does not alpha channel function?
**
**      Revision 1.36  2002/07/25 16:29:35  sm
**      - Further developing of texturing
**
**      Revision 1.35  2002/07/25 13:22:32  sm
**      - Introducing spot light
**      - Optimized light settings when drawing
**      - Further try of stencil maps
**
**      Revision 1.34  2002/07/23 07:04:05  sm
**      - Added torus support
**      - Precompute surface colors. So we don't need to collect
**        colors during mesh draw.
**      - Added simple texturing with appropriate scaling.
**
**      Revision 1.33  2002/07/22 18:45:16  sm
**      - Further probing of texture stencil via alpha channel.
**      - Why does Mesa loose the first texture?
**      - Nasty uncr.
**
**      Revision 1.32  2002/07/22 16:27:45  sm
**      - Fixed some errors concerning texture stencil
**
**      Revision 1.31  2002/07/22 12:46:08  sm
**      - Added Windows Lines III support for textures
**      - Fixed sphere computation
**
**      Revision 1.30  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.29  2002/07/21 21:09:37  sm
**      - Now having texture mapping! Texture mapping is only applied to
**        areas and cylinders.
**
**      Revision 1.28  2002/07/21 17:02:36  sm
**      - Finished advanced color mix support (correct Phong/Mork shading)
**      - Added first texture mapping support. Further development on
**        Windows now...
**
**      Revision 1.27  2002/07/20 10:49:34  sm
**      - Added custom light support (not finished yet)
**      - Added b3Light::b3IsActive() for compatibility.
**      - Added texture search path support like in brt3.
**
**      Revision 1.26  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.25  2002/01/30 19:46:41  sm
**      - Trying to print in debug mode (and want to see anything)
**
**      Revision 1.24  2002/01/21 16:56:46  sm
**      - Showing splash dialog only in release version.
**      - Prepared shape icons.
**
**      Revision 1.23  2002/01/20 12:48:51  sm
**      - Added splash screen
**      - Corrected repeat buttons (capture change)
**
**      Revision 1.22  2002/01/03 19:07:27  sm
**      - Cleaned up cut/paste
**
**      Revision 1.21  2002/01/01 13:50:21  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.20  2001/11/09 16:15:35  sm
**      - Image file encoder
**      - Performance meter for triangles / second added.
**      - Corrected Windows b3TimeSpan computation
**
**      Revision 1.19  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.18  2001/10/10 17:52:24  sm
**      - Texture loading (only reading into memory) running.
**      - Raytracing without OpenGL must be possible!
**
**      Revision 1.17  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.16  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.15  2001/09/01 06:16:42  sm
**      - Some merges
**      - Exchanging matrices between OpenGL nd Blizzard III.
**
**      Revision 1.14  2001/08/20 19:35:08  sm
**      - Index correction introduced (This is a hack!)
**      - Some OpenGL cleanups
**
**      Revision 1.13  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.12  2001/08/17 14:08:34  sm
**      - Now trying to draw BSPline surfaces with own routines.
**
**      Revision 1.11  2001/08/17 04:16:43  sm
**      - Using OpenGL NURBS zu render BSpline areas. But
**        I think mi tessalation is faster.
**
**      Revision 1.10  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.9  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.8  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
**
**      Revision 1.7  2001/08/14 19:07:43  sm
**      - Minor changes on resources.
**
**      Revision 1.6  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.5  2001/08/12 19:47:47  sm
**      - Now having correct orthogonal projection incl. aspect ratio
**
**      Revision 1.4  2001/08/11 20:17:22  sm
**      - Updated OpenGL on Un*x platform.
**      - This was a great day!
**
**      Revision 1.3  2001/08/11 19:59:15  sm
**      - Added orthogonal projection
**
**      Revision 1.2  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.1  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.3  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.2  2001/08/09 15:27:34  sm
**      - Following shapes are newly supported now:
**        o disk
**        o cylinder
**        o cone
**        o ellipsoid
**        o torus
**        o triangles
**      - Done some makefile fixes
**      - Everything is Windozable
**
**      Revision 1.1  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
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

static GLint light_num[] =
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
	glBgColor.b3Init(0.8,0.8,0.8);
}

void b3RenderContext::b3Init()
{
#ifdef BLZ3_USE_OPENGL
	char *extensions = glGetString(GL_EXTENSIONS);

	b3PrintF(B3LOG_FULL,  "b3RenderContext::b3Init()\n");
	b3PrintF(B3LOG_NORMAL,"OpenGL vendor:     %s\n",glGetString(GL_VENDOR));
	b3PrintF(B3LOG_NORMAL,"OpenGL renderer:   %s\n",glGetString(GL_RENDERER));
	b3PrintF(B3LOG_NORMAL,"OpenGL version:    %s\n",glGetString(GL_VERSION));
	b3PrintF(B3LOG_DEBUG, "OpenGL extensions: %s\n",extensions);

	if (strstr(extensions,"GL_ARB_vertex_program") != null)
	{
		b3PrintF(B3LOG_NORMAL,"Vertex shader support (nice).\n");
	}

	if (strstr(extensions,"GL_ARB_fragment_program") != null)
	{
		b3PrintF(B3LOG_NORMAL,"Pixel shader support (very fine).\n");
	}

	glDrawBuffer(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
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
	GLint   light;

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
	
/*************************************************************************
**                                                                      **
**                        b3RenderObject implementation                 **
**                                                                      **
*************************************************************************/

b3RenderObject::b3RenderObject()
{
	glVertexCount = 0;
	glGridCount   = 0;
	glPolyCount   = 0;

	glVertex      = null;
	glGrids       = null;
	glPolygons    = null;

	b3Recompute();
	b3RecomputeMaterial();

#ifdef BLZ3_USE_OPENGL
	glTextureId    = 0;
	glTextureData  = null;
	glTextureSize  = 0;
	glTextureSizeX = 0;
	glTextureSizeY = 0;
#endif
}

b3RenderObject::~b3RenderObject()
{
	b3CreateTexture(null,0);
}

void b3RenderObject::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = 0;
	gridCount = 0;
	polyCount = 0;
}

void b3RenderObject::b3AddCount(b3RenderContext *context)
{
	context->glVertexCount += glVertexCount;
	context->glPolyCount   += glPolyCount;
	context->glGridCount   += glGridCount;
}

/*************************************************************************
**                                                                      **
**                        Vertex/normal computation                     **
**                                                                      **
*************************************************************************/

void b3RenderObject::b3Recompute()
{
	glComputed = false;
}

void b3RenderObject::b3Update()
{
	if (!glComputed)
	{
		b3ComputeIndices();
		b3ComputeVertices();
		b3ComputeNormals();
		glComputed = true;
	}
}

void b3RenderObject::b3AllocVertices(b3RenderContext *context)
{
	b3_count new_vertCount = 0;
	b3_count new_gridCount = 0;
	b3_count new_polyCount = 0;

	b3GetCount(context,new_vertCount,new_gridCount,new_polyCount);

	if (glVertexCount != new_vertCount)
	{
		b3Free(glVertex);
		glVertex      = null;
		glVertexCount = new_vertCount;

		if (glVertexCount > 0)
		{
			glVertex = (b3_gl_vertex *)b3Alloc(glVertexCount * sizeof(b3_gl_vertex));
		}
		glComputed = false;
	}

	if (glGridCount != new_gridCount)
	{
		b3Free(glGrids);
		glGrids     = null;
		glGridCount = new_gridCount;

		if (glGridCount > 0)
		{
			glGrids = (b3_gl_line *)b3Alloc(glGridCount * sizeof(b3_gl_line));
		}
		glComputed = false;
	}

	if (glPolyCount != new_polyCount)
	{
		b3Free(glPolygons);
		glPolygons  = null;
		glPolyCount = new_polyCount;

		if (glPolyCount > 0)
		{
			glPolygons = (b3_gl_polygon *)b3Alloc(glPolyCount * sizeof(b3_gl_polygon));
		}
		glComputed = false;
	}
}

void b3RenderObject::b3FreeVertices()
{
	b3Free(glVertex);
	b3Free(glGrids);
	b3Free(glPolygons);
	glVertex      = null;
	glGrids       = null;
	glPolygons    = null;
	glVertexCount = 0;
	glGridCount   = 0;
	glPolyCount   = 0;
}

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}

void b3RenderObject::b3ComputeNormals(b3_bool normalize)
{
	b3_gl_vector normal;
	b3_gl_vector xDir,yDir;
	b3_index     i,k,v1,v2,v3,start,end;

	// Clear normals
	b3GetVertexRange(start,end);
	for (i = start;i < end;i++)
	{
		glVertex[i].n.x =
		glVertex[i].n.y =
		glVertex[i].n.z = 0;
	}

	// Collect normals
	for (i = k = 0;i < glPolyCount;i++)
	{
		v1 = glPolygons[k].a;
		v2 = glPolygons[k].b;
		v3 = glPolygons[k].c;
		k++;

		// Do some semantic checks
		if ((v1 < start) || (v1 >= end) ||
		    (v2 < start) || (v2 >= end) ||
		    (v3 < start) || (v3 >= end))
		{
			b3PrintF(B3LOG_NORMAL,"###### %d: %d # %d %d %d # %d (%s:l.%d)\n",i,
				start,v1,v2,v3,end,__FILE__,__LINE__);
		}

		b3Vector::b3Sub(&glVertex[v2].v,&glVertex[v1].v,&xDir);
		b3Vector::b3Sub(&glVertex[v3].v,&glVertex[v1].v,&yDir);
		b3Vector::b3CrossProduct(&xDir,&yDir,&normal);
		if (b3Vector::b3Normalize(&normal) > 0.001)
		{
			b3Vector::b3Add(&normal,&glVertex[v1].n);
			b3Vector::b3Add(&normal,&glVertex[v2].n);
			b3Vector::b3Add(&normal,&glVertex[v3].n);
		}
	}

	// Normalize
	if (normalize)
	{
		for (i = 0;i < glVertexCount;i++)
		{
			b3Vector::b3Normalize(&glVertex[i].n);
		}
	}
}

void b3RenderObject::b3GetVertexRange(b3_index &start,b3_index &end)
{
	start = 0;
	end   = glVertexCount;
}

b3_bool b3RenderObject::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3_bool   result = false;
	b3_index  i,start,end;

	b3Update();
	if (glComputed && (glVertex != null) && (glVertexCount > 0))
	{
		b3GetVertexRange(start,end);
		for (i = start;i < end;i++)
		{
			b3Vector::b3AdjustBound(&glVertex[i].v,lower,upper);
		}
		result = true;
	}

	return result;
}

void b3RenderObject::b3TransformVertices(
	b3_matrix *transformation,
	b3_bool    is_affine)
{
	b3_count i;

	if (glVertex != null)
	{
		if (is_affine)
		{
			for (i = 0;i < glVertexCount;i++)
			{
				b3Vector::b3MatrixMul4D(transformation,(b3_vector *)&glVertex[i].v);
				b3Vector::b3MatrixMul3D(transformation,(b3_vector *)&glVertex[i].n);
			}
		}
		else
		{
			for (i = 0;i < glVertexCount;i++)
			{
				b3Vector::b3MatrixMul4D(transformation,(b3_vector *)&glVertex[i].v);
			}
			b3ComputeNormals();
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Material definition                           **
**                                                                      **
*************************************************************************/

static b3Tx    glTextureBuffer;
static b3Mutex glTextureMutex;

b3Color b3RenderObject::m_GridColor(0.2f,0.2f,0.2f,0.0f);
b3Color b3RenderObject::m_SelectedColor(1.0f,0.1f,0.25f,0.0f);

void b3RenderObject::b3RecomputeMaterial()
{
#ifdef BLZ3_USE_OPENGL
	glMaterialComputed = false;
#endif
}

void b3RenderObject::b3GetDiffuseColor(b3Color &diffuse)
{
	diffuse.b3Init(0.0f, 0.5f, 1.0f);
}

b3_f64 b3RenderObject::b3GetColors(
	b3Color &ambient,
	b3Color &diffuse,
	b3Color &specular)
{
	ambient.b3Init(  0.5, 0.5, 0.5);
	diffuse.b3Init(  0.0, 0.5, 1.0);
	specular.b3Init( 1.0, 1.0, 1.0);
	return 1.0;
}

b3_bool b3RenderObject::b3GetChess(
	b3Color &bColor,
	b3Color &wColor,
	b3_res  &xRepeat,
	b3_res  &yRepeat)
{
	return false;
}

b3Tx *b3RenderObject::b3GetTexture(b3_f64 &xTrans,b3_f64 &yTrans,b3_f64 &xScale,b3_f64 &yScale)
{
	return null;
}

b3_bool b3RenderObject::b3GetImage(b3Tx *image)
{
	return false;
}

void b3RenderObject::b3DefineTexture()
{
#ifdef BLZ3_USE_OPENGL
	GLfloat blend[4];

	b3RenderContext::b3PkdColorToGL(B3_TRANSPARENT | B3_WHITE,blend);

	// Set texture parameter
	glTexEnvi(      GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,  GL_BLEND);
	glTexEnvfv(     GL_TEXTURE_2D,GL_TEXTURE_ENV_COLOR, blend);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,    GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,    GL_REPEAT);
	glTexImage2D(   GL_TEXTURE_2D,
		0,GL_RGBA,glTextureSizeX,glTextureSizeY,
		0,GL_RGBA,GL_UNSIGNED_BYTE,glTextureData);
#endif
}

void b3RenderObject::b3UpdateMaterial()
{
#ifdef BLZ3_USE_OPENGL
	if (!glMaterialComputed)
	{
		b3Tx    *tx;
		b3Color  black,white;
		b3Color  ambient,diffuse,specular;
		b3_res   xRep,yRep;

		glShininess = b3GetColors(ambient,diffuse,specular);
		glTextureTransX = 0;
		glTextureTransY = 0;
		if (b3GetChess(black,white,xRep,yRep))
		{
			glTextureScaleX = 0.5 * xRep;
			glTextureScaleY = 0.5 * yRep;
			b3CreateChess(null,black,white);
		}
		else
		{
			b3_f64 xScale = 1;
			b3_f64 yScale = 1;

			tx = b3GetTexture(glTextureTransX,glTextureTransY,xScale,yScale);
			if (tx != null)
			{
				glTextureScaleX = 1.0 / xScale;
				glTextureScaleY = 1.0 / yScale;
				b3CopyTexture(null,tx);
			}
			else
			{
				glTextureScaleX = 1;
				glTextureScaleY = 1;
				glTextureMutex.b3Lock();
				if (!glTextureBuffer.b3IsLoaded())
				{
#ifndef _DEBUG
					b3_res max  = 128;
#else
					b3_res max  =   8;
#endif
					glTextureBuffer.b3AllocTx(max,max,24);
				}
				glTextureMutex.b3Unlock();

				glTextureMutex.b3Lock();
				if (b3GetImage(&glTextureBuffer))
				{
					b3CreateImage(null,&glTextureBuffer);
				}
				else
				{
					// Free memory
					b3CreateTexture(null,0);
				}
				glTextureMutex.b3Unlock();
			}
		}

		if (glTextureSize > 0)
		{
			glBindTexture(  GL_TEXTURE_2D,glTextureId);
			b3DefineTexture();

			// Set material parameter
			b3RenderContext::b3PkdColorToGL(B3_WHITE,glAmbient);
			b3RenderContext::b3PkdColorToGL(B3_WHITE,glDiffuse);
			b3RenderContext::b3PkdColorToGL(B3_WHITE,glSpecular);
		}
		else
		{
#if 1
			ambient = specular = diffuse;
#endif
			b3RenderContext::b3ColorToGL(ambient, glAmbient);
			b3RenderContext::b3ColorToGL(diffuse, glDiffuse);
			b3RenderContext::b3ColorToGL(specular,glSpecular);
		}
		glMaterialComputed = true;
	}
#endif
}

void b3RenderObject::b3CreateTexture(
	b3RenderContext *context,
	b3_res           xSize,
	b3_res           ySize)
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
			b3PrintF(B3LOG_FULL,"b3RenderObject::b3CreateTexture(...,%4d,%4d) # %5d previous: %5d\n",
				xSize,ySize,size,glTextureSize);
			if (size != 0)
			{
				void   *ptr = b3Realloc(glTextureData,size * 4);
				GLenum  error;

				if (ptr == null)
				{
					B3_THROW(b3TxException,B3_TX_MEMORY);
				}
				glTextureData  = (GLubyte *)ptr;
				glTextureSize  =  size;
				glTextureSizeX = xSize;
				glTextureSizeY = ySize;
				b3PrintF(B3LOG_FULL,"   Allocated texture memory of %d pixel\n",size);

				if (glTextureId == 0)
				{
					glGenTextures(1,&glTextureId);
					error = glGetError();

					if (error != GL_NO_ERROR)
					{
						b3PrintF(B3LOG_NORMAL,"  glGetError() = %d\n",error);
						B3_THROW(b3TxException,B3_TX_MEMORY);
					}

					glBindTexture(GL_TEXTURE_2D,glTextureId);
					B3_ASSERT(glIsTexture(glTextureId));
					b3PrintF(B3LOG_FULL,"   Allocated texture id %d\n",glTextureId);
				}
			}
			else
			{
				if (glTextureId != 0)
				{
					b3PrintF(B3LOG_FULL,"   Freeing texture id %d\n",glTextureId);
					glDeleteTextures(1,&glTextureId);
					glTextureId = 0;
				}
				b3PrintF(B3LOG_FULL,"   Freeing texture data\n");
				b3Free(glTextureData);
				glTextureData  = null;
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
	catch(...)
	{
		// Restore to defined and unallocated state
		if (glTextureData != null)
		{
			b3Free(glTextureData);
		}
		if (glTextureId != 0)
		{
			glDeleteTextures(1,&glTextureId);
		}
		glTextureData  = null;
		glTextureId    = 0;
		glTextureSize  = 0;
		glTextureSizeX = 0;
		glTextureSizeY = 0;
		throw;
	}
#endif
}

void b3RenderObject::b3CreateChess(
	b3RenderContext *context,
	b3Color         &bColor,
	b3Color         &wColor)
{
#ifdef BLZ3_USE_OPENGL
	b3CreateTexture(null,2);

	b3RenderContext::b3ColorToGL(wColor,&glTextureData[ 0]);
	b3RenderContext::b3ColorToGL(bColor,&glTextureData[ 4]);
	b3RenderContext::b3ColorToGL(bColor,&glTextureData[ 8]);
	b3RenderContext::b3ColorToGL(wColor,&glTextureData[12]);
#endif
}

void b3RenderObject::b3CopyTexture(
	b3RenderContext *context,
	b3Tx            *input)
{
#ifdef BLZ3_USE_OPENGL
	b3Tx          scale;
	b3_pkd_color *lPtr;
#ifndef _DEBUG
	b3_res        max  = 128;
#else
	b3_res        max  =   8;
#endif
	b3_res        xMax = max,yMax = max,size;
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
	b3PrintF(B3LOG_FULL,"b3RenderObject::b3CopyTexture(...) # xSize: %4d ySize: %4d # xMax: %4d yMax: %4d\n",
		input->xSize,input->ySize,xMax,yMax);
	scale.b3AllocTx(xMax,yMax,24);
	scale.b3Scale(input);
	b3CreateTexture(context,xMax,yMax);

	lPtr = (b3_pkd_color *)scale.b3GetData();
	size = xMax * yMax;
	for (i = 0;i < size;i++)
	{
		b3RenderContext::b3PkdColorToGL(*lPtr++,&glTextureData[i << 2]);
	}
#endif
}

void b3RenderObject::b3CreateImage(
	b3RenderContext *context,
	b3Tx            *input)
{
#ifdef BLZ3_USE_OPENGL
	b3_pkd_color *lPtr = (b3_pkd_color *)input->b3GetData();
	b3_coord      size,i = 0;

	B3_ASSERT(lPtr != null);
	size = input->xSize * input->ySize;
	b3PrintF(B3LOG_FULL,"b3RenderObject::b3CreateImage(...) # size: %4d\n",size);
	b3CreateTexture(context,input->xSize,input->ySize);
	for (i = 0;i < size;i++)
	{
		b3RenderContext::b3PkdColorToGL(*lPtr++,&glTextureData[i << 2]);
	}
#endif
}

/*************************************************************************
**                                                                      **
**                        Rendering (the central part)                  **
**                                                                      **
*************************************************************************/

void b3RenderObject::b3Draw(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	b3_render_mode render_mode = b3GetRenderMode();
	b3Color        diffuse;
#endif

	b3Update();
	b3UpdateMaterial();
#ifdef _DEBUG
	b3_index       i;

	switch (render_mode)
	{
	case B3_RENDER_LINE:
		// This loop collects access vialoations
		// prior calling OpenGL routines. This
		// makes it possible to catch to faulty
		// index data. The access simply compute
		// the length of the lines to be drawn.
		for (i = 0;i < glGridCount;i++)
		{
			b3_vector aPoint,bPoint;
			b3_index  a,b;
			b3_f64    len;

			a = glGrids[i].a;
			aPoint.x = glVertex[a].v.x;
			aPoint.y = glVertex[a].v.y;
			aPoint.z = glVertex[a].v.z;

			b = glGrids[i].b;
			bPoint.x = glVertex[b].v.x;
			bPoint.y = glVertex[b].v.y;
			bPoint.z = glVertex[b].v.z;

			len = b3Vector::b3Distance(&aPoint,&bPoint);
		}
		break;

	case B3_RENDER_FILLED:
		for (i = 0;i < glPolyCount;i++)
		{
			b3_vector aPoint,bPoint,cPoint;
			b3_index  a,b,c;
			b3_f64    aLen,bLen;

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

			aLen = b3Vector::b3Distance(&aPoint,&bPoint);
			bLen = b3Vector::b3Distance(&aPoint,&cPoint);
		}
		break;

	default:
		// Nothing to do...
		break;
	}
#endif

#ifdef BLZ3_USE_OPENGL
	switch (render_mode)
	{
	case B3_RENDER_NOTHING:
		// Do nothing!
		break;

	case B3_RENDER_LINE:
		if (glGridCount > 0)
		{
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
			glColor3f(diffuse[b3Color::R],diffuse[b3Color::G],diffuse[b3Color::B]);

			// Put geometry :-)
			B3_ASSERT(glVertex != null);
			glInterleavedArrays(GL_T2F_N3F_V3F,0, glVertex);
			glDrawElements(GL_LINES,glGridCount * 2,GL_UNSIGNED_SHORT,glGrids);
		}
		break;

	case B3_RENDER_FILLED:
		if (glPolyCount > 0)
		{
			glEnable(GL_LIGHTING);
			if (glTextureSize > 0)
			{
				if (context->glDrawCachedTextures)
				{
					B3_ASSERT(glIsTexture(glTextureId));
					glBindTexture(GL_TEXTURE_2D,glTextureId);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D,0);
					b3DefineTexture();
				}
				glEnable(     GL_TEXTURE_2D);

				// Set repitition of chess fields by scaling texture
				// coordinates.
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glTranslatef(glTextureTransX,glTextureTransY,0.0);
				glScalef(    glTextureScaleX,glTextureScaleY,1.0);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}

			// Set material
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  glAmbient);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  glDiffuse);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, glSpecular);
			glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,glShininess);

			// Put geometry :-)
			B3_ASSERT(glVertex != null);
#ifndef _DEBUG
			glInterleavedArrays(GL_T2F_N3F_V3F,0, glVertex);
			glDrawElements(GL_TRIANGLES, glPolyCount * 3,GL_UNSIGNED_SHORT,glPolygons);
#else
			GLenum error = glGetError();
			glInterleavedArrays(GL_T2F_N3F_V3F,0, glVertex);
			error = glGetError();
			if (error == GL_NO_ERROR)
			{
				glDrawElements(GL_TRIANGLES, glPolyCount * 3,GL_UNSIGNED_SHORT,glPolygons);
				error = glGetError();
			}
#endif
		}
		break;

	default:
		b3PrintF(B3LOG_NORMAL,"Illegal render mode selected.\n");
		break;
	}
#endif
}
