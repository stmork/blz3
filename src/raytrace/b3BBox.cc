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
#include <float.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.15  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.14  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.13  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.12  2001/08/12 19:47:48  sm
**      - Now having correct orthogonal projection incl. aspect ratio
**
**      Revision 1.11  2001/08/11 20:17:22  sm
**      - Updated OpenGL on Un*x platform.
**      - This was a great day!
**
**      Revision 1.10  2001/08/11 19:59:16  sm
**      - Added orthogonal projection
**
**      Revision 1.9  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.8  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.7  2001/08/06 20:52:56  sm
**      - Some CR/LF removal
**
**      Revision 1.6  2001/08/06 19:58:58  sm
**      - Drawing area - the first shape we can see with OpenGL
**
**      Revision 1.5  2001/08/06 16:35:35  sm
**      - Drawing first area
**
**      Revision 1.4  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
**      Revision 1.3  2001/08/02 15:21:54  sm
**      - Some minor changes
**
**      Revision 1.2  2001/07/02 16:09:46  sm
**      - Added bounding box reorganization.
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
	b3Item::b3Register(&b3BBox::b3Init,&b3BBox::b3Init,CLASS_BBOX,true);
}
                                                                                                                     
b3BBox::b3BBox(b3_u32 class_type) : b3Item(sizeof(b3BBox),class_type)
{
}

b3BBox::b3BBox(b3_u32 *src) : b3Item(src)
{
	b3_index diff;

	Type = b3InitInt();
	b3InitVector(&Base);
	b3InitVector(&Size);
	b3InitNOP();
	b3InitMatrix(&Matrix);

	diff = (size >> 2) - parseIndex;
	if (diff < (B3_BOXSTRINGLEN >> 2))
	{
		b3InitString(BoxName,diff);
		BoxURL[0] = 0;
	}
	else
	{
		b3InitString(BoxName,B3_BOXSTRINGLEN);
		b3InitString(BoxURL, B3_BOXSTRINGLEN);
	}

}

void b3BBox::b3Dump(b3_count level)
{
	b3Item *bbox;

	level = b3GetClassType() & 0xffff;
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL,"Object %s (level %d)\n",BoxName,level);

	B3_FOR_BASE(&heads[1],bbox)
	{
		bbox->b3Dump(level);
	}
}

void b3BBox::b3Reorg(
	b3Base<b3Item> *depot,
	b3Base<b3Item> *base,
	b3_count        level,
	b3_count        rec)
{
	b3_count        new_level;
	b3BBox         *bbox;
	b3Base<b3Item> *sub_base;

	while (bbox = (b3BBox *)depot->First)
	{
		new_level = bbox->b3GetClassType() & 0xffff;
		if (new_level < level)
		{
			return;
		}
		if (new_level == level)
		{
			depot->b3Remove(bbox);
			base->b3Append(bbox);
			sub_base = &bbox->heads[1];
		}
		else
		{
			b3Reorg(depot,sub_base,new_level,rec + 1);
		}
	}
}

void b3BBox::b3AllocVertices(b3RenderContext *context)
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Shape        *shape;

	VertexCount =  8;
	GridCount   = 12;
	PolyCount   =  0;

#ifdef BLZ3_USE_OPENGL
	glVertices = bbox_vertices;
	glNormals  = bbox_normals;
	glGrids    = bbox_indices;
	glPolygons = null;
#endif

	B3_FOR_BASE(&heads[1],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3AllocVertices(context);

	}
	B3_FOR_BASE(&heads[0],item)
	{
		shape = (b3Shape *)item;
		shape->b3AllocVertices(context);
	}
}

void b3BBox::b3FreeVertices()
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Shape        *shape;

#ifdef BLZ3_USE_OPENGL
	glVertices = null;
	glNormals  = null;
	glGrids    = null;
	glPolygons = null;
