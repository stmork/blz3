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
#include "blz3/base/b3VertexBuffer.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Color.h"

#ifdef BLZ3_USE_OPENGL
#define B3_DISPLAY_LIST
#define B3_DISPLAY_LIST_COUNT 1
#endif

#ifndef _DEBUG
#define B3_MAX_TX_SIZE 128
#else
#define B3_MAX_TX_SIZE   8
#define VERBOSE
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.103  2004/11/29 16:35:02  smork
**      - Added additional VBO version which recomputes in host memory
**        and only transfers recomputed data to GPU.
**
**      Revision 1.102  2004/11/27 10:31:12  sm
**      - Removed b3Mem heritage from VBO handlers
**
**      Revision 1.101  2004/11/24 10:32:18  smork
**      - Optimized VBO mapping.
**
**      Revision 1.100  2004/11/23 09:01:10  smork
**      - Bump revision
**      - Added non OpenGL compiling for VBOs.
**
**      Revision 1.99  2004/11/21 14:56:57  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.98  2004/10/16 20:36:34  sm
**      - Fixed redraw problem after action in Lines.
**
**      Revision 1.97  2004/09/25 08:56:53  sm
**      - Removed VBOs from source.
**
**      Revision 1.96.2.4  2004/11/21 10:17:32  sm
**      - We have to map the object before getting the pointer. Then the
**        bounding boxes can be computed correctly to setup the far and
**        near clipping plane correctly. When mapping correctly the
**        transformation can occur correctly which plays the ananimation
**        in a way we expect ;-)
**        ** That's it **
**
**      Revision 1.96.2.3  2004/11/20 11:37:15  sm
**      - Added Windows VBO support
**
**      Revision 1.96.2.2  2004/11/19 19:38:43  sm
**      - OK. The arrays are drawing correctly and the ATi VBOs are drawing
**        something. The draw buffer seams to be defective. Now we should
**        look what nVIDIA is doing with my code.
**
**      Revision 1.96.2.1  2004/11/16 07:14:55  sm
**      - Added OO-version of vertex buffer objects.
**
**      Revision 1.96  2004/09/24 20:22:05  sm
**      - Some VBO adjustments.
**
**      Revision 1.95  2004/09/24 13:45:36  sm
**      - Extracted OpenGL extension vector buffer objects into own files.
**      - Some cleanup for Lines.
**
**      Revision 1.94  2004/09/24 11:42:14  sm
**      - First VBO run under Linux.
**
**      Revision 1.93  2004/09/23 21:27:38  sm
**      - VBOs still don't work.
**
**      Revision 1.92  2004/09/23 20:02:25  sm
**      - Introduced VBOs on Windows - with success!
**
**      Revision 1.91  2004/09/23 16:05:28  sm
**      - Some BLZ3_USE_OPENGL caveats removed.
**
**      Revision 1.90  2004/09/23 15:47:04  sm
**      - Splitted b3RenderContext into own file.
**      - Added vertex buffer object support which does not
**        run yet.
**
**      Revision 1.89  2004/09/19 15:36:18  sm
**      - Changed polygon/grid index data type from short (Hey! Are we
**        on Windows 3.11???) to long.
**
**      Revision 1.88  2004/09/11 13:30:50  sm
**      - Corrected link libraries in makefiles.
**      - Corrected GLint to GLenum for light control.
**
**      Revision 1.87  2004/08/26 07:08:10  sm
**      - Higher bitrate in DivX encoder.
**      - Disbled unnecessary AUTO_NORMAL option in OpenGL renderer.
**
**      Revision 1.86  2004/08/18 15:03:46  sm
**      - Added render support for flat triangle field (no Phong interpolation)
**
**      Revision 1.85  2004/08/14 15:06:22  sm
**      - Revert max array determiniation.
**
**      Revision 1.84  2004/08/11 08:41:34  sm
**      - Using Display list for setting shaded materials. Geometry
**        and lined materials are set conventionally.
**
**      Revision 1.83  2004/08/10 09:25:20  sm
**      - Merging
**
**      Revision 1.82  2004/08/09 10:09:05  sm
**      - Added color palette reduction and its tool.
**
**      Revision 1.81  2004/08/09 07:43:12  sm
**      - Some display list optimizations.
**
**      Revision 1.80  2004/08/07 09:17:22  sm
**      - Reflect new Cg compiler SDK
**
**      Revision 1.79  2004/08/07 07:13:32  sm
**      - Added OpenGL display list support. The display lists under Linux
**        are slower than the raw draw array variant.
**
**      Revision 1.78  2004/07/18 08:28:44  sm
**      - Added transformation optimazation: We don't need to recompute
**        vertices on unit matrix transformation. This simple test makes
**        animation a lot faster!
**
**      Revision 1.77  2004/07/15 12:12:20  sm
**      - Some FSAA debugging- Some FSAA debugging.
**
**      Revision 1.76  2004/07/14 09:07:40  sm
**      - Disabling FSAA = multi sampling
**      - Some more b3Sampler initializations.
**
**      Revision 1.75  2004/07/05 12:15:53  sm
**      - Added multi threaded sampler for OpenGL image creation.
**
**      Revision 1.74  2004/07/03 13:49:30  sm
**      - Added spline knot control dialog which is not completed yet.
**
**      Revision 1.73  2004/07/02 19:28:03  sm
**      - Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**      - Recoupled b3Scene include from CApp*Doc header files to allow
**        faster compilation.
**      - Removed intersection counter completely because of a mysterious
**        destruction problem of b3Mutex.
**
**      Revision 1.72  2004/06/29 12:49:12  sm
**      - Added some OpenGL shader comments.
**
**      Revision 1.71  2004/06/29 12:43:26  sm
**      - Fixed uninitialized data inside OpenGL texture creation. This
**        speeds up initial data scene load.
**
**      Revision 1.70  2004/06/27 11:36:54  sm
**      - Changed texture dialog for editing negative direction in
**        contrast to length.
**      - Set document to modified if materials or bumps are changed.
**      - Check for empty textures inside OpenGL subsystem. May this
**        be ticket no. 21?
**      - Animation values initialization fix.
**
**      Revision 1.69  2004/06/21 18:10:53  sm
**      - Fixed disk drawing problem. The Problem occured in
**        conjunction with optimizing code with the Visual C++
**        compiler. Should I install Visual.Net?
**
**      Revision 1.68  2004/05/28 14:06:29  sm
**      - Minor optimizations in shader
**
**      Revision 1.67  2004/03/07 13:08:52  sm
**      - Added dialog for Cook/Torrance material
**
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
	glDisplayList  = 0;
	glTextureId    = 0;
	glTextureData  = null;
	glTextureSize  = 0;
	glTextureSizeX = 0;
	glTextureSizeY = 0;
	glVertexElements  = null;
	glGridElements    = null;
	glPolygonElements = null;
