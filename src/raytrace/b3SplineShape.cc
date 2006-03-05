/*
**
**      $Filename:      b3SplineShape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing BSpline shapes
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.50  2006/03/05 21:22:36  sm
**      - Added precompiled support for faster comiling :-)
**
**      Revision 1.49  2005/11/27 11:48:11  sm
**      - Added some additional logging.
**
**      Revision 1.48  2005/11/27 11:36:04  sm
**      - Some more debugging output added.
**
**      Revision 1.47  2005/10/02 09:51:13  sm
**      - Added OpenEXR configuration.
**      - Added more excpetion handling.
**
**      Revision 1.46  2004/11/21 14:56:58  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.45.2.1  2004/11/19 19:38:43  sm
**      - OK. The arrays are drawing correctly and the ATi VBOs are drawing
**        something. The draw buffer seams to be defective. Now we should
**        look what nVIDIA is doing with my code.
**
**      Revision 1.45  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.44  2003/02/25 19:26:07  sm
**      - Fixed missing grid update on pick move
**
**      Revision 1.43  2003/02/25 15:56:21  sm
**      - Added SplineRot to control grid drawing.
**      - Added support for pixel format selection in dialog items
**      - Restructured b3PickInfo
**
**      Revision 1.42  2003/02/24 17:32:38  sm
**      - Added further picking support.
**      - Fixed geometry update delay.
**
**      Revision 1.41  2002/08/16 11:40:39  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.40  2002/08/15 13:56:44  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.39  2002/08/09 13:20:20  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.38  2002/08/07 12:38:43  sm
**      - Modified exception definition. Exceptions are identified with
**        a three character code to unify error codes. This is necessary
**        to convert error codes into error messages inside applications.
**      - Added some additional b3Hash methods.
**      - Added -Wall compiler option to all C++ files.
**      - Removed some compiler warnings.
**
**      Revision 1.37  2002/08/04 13:24:56  sm
**      - Found transformation bug: Normals have to be treated as
**        direction vectors, aren't them?
**      - b3PrepareInfo::m_PrepareProc initialized not only in
**        debug mode.
**
**      Revision 1.36  2002/08/03 18:05:11  sm
**      - Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**      - Moved b3PrepareInfo into b3Scene class as member. This
**        saves memory allocation calls and is an investment into
**        faster Lines III object transformation.
**
**      Revision 1.35  2002/08/02 18:55:45  sm
**      - SplineShape weren't be multi threadable - fixed.
**
**      Revision 1.34  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.33  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.32  2002/03/10 13:55:15  sm
**      - Added creation dialog for rotation shapes.
**      - Cleaned up derivation of b3SplineRotShape.
**      - Added support for foreign BLZ3_HOME directories.
**
**      Revision 1.31  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.30  2002/03/02 19:52:40  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.29  2002/03/02 16:47:50  sm
**      - Further cleanup of spline shapes and triangle shapes.
**      - Some cleanups of b3ControlLDC done: Now sending
**        messages to dialog via WM_NOTIFY.
**
**      Revision 1.28  2002/03/02 15:24:35  sm
**      - Templetized splines (uhff).
**      - Prepared spline shapes for their creation.
**        *** And now: Testing! Testing! Testing! ***
**
**      Revision 1.27  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.26  2002/01/21 16:56:46  sm
**      - Showing splash dialog only in release version.
**      - Prepared shape icons.
**
**      Revision 1.25  2002/01/20 12:48:51  sm
**      - Added splash screen
**      - Corrected repeat buttons (capture change)
**
**      Revision 1.24  2002/01/06 21:38:18  sm
**      - Nasty Un CR/LF
**      - b3Dir fix. Not tested, yet!
**      - make count/make count_all update
**
**      Revision 1.23  2002/01/06 16:30:47  sm
**      - Added Load/Save/Replace object
**      - Enhanced "New world"
**      - Added some non static methods to b3Dir (Un*x untested, yet!)
**      - Fixed missing sphere/ellipsoid south pole triangles
**      - Fixed Spline cylinder/ring triangle indexing
**
**      Revision 1.22  2002/01/03 15:50:14  sm
**      - Added cut/copy/paste
**
**      Revision 1.21  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.20  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.19  2001/12/30 16:54:15  sm
**      - Inserted safe b3Write() into Lines III
**      - Fixed b3World saving: b3StoreXXX() methods must ensure
**        buffer before the buffer itself is used.
**      - Extended b3Shape format with shape activation flag. Nice: The
**        new data structures don't confuse the old Lines II/Blizzard II and
**        even stores these new values.
**
**      Revision 1.18  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.17  2001/10/20 16:14:59  sm
**      - Some runtime environment cleanups. The CPU count is determined
**        only once.
**      - Introduced preparing routines for raytring to shapes.
**      - Found 5% performance loss: No problem, this was eaten by
**        bug fxing of the rotation spline shapes. (Phuu!)
**      - The next job is to implement different row sampler. Then we
**        should implemented the base set of the Blizzard II raytracer.
**
**      Revision 1.16  2001/10/18 14:48:26  sm
**      - Fixing refracting problem on some scenes with glasses.
**      - Fixing overlighting problem when using Mork shading.
**      - Finxing some memory leaks (espacially b3TriangleRefs)
**      - Adding texture support to conditions (stencil mapping).
**        Now conditions are ready to work compatible with
**        Blizzard II.
**
**      Revision 1.15  2001/10/17 21:09:06  sm
**      - Triangle support added for intersections, normal computations. So
**        Spline shapes can be computed, too. Now only CSG is missing.
**
**      Revision 1.14  2001/10/17 14:46:02  sm
**      - Adding triangle support.
**      - Renaming b3TriangleShape into b3Triangles and introducing
**        new b3TriangleShape as base class. This results in
**        source file renaming, too.
**      - Fixing soft shadow bug.
**      - Only scene loading background image when activated.
**      - Fixing LDC spline initialization.
**      - Converting Windows paths into right paths on Un*x
**
**      Revision 1.13  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.12  2001/10/10 17:52:24  sm
**      - Texture loading (only reading into memory) running.
**      - Raytracing without OpenGL must be possible!
**
**      Revision 1.11  2001/09/30 15:53:20  sm
**      - Removing nasty CR/LF
**
**      Revision 1.10  2001/09/04 20:37:53  sm
**      - Some minor updates
**
**      Revision 1.9  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.8  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.7  2001/08/17 19:28:54  sm
**      - Now able to draw every shape by lines or shaded.
**        It's great!
**
**      Revision 1.6  2001/08/17 14:08:34  sm
**      - Now trying to draw BSPline surfaces with own routines.
**
**      Revision 1.5  2001/08/17 04:16:43  sm
**      - Using OpenGL NURBS zu render BSpline areas. But
**        I think mi tessalation is faster.
**
**      Revision 1.4  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.3  2001/08/11 16:29:08  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.2  2001/08/11 15:59:59  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.1  2001/08/10 15:14:37  sm
**      - Now having all shapes implemented for drawing lines.
**
**
*/

