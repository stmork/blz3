/*
**
**      $Filename:      b3RenderShape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing shapes
**
**      (C) Copyright 2001  Steffen A. Mork
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Aux.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.56  2002/08/16 14:00:02  sm
**      - Lines III adjustments to new vertex computation.
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
**      Revision 1.53  2002/08/09 13:20:19  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.52  2002/08/05 16:04:55  sm
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
**      Revision 1.51  2002/08/02 18:55:44  sm
**      - SplineShape weren't be multi threadable - fixed.
**
**      Revision 1.50  2002/07/31 13:46:02  sm
**      - I have to spend more brain into correct shading of quadrics...
**
**      Revision 1.49  2002/07/31 08:53:22  sm
**      - Added simplified pixel format selection
**      - Some problems with normal computation occured
**
**      Revision 1.48  2002/07/31 07:30:44  sm
**      - New normal computation. Textures are rendered correctly and
**        quadrics are shaded correctly. Spheres and doughnuts have
**        got their own more simple computation.
**
**      Revision 1.47  2002/07/29 14:48:11  sm
**      - Circled shapes like cylinder, doughnuts etc. draw
**        textures correctly but renders shading a little bit
**        wrong at seam.
**      - Added support for multiple lights. This should be
**        configurable inside a scene (via b3ModellerInfo?)
**
**      Revision 1.46  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.45  2002/07/26 22:08:09  sm
**      - Some b3RenderObject derived classed didn't initialize
**        glTexCoord. It's time to use glInterleavedArrays() to
**        avoid such hazards.
**      - The nVidia driver seems to use only 16 bit for depth
**        buffer:-(
**
**      Revision 1.44  2002/07/26 09:13:33  sm
**      - Found alpha problem: the Linux OpenGL renderer didn't use the
**        b3RenderContext::b3Init() method! Now everything function very well:-)
**      - The Un*x OpenGL renderer has got a key press interface now.
**      - Corrected spot lights
**      - Textures needn't to be square any more (some less memory usage)
**
**      Revision 1.43  2002/07/25 16:29:35  sm
**      - Further developing of texturing
**
**      Revision 1.42  2002/07/25 13:22:32  sm
**      - Introducing spot light
**      - Optimized light settings when drawing
**      - Further try of stencil maps
**
**      Revision 1.41  2002/07/23 07:04:05  sm
**      - Added torus support
**      - Precompute surface colors. So we don't need to collect
**        colors during mesh draw.
**      - Added simple texturing with appropriate scaling.
**
**      Revision 1.40  2002/07/22 18:47:01  sm
**      - Marion nerves.
**
**      Revision 1.39  2002/07/22 18:46:22  sm
**      - Missing to commit...
**
**      Revision 1.37  2002/07/22 16:27:45  sm
**      - Fixed some errors concerning texture stencil
**
**      Revision 1.36  2002/07/22 12:46:08  sm
**      - Added Windows Lines III support for textures
**      - Fixed sphere computation
**
**      Revision 1.35  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.34  2002/07/21 21:09:37  sm
**      - Now having texture mapping! Texture mapping is only applied to
**        areas and cylinders.
**
**      Revision 1.33  2002/07/21 17:02:36  sm
**      - Finished advanced color mix support (correct Phong/Mork shading)
**      - Added first texture mapping support. Further development on
**        Windows now...
**
**      Revision 1.32  2002/03/10 21:14:40  sm
**      - Fixed rotation shapes with custom subdivision for rotation.
**
**      Revision 1.31  2002/03/10 20:34:18  sm
**      - Cleaned up and tested CB3ShapeDialgo derivates:
**        o Ordered meaning of methods
**        o Made registry entries of stencil creation unique for
**          each shape.
**        o Fixed some bugs.
**
**      Revision 1.30  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.29  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.28  2002/02/27 20:14:52  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.27  2002/02/20 20:23:57  sm
**      - Some type cleanups done.
**
**      Revision 1.26  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.25  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.24  2002/02/17 21:25:07  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.23  2002/01/10 17:31:11  sm
**      - Some minor GUI updates.
**      - b3BBox::b3Transform() changes m_Matrix member.
**      - Added image selection with image preview.
**
**      Revision 1.22  2002/01/06 21:38:18  sm
**      - Nasty Un CR/LF
**      - b3Dir fix. Not tested, yet!
**      - make count/make count_all update
**
**      Revision 1.21  2002/01/06 16:30:47  sm
**      - Added Load/Save/Replace object
**      - Enhanced "New world"
**      - Added some non static methods to b3Dir (Un*x untested, yet!)
**      - Fixed missing sphere/ellipsoid south pole triangles
**      - Fixed Spline cylinder/ring triangle indexing
**
**      Revision 1.20  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.19  2001/10/20 16:14:59  sm
**      - Some runtime environment cleanups. The CPU count is determined
**        only once.
**      - Introduced preparing routines for raytring to shapes.
**      - Found 5% performance loss: No problem, this was eaten by
**        bug fxing of the rotation spline shapes. (Phuu!)
**      - The next job is to implement different row sampler. Then we
**        should implemented the base set of the Blizzard II raytracer.
**
**      Revision 1.18  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.17  2001/10/10 17:52:24  sm
**      - Texture loading (only reading into memory) running.
**      - Raytracing without OpenGL must be possible!
**
**      Revision 1.16  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.15  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.14  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
**      Revision 1.13  2001/08/20 19:35:08  sm
**      - Index correction introduced (This is a hack!)
**      - Some OpenGL cleanups
**
**      Revision 1.12  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.11  2001/08/17 19:28:54  sm
**      - Now able to draw every shape by lines or shaded.
**        It's great!
**
**      Revision 1.10  2001/08/17 14:08:34  sm
**      - Now trying to draw BSPline surfaces with own routines.
**
**      Revision 1.9  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.8  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.7  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
**
**      Revision 1.6  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.5  2001/08/14 13:34:40  sm
**      - Corredted aspect ratio handling when doing somethiing with
**        the view
**      - New application icon
**      - Minor GUI bug fixes.
**
**      Revision 1.4  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.3  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.2  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.1  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.10  2001/08/09 15:27:34  sm
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
**      Revision 1.9  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.8  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.7  2001/08/06 20:49:39  sm
**      - Some conflicts solved
**
**      Revision 1.6  2001/08/06 19:58:59  sm
**      - Drawing area - the first shape we can see with OpenGL
**
**      Revision 1.5  2001/08/06 16:35:35  sm
**      - Drawing first area
**
**      Revision 1.4  2001/08/06 15:26:00  sm
**      - Splitted shapes into their own files
**      - Some preparations for shapde drawing.
**
**      Revision 1.3  2001/07/08 12:30:06  sm
**      - New tool to remove nasty CR/LF from Windoze.
**      - Removing some nasty CR/LF with that new tool.
**
**      Revision 1.2  2001/07/01 16:31:52  sm
**      - Creating MSVC Projects
**      - Welcome to Windows 32
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static b3_gl_line box_grids[] =
{
	{ 0,1 },
	{ 1,2 },
	{ 2,3 },
	{ 3,0 },
	{ 7,6 },
	{ 6,5 },
	{ 5,4 },
	{ 4,7 },
	{ 0,7 },
	{ 1,6 },
	{ 2,5 },
	{ 3,4 }
};

static b3_gl_polygon box_polygons[] =
{
	{  6, 7, 5 }, // top
	{  4, 5, 7 },
	{  0, 1, 3 }, // bottom
	{  2, 3, 1 },
	{ 13,12,10 }, // back
	{ 11,10,12 },
	{  9, 8,14 }, // front
	{ 15,14, 8 },
	{ 16,19,23 }, // right
	{ 20,23,19 },
	{ 18,17,21 }, // left
	{ 22,21,17 }
};

static b3_f32 box_texcoord[] =
{
	0,0,  1,0,  1,1,  0,1,  0,1, 1,1,  1,0,  0,0,
	0,0,  1,0,  1,1,  0,1,  0,1, 1,1,  1,0,  0,0,
	0,0,  1,0,  1,1,  0,1,  0,1, 1,1,  1,0,  0,0
};

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3ShapeRenderContext::b3ShapeRenderContext(b3_count new_subdiv)
{
	m_CylinderIndices  = null;
	m_CylinderPolygons = null;
	m_ConeIndices      = null;
	m_ConePolygons     = null;
	m_Between          = null;
	b3InitSubdiv(new_subdiv);
}

void b3ShapeRenderContext::b3InitSubdiv(b3_count new_subdiv)
{
	b3_gl_line    *gPtr;
	b3_gl_polygon *pPtr;
	b3_index       a,i;
	b3_f64         aux;

	m_SubDiv = (new_subdiv > B3_MAX_RENDER_SUBDIV ? B3_MAX_RENDER_SUBDIV : new_subdiv);
	if (m_SubDiv < 8)
	{
		m_SubDiv = 8;
	}

	for (i = 0;i <= m_SubDiv;i++)
	{
		aux    = i * M_PI * 2 / m_SubDiv;
		m_Sin[i] = sin(aux);
		m_Cos[i] = cos(aux);
	}

	b3Free(m_CylinderIndices);
	b3Free(m_CylinderPolygons);
	b3Free(m_ConeIndices);
	b3Free(m_ConePolygons);

	m_CylinderIndices  = (b3_gl_line *)b3Alloc
			((m_SubDiv + 1) * 3 * sizeof(b3_gl_line));
	m_CylinderPolygons = (b3_gl_polygon *)b3Alloc
			((m_SubDiv + 1) * 2 * sizeof(b3_gl_polygon));
	if ((m_CylinderIndices != null) && (m_CylinderPolygons != null))
	{
		gPtr = m_CylinderIndices;
		pPtr = m_CylinderPolygons;
		a = 0;
		for (i = 0;i <= m_SubDiv;i++)
		{
			// Concatinate vertices in this ascending order:
			//
			//   (c)
			// 1-----3
			// |
			// |(a)
			// |
			// 0-----2
			//   (b)

			// (a)
			B3_GL_LINIT(gPtr,a,  a+1);  // (a)
			B3_GL_LINIT(gPtr,a,  a+2);  // (b)
			B3_GL_LINIT(gPtr,a+1,a+3);  // (c)

			B3_GL_PINIT(pPtr,a,  a+2,a+1);
			B3_GL_PINIT(pPtr,a+3,a+1,a+2);

			a += 2;
		}
	}
	else
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}

	m_ConeIndices  = (b3_gl_line *)b3Alloc
		((m_SubDiv + 1) * 2 * sizeof(b3_gl_line));
	m_ConePolygons = (b3_gl_polygon *)b3Alloc
		((m_SubDiv + 1) * 1 * sizeof(b3_gl_polygon));
	if ((m_ConeIndices != null) && (m_ConePolygons != null))
	{
		gPtr = m_ConeIndices;
		pPtr = m_ConePolygons;
		for (i = 0;i <= m_SubDiv;i++)
		{
			B3_GL_LINIT(gPtr,0,  i+1);
			B3_GL_LINIT(gPtr,i+1,i+2);

			B3_GL_PINIT(pPtr,i+1,i+2,0);
		}
	}
	else
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}
}

b3_count b3ShapeRenderContext::b3GetSubdiv()
{
	return m_SubDiv;
}

b3_f64 *b3ShapeRenderContext::b3GetSinTable()
{
	return m_Sin;
}

b3_f64 *b3ShapeRenderContext::b3GetCosTable()
{
	return m_Cos;
}

b3_gl_line *b3ShapeRenderContext::b3GetCylinderIndices()
{
	return m_CylinderIndices;
}

b3_gl_polygon *b3ShapeRenderContext::b3GetCylinderPolygons()
{
	return m_CylinderPolygons;
}

b3_gl_line *b3ShapeRenderContext::b3GetConeIndices()
{
	return m_ConeIndices;
}

b3_gl_polygon *b3ShapeRenderContext::b3GetConePolygons()
{
	return m_ConePolygons;
}

/*************************************************************************
**                                                                      **
**                        b3ShapeRenderObject Implementation            **
**                                                                      **
*************************************************************************/