#endif
}

b3RenderObject::~b3RenderObject()
{
	if (glVertexElements != null)
	{
		delete glVertexElements;
	}
	if (glGridElements != null)
	{
		delete glGridElements;
	}
	if (glPolygonElements != null)
	{
		delete glPolygonElements;
	}
	b3CreateTexture(null,0);
	b3DeleteDisplayList();
}

void b3RenderObject::b3DeleteDisplayList()
{
#ifdef B3_DISPLAY_LIST
	if (glDisplayList != 0)
	{
		glDeleteLists(glDisplayList,B3_DISPLAY_LIST_COUNT);
		glDisplayList = 0;
	}
#endif
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
	context->glVertexCount += glVertexElements->b3GetCount();
	context->glPolyCount   += glPolygonElements->b3GetCount();
	context->glGridCount   += glGridElements->b3GetCount();
}

/*************************************************************************
**                                                                      **
**                        Vertex/normal computation                     **
**                                                                      **
*************************************************************************/

#ifdef VERBOSE
static void print_mapping(const char *text,b3_vbo_mapping map_mode)
{
	const char *mapping_text;

	switch(map_mode)
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
	b3PrintF(B3LOG_FULL,"      %s(%s)\n",text,mapping_text);
}
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

	glVertexElements->b3Map(map_mode);
}

void b3RenderObject::b3UnmapIndices()
{
	glGridElements->b3Unmap();
	glPolygonElements->b3Unmap();
}

void b3RenderObject::b3UnmapVertices()
{
	glVertexElements->b3Unmap();
}

void b3RenderObject::b3Recompute()
{
	glVertexElements->b3Recompute();
}

void b3RenderObject::b3RecomputeIndices()
{
	glGridElements->b3Recompute();
	glPolygonElements->b3Recompute();
}

void b3RenderObject::b3SetupVertexMemory(b3RenderContext *context)
{
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"##### >b3RenderObject::b3SetupVertexMemory() this = %p\n",this);
#endif

	b3PreAlloc();
	b3AllocVertexMemory(context);

