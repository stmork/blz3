/*
**
**      $Filename:      b3Shape.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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

#ifdef BLZ3_USE_OPENGL
static GLushort box_grids[] =
{
	0,1,
	1,2,
	2,3,
	3,0,
	7,6,
	6,5,
	5,4,
	4,7,
	0,7,
	1,6,
	2,5,
	3,4
};

static GLushort box_polygons[] =
{
	 0, 1, 3, // front
	 2, 3, 1,
	 6, 7, 5, // back
	 4, 5, 7,
	 9,14,10, // right
	13,10,14,
	 7, 8,12, // left
	11,12, 8,
	19,18,20, // top
	21,20,18,
	17,16,22, // bottom
	23, 6,16
};
#endif

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderShapeContext::b3RenderShapeContext(b3_count new_subdiv)
{
#ifdef BLZ3_USE_OPENGL
	CylinderIndices = null;
	ConeIndices     = null;
#endif
	Between         = null;
	b3InitSubdiv(new_subdiv);
}

void b3RenderShapeContext::b3InitSubdiv(b3_count new_subdiv)
{
	b3_f64   aux;
	b3_index i;

	subdiv = (new_subdiv > B3_MAX_RENDER_SUBDIV ? B3_MAX_RENDER_SUBDIV : new_subdiv);
	if (subdiv < 8)
	{
		subdiv = 8;
	}

	for (i = 0;i <= subdiv;i++)
	{
		aux    = i * M_PI * 2 / subdiv;
		Sin[i] = sin(aux);
		Cos[i] = cos(aux);
	}

#ifdef BLZ3_USE_OPENGL
	GLushort *ptr;
	b3_index  a;

	if (CylinderIndices != null)
	{
		b3Free(CylinderIndices);
	}
	if (ConeIndices != null)
	{
		b3Free(ConeIndices);
	}

	CylinderIndices = (GLushort *)b3Alloc
			((subdiv + 1) * 3 * 2 * sizeof(GLushort));
	if (CylinderIndices != null)
	{
		ptr = CylinderIndices;
		a = 0;
		for (i = 0;i <= subdiv;i++)
		{
			// Conactinate vertices in this ascending order:
			//
			//   (c)
			// 1-----3
			// |
			// |(a)
			// |
			// 0-----2
			//   (b)

			// (a)
			*ptr++ = a;
			*ptr++ = a + 1;

			// (b)
			*ptr++ = a;
			*ptr++ = a + 2;

			// (c)
			*ptr++ = a + 1;
			*ptr++ = a + 3;
			a += 2;
		}
	}

	ConeIndices = (GLushort *)b3Alloc
		((subdiv + 1) * 2 * 2 * sizeof(GLushort));
	if (ConeIndices != null)
	{
		ptr = ConeIndices;
		for (i = 0;i <= subdiv;i++)
		{
			*ptr++ = 0;
			*ptr++ = i+1;

			*ptr++ = i+1;
			*ptr++ = i+2;
		}
	}
#endif
}

b3_count b3RenderShapeContext::b3GetSubdiv()
{
	return subdiv;
}

b3_f64 *b3RenderShapeContext::b3GetSinTable()
{
	return Sin;
}

b3_f64 *b3RenderShapeContext::b3GetCosTable()
{
	return Cos;
}

b3_vector *b3RenderShapeContext::b3GetSplineAux()
{
	if (Between == null)
	{
		Between = (b3_vector *)b3Alloc(
			B3_MAX_CONTROLS * B3_MAX_CONTROLS * sizeof(b3_vector));
	}
	return Between;
}

#ifdef BLZ3_USE_OPENGL
GLushort *b3RenderShapeContext::b3GetCylinderIndices()
{
	return CylinderIndices;
}

GLushort *b3RenderShapeContext::b3GetConeIndices()
{
	return ConeIndices;
}
#endif


/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderShapeObject::b3RenderShapeObject()
{
	Epsilon     = 0.001;
	Between     = null;
}

b3_count b3RenderShapeObject::b3GetIndexOverhead (
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
	if ((xs - x1) > Epsilon) Overhead++;
	if ((x2 - xe) > Epsilon) Overhead++;

	return ((xs > 0)||(xe < SinCosSteps)) ? -Overhead : Overhead;
}

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderShape::b3RenderShape(b3_size class_size,b3_u32 class_type) : b3Shape(class_size, class_type)
{
}

b3RenderShape::b3RenderShape(b3_u32 class_type) : b3Shape(sizeof(b3RenderShape), class_type)
{
}

b3RenderShape::b3RenderShape(b3_u32 *src) : b3Shape(src)
{
}

void b3RenderShape::b3ComputeSphereVertices(
	b3_vector   &Base,
	b3_vector   &Dir)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_index   i,j;
	b3_count   Circles;
	b3_f64     cx,sx,cy,sy,a;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32     Rad;
	b3_vector  Aux,Dir1,Dir2,Dir3;

	Vector = (b3_vector *)glVertices;
	Aux    = Base;
	Dir1.x = Rad = b3Length (&Dir);
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

			Vector->x = Base.x + cx * sy * Dir1.x + sx * sy * Dir2.x + cy * Dir3.x;
			Vector->y = Base.y + cx * sy * Dir1.y + sx * sy * Dir2.y + cy * Dir3.y;
			Vector->z = Base.z + cx * sy * Dir1.z + sx * sy * Dir2.z + cy * Dir3.z;
			Vector++;
		}
	}
	/*
		PrintF ("\n");
		PrintF ("Points: %3ld\n",Points);
		PrintF ("Circles:%3ld\n",Circles);
	*/