b3ShapeRenderObject::b3ShapeRenderObject(b3_size class_size,b3_u32 class_type) :
	b3Shape(class_size, class_type)
{
}

b3ShapeRenderObject::b3ShapeRenderObject(b3_u32 class_type) :
	b3Shape(sizeof(b3ShapeRenderObject), class_type)
{
}

b3ShapeRenderObject::b3ShapeRenderObject(b3_u32 *src) :
	b3Shape(src)
{
}

void b3ShapeRenderObject::b3ComputeBound(b3_stencil_limit *limit)
{
	b3Item           *item;
	b3Condition      *cond;
	b3_stencil_bound  info;

	// Get outer limits
	b3GetStencilBoundInfo(&info);
	limit->x1 = info.xMin;
	limit->y1 = info.yMin;
	limit->x2 = info.xMax;
	limit->y2 = info.yMax;

	// Do any stencil make the limits closer?
	B3_FOR_BASE(b3GetConditionHead(),item)
	{
		cond = (b3Condition *)item;
		cond->b3ComputeBound(limit);
	}
}

/*************************************************************************
**                                                                      **
**                        Retrieving material properties                **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3GetDiffuseColor(b3_color *color)
{
	b3Item     *item;
	b3Material *material;
	b3_color    ambient,specular;
	b3_polar    polar;

	color->r = 0.1f;
	color->g = 0.5f;
	color->b = 1.0f;
	color->a = 0.0f;

	B3_FOR_BASE(b3GetMaterialHead(),item)
	{
		material = (b3Material *)item;
		if (material->b3GetColors(&polar,color,&ambient,&specular))
		{
			return;
		}
	}
}

b3_f64 b3ShapeRenderObject::b3GetColors(
	b3_color *ambient,
	b3_color *diffuse,
	b3_color *specular)
{
	b3Item     *item;
	b3Material *material;
	b3_polar    polar;

	B3_FOR_BASE(b3GetMaterialHead(),item)
	{
		material = (b3Material *)item;
		if (material->b3GetColors(&polar,diffuse,ambient,specular))
		{
			return material->b3GetSpecularExponent(&polar);
		}
	}

	return b3RenderObject::b3GetColors(ambient,diffuse,specular);
}

void b3ShapeRenderObject::b3GetGridColor(b3_color *color)
{
	b3Color::b3Init(color, 0.2f, 0.2f, 0.2f);
}

b3_bool b3ShapeRenderObject::b3GetChess(
	b3_color *black,
	b3_color *white,
	b3_res   &xRepeat,
	b3_res   &yRepeat)
{
	b3Item  *item;
	b3_bool  result = false;

	item   = b3GetMaterialHead()->First;
	if (item != null)
	{
		result = item->b3GetClassType() == CHESS;
		if (result)
		{
			b3MatChess *chess = (b3MatChess *)item;
			*black  = chess->m_DiffColor[0];
			*white  = chess->m_DiffColor[1];
			xRepeat = chess->m_xTimes;
			yRepeat = chess->m_yTimes;
		}
	}
	return result;
}

b3Tx *b3ShapeRenderObject::b3GetTexture(
	b3_f64 &xTrans,
	b3_f64 &yTrans,
	b3_f64 &xScale,
	b3_f64 &yScale)
{
	b3Item *item;
	b3Tx   *tx = null;

	item = b3GetMaterialHead()->First;
	if ((item != null) && (item->b3GetClassType() == TEXTURE) && (item->Succ == null))
	{
		b3MatTexture     *mat = (b3MatTexture *)item;
		b3_stencil_limit  limit;
		b3_f64            xLocSize;
		b3_f64            yLocSize;

		B3_ASSERT(mat->m_Texture != null);
		b3ComputeBound(&limit);
		xLocSize = limit.x2 - limit.x1;
		yLocSize = limit.y2 - limit.y1;

		tx     = mat->m_Texture;
		xTrans = (mat->m_xStart - limit.x1) / xLocSize;
		yTrans = (mat->m_yStart - limit.y1) / yLocSize;
		xScale =  mat->m_xScale / xLocSize;
		yScale =  mat->m_yScale / yLocSize;

		b3PrintF(B3LOG_FULL,"b3ShapeRenderObject::b3GetTexture(%2.2f,%2.2f,%2.2f,%2.2f) = %s\n",
			xTrans,yTrans,xScale,yScale,tx->b3Name());
	}
	return tx;
}

b3_bool b3ShapeRenderObject::b3GetImage(b3Tx *image)
{
	b3Item       *item;
	b3_bool       result = false;
	b3_u32        type;

	for( item  = b3GetMaterialHead()->First;
	    (item != null) && (!result);
	     item  = item->Succ)
	{
		type   = item->b3GetClassType();
		result = ((type != MAT_NORMAL) && (type != CHESS));
	}

	if (result)
	{
		b3Material       *material;
		b3_stencil_limit  limit;
		b3_polar          polar;
		b3_pkd_color     *lPtr = (b3_pkd_color *)image->b3GetData();
		b3_pkd_color      color;
		b3_color          diffuse;
		b3_color          ambient;
		b3_color          specular;
		b3_coord          x,y;
		b3_f64            fx,fxStep;
		b3_f64            fy,fyStep;
		b3_bool           loop;

		b3ComputeBound(&limit);
		fxStep = (limit.x2 - limit.x1 - 2 * epsilon) / image->xSize;
		fyStep = (limit.y2 - limit.y1 - 2 * epsilon) / image->ySize;

		fy = limit.y1 + epsilon;
		for (y = 0;y < image->ySize;y++)
		{
			fx = limit.x1 + epsilon;
			for (x = 0;x < image->xSize;x++)
			{
				b3Vector::b3Init(&polar.box_polar,   fx,fy);
				b3Vector::b3Init(&polar.object_polar,fx,fy);
				b3Vector::b3Init(&polar.polar,       fx,fy);

				color = B3_BLACK;
				for(material  = (b3Material *)b3GetMaterialHead()->First,loop = true;
				   (material != null) && loop;
				    material  = (b3Material *)material->Succ)
				{
					if (material->b3GetColors(&polar,&diffuse,&ambient,&specular))
					{
						diffuse.a = b3CheckStencil(&polar) ? 0 : 1;
						color     = b3Color::b3GetColor(&diffuse);
						loop      = false;
					}
				}
				*lPtr++ = color;
				fx += fxStep;
			}
			fy += fyStep;
		}
		b3PrintF(B3LOG_FULL,"b3ShapeRenderObject::b3GetImage()\n");
	}
	return result;
}

b3_render_mode b3ShapeRenderObject::b3GetRenderMode()
{
	return b3IsActive() ? B3_RENDER_FILLED : B3_RENDER_LINE;
}

/*************************************************************************
**                                                                      **
**                        Searching for equal indices                   **
**                                                                      **
*************************************************************************/