#ifdef VERBOSE
	b3MapVertices(B3_MAP_VBO_R);
	b3MapIndices(B3_MAP_VBO_R);

	b3_gl_vertex  *glVertex   = *glVertexElements;
	b3_gl_line    *glGrids    = *glGridElements;
	b3_gl_polygon *glPolygons = *glPolygonElements;

	b3PrintF(B3LOG_FULL,"       %5d vertices: %p - %s\n",
		 glVertexElements->b3GetCount(), glVertex,
		 glVertexElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL,"       %5d grids:    %p - %s\n",
		 glGridElements->b3GetCount(), glGrids,
		 glGridElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL,"       %5d polygons: %p - %s\n",
		 glPolygonElements->b3GetCount(), glPolygons,
		 glPolygonElements->b3IsCustom() ? "custom" : "buffer");

	b3UnmapVertices();
	b3UnmapIndices();

	b3PrintF(B3LOG_FULL,"##### <b3RenderObject::b3SetupVertexMemory() this = %p\n",this);
#endif
}

void b3RenderObject::b3PreAlloc()
{
	if (!glInit)
	{
#ifdef BLZ3_USE_OPENGL
		if (b3VectorBufferObjects::b3HasVBO())
		{
#if 1
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
#else
		glVertexElements  = new b3SimpleVertexElements();
		glGridElements    = new b3SimpleGridElements();
		glPolygonElements = new b3SimplePolygonElements();
#endif
		glInit = true;
	}
}

void b3RenderObject::b3AllocVertexMemory(b3RenderContext *context)
{
	b3_count new_vertCount = 0;
	b3_count new_gridCount = 0;
	b3_count new_polyCount = 0;

	b3GetCount(context,new_vertCount,new_gridCount,new_polyCount);

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"       b3GetCount(NV=%d, NG=%d, NP=%d)\n",
		new_vertCount,new_gridCount,new_polyCount);
	b3PrintF(B3LOG_FULL,"      >b3AllocVertexMemory( V=%d,  G=%d,  P=%d)\n",
		glVertexElements->b3GetCount(),
		glGridElements->b3GetCount(),
		glPolygonElements->b3GetCount());
#endif

	glVertexElements->b3AllocVertexMemory( context,new_vertCount);
	glVertexElements->b3SetCustom(new_vertCount <= 0);

	glGridElements->b3AllocVertexMemory(   context,new_gridCount);
	glGridElements->b3SetCustom(new_gridCount <= 0);

	glPolygonElements->b3AllocVertexMemory(context,new_polyCount);
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
		b3PrintF(B3LOG_FULL,"##### >b3RenderObject::b3Update() this = %p\n",this);
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
		b3PrintF(B3LOG_FULL,"      >b3RenderObject::b3UpdateIndices()\n");

		b3_gl_line    *glGrids    = *glGridElements;
		b3_gl_polygon *glPolygons = *glPolygonElements;
		
		b3PrintF(B3LOG_FULL,"       %5d grids:    %p - %s\n",
			 glGridElements->b3GetCount(), glGrids,
			 glGridElements->b3IsCustom() ? "custom" : "buffer");
		b3PrintF(B3LOG_FULL,"       %5d polygons: %p - %s\n",
			 glPolygonElements->b3GetCount(), glPolygons,
			 glPolygonElements->b3IsCustom() ? "custom" : "buffer");
#endif

		b3ComputeIndices();

		glGridElements->b3CustomData();
		glPolygonElements->b3CustomData();

		glGridElements->b3Recomputed();
		glPolygonElements->b3Recomputed();

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL,"      <b3RenderObject::b3UpdateIndices()\n");
#endif
	}

	if (mode & B3_UPDATE_VERTICES)
	{
#ifdef VERBOSE
		b3PrintF(B3LOG_FULL,"      >b3RenderObject::b3UpdateVertices()\n");

		b3_gl_vertex  *glVertex   = *glVertexElements;

		b3PrintF(B3LOG_FULL,"       %5d vertices: %p - %s\n",
			 glVertexElements->b3GetCount(), glVertex,
			 glVertexElements->b3IsCustom() ? "custom" : "buffer");
#endif
		b3ComputeVertices();
		b3ComputeNormals();

		glVertexElements->b3CustomData();
		glVertexElements->b3Recomputed();

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL,"      <b3RenderObject::b3UpdateVertices()\n");
#endif
	}

	switch (mode)
	{
	case B3_UPDATE_VERTICES | B3_UPDATE_INDICES:
	case B3_UPDATE_VERTICES:
		b3UnmapVertices();
		// Walk Through !!
	case B3_UPDATE_INDICES:
		b3UnmapIndices();
		break;
	}

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"##### <b3RenderObject::b3Update()\n");
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
	b3PrintF(B3LOG_FULL,"        >b3RenderObject::b3ComputeNormals()\n");