#endif
 }

void b3RenderShape::b3ComputeCylinderVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     sx,sy,b,h,start,end;
	b3_index   i;
	b3_count   iMax;
	b3_vector  Bottom;

	Vector = (b3_vector *)glVertices;
	h      = Limit.y2 - Limit.y1;
	b      = Limit.y1;

	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;
	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = 2;

	if ((i - start) > Epsilon)
	{
		b = Limit.x1 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx = Cos[i % SinCosSteps];
		sy = Sin[i % SinCosSteps];

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		b = Limit.x2 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;
		xSize++;
	}
#endif
}

void b3RenderShape::b3ComputeCylinderIndices()
{
#ifdef BLZ3_USE_OPENGL
	b3_count Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		GridCount = 1;
		Overhead = -Overhead;
	}
	else
	{
		GridCount = 0;
	}
	GridCount += Overhead * 3;
#endif
}

void b3RenderShape::b3ComputeConeVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     sx,sy,b,a,h,start,end;
	b3_index   i;
	b3_count   iMax;
	b3_vector  Bottom;

	Vector   = (b3_vector *)glVertices;

	h        = Limit.y2 - Limit.y1;
	b        = Limit.y1;
	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = 1;

	if (Limit.y2 < 1)
	{
		ySize++;
		if ((i - start) > Epsilon)
		{
			a = Limit.x1 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = Bottom.x + (1-b) * sx * Dir1.x + (1-b) * sy * Dir2.x;
			Vector->y = Bottom.y + (1-b) * sx * Dir1.y + (1-b) * sy * Dir2.y;
			Vector->z = Bottom.z + (1-b) * sx * Dir1.z + (1-b) * sy * Dir2.z;
			Vector++;

			Vector->x = Bottom.x + (1-h) * sx * Dir1.x + (1-h) * sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + (1-h) * sx * Dir1.y + (1-h) * sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + (1-h) * sx * Dir1.z + (1-h) * sy * Dir2.z + h * Dir3.z;
			Vector++;
			xSize++;
		}

		for (;i <= iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * Cos[i % SinCosSteps];
			sy = (1-h) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
			Vector++;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = Bottom.x + (1-b) * sx * Dir1.x + (1-b) * sy * Dir2.x;
			Vector->y = Bottom.y + (1-b) * sx * Dir1.y + (1-b) * sy * Dir2.y;
			Vector->z = Bottom.z + (1-b) * sx * Dir1.z + (1-b) * sy * Dir2.z;
			Vector++;

			Vector->x = Bottom.x + (1-h) * sx * Dir1.x + (1-h) * sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + (1-h) * sx * Dir1.y + (1-h) * sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + (1-h) * sx * Dir1.z + (1-h) * sy * Dir2.z + h * Dir3.z;

			xSize++;
		}
	}
	else
	{
		Vector->x = Base.x + Dir3.x;
		Vector->y = Base.y + Dir3.y;
		Vector->z = Base.z + Dir3.z;
		Vector++;

		if ((i - start) > Epsilon)
		{
			a  = Limit.x1 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;
			xSize++;
		}

		for (;i <= iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;

			xSize++;
		}
	}