/*************************************************************************
**                                                                      **
**                        BSpline shapes                                **
**                                                                      **
*************************************************************************/

b3SplineShape::b3SplineShape(b3_size class_size,b3_u32 class_type) :
	b3TriangleShape(class_size, class_type)
{
	m_Controls = null;
}

b3SplineShape::b3SplineShape(b3_u32 class_type) : b3TriangleShape(sizeof(b3SplineShape), class_type)
{
	m_Controls = null;
}

b3SplineShape::b3SplineShape(b3_u32 *src) : b3TriangleShape(src)
{				 
	b3_index i;
	b3_count control_count;

	b3InitVector();
	b3InitVector();
	b3InitSpline(&m_Spline[0],null,m_Knots[0]);
	b3InitSpline(&m_Spline[1],null,m_Knots[1]);

	// Copy knots
	for (i = 0;i < B3_MAX_KNOTS;i++) m_Knots[0][i] = b3InitFloat();
	for (i = 0;i < B3_MAX_KNOTS;i++) m_Knots[1][i] = b3InitFloat();

	// Copy control points
	control_count = m_Spline[0].control_max * m_Spline[1].control_max;
	m_Controls    = (b3_vector *)b3Item::b3Alloc(control_count * sizeof(b3_vector));
	m_Spline[0].controls =
	m_Spline[1].controls = m_Controls;
	for (i = 0;i < control_count;i++)
	{
		b3InitVector(&m_Controls[i]);
	}
	b3InitActivation();
}