b3_count b3ShapeRenderObject::b3GetIndexOverhead (
	b3_f64 xLeft,
	b3_f64 yLeft)
{
	b3_count Overhead;
	b3_index xs,xe;
	b3_f64   x1,x2;

	if  (Limit.x1 < xLeft) Limit.x1 = xLeft;
	if  (Limit.y1 < yLeft) Limit.y1 = yLeft;
	x1 = Limit.x1 * SinCosSteps;
	x2 = Limit.x2 * SinCosSteps;
	xs = (b3_index)ceil(x1);
	xe = (b3_index)floor(x2);
	Overhead = xe - xs;
	if ((xs - x1) > epsilon) Overhead++;
	if ((x2 - xe) > epsilon) Overhead++;

	return ((xs > 0) || (xe < SinCosSteps)) ? -Overhead : Overhead;
}

#define no_USE_FIND_VERTEX

b3_index b3ShapeRenderObject::b3FindVertex(b3_index index)
{
#ifdef USE_FIND_VERTEX
	b3_vector *point = &glVertex[index].v;

	for (b3_index i = 0;i < glVertexCount;i++)
	{
		if (b3Vector::b3Distance(point,&glVertex[i].v) < epsilon)
		{
			return i;
		}
	}

	B3_ASSERT(false);
#endif
	return index;
}