#endif
}

void b3RenderShape::b3ComputeConeIndices()
{
#ifdef BLZ3_USE_OPENGL
	b3_count Overhead;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		GridCount = 1;
		Overhead  = -Overhead;
	}
	else
	{
		GridCount = 0;
	}
	if (Limit.y2 < 1)
	{
		glGrids    = GridsCyl;
		GridCount += Overhead * 3;
	}
	else
	{
		glGrids    = GridsCone;
		GridCount += Overhead * 2;
	}
#endif
}

void b3RenderShape::b3ComputeEllipsoidVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_index   i,j;
	b3_count   iMax,Circles = 0;
	b3_f64     RadX,RadY,sx,sy;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32     start,end,a;

	Vector = (b3_vector *)glVertices;
	start  = (Limit.y1 + 1) * SinCosSteps * 0.25;
	end    = (Limit.y2 + 1) * SinCosSteps * 0.25;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > Epsilon)	/* underflow */
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
	if ((end - iMax) > Epsilon)	/* Overflow */
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

	if ((i - start) > Epsilon)
	{
		a  = Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
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

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		a  = Limit.x2 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}
	/*
		PrintF ("\n");
		PrintF ("Points: %3ld\n",Points);
		PrintF ("Circles:%3ld\n",Circles);
	*/
#endif
}

void b3RenderShape::b3ComputeEllipsoidIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *Index;
	b3_bool   EndLine = false;
	b3_index  i,j,Number,s,ys,ye;
	b3_count  Heights,Widths,Overhead;
	b3_f64    y1,y2;

	GridCount = 0;
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
	if ((ys - y1) > Epsilon) Heights++;
	if ((y2 - ye) > Epsilon) Heights++;

	Widths = Heights - 1;
	if ((SinCosSteps * 0.5 - y2) > Epsilon) Widths++;
	if (                     y1  > Epsilon) Widths++;

	if (EndLine) Number = (Widths + Heights) * Overhead + Heights;
	else         Number = (Widths + Heights) * Overhead;
	glGrids = Index = (GLushort *)b3RenderObject::b3Alloc
		(Number * 2 * sizeof(GLushort));
	if (Index == null)
	{
		return;
	}

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + 1;
		}
		GridCount += Heights;

		if (y1 <= Epsilon) j = 1;
		else               j = 0;
		if ((SinCosSteps * 0.5 - y2) <= Epsilon) while (j < Heights)
		{
			*Index++ = s + j;
			*Index++ = s + j + Heights + 1;
			GridCount++;
			j++;
		}
		else  while (j <= Heights)
		{
			*Index++ = s + j;
			*Index++ = s + j + Heights + 1;
			GridCount++;
			j++;
		}
		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + 1;
		}
		GridCount += Heights;
	}
	B3_ASSERT(GridCount <= Number);
	/*
		PrintF ("\n");
		PrintF ("Heights:  %ld\n",Heights);
		PrintF ("Widths:   %ld\n",Widths);
		PrintF ("Number:   %ld\n",Number);
		PrintF ("Overhead: %ld\n",Overhead);
		PrintF ("n:        %ld\n",n);
	*/
#endif
}

