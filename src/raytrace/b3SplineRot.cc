/*
**
**      $Filename:      b3SplineRot.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing BSpline rotation shape
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
**      Revision 1.23  2003/02/25 15:56:21  sm
**      - Added SplineRot to control grid drawing.
**      - Added support for pixel format selection in dialog items
**      - Restructured b3PickInfo
**
**      Revision 1.22  2003/02/24 17:32:38  sm
**      - Added further picking support.
**      - Fixed geometry update delay.
**
**      Revision 1.21  2003/02/18 16:52:57  sm
**      - Fixed no name error on new scenes (ticket no. 4).
**      - Introduced new b3Matrix class and renamed methods.
**
**      Revision 1.20  2002/08/24 13:22:02  sm
**      - Extensive debugging on threading code done!
**        o Cleaned up POSIX threads
**        o Made safe thread handling available in raytracing code
**        o b3PrepareInfo instantiates threads only once.
**      - Added new thread options to gcc: "-D_REENTRAND -pthread"
**        which I only can assume what they are doing;-)
**      - Time window in motion blur moved from [-0.5,0.5] to [0,1]
**        and corrected upper time limit.
**
**      Revision 1.19  2002/08/16 11:40:39  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.18  2002/08/15 13:56:43  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.17  2002/08/09 13:20:20  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.16  2002/08/04 13:24:56  sm
**      - Found transformation bug: Normals have to be treated as
**        direction vectors, aren't them?
**      - b3PrepareInfo::m_PrepareProc initialized not only in
**        debug mode.
**
**      Revision 1.15  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.14  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.13  2002/03/13 19:01:59  sm
**      - Fixed some GCC warnings.
**
**      Revision 1.12  2002/03/10 21:14:41  sm
**      - Fixed rotation shapes with custom subdivision for rotation.
**
**      Revision 1.11  2002/03/10 13:55:15  sm
**      - Added creation dialog for rotation shapes.
**      - Cleaned up derivation of b3SplineRotShape.
**      - Added support for foreign BLZ3_HOME directories.
**
**      Revision 1.10  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.9  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.8  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.7  2001/09/22 16:19:53  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.6  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.5  2001/08/17 19:28:54  sm
**      - Now able to draw every shape by lines or shaded.
**        It's great!
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
**      Revision 1.1  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3SplineRotShape Implementation               **
**                                                                      **
*************************************************************************/

b3SplineRotShape::b3SplineRotShape(b3_u32 class_type) :
	b3TriangleShape(sizeof(b3SplineRotShape), class_type)
{
	m_Controls = null;
	b3Vector::b3Init(&m_Axis.pos);
	b3Vector::b3Init(&m_Axis.dir,0,0,1);
}

b3SplineRotShape::b3SplineRotShape(b3_u32 *src) :
	b3TriangleShape(src)
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

void b3SplineRotShape::b3StoreShape()
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


void b3SplineRotShape::b3Init(
	b3_count Degree,
	b3_count ControlNum,
	b3_bool  Closed,
	b3_count SubDiv)
{
	m_rSubDiv = SubDiv;

	// Allocate controls
	m_Controls      = (b3_vector *)b3Item::b3Alloc(m_Spline.control_max * sizeof(b3_vector));

	// Init horizontal spline
	m_Spline.knots    = m_Knots;
	m_Spline.controls = m_Controls;
	m_Spline.offset   = 1;
	m_Spline.b3InitCurve(Degree,ControlNum,Closed);
}

