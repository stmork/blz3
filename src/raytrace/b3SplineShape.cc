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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineCurve::b3SplineCurve(b3_size class_size,b3_u32 class_type) :
	b3TriangleShape(class_size, class_type)
{
}

b3SplineCurve::b3SplineCurve(b3_u32 class_type) :
	b3TriangleShape(sizeof(b3SplineCurve), class_type)
{
}

b3SplineCurve::b3SplineCurve(b3_u32 *src) : b3TriangleShape(src)
{
	b3_index i;

	m_rSubDiv = b3InitInt();
	for (i = 0;i < B3_MAX_KNOTS;i++)
	{
		m_Knots[i] = b3InitFloat();
	}
	b3InitSpline(&m_Spline,null,m_Knots);
	b3InitVector(&m_Axis.pos);
	b3InitVector(&m_Axis.dir);

	m_Controls = (b3_vector *)b3Item::b3Alloc(m_Spline.control_max * sizeof(b3_vector));
	m_Spline.controls = m_Controls;
	for (i = 0;i < m_Spline.control_max;i++)
	{
		b3InitVector(&m_Controls[i]);
	}
	b3InitActivation();
}

void b3SplineCurve::b3StoreShape()
{
	b3_index i;

	b3StoreInt(m_rSubDiv);
	for (i = 0;i < B3_MAX_KNOTS;i++)
	{
		b3StoreFloat(m_Knots[i]);
	}
	b3StoreSpline(&m_Spline);
	b3StoreVector(&m_Axis.pos);
	b3StoreVector(&m_Axis.dir);
	for (i = 0;i < m_Spline.control_max;i++)
	{
		b3StoreVector(&m_Controls[i]);
	}
}

b3_bool b3SplineCurve::b3Prepare()
{
	b3Spline     MySpline;
	b3_triangle *Triangle;
	b3_matrix    Matrix;
	b3_res       xSize,ySize,x,y;
	b3_vector    Between[B3_MAX_CONTROLS+1];
	b3_vector    VertexField[B3_MAX_SUBDIV+1];


	// Create aux BSpline
	MySpline          = m_Spline;
	MySpline.controls = Between;

	xSize       = m_rSubDiv;
	ySize       = MySpline.subdiv;
	m_TriaCount = xSize * ySize * 2;

	if (!MySpline.closed) ySize++;
	m_VertexCount = xSize * ySize;

	// Reallocating new tria shape
	b3Item::b3Free(m_Vertices);
	b3Item::b3Free(m_Triangles);
	m_Vertices  = (b3_vertex *)b3Item::b3Alloc(m_VertexCount * sizeof(b3_vertex));
	m_Triangles = (b3_triangle *)b3Item::b3Alloc(m_TriaCount * sizeof(b3_triangle));
	if ((m_Vertices == null) || (m_Triangles == null))
	{
		throw new b3WorldException(B3_WORLD_MEMORY);
	}


	// computing vertices
	MySpline.offset = 1;
	for (x = 0;x < xSize;x++)
	{
		b3MatrixRotVec (null,&Matrix,&m_Axis,M_PI * 2.0 * x / xSize);
		for (y = 0;y < MySpline.control_num;y++)
		{
			b3MatrixVMul (&Matrix,&m_Controls[y],&Between[y],true);
		}

		MySpline.b3DeBoor (VertexField,0);
		for (y = 0;y < ySize;y++)
		{
			m_Vertices[y * xSize + x].Point.x = VertexField[y].x;
			m_Vertices[y * xSize + x].Point.y = VertexField[y].y;
			m_Vertices[y * xSize + x].Point.z = VertexField[y].z;
		}
	}


	// computing triangles
	Triangle = m_Triangles;
	for (y = 0;y < MySpline.subdiv;y++)
	{
		for (x = 0;x < xSize;x++)
		{
			Triangle->P1 =  x            + xSize *  y;
			Triangle->P2 = (x+1) % xSize + xSize *  y;
			Triangle->P3 =  x            + xSize * ((y+1) % ySize);
			Triangle++;

			Triangle->P1 = (x+1) % xSize + xSize * ((y+1) % ySize);
			Triangle->P2 =  x            + xSize * ((y+1) % ySize);
			Triangle->P3 = (x+1) % xSize + xSize *  y;
			Triangle++;
		}
	}


	/* initializing values */
	m_xSize = xSize;
	m_ySize = MySpline.subdiv;
	m_Flags = PHONG;

	return b3TriangleShape::b3Prepare();
}

