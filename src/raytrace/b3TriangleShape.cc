/*
**
**      $Filename:      b3TriangleShape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing triangle shape
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
#include "blz3/base/b3Cubic.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.41  2003/03/22 14:38:41  sm
**      - Some optimizations continued concerning triangles.
**
**      Revision 1.40  2003/03/20 21:04:58  sm
**      - Made some triangle intersection optimizations.
**
**      Revision 1.39  2003/03/19 20:33:07  sm
**      - Triangles intersection optimized.
**
**      Revision 1.38  2003/02/22 17:21:35  sm
**      - Changed some global variables into static class members:
**        o b3Scene::epsilon
**        o b3Scene::m_TexturePool et. al.
**        o b3SplineTemplate<class VECTOR>::bspline_errno
**
**      Revision 1.37  2003/02/02 14:22:32  sm
**      - Added TGF import facility.
**
**      Revision 1.36  2003/01/18 14:13:49  sm
**      - Added move/rotate stepper operations
**      - Cleaned up resource IDs
**
**      Revision 1.35  2003/01/07 16:14:38  sm
**      - Lines III: object editing didn't prepared any more. Fixed.
**      - Some prepare optimizations.
**
**      Revision 1.34  2003/01/06 19:16:03  sm
**      - Removed use of b3TriangleRef into an b3Array<b3_index>.
**      - Camera transformations are now matrix transformations.
**
**      Revision 1.33  2002/12/20 15:32:55  sm
**      - Made some ICC optimazations :-)
**
**      Revision 1.32  2002/08/16 13:20:14  sm
**      - Removed some unused methods.
**      - Allocation bug found in brt3 - the Un*x version of the
**        Blizzard III raytracer: It's necessary to use b3ShapeRenderContext
**        rather than b3renderContext which doesn't initialize subdivision
**        for shapes.
**
**      Revision 1.31  2002/08/15 13:56:44  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.30  2002/08/09 13:20:20  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.29  2002/08/04 13:24:56  sm
**      - Found transformation bug: Normals have to be treated as
**        direction vectors, aren't them?
**      - b3PrepareInfo::m_PrepareProc initialized not only in
**        debug mode.
**
**      Revision 1.28  2002/03/10 13:55:15  sm
**      - Added creation dialog for rotation shapes.
**      - Cleaned up derivation of b3SplineRotShape.
**      - Added support for foreign BLZ3_HOME directories.
**
**      Revision 1.27  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.26  2002/03/02 19:52:40  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.25  2002/03/02 16:47:50  sm
**      - Further cleanup of spline shapes and triangle shapes.
**      - Some cleanups of b3ControlLDC done: Now sending
**        messages to dialog via WM_NOTIFY.
**
**      Revision 1.24  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.23  2002/02/17 21:25:07  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.22  2002/01/21 16:56:46  sm
**      - Showing splash dialog only in release version.
**      - Prepared shape icons.
**
**      Revision 1.21  2002/01/20 12:48:51  sm
**      - Added splash screen
**      - Corrected repeat buttons (capture change)
**
**      Revision 1.20  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.19  2001/12/31 12:15:55  sm
**      - Fixed obsolete b3AnimElement handling
**
**      Revision 1.18  2001/12/31 11:05:18  sm
**      - Added TestData for testing Blizzard data structures for reading
**        and writing.
**      - Fixed bugs found with previous mentioned tool:
**        o Some b3AnimElement errors found to be fixed under Windows.
**        o b3TriangleShape destructor handled unchecked m_GridList pointer
**      - Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**        B3LOG_NORMAL to B3LOG_DEBUG.
**
**      Revision 1.17  2001/11/01 09:43:11  sm
**      - Some image logging cleanups.
**      - Texture preparing now in b3Prepare().
**      - Done some minor fixes.
**
**      Revision 1.16  2001/10/20 16:15:00  sm
**      - Some runtime environment cleanups. The CPU count is determined
**        only once.
**      - Introduced preparing routines for raytring to shapes.
**      - Found 5% performance loss: No problem, this was eaten by
**        bug fxing of the rotation spline shapes. (Phuu!)
**      - The next job is to implement different row sampler. Then we
**        should implemented the base set of the Blizzard II raytracer.
**
**      Revision 1.15  2001/10/19 14:46:58  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.14  2001/10/18 14:48:26  sm
**      - Fixing refracting problem on some scenes with glasses.
**      - Fixing overlighting problem when using Mork shading.
**      - Finxing some memory leaks (espacially b3TriangleRefs)
**      - Adding texture support to conditions (stencil mapping).
**        Now conditions are ready to work compatible with
**        Blizzard II.
**
**      Revision 1.13  2001/10/17 21:09:06  sm
**      - Triangle support added for intersections, normal computations. So
**        Spline shapes can be computed, too. Now only CSG is missing.
**
**      Revision 1.12  2001/10/17 14:46:02  sm
**      - Adding triangle support.
**      - Renaming b3TriangleShape into b3Triangles and introducing
**        new b3TriangleShape as base class. This results in
**        source file renaming, too.
**      - Fixing soft shadow bug.
**      - Only scene loading background image when activated.
**      - Fixing LDC spline initialization.
**      - Converting Windows paths into right paths on Un*x
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3TriangleShape::b3TriangleShape(b3_size class_size, b3_u32 class_type) :
	b3SimpleShape(class_size, class_type)
{
	m_Vertices    = null;
	m_Triangles   = null;
	m_GridList    = null;
	m_GridCount   = 0;
	m_GridSize    = 0;
	m_VertexCount = 0;
	m_TriaCount   = 0;
	m_xSize       = 0;
	m_ySize       = 0;
	m_Flags       = 0;
}

b3TriangleShape::b3TriangleShape(b3_u32 class_type) :
	b3SimpleShape(sizeof(b3TriangleShape), class_type)
{
	m_Vertices    = null;
	m_Triangles   = null;
	m_GridList    = null;
	m_GridCount   = 0;
	m_GridSize    = 0;
	m_VertexCount = 0;
	m_TriaCount   = 0;
	m_xSize       = 0;
	m_ySize       = 0;
	m_Flags       = 0;
}

b3TriangleShape::b3TriangleShape(b3_u32 *src) :
	b3SimpleShape(src)
{
	m_Vertices    = null;
	m_Triangles   = null;
	m_GridList    = null;
	m_GridCount   = 0;
	m_GridSize    = 0;
	m_VertexCount = 0;
	m_TriaCount   = 0;
	m_xSize       = 0;
	m_ySize       = 0;
	m_Flags       = 0;
}

b3TriangleShape::~b3TriangleShape()
{
	b3FreeTriaRefs();
}

b3_bool b3TriangleShape::b3Init(
	b3_count vertex_count,
	b3_count tria_count,
	b3_res   xSize,
	b3_res   ySize)
{
	if (m_VertexCount != vertex_count)
	{
		b3Item::b3Free(m_Vertices);
		m_VertexCount = vertex_count;
		m_Vertices    = (b3_vertex *)b3Item::b3Alloc(m_VertexCount * sizeof(b3_vertex));
	}

	if (m_TriaCount != tria_count)
	{
		b3Item::b3Free(m_Triangles);
		m_TriaCount = tria_count;
		m_Triangles = (b3_triangle *)b3Item::b3Alloc(m_TriaCount * sizeof(b3_triangle));
	}
	m_xSize = xSize;
	m_ySize = ySize;

	return (m_Vertices != null) && (m_Triangles != null);
}

void b3TriangleShape::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3_index   i;
	b3_vertex *ptr = m_Vertices;

	for (i = 0;i < m_VertexCount;i++)
	{
		b3Vector::b3MatrixMul4D (transformation,&ptr->Point);
		ptr++;
	}
	b3TransformVertices(transformation,is_affine);
}

void b3TriangleShape::b3AddCubicItem (
	b3_count grid,
	b3_index triangle)
{
	b3_index max;

	if((grid >= 0) && (grid < m_GridCount))
	{
		max = m_GridList[grid].b3GetCount();
		if (max > 0)
		{
			if (m_GridList[grid][max - 1] == triangle)
			{
				return;
			}
		}
		m_GridList[grid].b3Add(triangle);
	}
}

void b3TriangleShape::b3SearchCubicItem (
	b3_vector *P1,
	b3_vector *P2,
	b3_vector *P3,
	b3_index   index,
	b3_index   rec,
	b3_count   MaxRec)
{
	b3_index  I1,I2,I3;
	b3_index  x1,y1,z1;
	b3_index  x2,y2,z2;
	b3_index  x3,y3,z3;
	b3_vector P4;

	if (rec > MaxRec)
	{
		return;
	}

	P4.x = (P1->x + P2->x + P3->x) / 3.0;
	P4.y = (P1->y + P2->y + P3->y) / 3.0;
	P4.z = (P1->z + P2->z + P3->z) / 3.0;

	x1 = (b3_index)P1->x; y1 = (b3_index)P1->y; z1 = (b3_index)P1->z;
	x2 = (b3_index)P2->x; y2 = (b3_index)P2->y; z2 = (b3_index)P2->z;
	x3 = (b3_index)P3->x; y3 = (b3_index)P3->y; z3 = (b3_index)P3->z;

	I1 = B3_ABS(x1 - x2) + B3_ABS(y1 - y2) + B3_ABS(z1 - z2);
	I2 = B3_ABS(x2 - x3) + B3_ABS(y2 - y3) + B3_ABS(z2 - z3);
	I3 = B3_ABS(x3 - x1) + B3_ABS(y3 - y1) + B3_ABS(z3 - z1);

	if (I1 <= 1)
	{
		b3AddCubicItem (GRID_INDEX(x1,y1,z1,m_GridSize),index);
		b3AddCubicItem (GRID_INDEX(x2,y2,z2,m_GridSize),index);
	}
	else
	{
		b3SearchCubicItem (P1,P2,&P4,index,rec+1,MaxRec);
	}

	if (I2 <= 1)
	{
		b3AddCubicItem (GRID_INDEX(x2,y2,z2,m_GridSize),index);
		b3AddCubicItem (GRID_INDEX(x3,y3,z3,m_GridSize),index);
	}
	else
	{
		b3SearchCubicItem (P2,P3,&P4,index,rec+1,MaxRec);
	}

	if (I3 <= 1)
	{
		b3AddCubicItem (GRID_INDEX(x3,y3,z3,m_GridSize),index);
		b3AddCubicItem (GRID_INDEX(x1,y1,z1,m_GridSize),index);
	}
	else
	{
		b3SearchCubicItem (P3,P1,&P4,index,rec+1,MaxRec);
	}
}

static b3_s32 b3IntLog3 (b3_u32 value)
{
	b3_s32 Log = -1;

	while (value > 0)
	{
		value = (value + value) / 3;
		Log++;
	}
	return Log;
}

void b3TriangleShape::b3PrepareGridList ()
{
	b3_index  i,MaxRec;
	b3_count  max;
	b3_vector P1,P2,P3;
#ifdef _DEBUG
	b3_count  degenerated = 0;
#endif

	MaxRec = b3IntLog3(m_GridSize) << 1;
	max    = m_GridSize * m_GridSize * m_GridSize;

	if (max > 1)
	{
		for (i = 0;i < m_TriaCount;i++)
		{
			P1.x = (m_Vertices[m_Triangles[i].P1].Point.x - m_Base.x) / m_Size.x;
			P1.y = (m_Vertices[m_Triangles[i].P1].Point.y - m_Base.y) / m_Size.y;
			P1.z = (m_Vertices[m_Triangles[i].P1].Point.z - m_Base.z) / m_Size.z;
			P2.x = (m_Vertices[m_Triangles[i].P2].Point.x - m_Base.x) / m_Size.x;
			P2.y = (m_Vertices[m_Triangles[i].P2].Point.y - m_Base.y) / m_Size.y;
			P2.z = (m_Vertices[m_Triangles[i].P2].Point.z - m_Base.z) / m_Size.z;
			P3.x = (m_Vertices[m_Triangles[i].P3].Point.x - m_Base.x) / m_Size.x;
			P3.y = (m_Vertices[m_Triangles[i].P3].Point.y - m_Base.y) / m_Size.y;
			P3.z = (m_Vertices[m_Triangles[i].P3].Point.z - m_Base.z) / m_Size.z;
			if (b3Vector::b3QuadLength(&m_Triangles[i].Normal) > (b3Scene::epsilon * b3Scene::epsilon))
			{
				b3SearchCubicItem (&P1,&P2,&P3,i,-1,MaxRec);
			}
#ifdef _DEBUG
			else
			{
				degenerated++;
			}
#endif
		}
	}
	else
	{
		for (i = 0;i < m_TriaCount;i++)
		{
			if (b3Vector::b3QuadLength(&m_Triangles[i].Normal) > (b3Scene::epsilon * b3Scene::epsilon))
			{
				b3AddCubicItem(0,i);
			}
#ifdef _DEBUG
			else
			{
				degenerated++;
			}
#endif
		}
	}

#ifdef _DEBUG
	if (degenerated > 0)
	{
		b3PrintF(B3LOG_DEBUG,"%d triangles degenerated.\n",degenerated);
	}
#endif
}

void b3TriangleShape::b3FreeTriaRefs()
{
	if (m_GridList != null)
	{
		delete [] m_GridList;
		m_GridList  = null;
		m_GridCount = 0;
	}
}

b3_bool b3TriangleShape::b3Prepare()
{
	b3_vector   Start,End,disp;
	b3_index    P1,P2,P3,i,max;
	b3_f64      Denom;
	b3_triainfo info;

	if ((m_xSize < 1) || (m_ySize < 1))
	{
		if (m_xSize < 1) m_xSize = 1;
		if (m_ySize < 1) m_ySize = 1;
		B3_ASSERT(b3GetConditionHead()->First == null);
	}

	m_TriaInfos.b3Clear();
	for (i = 0;i < m_TriaCount;i++)
	{
		P1 = m_Triangles[i].P1;		/* Base */
		P2 = m_Triangles[i].P2;		/* Dir1 */
		P3 = m_Triangles[i].P3;		/* Dir2 */

		info.O.x  = m_Vertices[P1].Point.x;
		info.O.y  = m_Vertices[P1].Point.y;
		info.O.z  = m_Vertices[P1].Point.z;

		info.R1.x = m_Vertices[P2].Point.x - m_Vertices[P1].Point.x;
		info.R1.y = m_Vertices[P2].Point.y - m_Vertices[P1].Point.y;
		info.R1.z = m_Vertices[P2].Point.z - m_Vertices[P1].Point.z;

		info.R2.x = m_Vertices[P3].Point.x - m_Vertices[P1].Point.x;
		info.R2.y = m_Vertices[P3].Point.y - m_Vertices[P1].Point.y;
		info.R2.z = m_Vertices[P3].Point.z - m_Vertices[P1].Point.z;

		info.Normal.x = info.R1.y * info.R2.z - info.R1.z * info.R2.y;
		info.Normal.y = info.R1.z * info.R2.x - info.R1.x * info.R2.z;
		info.Normal.z = info.R1.x * info.R2.y - info.R1.y * info.R2.x;

		m_TriaInfos.b3Add(info);

		if ((m_Flags & NORMAL_FACE_VALID)==0)
		{
			disp.x = m_Triangles[i].Normal.x = info.Normal.x;
			disp.y = m_Triangles[i].Normal.y = info.Normal.y;
			disp.z = m_Triangles[i].Normal.z = info.Normal.z;
			if ((m_Flags & NORMAL_VERTEX_VALID)==0)
			{
#ifdef NORMAL_NORMALIZED
				b3Vector::b3Normalize(&disp);
#endif
				b3Vector::b3Add(&disp,&m_Vertices[P1].Normal);
				b3Vector::b3Add(&disp,&m_Vertices[P2].Normal);
				b3Vector::b3Add(&disp,&m_Vertices[P3].Normal);
			}
		}
	}

	b3Vector::b3InitBound(&Start,&End);
	for (i = 0;i < m_VertexCount;i++)
	{
		b3Vector::b3AdjustBound(&m_Vertices[i].Point,&Start,&End);

		if ((m_Flags & NORMAL_VERTEX_VALID)==0)
		{
			b3Vector::b3Normalize(&m_Vertices[i].Normal);
		}
	}