b3_bool b3SplineRotShape::b3Prepare()
{
	b3Spline     MySpline;
	b3_triangle *Triangle;
	b3_matrix    Matrix;
	b3_res       xSize,ySize,x,y;
	b3_count     VertexCount,TriaCount;
	b3_vector    Between[B3_MAX_CONTROLS+1];
	b3_vector    VertexField[B3_MAX_SUBDIV+1];

	// Create aux BSpline
	MySpline          = m_Spline;
	MySpline.controls = Between;

	xSize       = m_rSubDiv;
	ySize       = MySpline.subdiv;
	TriaCount = xSize * ySize * 2;

	if (!MySpline.closed) ySize++;
	VertexCount = xSize * ySize;

	// Reallocating new tria shape
	if (!b3TriangleShape::b3Init(VertexCount,TriaCount,m_rSubDiv,MySpline.subdiv))
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}


	// computing vertices
	MySpline.offset = 1;
	for (x = 0;x < xSize;x++)
	{
		b3Matrix::b3RotateVector (null,&Matrix,&m_Axis,M_PI * 2.0 * x / xSize);
		for (y = 0;y < MySpline.control_num;y++)
		{
			b3Matrix::b3VMul (&Matrix,&m_Controls[y],&Between[y],true);
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

void b3SplineRotShape::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3_vector *control;
	b3_index   offset;
	b3_index   x;

	control = m_Spline.controls;
	offset  = m_Spline.offset;

	// Transform rotation axis
	b3Matrix::b3VMul (transformation,&m_Axis.pos,&m_Axis.pos,true);
	b3Matrix::b3VMul (transformation,&m_Axis.dir,&m_Axis.dir,false);

	// Transform control points
	for (x = 0;x < m_Spline.control_num;x++)
	{
		b3Vector::b3MatrixMul4D(transformation,control);
		control += offset;
	}
	b3TriangleShape::b3Transform(transformation,is_affine);
}

void b3SplineRotShape::b3SetupPicking(b3PickInfo *info)
{
	b3_vector *control;
	b3_index   offset;
	b3_index   x;

	info->b3AddPickPoint(&m_Axis.pos,"b");
	info->b3AddPickDir(&m_Axis.pos,&m_Axis.dir,"d");

	control = m_Spline.controls;
	offset  = m_Spline.offset;
	for (x = 0;x < m_Spline.control_num;x++)
	{
		info->b3AddPickPoint(control);
		info->b3AddVertex(control);
		control += offset;
	}

	for (x = (m_Spline.closed ? 0 : 1);x < m_Spline.control_num;x++)
	{
		info->b3AddLine(x,(x + m_Spline.control_num - 1) % m_Spline.control_num);
	}
}

void b3SplineRotShape::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	SinCosSteps = m_rSubDiv;
	m_ySubDiv   = m_Spline.subdiv;
	m_xSubDiv   = m_rSubDiv;
	if (!m_Spline.closed)
	{
		m_ySubDiv++;
	}

	vertCount = (m_Spline.subdiv + 1) * m_rSubDiv;
	gridCount = m_rSubDiv * (m_Spline.subdiv + m_ySubDiv);
	polyCount = m_rSubDiv *  m_Spline.subdiv * 2;
}

void b3SplineRotShape::b3ComputeVertices()
{
	b3_matrix     Matrix;
	b3Spline      AuxSpline;
	b3_vector     AuxControls[B3_MAX_CONTROLS + 1];
	b3_vector     SplVector[B3_MAX_SUBDIV + 1];
	b3_gl_vertex *Vector;
	b3_index      i,a,x;
	b3_count      count;
	b3_f64        fx,fxStep;
	b3_f64        fy,fyStep;

	// Build rotation matrix
	b3Matrix::b3RotateVector (null,&Matrix,&m_Axis,M_PI * 2 / m_rSubDiv);

	// Copy BSpline
	AuxSpline          = m_Spline;
	AuxSpline.controls = AuxControls;
	for (i = 0;i < AuxSpline.control_num;i++)
	{
		AuxControls[i] = m_Controls[i];
	}

	Vector = glVertex;
	fy     = 0;
	fyStep = 1.0 / (b3_f64)m_rSubDiv;
	for (a = 0;a < m_rSubDiv;a++)
	{
		// Compute curve
		count  = AuxSpline.b3DeBoor(SplVector,0);
		fx     = 0;
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

		// Rotate control points
		for (i = 0;i < AuxSpline.control_num;i++)
		{
			b3Matrix::b3VMul (&Matrix,&AuxControls[i],&AuxControls[i],true);
		}
	}

	xSize  = m_rSubDiv;
	ySize  = AuxSpline.subdiv;
}

void b3SplineRotShape::b3ComputeIndices()
{
	b3_gl_line    *gPtr;
	b3_gl_polygon *pPtr;
	b3_index       a,x1,x2,y1,y2;
	b3_count       yStep;
	b3_f64         sStep;

	yStep = m_Spline.subdiv + 1;
	sStep = (b3_f64)m_Spline.subdiv / m_rSubDiv;

	gPtr  = glGrids;
	pPtr  = glPolygons;
	x1    = 0;
	
	// for each curve
	for (a = 0;a < m_rSubDiv;a++)
	{
		x2 = (a + 1) % m_rSubDiv * yStep;

		// curve itself
		for (y1 = 0;y1 < m_Spline.subdiv;y1++)
		{
			y2 = (y1 + 1) % m_ySubDiv;

			B3_GL_LINIT(gPtr,x1+y1,x1+y2);

			B3_GL_PINIT(pPtr,x1+y1,x2+y1,x1+y2);
			B3_GL_PINIT(pPtr,x2+y2,x1+y2,x2+y1);
		}

		// lines between curves
		for (y1 = 0;y1 < m_ySubDiv;y1++)
		{
			B3_GL_LINIT(gPtr,x1+y1,x2+y1);
		}
		x1 += yStep;
	}
}
