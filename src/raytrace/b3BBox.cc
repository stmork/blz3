/*
**
**      $Filename:      b3BBox.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing bounding boxes
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
**	Revision 1.56  2002/07/27 18:51:31  sm
**	- Drawing changed to glInterleavedArrays(). This means that
**	  extra normal and texture arrays are omitted. This simplifies
**	  correct programming, too.
**
**	Revision 1.55  2002/07/25 13:22:32  sm
**	- Introducing spot light
**	- Optimized light settings when drawing
**	- Further try of stencil maps
**	
**	Revision 1.54  2002/07/21 17:02:36  sm
**	- Finished advanced color mix support (correct Phong/Mork shading)
**	- Added first texture mapping support. Further development on
**	  Windows now...
**	
**	Revision 1.53  2002/03/13 19:01:59  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.52  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**	Revision 1.51  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.50  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**	Revision 1.49  2002/03/02 19:52:39  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.48  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.47  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**	Revision 1.46  2002/02/19 16:26:49  sm
**	- Further CSG interval computing cleanup done.
**	
**	Revision 1.45  2002/02/18 17:50:31  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.44  2002/02/17 21:25:06  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.43  2002/01/21 16:56:46  sm
**	- Showing splash dialog only in release version.
**	- Prepared shape icons.
**	
**	Revision 1.42  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.41  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.40  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.39  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**	Revision 1.38  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**	
**	Revision 1.37  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.36  2002/01/08 16:04:08  sm
**	- New copy dialog added
**	- Merge with daily work
**	
**	Revision 1.35  2002/01/07 16:18:51  sm
**	- Added b3Item clone
**	- Added Drag & Drop
**	
**	Revision 1.34  2002/01/06 21:38:18  sm
**	- Nasty Un CR/LF
**	- b3Dir fix. Not tested, yet!
**	- make count/make count_all update
**	
**	Revision 1.33  2002/01/05 22:17:47  sm
**	- Recomputing bounding boxes correctly
**	- Found key input bug: The accelerator are the problem
**	- Code cleanup
**	
**	Revision 1.32  2002/01/04 17:53:54  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.31  2002/01/03 19:07:27  sm
**	- Cleaned up cut/paste
**	
**	Revision 1.30  2002/01/03 15:50:14  sm
**	- Added cut/copy/paste
**	
**	Revision 1.29  2002/01/02 15:48:37  sm
**	- Added automated expand/collapse to hierarchy tree.
**	
**	Revision 1.28  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.27  2001/12/30 16:54:15  sm
**	- Inserted safe b3Write() into Lines III
**	- Fixed b3World saving: b3StoreXXX() methods must ensure
**	  buffer before the buffer itself is used.
**	- Extended b3Shape format with shape activation flag. Nice: The
**	  new data structures don't confuse the old Lines II/Blizzard II and
**	  even stores these new values.
**	
**	Revision 1.26  2001/12/30 14:16:57  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.25  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.24  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.23  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
**	Revision 1.22  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	Revision 1.21  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.20  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.19  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.18  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.17  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.16  2001/09/01 15:54:54  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**
**	Revision 1.15  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.14  2001/08/16 14:41:24  sm
**	- Some more shading shapes added (only BSPline shapes are missing)
**
**	Revision 1.13  2001/08/14 15:37:50  sm
**	- Made some cleanups when OpenGL isn't available.
**
**	Revision 1.12  2001/08/12 19:47:48  sm
**	- Now having correct orthogonal projection incl. aspect ratio
**
**	Revision 1.11  2001/08/11 20:17:22  sm
**	- Updated OpenGL on Un*x platform.
**	- This was a great day!
**
**	Revision 1.10  2001/08/11 19:59:16  sm
**	- Added orthogonal projection
**
**	Revision 1.9  2001/08/10 15:14:36  sm
**	- Now having all shapes implemented for drawing lines.
**
**	Revision 1.8  2001/08/08 20:12:59  sm
**	- Fixing some makefiles
**	- introducing check/BlzDump (BlzDump moved from tools)
**	- Some further line drawing added
**	- b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**	  b3RenderObject.
**
**	Revision 1.7  2001/08/06 20:52:56  sm
**	- Some CR/LF removal
**
**	Revision 1.6  2001/08/06 19:58:58  sm
**	- Drawing area - the first shape we can see with OpenGL
**
**	Revision 1.5  2001/08/06 16:35:35  sm
**	- Drawing first area
**
**	Revision 1.4  2001/08/02 15:37:17  sm
**	- Now we are able to draw Blizzard Scenes with OpenGL.
**
**	Revision 1.3  2001/08/02 15:21:54  sm
**	- Some minor changes
**
**	Revision 1.2  2001/07/02 16:09:46  sm
**	- Added bounding box reorganization.
**
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

#ifdef BLZ3_USE_OPENGL
static GLushort bbox_indices[12 * 2] =
{
	0,1,
	1,2,
	2,3,
	3,0,
	4,5,
	5,6,
	6,7,
	7,4,
	0,4,
	1,5,
	2,6,
	3,7
};
#endif

void b3InitBBox::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering bounding boxes...\n");
	b3Item::b3Register(&b3BBox::b3StaticInit,&b3BBox::b3StaticInit,CLASS_BBOX,true);
}
                                                                                                                     
b3BBox::b3BBox(b3_u32 class_type) : b3Item(sizeof(b3BBox),class_type)
{
	b3AllocHeads(2);
	m_Heads[0].b3InitBase(CLASS_SHAPE);
	m_Heads[1].b3InitBase(CLASS_BBOX);

	b3MatrixUnit(&m_Matrix);
	m_Type       = 0;
	m_DimBase.x  = 0;
	m_DimBase.y  = 0;
	m_DimBase.z  = 0;
	m_DimSize.x  = 0;
	m_DimSize.y  = 0;
	m_DimSize.z  = 0;
	m_BoxName[0] = 0;
	m_BoxURL[0]  = 0;

	m_CSGIntersectionCount = 0;
	m_ShapeCount           = 0;
}

b3BBox::b3BBox(b3_u32 *src) : b3Item(src)
{
	b3_size diff;

	m_Type = b3InitInt();
	b3InitVector(&m_DimBase);
	b3InitVector(&m_DimSize);
	b3InitNOP();
	b3InitMatrix(&m_Matrix);

	diff = m_ItemSize - (b3_size)(m_ParseIndex << 2);
	if (diff < B3_BOXSTRINGLEN)
	{
		b3InitString(m_BoxName,diff);
		m_BoxURL[0] = 0;
	}
	else
	{
		b3InitString(m_BoxName,B3_BOXSTRINGLEN);
		b3InitString(m_BoxURL, B3_BOXSTRINGLEN);
	}

	m_CSGIntersectionCount = 0;
	m_ShapeCount           = 0;
}

void b3BBox::b3Write()
{
	b3StoreInt(m_Type);
	b3StoreVector(&m_DimBase);
	b3StoreVector(&m_DimSize);
	b3StoreNull(); // This is Custom
	b3StoreMatrix(&m_Matrix);

	b3StoreString(m_BoxName,B3_BOXSTRINGLEN);
	b3StoreString(m_BoxURL, B3_BOXSTRINGLEN);
}

b3Base<b3Item> *b3BBox::b3GetShapeHead()
{
	return &m_Heads[0];
}

b3Base<b3Item> *b3BBox::b3GetBBoxHead()
{
	return &m_Heads[1];
}

b3_bool b3BBox::b3Prepare()
{
	b3Item     *item;
	b3Shape    *shape;
	b3CSGShape *csgShape;
	b3BBox     *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if (!bbox->b3Prepare())
		{
			return false;
		}
	}

	m_ShapeCount = 0;
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		if (!shape->b3Prepare())
		{
			b3PrintF(B3LOG_NORMAL,
				"Shape not initialized [%s].\n",
				b3GetName());
			return false;
		}

		m_ShapeCount++;
	}

	m_CSGIntersectionCount = 0;
	if (b3GetShapeHead()->b3GetClass() == CLASS_CSG)
	{
		B3_FOR_BASE(b3GetShapeHead(),item)
		{
			csgShape = (b3CSGShape *)item;
			m_CSGIntersectionCount += csgShape->b3GetMaxIntersections();
		}
		if ((m_CSGIntersectionCount >= B3_MAX_CSG_INTERSECTIONS_PER_BBOX) ||
		    (m_ShapeCount >= B3_MAX_CSG_SHAPES_PER_BBOX))
		{
			b3PrintF(B3LOG_NORMAL,
				"Not enough static memory for CSG computation [%s].\n",
				b3GetName());
			b3PrintF(B3LOG_NORMAL,
				"Found %d shapes with %d max. possible intersections.\n",
				m_ShapeCount,m_CSGIntersectionCount);
    		return false;
    	}
    }

	return true;
}

char *b3BBox::b3GetName()
{
	return m_BoxName;
}

void b3BBox::b3Dump(b3_count level)
{
	b3Item *bbox;

	level = b3GetClassType() & 0xffff;
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL,"Object %s (level %d)\n",m_BoxName,level);

	B3_FOR_BASE(b3GetBBoxHead(),bbox)
	{
		bbox->b3Dump(level);
	}
}

void b3BBox::b3Reorg(
	b3Base<b3Item> *depot,
	b3Base<b3Item> *base,
	b3_count        level,
	b3_count        rec,
	b3Item         *insert_after)
{
	b3_count        new_level;
	b3BBox         *bbox;
	b3Base<b3Item> *sub_base = null;

	while ((bbox = (b3BBox *)depot->First) != null)
	{
		new_level = bbox->b3GetClassType() & 0xffff;
		if (new_level < level)
		{
			return;
		}
		if (new_level == level)
		{
			depot->b3Remove(bbox);
			insert_after != null ? base->b3Insert(insert_after,bbox) : base->b3Append(bbox);
			sub_base = bbox->b3GetBBoxHead();
		}
		else
		{
			B3_ASSERT(sub_base != null);
			b3Reorg(depot,sub_base,new_level,rec + 1);
		}
	}
}

void b3BBox::b3Recount(b3Base<b3Item> *base,b3_count level)
{
	b3Item *item;
	b3BBox *bbox;

	B3_FOR_BASE(base,item)
	{
		bbox = (b3BBox *)item;
		b3Recount(bbox->b3GetBBoxHead(),level + 1);
		bbox->ClassType &= 0xffff0000;
		bbox->ClassType |= level;
	}
}

void b3Scene::b3Reorg()
{
	b3Base<b3Item>  depot;
	b3Link<b3Item> *first;
	b3_count        level;

	depot = *b3GetBBoxHead();
	b3GetBBoxHead()->b3InitBase(CLASS_BBOX);
	if ((first = depot.First) != null)
	{
		level = first->b3GetClassType() & 0xffff;
		b3BBox::b3Reorg(&depot,b3GetBBoxHead(),level,1);
	}
	b3BBox::b3Recount(b3GetBBoxHead());
}

void b3BBox::b3AllocVertices(b3RenderContext *context)
{
	b3Item              *item;
	b3BBox              *bbox;
	b3ShapeRenderObject *shape;

	glVertexCount =  8;
	glGridCount   = 12;
	glPolyCount   =  0;

#ifdef BLZ3_USE_OPENGL
	glVertex   = bbox_vertex;
	glGrids    = bbox_indices;
	glPolygons = null;

	memset(bbox_vertex,0,sizeof(bbox_vertex));
#endif

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3AllocVertices(context);
		bbox->b3AddCount(context);
	}
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3ShapeRenderObject *)item;
		shape->b3AllocVertices(context);
		shape->b3AddCount(context);
	}
}

void b3BBox::b3FreeVertices()
{
	b3Item              *item;
	b3BBox              *bbox;
	b3ShapeRenderObject *shape;

#ifdef BLZ3_USE_OPENGL
	glVertex   = null;
	glGrids    = null;
	glPolygons = null;
#endif

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3ShapeRenderObject *)item;
		shape->b3FreeVertices();
	}
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3FreeVertices();

	}

	b3RenderObject::b3FreeVertices();
}

void b3BBox::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3_index        i = 0;

	bbox_vertex[0].v.x = m_DimBase.x;
	bbox_vertex[0].v.y = m_DimBase.y;
	bbox_vertex[0].v.z = m_DimBase.z;

	bbox_vertex[1].v.x = m_DimBase.x;
	bbox_vertex[1].v.y = m_DimBase.y;
	bbox_vertex[1].v.z = m_DimBase.z + m_DimSize.z;

	bbox_vertex[2].v.x = m_DimBase.x + m_DimSize.x;
	bbox_vertex[2].v.y = m_DimBase.y;
	bbox_vertex[2].v.z = m_DimBase.z + m_DimSize.z;

	bbox_vertex[3].v.x = m_DimBase.x + m_DimSize.x;
	bbox_vertex[3].v.y = m_DimBase.y;
	bbox_vertex[3].v.z = m_DimBase.z;

	bbox_vertex[4].v.x = m_DimBase.x;
	bbox_vertex[4].v.y = m_DimBase.y + m_DimSize.y;
	bbox_vertex[4].v.z = m_DimBase.z;

	bbox_vertex[5].v.x = m_DimBase.x;
	bbox_vertex[5].v.y = m_DimBase.y + m_DimSize.y;
	bbox_vertex[5].v.z = m_DimBase.z + m_DimSize.z;

	bbox_vertex[6].v.x = m_DimBase.x + m_DimSize.x;
	bbox_vertex[6].v.y = m_DimBase.y + m_DimSize.y;
	bbox_vertex[6].v.z = m_DimBase.z + m_DimSize.z;

	bbox_vertex[7].v.x = m_DimBase.x + m_DimSize.x;
	bbox_vertex[7].v.y = m_DimBase.y + m_DimSize.y;
	bbox_vertex[7].v.z = m_DimBase.z;

	glVertexCount = 8;
	glComputed    = true;
#endif
}

void b3BBox::b3ComputeNormals(b3_bool normalize)
{
}

void b3BBox::b3GetGridColor(b3_color *color)
{
	color->r = 0.5;
	color->g = 0.5;
	color->b = 0.5;
	color->a = 0.0;
}

void b3BBox::b3Draw()
{
	b3Item              *item;
	b3BBox              *bbox;
	b3ShapeRenderObject *shape;

	// Draw this
	b3RenderObject::b3Draw();

	// Draw our shapes
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3ShapeRenderObject *)item;
		shape->b3Draw();
	}

	// Draw subsequent BBoxes
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw();
	}
}

b3_bool b3BBox::b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance)
{
	b3Item              *item;
	b3BBox              *bbox;
	b3ShapeRenderObject *shape;
	b3_vector            subLower;
	b3_vector            subUpper;
	b3_bool              result = false;

	b3Vector::b3InitBound(&subLower,&subUpper);
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape   = (b3ShapeRenderObject *)item;
		result |= shape->b3ComputeBounds(&subLower,&subUpper);
	}

	if (result)
	{
		// Use fresh data
		m_DimSize.x  = (subUpper.x - subLower.x) * tolerance * 0.5;
		m_DimSize.y  = (subUpper.y - subLower.y) * tolerance * 0.5;
		m_DimSize.z  = (subUpper.z - subLower.z) * tolerance * 0.5;
		subLower.x  -= m_DimSize.x;
		subLower.y  -= m_DimSize.y;
		subLower.z  -= m_DimSize.z;
		subUpper.x  += m_DimSize.x;
		subUpper.y  += m_DimSize.y;
		subUpper.z  += m_DimSize.z;
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(&subLower,&subUpper,tolerance);
	}

	// Check lower bound
	if (subLower.x < lower->x) lower->x = subLower.x;
	if (subLower.y < lower->y) lower->y = subLower.y;
	if (subLower.z < lower->z) lower->z = subLower.z;

	// Check upper bound
	if (subUpper.x > upper->x) upper->x = subUpper.x;
	if (subUpper.y > upper->y) upper->y = subUpper.y;
	if (subUpper.z > upper->z) upper->z = subUpper.z;

	// Compute bounds of thos BBox
	if (result)
	{
		m_DimSize.x = subUpper.x - subLower.x;
		m_DimSize.y = subUpper.y - subLower.y;
		m_DimSize.z = subUpper.z - subLower.z;
		m_DimBase   = subLower;
	}

	return result;
}

b3_bool b3Scene::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result = false;

	b3Vector::b3InitBound(lower,upper);

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(lower,upper,m_BBoxOverSize);
	}
	return result;
}

b3_count b3BBox::b3Count()
{
	b3_count count = 1;

	b3Item *item;
	b3BBox *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox   = (b3BBox *)item;
		count += bbox->b3Count();
	}
	return count;
}

b3_bool b3BBox::b3Transform(b3_matrix *transformation,b3_bool force_action)
{
	b3Item  *item;
	b3Shape *shape;
	b3BBox  *bbox;
	b3_bool  transformed = false;

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		if (force_action || shape->b3IsActive())
		{
			shape->b3Transform(transformation);
			transformed = true;
		}
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if(bbox->b3Transform(transformation,force_action))
		{
			transformed = true;
		}
	}

	if (force_action || b3IsActive())
	{
		b3MatrixMMul(&m_Matrix,transformation,&m_Matrix);
		transformed = true;
	}

	if (transformed)
	{
		b3Recompute();
	}

	return transformed;
}

void b3Scene::b3Transform(b3_matrix *transformation,b3_bool force_action)
{
	b3Item *item;
	b3BBox *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Transform(transformation,force_action);
	}
}

char *b3Scene::b3GetName()
{
	return m_Filename;
}

void b3Scene::b3Draw()
{
	b3Item *item;
	b3BBox *bbox;

	b3PrintT("OpenGL - Drawing start");
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw();
	}
	b3PrintT("OpenGL - Drawing end");
}

void b3Scene::b3AllocVertices(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;

	context->glVertexCount = 0;
	context->glPolyCount   = 0;
	context->glGridCount   = 0;
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3AllocVertices(context);
		bbox->b3AddCount(context);
	}
}

void b3BBox::b3Activate(b3_bool activate)
{
	b3Item  *item;
	b3Shape *shape;
	b3BBox  *bbox;

	if (activate) m_Type |=   BBF_ACTIVE;
	else          m_Type &= (~BBF_ACTIVE);

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Activate(activate);
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Activate(activate);
	}
}

void b3Scene::b3Activate(b3_bool activate)
{
	b3Item  *item;
	b3BBox  *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Activate(activate);
	}
}

b3_bool b3BBox::b3IsActive()
{
	return (m_Type & BBF_ACTIVE) != 0;
}

void b3BBox::b3Expand(b3_bool expand)
{
	if (expand) m_Type |=   BBF_EXPANDED;
	else        m_Type &= (~BBF_EXPANDED);
}

b3_bool b3BBox::b3IsExpanded()
{
	return (m_Type & BBF_EXPANDED) != 0;
}

b3_count b3Scene::b3GetBBoxCount()
{
	b3Item   *item;
	b3BBox   *bbox;
	b3_count  count = 0;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		count += bbox->b3Count();
	}
	return count;
}

b3_bool b3BBox::b3FindBBox(b3Base<b3Item> *base,b3BBox *search)
{
	b3Item  *item;
	b3BBox  *bbox;

	B3_FOR_BASE(base,item)
	{
		bbox = (b3BBox *)item;
		if (bbox == search)
		{
			return true;
		}
		if (b3FindBBox(bbox->b3GetBBoxHead(),search))
		{
			return true;
		}
	}
	return false;
}

/*************************************************************************
**                                                                      **
**                        Find BBox where bbox belongs to               **
**                                                                      **
*************************************************************************/