#endif

	b3_gl_vector   normal;
	b3_gl_vector   xDir,yDir;
	b3_gl_vertex  *glVertex      = *glVertexElements;
	b3_gl_polygon *glPolygons    = *glPolygonElements;
	b3_count       glVertexCount =  glVertexElements->b3GetCount();
	b3_count       glGridCount   =  glGridElements->b3GetCount();
	b3_count       glPolyCount   =  glPolygonElements->b3GetCount();
	b3_index       i,start,end,v1,v2,v3;

#ifdef VERBOSE
	b3_gl_line    *glGrids    = *glGridElements;

	b3PrintF(B3LOG_FULL,"       %5d vertices: %p - %s\n",
		 glVertexElements->b3GetCount(), glVertex,
		 glVertexElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL,"       %5d grids:    %p - %s\n",
		 glGridElements->b3GetCount(), glGrids,
		 glGridElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL,"       %5d polygons: %p - %s\n",
		 glPolygonElements->b3GetCount(), glPolygons,
		 glPolygonElements->b3IsCustom() ? "custom" : "buffer");
#endif

	// Clear normals
	b3GetVertexRange(start,end);
	for (i = start;i < end;i++)
	{
		glVertex[i].n.x =
		glVertex[i].n.y =
		glVertex[i].n.z = 0;
	}

	// Collect normals
	for (i = 0;i < glPolyCount;i++)
	{
		v1 = glPolygons[i].a;
		v2 = glPolygons[i].b;
		v3 = glPolygons[i].c;

		// Do some semantic checks
		if ((v1 < start) || (v1 >= end) ||
		    (v2 < start) || (v2 >= end) ||
		    (v3 < start) || (v3 >= end))
		{
			b3PrintF(B3LOG_NORMAL,"###### %d: %d # %d %d %d # %d # (V: %d  P: %d  G: %d) # (%s:l.%d)\n",i,
				start,v1,v2,v3,end,glVertexCount,glPolyCount,glGridCount,__FILE__,__LINE__);
		}
		else
		{
			b3Vector::b3Sub(&glVertex[v2].v,&glVertex[v1].v,&xDir);
			b3Vector::b3Sub(&glVertex[v3].v,&glVertex[v1].v,&yDir);
			b3Vector::b3CrossProduct(&xDir,&yDir,&normal);
			if (b3Vector::b3Normalize(&normal) > 1e-5)
			{
				b3Vector::b3Add(&normal,&glVertex[v1].n);
				b3Vector::b3Add(&normal,&glVertex[v2].n);
				b3Vector::b3Add(&normal,&glVertex[v3].n);
			}
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
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"        <b3RenderObject::b3ComputeNormals()\n");
#endif
}

void b3RenderObject::b3GetVertexRange(b3_index &start,b3_index &end)
{
	start = 0;
	end   = glVertexElements->b3GetCount();
}

b3_bool b3RenderObject::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3Update();

	b3MapVertices(B3_MAP_VBO_R);

	b3_gl_vertex *glVertex = *glVertexElements;
	b3_bool       result = false;
	b3_index      i,start,end;

	if (glVertexElements->b3IsComputed() && (glVertex != null) && (glVertexElements->b3GetCount() > 0))
	{
		b3GetVertexRange(start,end);
		for (i = start;i < end;i++)
		{
			b3Vector::b3AdjustBound(&glVertex[i].v,lower,upper);
		}
		result = true;
	}
	b3UnmapVertices();

	return result;
}

void b3RenderObject::b3TransformVertices(
	b3_matrix *transformation,
	b3_bool    is_affine)
{
	b3MapVertices(B3_MAP_VBO_RW);
	b3MapIndices(B3_MAP_VBO_R);

	b3_count      glVertexCount =  glVertexElements->b3GetCount();
	b3_gl_vertex *glVertex      = *glVertexElements;
	b3_count      i;

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
	b3UnmapVertices();
	b3UnmapIndices();
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

		b3DeleteDisplayList();

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
			if ((tx != null) && (tx->b3IsLoaded()))
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
					b3_res max  = B3_MAX_TX_SIZE;

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
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL,"b3RenderObject::b3CreateTexture(...,%4d,%4d) # %5d previous: %5d\n",
				xSize,ySize,size,glTextureSize);
#endif
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
#ifdef VERBOSE
				b3PrintF(B3LOG_FULL,"   Allocated texture memory of %d pixel\n",size);
#endif

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
#ifdef VERBOSE
					b3PrintF(B3LOG_FULL,"   Allocated texture id %d\n",glTextureId);