void b3RenderShape::b3ComputeBoxVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_vector  Aux;
	b3_index   i;

	Vector = (b3_vector *)glVertices;

	*Vector = Aux = Base;
	Vector++;

	Vector->x = (Aux.x += Dir1.x);
	Vector->y = (Aux.y += Dir1.y);
	Vector->z = (Aux.z += Dir1.z);
	Vector++;

	Vector->x = (Aux.x += Dir2.x);
	Vector->y = (Aux.y += Dir2.y);
	Vector->z = (Aux.z += Dir2.z);
	Vector++;

	Vector->x = (Aux.x -= Dir1.x);
	Vector->y = (Aux.y -= Dir1.y);
	Vector->z = (Aux.z -= Dir1.z);
	Vector++;

	Vector->x = (Aux.x += Dir3.x);
	Vector->y = (Aux.y += Dir3.y);
	Vector->z = (Aux.z += Dir3.z);
	Vector++;

	Vector->x = (Aux.x += Dir1.x);
	Vector->y = (Aux.y += Dir1.y);
	Vector->z = (Aux.z += Dir1.z);
	Vector++;

	Vector->x = (Aux.x -= Dir2.x);
	Vector->y = (Aux.y -= Dir2.y);
	Vector->z = (Aux.z -= Dir2.z);
	Vector++;

	Vector->x = (Aux.x -= Dir1.x);
	Vector->y = (Aux.y -= Dir1.y);
	Vector->z = (Aux.z -= Dir1.z);

	xSize = ySize = 1;

	for (i = 0;i < 8;i++)
	{
		Vector[i +  8] = Vector[i + 16] = Vector[i];
	}
#endif
}

void b3RenderShape::b3ComputeBoxIndices()
{
#ifdef BLZ3_USE_OPENGL
	glGrids    = box_grids;
	glPolygons = box_polygons;
#endif
}

void b3RenderShape::b3ComputeTorusVertices(
	b3_vector   &Base,
	b3_vector   &Dir1,
	b3_vector   &Dir2,
	b3_vector   &Dir3,
	b3_f64       aRad,
	b3_f64       bRad)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_f64     RadX,RadY,sx,sy,start,end,a;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_index   i,j;
	b3_count   iMax,Circles=0;
	b3_vector  Aux;

	Vector = (b3_vector *)glVertices;
	start  = Limit.y1 * SinCosSteps;
	end    = Limit.y2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > Epsilon)	/* underflow */
	{
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 1.0 / SinCosSteps;
	for (j = i;j <= iMax;j++)		/* integers */
	{
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > Epsilon)	/* Overflow */
	{
		LocalSin[Circles] = Limit.y2;
		Circles++;
	}
	xSize = 0;
	ySize = Circles;

	for (j=0;j<Circles;j++)		/* Calculate Values */
	{
		LocalCos[j] = cos(LocalSin[j] * M_PI * 2) * bRad;
		LocalSin[j] = sin(LocalSin[j] * M_PI * 2) * bRad;
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);

	if ((i - start) > Epsilon)
	{
		a     = Limit.x1 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx    = Cos[i];
		sy    = Sin[i];
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		a     = Limit.x2 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}
	/*
		PrintF ("\n");
		PrintF ("Points: %3ld\n",Points);
		PrintF ("Circles:%3ld\n",Circles);
	*/
#endif
}

void b3RenderShape::b3ComputeTorusIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *Index;
	b3_bool   EndLine = false,EndCol = false;
	b3_index  i,j,Number,s,ys,ye;
	b3_count  Heights,Widths,Overhead;
	b3_f64    y1,y2;

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
	if ((ys - y1) > Epsilon) Heights++;
	if ((y2 - ye) > Epsilon) Heights++;
	if ((ys > 0) || (ye < SinCosSteps))
	{
		EndCol = true;
	}

	Widths = Heights;
	if (EndCol) Widths++;

	Number = (Widths + Heights) * Overhead;
	if (EndLine) Number += Heights;

	GridCount = 0;
	glGrids = Index = (GLushort *)b3RenderObject::b3Alloc
		(Number * 2 * sizeof(GLushort));
	if (Index == null)
	{
		return;
	}

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + 1;
		}
		GridCount += Heights;

		for (j = 0;j < Widths;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + Heights + 1;
		}
		GridCount += Widths;

		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + 1;
		}
		GridCount += Heights;
	}

	/*
		PrintF ("\n");
		PrintF ("Heights:  %ld\n",Heights);
		PrintF ("Widths:   %ld\n",Widths);
		PrintF ("Number:   %ld\n",Number);
		PrintF ("Overhead: %ld\n",Overhead);
		PrintF ("n:        %ld\n",n);
	*/
#endif
}