#	ifndef ONE_GRID
	m_GridSize = (b3_count)b3Cubic::b3Cbrt (0.125 * m_TriaCount);
	if (m_GridSize < 1)
	{
		m_GridSize = 1;
	}
#	else
	m_GridSize = 1;
#	endif

	if (((End.x - Start.x) < 0.1) ||
	    ((End.y - Start.y) < 0.1) ||
	    ((End.z - Start.z) < 0.1))
	{
		m_GridSize = 1;
	}
	Start.x -= 0.1f;
	Start.y -= 0.1f;
	Start.z -= 0.1f;
	End.x   += 0.1f;
	End.y   += 0.1f;
	End.z   += 0.1f;

	Denom    = 1.0 / m_GridSize;
	m_Base   = Start;
	m_Size.x = (End.x - Start.x) * Denom;
	m_Size.y = (End.y - Start.y) * Denom;
	m_Size.z = (End.z - Start.z) * Denom;
	b3Vector::b3SetMinimum(&m_Size,b3Scene::epsilon);

	max = m_GridSize * m_GridSize * m_GridSize;
	if (max != m_GridCount)
	{
		b3FreeTriaRefs();
	}
	if (m_GridList == null)
	{
		m_GridList  = new b3Array<b3_index>[max];
		m_GridCount = max;
	}
	else
	{
		for (i = 0;i < max;i++)
		{
			m_GridList[i].b3Clear();
		}
	}
	b3PrepareGridList();
	return b3Shape::b3Prepare();
}