void b3SplineShape::b3StoreShape()
{
	b3_index i;
	b3_count control_count;

	b3StoreVector();
	b3StoreVector();
	b3StoreSpline(&m_Spline[0]);
	b3StoreSpline(&m_Spline[1]);

	// Store knots
	for (i = 0;i < B3_MAX_KNOTS;i++)
	{
		b3StoreFloat(m_Knots[0][i]);
	}
	for (i = 0;i < B3_MAX_KNOTS;i++)
	{
		b3StoreFloat(m_Knots[1][i]);
	}

	// Store control points
	control_count = m_Spline[0].control_max * m_Spline[1].control_max;
	for (i = 0;i < control_count;i++)
	{
		b3StoreVector(&m_Controls[i]);
	}
}

void b3SplineShape::b3Init(
	b3_count hDegree,
	b3_count vDegree,
	b3_count hControlNum,
	b3_count vControlNum)
{
	// Allocate controls
	m_Controls      = (b3_vector *)b3Item::b3Alloc(
		m_Spline[0].control_max *
		m_Spline[1].control_max * sizeof(b3_vector));

	// Init horizontal spline
	m_Spline[0].knots    = m_Knots[0];
	m_Spline[0].controls = m_Controls;
	m_Spline[0].offset   = 1;
	m_Spline[0].b3InitCurve(hDegree,hControlNum,b3GetClassType() != SPLINES_AREA);

	// Init vertical spline
	m_Spline[1].knots    = m_Knots[1];
	m_Spline[1].controls = m_Controls;
	m_Spline[1].offset   = m_Spline[0].control_max;
	m_Spline[1].b3InitCurve(vDegree,vControlNum,b3GetClassType() == SPLINES_RING);
}

void b3SplineShape::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	// Compute number of grid vertices
	m_GridVertexCount = (B3_MAX_CONTROLS + B3_MAX_CONTROLS) * (B3_MAX_SUBDIV + 1);

	// Compute number of solid vertices. That
	// are usually much more.
	m_xSubDiv = m_Spline[0].subdiv;
	m_ySubDiv = m_Spline[1].subdiv;

	if (!m_Spline[0].closed) m_xSubDiv++;
	if (!m_Spline[1].closed) m_ySubDiv++;
	m_SolidVertexCount = (m_xSubDiv + 1) * (m_ySubDiv * 1);

	// Get enough memory
	vertCount = m_GridVertexCount + m_SolidVertexCount;
	gridCount =
		B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]) * m_Spline[0].subdiv +
		B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]) * m_Spline[1].subdiv;
	polyCount = m_Spline[0].subdiv * m_Spline[1].subdiv * 2;
}

void b3SplineShape::b3ComputeGridVertices()
{
	b3_gl_vertex *Vector = *glVertexElements;
	b3_vector     SplVector[B3_MAX_SUBDIV + 1];
	b3_vector     ControlArray[B3_MAX_CONTROLS * B3_MAX_CONTROLS];
	b3_count      CurveNum,Points = 0;
	b3_index      x,y,t;
	b3Spline      MySpline;

	m_Spline[0].controls =  m_Controls;
	m_Spline[1].controls =  m_Controls;
	m_Spline[0].knots    = &m_Knots[0][0];
	m_Spline[1].knots    = &m_Knots[1][0];

	// building horizontal splines
	// first create controls for segments of vertical spline...
	b3Spline::b3DeBoorSurfaceControl (&m_Spline[0],&m_Spline[1],ControlArray);
	MySpline          = m_Spline[0];
	MySpline.offset   = CurveNum = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]);
	MySpline.controls = ControlArray;

	// ... then create real horizontal spline curve.
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"SplineShape: horizontal\n");
	m_Spline[0].b3Dump();