b3Base<b3Item> *b3BBox::b3FindBBoxHead(b3BBox *bbox)
{
	b3Item         *item;
	b3BBox         *inc_bbox;
	b3Base<b3Item> *base,*result;

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		if (item == bbox)
		{
			return base;
		}

		inc_bbox = (b3BBox *)item;
		result   = inc_bbox->b3FindBBoxHead(bbox);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

b3Base<b3Item> *b3Scene::b3FindBBoxHead(b3BBox *bbox)
{
	b3Item         *item;
	b3BBox         *inc_bbox;
	b3Base<b3Item> *base,*result;

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		if (item == bbox)
		{
			return base;
		}

		inc_bbox = (b3BBox *)item;
		result   = inc_bbox->b3FindBBoxHead(bbox);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

/*************************************************************************
**                                                                      **
**                        Find BBox where shape belongs to              **
**                                                                      **
*************************************************************************/

b3BBox *b3BBox::b3FindParentBBox(b3Shape *shape)
{
	b3Item         *item;
	b3BBox         *bbox,*result;
	b3Base<b3Item> *base;

	base = b3GetShapeHead();
	B3_FOR_BASE(base,item)
	{
		if (item == shape)
		{
			return this;
		}
	}

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		bbox   = (b3BBox *)item;
		result = bbox->b3FindParentBBox(shape);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

b3BBox *b3Scene::b3FindParentBBox(b3Shape *shape)
{
	b3Item         *item;
	b3BBox         *bbox,*result;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox   = (b3BBox *)item;
		result = bbox->b3FindParentBBox(shape);
		if (result != null)
		{
			return result;
		}
	}
	return null;
}

b3_bool b3BBox::b3BacktraceRecompute(b3BBox *search)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result;

	// Found?
	if (search == this)
	{
		b3Recompute();
		return true;
	}

	// Search children
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox   = (b3BBox *)item;
		result = bbox->b3BacktraceRecompute(search);
		if (result)
		{
			bbox->b3Recompute();
			return result;
		}
	}
	return false;
}

b3_bool b3Scene::b3BacktraceRecompute(b3BBox *search)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if (bbox == search)
		{
			bbox->b3Recompute();
			return true;
		}

		if ((result = bbox->b3BacktraceRecompute(search)) != null)
		{
			bbox->b3Recompute();
			return result;
		}
	}
	return false;
}