void b3ShapeRenderObject::b3ComputeQuadricNormals(b3_bool normalize)
{
#ifdef USE_FIND_VERTEX
	b3_vector normal;
	b3_vector xDir,yDir;
	b3_index  i,k,v1,v2,v3,n1,n2,n3,start,end;

	// Clear normals
	b3GetVertexRange(start,end);
	for (i = start;i < end;i++)
	{
		b3Vector::b3Init(&glVertex[i].n);
	}

	for (i = k = 0;i < glPolyCount;i++)
	{
		n1 = b3FindVertex(v1 = glPolygons[k++]);
		n2 = b3FindVertex(v2 = glPolygons[k++]);
		n3 = b3FindVertex(v3 = glPolygons[k++]);

		b3Vector::b3Sub(&glVertex[n2].v,&glVertex[n1].v,&xDir);
		b3Vector::b3Sub(&glVertex[n3].v,&glVertex[n1].v,&yDir);
		b3Vector::b3CrossProduct(&xDir,&yDir,&normal);
		if (b3Vector::b3Normalize(&normal) > epsilon)
		{
			b3Vector::b3Add(&normal,&glVertex[v1].n);
			b3Vector::b3Add(&normal,&glVertex[v2].n);
			b3Vector::b3Add(&normal,&glVertex[v3].n);

			if (n1 != v1) b3Vector::b3Add(&normal,&glVertex[n1].n);
			if (n2 != v2) b3Vector::b3Add(&normal,&glVertex[n2].n);
			if (n3 != v3) b3Vector::b3Add(&normal,&glVertex[n3].n);
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
#else
	b3RenderObject::b3ComputeNormals(normalize);
#endif
}

/*************************************************************************
**                                                                      **
**                        Sphere computation                            **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeSphereVertices(
	b3_vector   &Base,
	b3_vector   &Dir)
{
	b3_gl_vertex *Vector;
	b3_index      i,j;
	b3_count      Circles;
	b3_f64        cx,sx,cy,sy,a;
	b3_f64        LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32        Rad;
	b3_vector     Aux,Dir1,Dir2,Dir3;

	Vector = glVertex;
	Aux    = Base;
	Dir1.x = Rad = b3Vector::b3Length (&Dir);
	Dir1.y = 0;
	Dir1.z = 0;
	Dir2.x = 0;
	Dir2.y = Rad;
	Dir2.z = 0;
	Dir3.x = 0;
	Dir3.y = 0;
	Dir3.z = Rad;
	Circles = (SinCosSteps >> 1) + 1;
	xSize	 = SinCosSteps + 1;
	ySize  = Circles;

	a = 2.0/SinCosSteps;
	for (j =0 ;j < Circles;j++)
	{
		LocalCos[j] = Rad = -cos(j * a * M_PI);
		LocalSin[j] = sqrt(1.0-Rad*Rad);
	}

	for (i = 0;i <= SinCosSteps;i++)
	{
		cx = Cos[i];
		sx = Sin[i];
		for (j=0;j<Circles;j++)
		{
			cy = LocalCos[j];
			sy = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + cx * sy * Dir1.x + sx * sy * Dir2.x + cy * Dir3.x;
			Vector->v.y = Base.y + cx * sy * Dir1.y + sx * sy * Dir2.y + cy * Dir3.y;
			Vector->v.z = Base.z + cx * sy * Dir1.z + sx * sy * Dir2.z + cy * Dir3.z;
			Vector++;
		}
	}
 }

void b3ShapeRenderObject::b3ComputeSphereNormals(b3_vector &base,b3_bool normalize)
{
	b3_index i;

	for (i = 0;i < glVertexCount;i++)
	{
		b3Vector::b3Sub(&base,&glVertex[i].v,&glVertex[i].n);
		if (normalize)
		{
			b3Vector::b3Normalize(&glVertex[i].n);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Cylinder computation                          **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeCylinderVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
	b3_gl_vertex *Vector = glVertex;
	b3_f64        sx,sy,b,h,start,end;
	b3_index      i;
	b3_count      iMax;
	b3_vector     Bottom;

	h        = Limit.y2 - Limit.y1;
	b        = Limit.y1;
	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;
	start    = Limit.x1 * SinCosSteps;
	end      = Limit.x2 * SinCosSteps;
	i        = (b3_index)ceil(start);
	iMax     = (b3_count)floor(end);
	xSize    = 0;
	ySize    = 2;
	glVertexCount = 0;

	if ((i - start) > epsilon)
	{
		b  = Limit.x1 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->t.s = 0;
		Vector->t.t = 0;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->t.s = 0;
		Vector->t.t = 1;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		b3_f64 s = ((double)i / SinCosSteps) / (Limit.x2 - Limit.x1) - Limit.x1;

		sx = Cos[i % SinCosSteps];
		sy = Sin[i % SinCosSteps];

		Vector->t.s = s;
		Vector->t.t = 0;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->t.s = s;
		Vector->t.t = 1;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		xSize++;
	}

	if ((end - iMax) > epsilon)
	{
		b = Limit.x2 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->t.s = 1;
		Vector->t.t = 0;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->t.s = 1;
		Vector->t.t = 1;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		xSize++;
	}
}

void b3ShapeRenderObject::b3ComputeCylinderIndices()
{
	b3_count Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		glGridCount = 1;
		Overhead = -Overhead;
	}
	else
	{
		glGridCount = 0;
	}
	glGridCount += Overhead * 3;
	glPolyCount  = Overhead * 2;
}

/*************************************************************************
**                                                                      **
**                        Cone computation                              **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeConeVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
	b3_gl_vertex *Vector = glVertex;
	b3_f64        sx,sy,b,h,d,a,start,end;
	b3_index      i;
	b3_count      iMax;
	b3_vector     Bottom;

	d        = Limit.y2 - Limit.y1;
	b        = Limit.y1;
	h        = Limit.y2;
	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize  = 0;
	ySize  = 1;
	glVertexCount = 0;

	if (Limit.y2 < 1)
	{
		ySize++;
		if ((i - start) > epsilon)
		{
			a = Limit.x1 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);
			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * cos(a);
			sy = (1-h) * sin(a);
			Vector->t.s = 0;
			Vector->t.t = 1;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		for (;i <= iMax;i++)
		{
			b3_f64 s = ((double)i / SinCosSteps) / (Limit.x2 - Limit.x1) - Limit.x1;

			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->t.s = s;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * Cos[i % SinCosSteps];
			sy = (1-h) * Sin[i % SinCosSteps];
			Vector->t.s = s;
			Vector->t.t = 1;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;
			Vector++;

			glVertexCount += 2;
			xSize++;
		}

		if ((end - iMax) > epsilon)
		{
			a  = Limit.x2 * M_PI * 2;

			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);
			Vector->t.s = 1;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * cos(a);
			sy = (1-h) * sin(a);
			Vector->t.s = 1;
			Vector->t.t = 1;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;

			glVertexCount += 2;
			xSize++;
		}
	}
	else
	{
		Vector->t.s = 0.5;
		Vector->t.t = 1;
		Vector->v.x = Base.x + Dir3.x;
		Vector->v.y = Base.y + Dir3.y;
		Vector->v.z = Base.z + Dir3.z;
		Vector++;
		glVertexCount++;

		if ((i - start) > epsilon)
		{
			a  = Limit.x1 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			glVertexCount++;
			xSize++;
		}

		for (;i <= iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];

			Vector->t.s = ((double)i / SinCosSteps) / (Limit.x2 - Limit.x1) - Limit.x1;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			glVertexCount++;
			xSize++;
		}

		if ((end - iMax) > epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->t.s = 1;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;

			glVertexCount++;
			xSize++;
		}
	}
}

void b3ShapeRenderObject::b3ComputeConeIndices()
{
	b3_count Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		glGridCount = 1;
		Overhead  = -Overhead;
	}
	else
	{
		glGridCount = 0;
	}
	if (Limit.y2 < 1)
	{
		glGrids      = GridsCyl;
		glPolygons   = PolysCyl;
		glGridCount += Overhead * 3;
		glPolyCount  = Overhead * 2;
	}
	else
	{
		glGrids      = GridsCone;
		glPolygons   = PolysCone;
		glGridCount += Overhead * 2;
		glPolyCount  = Overhead;
	}
}

/*************************************************************************
**                                                                      **
**                        Ellipsoid computation                         **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeEllipsoidVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
	b3_gl_vertex *Vector = glVertex;
	b3_index      i,j;
	b3_count      iMax,Circles = 0;
	b3_f64        RadX,RadY,sx,sy;
	b3_f64        LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32        start,end,a;

	start  = (Limit.y1 + 1) * SinCosSteps * 0.25;
	end    = (Limit.y2 + 1) * SinCosSteps * 0.25;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > epsilon)	/* underflow */
	{
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 4.0 / SinCosSteps;
	for (j=i;j<=iMax;j++)		/* integers */
	{
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > epsilon)	/* Overflow */
	{
		LocalSin[Circles] = Limit.y2;
		Circles++;
	}

	for (j=0;j<Circles;j++)		/* Calculate Values */
	{
		LocalSin[j] = a = sin(LocalSin[j] * M_PI * 0.5);
		LocalCos[j] = sqrt(1.0-a*a);
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = Circles;
	glVertexCount = 0;

	if ((i - start) > epsilon)
	{
		a  = Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx = Cos[i];
		sy = Sin[i];

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	if ((end - iMax) > epsilon)
	{
		a  = Limit.x2 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}
}

void b3ShapeRenderObject::b3ComputeEllipsoidIndices()
{
	b3_gl_line    *gPtr;
	b3_gl_polygon *pPtr;
	b3_bool        EndLine = false;
	b3_index       i,j,Number,s,ys,ye;
	b3_count       Heights,Widths,Overhead;
	b3_f64         y1,y2;

	glGridCount = 0;
	glPolyCount = 0;
	b3ComputeBound(&Limit);
	Overhead  = b3GetIndexOverhead (0.0,-1.0);
	if (Overhead < 0)
	{
		EndLine  = true;
		Overhead = -Overhead;
	}

	y1 = (Limit.y1 + 1) * SinCosSteps * 0.25;
	y2 = (Limit.y2 + 1) * SinCosSteps * 0.25;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	Heights = ye - ys;
	if ((ys - y1) > epsilon) Heights++;
	if ((y2 - ye) > epsilon) Heights++;

	Widths = Heights - 1;
	if ((SinCosSteps * 0.5 - y2) > epsilon) Widths++;
	if (                     y1  > epsilon) Widths++;

	if (EndLine) Number = (Widths + Heights + 1) * Overhead + Heights;
	else         Number = (Widths + Heights + 1) * Overhead;

	// Realloc buffers
	b3RenderObject::b3Free(glGrids);
	b3RenderObject::b3Free(glPolygons);
	glGrids    = gPtr = (b3_gl_line *)b3RenderObject::b3Alloc
		(Number * sizeof(b3_gl_line));
	glPolygons = pPtr = (b3_gl_polygon *)b3RenderObject::b3Alloc
		(Number * sizeof(b3_gl_polygon));
	if ((gPtr == null) || (pPtr == null))
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			// This marks a longitude
			B3_GL_LINIT(gPtr,s+j,s+j + 1);
		}
		glGridCount += Heights;

		if (y1 <= epsilon)
		{
			// NOTE: j = 0 substitution
			B3_GL_PINIT(pPtr,s + Heights + 2,s + Heights + 1,s + 1);
			glPolyCount++;
			j = 1;
		}
		else
		{
			j = 0;
		}
		while(j < Heights)
		{
			B3_GL_LINIT(gPtr,s+j,s+j + Heights + 1);
			glGridCount++;

			B3_GL_PINIT(pPtr,s+j,              s+j + 1,          s+j + Heights + 1);
			B3_GL_PINIT(pPtr,s+j + Heights + 2,s+j + Heights + 1,s+j + 1);
			glPolyCount += 2;
			j++;
		}

		if ((SinCosSteps * 0.5 - y2) > epsilon)
		{
			B3_GL_LINIT(gPtr,s+j,s+j + Heights + 1);
			glGridCount++;

			j++;
		}
		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			B3_GL_LINIT(gPtr,s+j,s+j + 1);
		}
		glGridCount += Heights;
	}

	B3_ASSERT(glGridCount <= Number);
}

/*************************************************************************
**                                                                      **
**                        Box computation                               **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeBoxVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
	b3_f32    *tex_coord = box_texcoord;
	b3_vector  Aux;
	b3_index   i;

	glVertex[0].t.s = *tex_coord++;
	glVertex[0].t.t = *tex_coord++;
	glVertex[0].v.x = Aux.x = Base.x;
	glVertex[0].v.y = Aux.y = Base.y;
	glVertex[0].v.z = Aux.z = Base.z;

	glVertex[1].t.s = *tex_coord++;
	glVertex[1].t.t = *tex_coord++;
	glVertex[1].v.x = (Aux.x += Dir1.x);
	glVertex[1].v.y = (Aux.y += Dir1.y);
	glVertex[1].v.z = (Aux.z += Dir1.z);

	glVertex[2].t.s = *tex_coord++;
	glVertex[2].t.t = *tex_coord++;
	glVertex[2].v.x = (Aux.x += Dir2.x);
	glVertex[2].v.y = (Aux.y += Dir2.y);
	glVertex[2].v.z = (Aux.z += Dir2.z);
	        
	glVertex[3].t.s = *tex_coord++;
	glVertex[3].t.t = *tex_coord++;
	glVertex[3].v.x = (Aux.x -= Dir1.x);
	glVertex[3].v.y = (Aux.y -= Dir1.y);
	glVertex[3].v.z = (Aux.z -= Dir1.z);

	glVertex[4].t.s = *tex_coord++;
	glVertex[4].t.t = *tex_coord++;
	glVertex[4].v.x = (Aux.x += Dir3.x);
	glVertex[4].v.y = (Aux.y += Dir3.y);
	glVertex[4].v.z = (Aux.z += Dir3.z);
	        
	glVertex[5].t.s = *tex_coord++;
	glVertex[5].t.t = *tex_coord++;
	glVertex[5].v.x = (Aux.x += Dir1.x);
	glVertex[5].v.y = (Aux.y += Dir1.y);
	glVertex[5].v.z = (Aux.z += Dir1.z);

	glVertex[6].t.s = *tex_coord++;
	glVertex[6].t.t = *tex_coord++;
	glVertex[6].v.x = (Aux.x -= Dir2.x);
	glVertex[6].v.y = (Aux.y -= Dir2.y);
	glVertex[6].v.z = (Aux.z -= Dir2.z);
	        
	glVertex[7].t.s = *tex_coord++;
	glVertex[7].t.t = *tex_coord++;
	glVertex[7].v.x = (Aux.x -= Dir1.x);
	glVertex[7].v.y = (Aux.y -= Dir1.y);
	glVertex[7].v.z = (Aux.z -= Dir1.z);

	xSize = ySize = 1;

	for (i = 0;i < 8;i++)
	{
		glVertex[i +  8] =
		glVertex[i + 16] = glVertex[i];
	}
}

void b3ShapeRenderObject::b3ComputeBoxIndices()
{
	glGrids    = box_grids;
	glPolygons = box_polygons;
}

/*************************************************************************
**                                                                      **
**                        Torus computation                             **
**                                                                      **
*************************************************************************/

void b3ShapeRenderObject::b3ComputeTorusVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3,
	b3_f64       aRad,
	b3_f64       bRad)
{
	b3_gl_vertex *Vector;
	b3_f64        start,end,a;
	b3_f64        sx,RadX,LocalSin[B3_MAX_RENDER_SUBDIV+1];
	b3_f64        sy,RadY,LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f64        relTex[B3_MAX_RENDER_SUBDIV+1];
	b3_index      i,j;
	b3_count      iMax,Circles=0;
	b3_vector     Aux;

	Vector = glVertex;
	start  = Limit.y1 * SinCosSteps;
	end    = Limit.y2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > epsilon)	/* underflow */
	{
		relTex[Circles]   = 0;
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 1.0 / SinCosSteps;
	for (j = i;j <= iMax;j++)		/* integers */
	{
		relTex[Circles]   = ((double)j / SinCosSteps) / (Limit.x2 - Limit.x1) - Limit.x1;
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > epsilon)	/* Overflow */
	{
		relTex[Circles]   = 1;
		LocalSin[Circles] = Limit.y2;
		Circles++;
	}
	xSize = 0;
	ySize = Circles;
	glVertexCount = 0;

	for (j = 0;j < Circles;j++)		/* Calculate Values */
	{
		LocalCos[j] = cos(LocalSin[j] * M_PI * 2) * bRad;
		LocalSin[j] = sin(LocalSin[j] * M_PI * 2) * bRad;
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);

	if ((i - start) > epsilon)
	{
		a     = Limit.x1 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = relTex[j];
			Vector->v.x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			b3Vector::b3Sub(&Aux,&Vector->v,&Vector->n);
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx    = Cos[i];
		sy    = Sin[i];
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = ((double)i / SinCosSteps) / (Limit.x2 - Limit.x1) - Limit.x1;
			Vector->t.t = relTex[j];
			Vector->v.x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			b3Vector::b3Sub(&Aux,&Vector->v,&Vector->n);
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}

	if ((end - iMax) > epsilon)
	{
		a     = Limit.x2 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0;j < Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 1;
			Vector->t.t = relTex[j];
			Vector->v.x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			b3Vector::b3Sub(&Aux,&Vector->v,&Vector->n);
			Vector++;
		}
		glVertexCount += Circles;
		xSize++;
	}
}