#endif
	for (y = 0;y < CurveNum;y++)
	{
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL,"     y: %d\n", y);
#endif
		Points = MySpline.b3DeBoor (SplVector,y);
		for (t = 0;t < Points;t++)
		{
			Vector->v.x = SplVector[t].x;
			Vector->v.y = SplVector[t].y;
			Vector->v.z = SplVector[t].z;
			Vector++;
		}
	}
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"SplineShape: horizontal done\n");
#endif

	// building vertical splines
	// first create controls for segments of horizontal spline...
	b3Spline::b3DeBoorSurfaceControl (&m_Spline[1],&m_Spline[0],ControlArray);
	MySpline          = m_Spline[1];
	MySpline.offset   = CurveNum = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]);
	MySpline.controls = ControlArray;

	// ... then create real vertical spline curve.
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"SplineShape: vertical\n");
	m_Spline[1].b3Dump();
#endif
	for (x = 0;x < CurveNum;x++)
	{
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL,"     x: %d\n",x);
#endif
		Points = MySpline.b3DeBoor (SplVector,x);
		for (t = 0;t < Points;t++)
		{
			Vector->v.x = SplVector[t].x;
			Vector->v.y = SplVector[t].y;
			Vector->v.z = SplVector[t].z;
			Vector++;
		}
	}
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"SplineShape: vertical done\n");
#endif
}

void b3SplineShape::b3ComputeSolidVertices()
{
	b3Spline      MySpline;
	b3_index      x,y;
	b3_f64        fx,fxStep;
	b3_f64        fy,fyStep;
	b3_count      SubDiv,index,count;
	b3_gl_vertex *Vector;
	b3_gl_vertex *glVertex = *glVertexElements;
	b3_vector    *Aux;
	b3_vector     SplVector[B3_MAX_SUBDIV + 1];
	b3_vector     Between[(B3_MAX_SUBDIV + 1) * (B3_MAX_SUBDIV + 1)];

	// Building horizontal BSplines
	Aux    = Between;
	SubDiv = m_Spline[0].subdiv + 1;
	for (x = 0;x < SubDiv;x++)
	{
		Aux += m_Spline[1].b3DeBoor (Aux,x * m_Spline[0].offset);
	}

	// Create aux BSpline
	MySpline          = m_Spline[0];
	MySpline.offset   = m_Spline[1].subdiv + 1;
	MySpline.controls = Between;

	Vector = &glVertex[m_GridVertexCount];
	index  = 0;
	fy     = 0;
	fyStep = 1.0 / (b3_f64)m_ySubDiv;
	for (y = 0;y < m_ySubDiv;y++)
	{
		count   = MySpline.b3DeBoor (SplVector,y);
		index  += count;

		fx = 0;
		fxStep = 1.0 / (b3_f64)count;
		for (x = 0;x < count;x++)
		{
			Vector->t.s = fx;
			Vector->t.t = fy;
			Vector->v.x = SplVector[x].x;
			Vector->v.y = SplVector[x].y;
			Vector->v.z = SplVector[x].z;
			Vector++;
			fx += fxStep;
		}
		fy += fyStep;
	}
	B3_ASSERT(index <= m_SolidVertexCount);
}

void b3SplineShape::b3ComputeVertices()
{
	if (m_GridVertexCount  > 0) b3ComputeGridVertices();
	if (m_SolidVertexCount > 0) b3ComputeSolidVertices();
}

void b3SplineShape::b3ComputeGridIndices()
{
	b3_gl_line *gPtr = *glGridElements;
	b3_index    x,y,i=0;
	b3_count    max;

	// horizontal splines
	max = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]);
	for (y = 0;y < max;y++)
	{
		for (x = 1;x < m_Spline[0].subdiv;x++)
		{
			B3_GL_LINIT(gPtr,i+x-1,i+x);
		}

		B3_GL_LINIT(gPtr,i+x-1,m_Spline[0].closed ? i : i + x);

		i += (m_Spline[0].subdiv + 1);
	}

	// vertical splines
	max = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]);
	for (x = 0;x < max;x++)
	{
		for (y = 1;y < m_Spline[1].subdiv;y++)
		{
			B3_GL_LINIT(gPtr,i+y-1,i+y);
		}

		B3_GL_LINIT(gPtr,i+y-1,m_Spline[1].closed ? i : i + y);

		i += (m_Spline[1].subdiv + 1);
	}
}