#endif

	B3_FOR_BASE(&heads[0],item)
	{
		shape = (b3Shape *)item;
		shape->b3FreeVertices();
	}
	B3_FOR_BASE(&heads[1],item)
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

	bbox_vertices[i++] = Base.x;
	bbox_vertices[i++] = Base.y;
	bbox_vertices[i++] = Base.z;

	bbox_vertices[i++] = Base.x;
	bbox_vertices[i++] = Base.y;
	bbox_vertices[i++] = Base.z + Size.z;

	bbox_vertices[i++] = Base.x + Size.x;
	bbox_vertices[i++] = Base.y;
	bbox_vertices[i++] = Base.z + Size.z;

	bbox_vertices[i++] = Base.x + Size.x;
	bbox_vertices[i++] = Base.y;
	bbox_vertices[i++] = Base.z;

	bbox_vertices[i++] = Base.x;
	bbox_vertices[i++] = Base.y + Size.y;
	bbox_vertices[i++] = Base.z;

	bbox_vertices[i++] = Base.x;
	bbox_vertices[i++] = Base.y + Size.y;
	bbox_vertices[i++] = Base.z + Size.z;

	bbox_vertices[i++] = Base.x + Size.x;
	bbox_vertices[i++] = Base.y + Size.y;
	bbox_vertices[i++] = Base.z + Size.z;

	bbox_vertices[i++] = Base.x + Size.x;
	bbox_vertices[i++] = Base.y + Size.y;
	bbox_vertices[i++] = Base.z;

	glComputed = true;
#endif
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
	b3Item         *item;
	b3BBox         *bbox;
	b3Shape        *shape;

	// Draw this
	b3RenderObject::b3Draw();

	// Draw our shapes
	B3_FOR_BASE(&heads[0],item)
	{
		shape = (b3Shape *)item;
		shape->b3Draw();
	}

	// Draw subsequent BBoxes
	B3_FOR_BASE(&heads[1],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw();

	}
}

b3_bool b3BBox::b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance)
{
	b3_vector subLower;
	b3_vector subUpper;
	b3Item    *item;
	b3BBox    *bbox;
	b3Shape   *shape;
	b3_bool    result = false;

	subLower.x =  FLT_MAX;
	subLower.y =  FLT_MAX;
	subLower.z =  FLT_MAX;
	subUpper.x = -FLT_MAX;
	subUpper.y = -FLT_MAX;
	subUpper.z = -FLT_MAX;

	B3_FOR_BASE(&heads[0],item)
	{
		shape   = (b3Shape *)item;
		result |= shape->b3ComputeBounds(&subLower,&subUpper);
	}

	if (result)
	{
		// Use fresh data
		Size.x      = (subUpper.x - subLower.x) * tolerance * 0.5;
		Size.y      = (subUpper.y - subLower.y) * tolerance * 0.5;
		Size.z      = (subUpper.z - subLower.z) * tolerance * 0.5;
		subLower.x -= Size.x;
		subLower.y -= Size.y;
		subLower.z -= Size.z;
		subUpper.x += Size.x;
		subUpper.y += Size.y;
		subUpper.z += Size.z;
	}
	else
	{
		// Use predefined data
		subLower   = Base;
		subUpper.x = Base.x + Size.x;
		subUpper.y = Base.y + Size.y;
		subUpper.z = Base.z + Size.z;
		result     = true;
	}

	B3_FOR_BASE(&heads[1],item)
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
		Size.x = subUpper.x - subLower.x;
		Size.y = subUpper.y - subLower.y;
		Size.z = subUpper.z - subLower.z;
		Base   = subLower;
	}

	return result;
}

void b3Scene::b3Reorg()
{
	b3Base<b3Item>  depot;
	b3Link<b3Item> *first;
	b3_count        level;

	depot = heads[0];
	heads[0].b3InitBase(CLASS_BBOX);
	if (first = depot.First)
	{
		level = first->b3GetClassType() & 0xffff;
		b3BBox::b3Reorg(&depot,&heads[0],level,1);
	}
}

void b3Scene::b3Draw()
{
	b3Item         *item;
	b3BBox         *bbox;

	B3_FOR_BASE(&heads[0],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw();
	}
}

b3_bool b3Scene::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result = false;

	lower->x =  FLT_MAX;
	lower->y =  FLT_MAX;
	lower->z =  FLT_MAX;
	upper->x = -FLT_MAX;
	upper->y = -FLT_MAX;
	upper->z = -FLT_MAX;

	B3_FOR_BASE(&heads[0],item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(lower,upper,BBoxOverSize);
	}
	return result;
}

void b3Scene::b3AllocVertices(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;

	B3_FOR_BASE(&heads[0],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3AllocVertices(context);
	}
}
 