#endif
				}
			}
			else
			{
				if (glTextureId != 0)
				{
#ifdef VERBOSE
					b3PrintF(B3LOG_FULL,"   Freeing texture id %d\n",glTextureId);
#endif
					glDeleteTextures(1,&glTextureId);
					glTextureId = 0;
				}
#ifdef VERBOSE
				b3PrintF(B3LOG_FULL,"   Freeing texture data\n");
#endif
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
	b3PrintF(B3LOG_FULL,"b3RenderObject::b3CopyTexture(...) # xSize: %4d ySize: %4d # xMax: %4d yMax: %4d\n",
		input->xSize,input->ySize,xMax,yMax);
#endif
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
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"b3RenderObject::b3CreateImage(...) # size: %4d\n",size);
#endif
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
	b3_render_mode render_mode = b3GetRenderMode();

#ifdef BLZ3_USE_OPENGL
#ifdef B3_DISPLAY_LIST

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"##### >b3RenderObject::b3Draw() this = %p\n",this);
#endif
	if ((!glMaterialComputed) || (glDisplayList == 0))
	{
		b3UpdateMaterial();
		glDisplayList = glGenLists(B3_DISPLAY_LIST_COUNT);

		glNewList(glDisplayList,GL_COMPILE);
		b3SelectMaterialForFilledDrawing(context);
		glEndList();
	}

#ifdef _DEBUG
	b3CheckGeometry(context,render_mode);
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
#else
	if ((!glVerticesComputed) || (!glIndicesComputed) || (!glMaterialComputed))
	{
		b3Update();
		b3UpdateMaterial();
	}

#ifdef _DEBUG
	b3CheckGeometry(context,render_mode);
#endif

	b3DrawGeometry(context,render_mode);
#endif
#endif
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"##### <b3RenderObject::b3Draw()\n");
#endif
}

void b3RenderObject::b3CheckGeometry(
	b3RenderContext *context,
	b3_render_mode   render_mode)
{
#ifdef _DEBUG
	b3MapVertices(B3_MAP_VBO_R);
	b3MapIndices(B3_MAP_VBO_R);

	b3_gl_vertex  *glVertex   = *glVertexElements;
	b3_gl_line    *glGrids    = *glGridElements;
	b3_gl_polygon *glPolygons = *glPolygonElements;
	b3_index       i;

#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"       %5d vertices: %p - %s\n",
		 glVertexElements->b3GetCount(), glVertex,
		 glVertexElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL,"       %5d grids:    %p - %s\n",
		 glGridElements->b3GetCount(), glGrids,
		 glGridElements->b3IsCustom() ? "custom" : "buffer");
	b3PrintF(B3LOG_FULL,"       %5d polygons: %p - %s\n",
		 glPolygonElements->b3GetCount(), glPolygons,
		 glPolygonElements->b3IsCustom() ? "custom" : "buffer");
#endif

	if (glVertex != null)
	{
		switch (render_mode)
		{
		case B3_RENDER_LINE:
			// This loop collects access vialoations
			// prior calling OpenGL routines. This
			// makes it possible to catch to faulty
			// index data. The access simply compute
			// the length of the lines to be drawn.
			for (i = 0;i < glGridElements->b3GetCount();i++)
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
			for (i = 0;i < glPolygonElements->b3GetCount();i++)
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

		case B3_RENDER_NOTHING:
			// Nothing to do...
			break;

		default:
			b3PrintF(B3LOG_NORMAL,"Illegal render mode selected.\n");
			break;
		}
	}
	b3UnmapVertices();
	b3UnmapIndices();
#endif
}

void b3RenderObject::b3DrawGeometry(
	b3RenderContext *context,
	b3_render_mode   render_mode)
{    
#ifdef BLZ3_USE_OPENGL
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
#endif
}

void b3RenderObject::b3SelectMaterialForLineDrawing(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	b3Color        diffuse;

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
#endif
}

void b3RenderObject::b3SelectMaterialForFilledDrawing(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
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
#endif
}

void b3RenderObject::b3DrawLinedGeometry(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
#ifndef _DEBUG
	glVertexElements->b3Draw();
	glGridElements->b3Draw();
#else
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"       b3Draw lined:  %d vertices, %d lines\n",
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
#endif
}

void b3RenderObject::b3DrawFilledGeometry(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
#ifndef _DEBUG
	glVertexElements->b3Draw();
	glPolygonElements->b3Draw();
#else
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"       b3Draw filled: %d vertices, %d polygons\n",
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
#endif
}