void b3SplineCurve::b3Transform(b3_matrix *transformation)
{
	b3_vector *control;
	b3_index   offset;
	b3_index   x;

	control = m_Spline.controls;
	offset  = m_Spline.offset;

	// Transform rotation axis
	b3MatrixVMul (transformation,&m_Axis.pos,&m_Axis.pos,true);
	b3MatrixVMul (transformation,&m_Axis.dir,&m_Axis.dir,false);

	// Transform control points
	for (x = 0;x < m_Spline.control_num;x++)
	{
		b3Vector::b3MatrixMul4D(transformation,control);
		control += offset;
	}
	b3TriangleShape::b3Transform(transformation);
}


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
	m_Controls      = (b3_vector *)b3Item::b3Alloc(control_count * sizeof(b3_vector));
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
	b3ShapeRenderContext *context = (b3ShapeRenderContext *)ctx;
	
	Between   = context->b3GetSplineAux();

	// Compute number of grid vertices
#ifdef BLZ3_USE_OPENGL
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
#endif
}

void b3SplineShape::b3ComputeGridVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_count   CurveNum,Points;
	b3_index   x,y;
	b3Spline   MySpline;

	Points        = 0;
	Vector        = (b3_vector *)glVertices;

	m_Spline[0].controls =  m_Controls;
	m_Spline[1].controls =  m_Controls;
	m_Spline[0].knots    = &m_Knots[0][0];
	m_Spline[1].knots    = &m_Knots[1][0];


	// building horizontal splines
	// first create controls for segments of vertical spline...
	b3Spline::b3DeBoorSurfaceControl (&m_Spline[0],&m_Spline[1],Between);
	MySpline          = m_Spline[0];
	MySpline.offset   = CurveNum = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]);
	MySpline.controls = Between;

	// ... then create real horizontal spline curve.
	for (y = 0;y < CurveNum;y++)
	{
		Points         = MySpline.b3DeBoor (Vector,y);
		Vector        += Points;
	}

	// building vertical splines
	// first create controls for segments of horizontal spline...
	b3Spline::b3DeBoorSurfaceControl (&m_Spline[1],&m_Spline[0],Between);
	MySpline          = m_Spline[1];
	MySpline.offset   = CurveNum = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]);
	MySpline.controls = Between;

	// ... then create real vertical spline curve.
	for (x = 0;x < CurveNum;x++)
	{
		Points         = MySpline.b3DeBoor (Vector,x);
		Vector        += Points;
	}
#endif
}

void b3SplineShape::b3ComputeSolidVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3Spline   MySpline;
	b3_index   x,y;
	b3_count   SubDiv,index,count;
	b3_vector *Vector;

	// Building horizontal BSplines
	Vector = Between;
	SubDiv = m_Spline[0].subdiv + 1;
	for (x = 0;x < SubDiv;x++)
	{
		Vector += m_Spline[1].b3DeBoor (Vector,x * m_Spline[0].offset);
	}

	// Create aux BSpline
	MySpline          = m_Spline[0];
	MySpline.offset   = m_Spline[1].subdiv + 1;
	MySpline.controls = Between;

	Vector = (b3_vector *)&glVertices[m_GridVertexCount * 3];
	index  = 0;
	for (y = 0;y < m_ySubDiv;y++)
	{
		count   = MySpline.b3DeBoor (Vector,y);
		Vector += count;
		index  += count;
	}
	B3_ASSERT(index <= m_SolidVertexCount);
#endif
}

void b3SplineShape::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	if (m_GridVertexCount  > 0) b3ComputeGridVertices();
	if (m_SolidVertexCount > 0) b3ComputeSolidVertices();
#endif
}