void b3SplineShape::b3ComputeSolidIndices()
{
	b3_gl_polygon *pPtr;
	b3_index       x,y;
	b3_count       xSubDiv = m_Spline[0].subdiv,xModulo,xOffset;
	b3_count       ySubDiv = m_Spline[1].subdiv,yModulo;

	pPtr = *glPolygonElements;
	xModulo = m_Spline[0].closed ? xSubDiv : xSubDiv + 1;
	yModulo = m_Spline[1].closed ? ySubDiv : ySubDiv + 1;
	xOffset = m_Spline[0].subdiv + 1;
	for (y = 0;y < ySubDiv;y++)
	{
		for (x = 0;x < xSubDiv;x++)
		{
			B3_GL_PINIT(pPtr,
				m_GridVertexCount +  x              + xOffset *   y,
				m_GridVertexCount + (x+1) % xModulo + xOffset *   y,
				m_GridVertexCount +  x              + xOffset * ((y+1) % yModulo));

			B3_GL_PINIT(pPtr,
				m_GridVertexCount + (x+1) % xModulo + xOffset * ((y+1) % yModulo),
				m_GridVertexCount +  x              + xOffset * ((y+1) % yModulo),
				m_GridVertexCount + (x+1) % xModulo + xOffset *   y);
		}
	}
	glPolygonElements->b3SetCount(xSubDiv * ySubDiv * 2);
}

void b3SplineShape::b3ComputeIndices()
{
	if (glGridElements->b3GetCount() > 0) b3ComputeGridIndices();
	if (glPolygonElements->b3GetCount() > 0) b3ComputeSolidIndices();
}

void b3SplineShape::b3GetVertexRange(b3_index &start,b3_index &end)
{
	start = m_GridVertexCount;
	end   = m_GridVertexCount + m_ySubDiv * (m_Spline[0].subdiv + 1);
}

#ifdef GLU_NURBS
void b3SplineShape::b3Draw()
{
	// This doesn't work due to the fact that Linux GLU
	// doesn't support NURBS rendering. This doesn't matter
	// because the GLU NURBS support isn't very fast...
	if (!glSolid)
	{
		b3RenderObject::b3Draw();
	}
	else
	{
		b3_color color;

		b3GetDiffuseColor(&color);

		// Leave this for documentation purposes...
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(color.r,color.g,color.b);
		gluBeginSurface(glNURBS);
		gluNurbsSurface(glNURBS,
			m_Spline[0].knot_num,m_Spline[0].knots,
			m_Spline[1].knot_num,m_Spline[1].knots,
			m_Spline[0].offset * 3,
			m_Spline[1].offset * 3,
			(GLfloat *)m_Controls,
			m_Spline[0].degree + 1,
			m_Spline[1].degree + 1,
			GL_MAP2_VERTEX_3);
		gluEndSurface(glNURBS);
	}
}
#endif

void b3SplineShape::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3_vector *control;
	b3_index   offset;
	b3_index   x,y;

	control = m_Spline[0].controls;
	offset  = m_Spline[0].offset;

	// Transform control points
	for (y = 0;y < m_Spline[1].control_num;y++)
	{
		control  = &m_Spline[0].controls[y * m_Spline[1].offset];
		for (x = 0;x < m_Spline[0].control_num;x++)
		{
			b3Vector::b3MatrixMul4D(transformation,control);
			control += offset;
		}
	}
	b3TriangleShape::b3Transform(transformation,is_affine);
}

void b3SplineShape::b3SetupPicking(b3PickInfo *info)
{
	b3_vector    *control;
	b3_index      offset;
	b3_index      x,y;

	control = m_Spline[0].controls;
	offset  = m_Spline[0].offset;

	// Transform control points
	for (y = 0;y < m_Spline[1].control_num;y++)
	{
		control  = &m_Spline[0].controls[y * m_Spline[1].offset];
		for (x = 0;x < m_Spline[0].control_num;x++)
		{
			info->b3AddPickPoint(control);
			control += offset;
		}
	}
}