void b3ShapeRenderObject::b3ComputeTorusNormals()
{
	for (int i = 0;i < glVertexCount;i++)
	{
		b3Vector::b3Normalize(&glVertex[i].n);
	}
}

void b3ShapeRenderObject::b3ComputeTorusIndices()
{
	b3_gl_line    *gPtr;
	b3_gl_polygon *pPtr;
	b3_bool        EndLine = false,EndCol = false;
	b3_index       i,j,Number,s,ys,ye;
	b3_count       Heights,Widths,Overhead;
	b3_f64         y1,y2;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		EndLine  =  true;
		Overhead = -Overhead;
	}

	y1 = Limit.y1 * SinCosSteps;
	y2 = Limit.y2 * SinCosSteps;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	Heights = ye - ys;
	if ((ys - y1) > epsilon) Heights++;
	if ((y2 - ye) > epsilon) Heights++;
	if ((ys > 0) || (ye < SinCosSteps))
	{
		EndCol = true;
	}

	Widths = Heights;
	if (EndCol) Widths++;

	Number = (Widths + Heights + 1) * Overhead;
	if (EndLine) Number += Heights;

	glGridCount = 0;
	glPolyCount = 0;
	b3RenderObject::b3Free(glGrids);
	b3RenderObject::b3Free(glPolygons);
	glGrids    = gPtr = (b3_gl_line *)b3RenderObject::b3Alloc
		(Number * sizeof(b3_gl_line));
	glPolygons = pPtr = (b3_gl_polygon *)b3RenderObject::b3Alloc
		(Number * sizeof(b3_gl_polygon));
	if ((gPtr == null) || (pPtr == null))
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			B3_GL_LINIT(gPtr,s+j,s+j + 1);

			B3_GL_PINIT(pPtr,s+j,              s+j + 1,          s+j + Heights + 1);
			B3_GL_PINIT(pPtr,s+j + Heights + 2,s+j + Heights + 1,s+j + 1);
		}
		glGridCount += Heights;

		for (j = 0;j < Widths;j++)
		{
			B3_GL_LINIT(gPtr,s+j,s+j + Heights + 1);

			glPolyCount += 2;
		}
		glGridCount += Widths;

		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			B3_GL_LINIT(gPtr,s+j,s+j + 1);
		}
		glGridCount += Heights;
	}
}
