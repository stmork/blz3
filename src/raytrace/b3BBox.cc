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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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
static GLushort indices[12 * 2] =
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

void b3BBox::b3Draw()
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Shape        *shape;

#ifdef BLZ3_USE_OPENGL
	glColor3f(0.5f,0.5f,0.5f);
	b3RenderObject::b3Draw();
#endif
	B3_FOR_BASE(&heads[1],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw();

	}
#ifdef BLZ3_USE_OPENGL
	glColor3f(0.2f,0.2f,0.2f);
#endif
	B3_FOR_BASE(&heads[0],item)
	{
		shape = (b3Shape *)item;
		shape->b3Draw();
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

	glVertices = vertices;
	glNormals  = null;
	glGrids    = indices;
	glPolygons = null;

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

	glVertices = null;
	glGrids    = null;
	glPolygons = null;

	B3_FOR_BASE(&heads[1],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3FreeVertices();

	}
	B3_FOR_BASE(&heads[0],item)
	{
		shape = (b3Shape *)item;
		shape->b3FreeVertices();
	}

	b3RenderObject::b3FreeVertices();
}

void b3BBox::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3_index        i = 0;

	vertices[i++] = Base.x;
	vertices[i++] = Base.y;
	vertices[i++] = Base.z;

	vertices[i++] = Base.x;
	vertices[i++] = Base.y;
	vertices[i++] = Base.z + Size.z;

	vertices[i++] = Base.x + Size.x;
	vertices[i++] = Base.y;
	vertices[i++] = Base.z + Size.z;

	vertices[i++] = Base.x + Size.x;
	vertices[i++] = Base.y;
	vertices[i++] = Base.z;

	vertices[i++] = Base.x;
	vertices[i++] = Base.y + Size.y;
	vertices[i++] = Base.z;

	vertices[i++] = Base.x;
	vertices[i++] = Base.y + Size.y;
	vertices[i++] = Base.z + Size.z;

	vertices[i++] = Base.x + Size.x;
	vertices[i++] = Base.y + Size.y;
	vertices[i++] = Base.z + Size.z;

	vertices[i++] = Base.x + Size.x;
	vertices[i++] = Base.y + Size.y;
	vertices[i++] = Base.z;
#endif
	glComputed = true;
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

#ifdef BLZ3_USE_OPENGL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT); 
	glEnableClientState(GL_VERTEX_ARRAY);
#endif

	B3_FOR_BASE(&heads[0],item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw();
	}
#ifdef BLZ3_USE_OPENGL
	glPopMatrix();
#endif
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
 