void b3SplineShape::b3SetupGrid(b3PickInfo *info)
{
	b3_vector    *control;
	b3_index      offset;
	b3_index      x,y,start;

	control = m_Spline[0].controls;
	offset  = m_Spline[0].offset;

	// Transform control points
	for (y = 0;y < m_Spline[1].control_num;y++)
	{
		control  = &m_Spline[0].controls[y * m_Spline[1].offset];
		for (x = 0;x < m_Spline[0].control_num;x++)
		{
			info->b3AddVertex(control);
			control += offset;
		}
	}
	// Compute horizontal grid lines
	for (y = 0;y < m_Spline[1].control_num;y++)
	{
		start = y * m_Spline[0].control_num;
		for (x = 1;x < m_Spline[0].control_num;x++)
		{
			info->b3AddLine(
				start + x - 1,
				start + x);
		}
		if (m_Spline[0].closed)
		{
			info->b3AddLine(start,start + m_Spline[0].control_num - 1);
		}
	}

	// Compute vertical grid lines
	offset = m_Spline[0].control_num;
	for (x = 0;x < m_Spline[0].control_num;x++)
	{
		for (y = 1;y < m_Spline[1].control_num;y++)
		{
			info->b3AddLine(
				x + (y - 1) * offset,
				x +  y      * offset);
		}
		if (m_Spline[1].closed)
		{
			info->b3AddLine(x,x + (m_Spline[1].control_num - 1) * offset);
		}
	}
}

b3_bool b3SplineShape::b3Prepare() throw(b3WorldException)
{
	b3_vertex   *Vertex;
	b3_triangle *Triangle;
	b3_vector   *Vector;
	b3_vector   *Between;
	b3Spline     MySpline;
	b3_res       xSize,ySize,x,y;
	b3_count     SubDiv,TriaCount,VertexCount;
	b3_vector    VertexField[B3_MAX_SUBDIV+1];

	Between = (b3_vector *)b3Item::b3Alloc (sizeof(b3_vector) *
		(B3_MAX_SUBDIV + 1) * (B3_MAX_SUBDIV + 1));
	if (Between == null)
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}

	xSize       = m_Spline[0].subdiv;
	ySize       = m_Spline[1].subdiv;
	TriaCount = xSize * ySize * 2;

	if (!m_Spline[0].closed) xSize++;
	if (!m_Spline[1].closed) ySize++;
	VertexCount = xSize * ySize;

	// Reallocating new tria shape
	if (!b3TriangleShape::b3Init(VertexCount,TriaCount,m_Spline[0].subdiv,m_Spline[1].subdiv))
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}

	// building horizontal splines
	Vector = Between;
	SubDiv = m_Spline[0].subdiv + 1;
	for (x = 0;x < SubDiv;x++)
	{
		Vector += m_Spline[1].b3DeBoor (Vector,x * m_Spline[0].offset);
	}

	MySpline          = m_Spline[0];
	MySpline.offset   = m_Spline[1].subdiv + 1;
	MySpline.controls = Between;

	Vertex = m_Vertices;
	for (y = 0;y < ySize;y++)
	{
		MySpline.b3DeBoor (VertexField,y);
		for (x = 0;x < xSize;x++)
		{
			Vertex->Point.x = VertexField[x].x;
			Vertex->Point.y = VertexField[x].y;
			Vertex->Point.z = VertexField[x].z;
			Vertex++;
		}
	}

	Triangle = m_Triangles;
	for (y = 0;y < m_Spline[1].subdiv;y++)
	{
		for (x = 0;x < m_Spline[0].subdiv;x++)
		{
			Triangle->P1  =  x            + xSize *  y;
			Triangle->P2  = (x+1) % xSize + xSize *  y;
			Triangle->P3  =  x            + xSize * ((y+1) % ySize);
			Triangle++;

			Triangle->P1  = (x+1) % xSize + xSize * ((y+1) % ySize);
			Triangle->P2  =  x            + xSize * ((y+1) % ySize);
			Triangle->P3  = (x+1) % xSize + xSize *  y;
			Triangle++;
		}
	}

	m_xSize = m_Spline[0].subdiv;
	m_ySize = m_Spline[1].subdiv;
	m_Flags = PHONG;

	b3Item::b3Free(Between);

	return b3TriangleShape::b3Prepare();
}