void b3SplineShape::b3ComputeGridIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	b3_index  x,y,i=0;
	b3_count  max;

	// horizontal splines
	gPtr = glGrids;
	max = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]);
	for (y = 0;y < max;y++)
	{
		for (x = 1;x < m_Spline[0].subdiv;x++)
		{
			*gPtr++ = i + x - 1;
			*gPtr++ = i + x;
		}

		*gPtr++ = i + x - 1;
		*gPtr++ = (m_Spline[0].closed ? i : i + x);

		i += (m_Spline[0].subdiv + 1);
	}

	// vertical splines
	max = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]);
	for (x = 0;x < max;x++)
	{
		for (y = 1;y < m_Spline[1].subdiv;y++)
		{
			*gPtr++ = i + y - 1;
			*gPtr++ = i + y;
		}

		*gPtr++ = i + y - 1;
		*gPtr++ = (m_Spline[1].closed ? i : i + y);

		i += (m_Spline[1].subdiv + 1);
		
	}
#endif
}

void b3SplineShape::b3ComputeSolidIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort   *pPtr;
	b3_index    x,y;
	b3_count    xSubDiv = m_Spline[0].subdiv,xModulo,xOffset;
	b3_count    ySubDiv = m_Spline[1].subdiv,yModulo;

	pPtr = glPolygons;
	xModulo = m_Spline[0].closed ? xSubDiv : xSubDiv + 1;
	yModulo = m_Spline[1].closed ? ySubDiv : ySubDiv + 1;
	xOffset = m_Spline[0].subdiv + 1;
	for (y = 0;y < ySubDiv;y++)
	{
		for (x = 0;x < xSubDiv;x++)
		{
			*pPtr++ = m_GridVertexCount +  x              + xOffset *   y;
			*pPtr++ = m_GridVertexCount + (x+1) % xModulo + xOffset *   y;
			*pPtr++ = m_GridVertexCount +  x              + xOffset * ((y+1) % yModulo);

			*pPtr++ = m_GridVertexCount + (x+1) % xModulo + xOffset * ((y+1) % yModulo);
			*pPtr++ = m_GridVertexCount +  x              + xOffset * ((y+1) % yModulo);
			*pPtr++ = m_GridVertexCount + (x+1) % xModulo + xOffset *   y;
		}
	}
	glPolyCount = xSubDiv * ySubDiv * 2;
#endif
}

void b3SplineShape::b3ComputeIndices()
{
	if (glGridCount > 0) b3ComputeGridIndices();
	if (glPolyCount > 0) b3ComputeSolidIndices();
}

void b3SplineShape::b3GetVertexRange(b3_index &start,b3_index &end)
{
#ifdef BLZ3_USE_OPENGL
	start = m_GridVertexCount;
	end   = m_GridVertexCount + m_ySubDiv * (m_Spline[0].subdiv + 1);
#endif
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

void b3SplineShape::b3Transform(b3_matrix *transformation)
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
	b3TriangleShape::b3Transform(transformation);
}

b3_bool b3SplineShape::b3Prepare()
{
	b3_vertex   *Vertex;
	b3_triangle *Triangle;
	b3_vector   *Vector;
	b3_vector   *Between;
	b3Spline     MySpline;
	b3_res       xSize,ySize,x,y;
	b3_count     SubDiv;
	b3_vector    VertexField[B3_MAX_SUBDIV+1];

	Between = (b3_vector *)b3Item::b3Alloc (sizeof(b3_vector) *
		(B3_MAX_SUBDIV + 1) * (B3_MAX_SUBDIV + 1));
	if (Between == null)
	{
		throw new b3WorldException(B3_WORLD_MEMORY);
	}

	xSize       = m_Spline[0].subdiv;
	ySize       = m_Spline[1].subdiv;
	m_TriaCount = xSize * ySize * 2;

	if (!m_Spline[0].closed) xSize++;
	if (!m_Spline[1].closed) ySize++;
	m_VertexCount = xSize * ySize;

	// Reallocating new tria shape
	b3Item::b3Free(m_Vertices);
	b3Item::b3Free(m_Triangles);
	m_Vertices  = (b3_vertex *)b3Item::b3Alloc(m_VertexCount * sizeof(b3_vertex));
	m_Triangles = (b3_triangle *)b3Item::b3Alloc(m_TriaCount * sizeof(b3_triangle));
	if ((m_Vertices == null) || (m_Triangles == null))
	{
		throw new b3WorldException(B3_WORLD_MEMORY);
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
