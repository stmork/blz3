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

void b3SplineRotShape::b3Transform(b3_matrix *transformation)
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
#ifdef BLZ3_USE_OPENGL
	b3_matrix  Matrix;
	b3Spline   AuxSpline;
	b3_vector  AuxControls[B3_MAX_CONTROLS + 1];
	b3_vector *Vector;
	GLfloat   *Tex;
	b3_index   i,a,x;
	b3_count   count;
	b3_f64     fx,fxStep;
	b3_f64     fy,fyStep;

	// Build rotation matrix
	b3MatrixRotVec (null,&Matrix,&m_Axis,M_PI * 2 / m_rSubDiv);

	// Copy BSpline
	AuxSpline          = m_Spline;
	AuxSpline.controls = AuxControls;
	for (i = 0;i < AuxSpline.control_num;i++)
	{
		AuxControls[i] = m_Controls[i];
	}

	Vector = (b3_vector *)glVertices;
	Tex    = glTexCoord;
	fy     = 0;
	fyStep = 1.0 / (b3_f64)m_rSubDiv;
	for (a = 0;a < m_rSubDiv;a++)
	{
		// Compute curve
		count   = AuxSpline.b3DeBoor(Vector,0);
		Vector += count;

		fx = 0;
		fxStep = 1.0 / (b3_f64)count;
		for (x = 0;x < count;x++)
		{
			*Tex++ = fx;
			*Tex++ = fy;
			fx += fxStep;
		}
		fy += fyStep;

		// Rotate control points
		for (i = 0;i < AuxSpline.control_num;i++)
		{
			b3MatrixVMul (&Matrix,&AuxControls[i],&AuxControls[i],true);
		}
	}

	xSize  = m_rSubDiv;
	ySize  = AuxSpline.subdiv;
#endif
}

void b3SplineRotShape::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_index   a,x1,x2,y1,y2;
	b3_count   yStep;
	b3_f64     sStep;

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

			*gPtr++ = x1 + y1;
			*gPtr++ = x1 + y2;

			*pPtr++ = x1 + y1;
			*pPtr++ = x2 + y1;
			*pPtr++ = x1 + y2;

			*pPtr++ = x2 + y2;
			*pPtr++ = x1 + y2;
			*pPtr++ = x2 + y1;
		}

		// lines between curves
		for (y1 = 0;y1 < m_ySubDiv;y1++)
		{
			*gPtr++ = x1 + y1;
			*gPtr++ = x2 + y1;
		}
		x1 += yStep;
	}
#endif
}
