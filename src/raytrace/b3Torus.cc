/*
**
**      $Filename:      b3Torus.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing torus
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
**	$Log$
**	Revision 1.11  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**
**	Revision 1.10  2001/09/22 16:19:53  sm
**	- Adding basic shape intersection routines
**
**	Revision 1.9  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.8  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.7  2001/08/16 14:41:24  sm
**	- Some more shading shapes added (only BSPline shapes are missing)
**
**	Revision 1.6  2001/08/11 16:29:08  sm
**	- Nasty UnCR done
**	- Compiling but not running OpenGL under Unix
**
**	Revision 1.5  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	Revision 1.4  2001/08/10 15:14:37  sm
**	- Now having all shapes implemented for drawing lines.
**
**	Revision 1.3  2001/08/09 15:27:34  sm
**	- Following shapes are newly supported now:
**	  o disk
**	  o cylinder
**	  o cone
**	  o ellipsoid
**	  o torus
**	  o triangles
**	- Done some makefile fixes
**	- Everything is Windozable
**
**	Revision 1.2  2001/08/08 20:12:59  sm
**	- Fixing some makefiles
**	- introducing check/BlzDump (BlzDump moved from tools)
**	- Some further line drawing added
**	- b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**	  b3RenderObject.
**
**	Revision 1.1  2001/08/06 15:26:00  sm
**	- Splitted shapes into their own files
**	- Some preparations for shapde drawing.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Torus::b3Torus(b3_u32 class_type) : b3RenderShape(sizeof(b3Torus), class_type)
{
}

b3Torus::b3Torus(b3_u32 *src) : b3RenderShape(src)
{
	b3_f64 denom,scale;

	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);
	b3InitFloat(); // This is lSize
	b3InitFloat(); // This is Denom
	b3InitFloat(); // This is DirLen[0]
	b3InitFloat(); // This is DirLen[1]
	b3InitFloat(); // This is DirLen[2]
	m_aRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir1 and Dir2
	m_bRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir3

	denom = b3Length (&m_Dir1);
	if (denom != 0)
	{
		scale     = denom;
		m_Dir1.x /= denom;
		m_Dir1.y /= denom;
		m_Dir1.z /= denom;
	}
	else
	{
		scale = 1;
	}

	denom = b3Length (&m_Dir2);
	if (denom != 0)
	{
		scale    += denom;
		m_Dir2.x /= denom;
		m_Dir2.y /= denom;
		m_Dir2.z /= denom;
	}
	else
	{
		scale += 1;
	}
	m_aRad *= (scale * 0.5);

	denom = b3Length (&m_Dir3);
	if (denom != 0)
	{
		m_bRad    = denom;
		m_Dir3.x /= denom;
		m_Dir3.y /= denom;
		m_Dir3.z /= denom;
	}

	m_aQuad = m_aRad * m_aRad;
	m_bQuad = m_bRad * m_bRad;
}

void b3Torus::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount = (SinCosSteps + 2) * (SinCosSteps + 2);
}

void b3Torus::b3ComputeVertices()
{
	b3ComputeTorusVertices(m_Base,m_Dir1,m_Dir2,m_Dir3,m_aRad,m_bRad);
}

void b3Torus::b3ComputeIndices()
{
	b3ComputeTorusIndices();
}

void b3Torus::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3MatrixVMul (transformation,&m_Dir3,&m_Dir3,false);
	b3Recompute();
}
