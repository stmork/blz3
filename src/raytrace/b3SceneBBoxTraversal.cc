/*
**
**	$Filename:	b3SceneBBoxTraversal.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Traversing scene and bounding boxes.
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/12/30 16:27:39  sm
**	- Removed assertion problem when starting Lines III: The
**	  image list were initialized twice due to double calling
**	  OnInitDialog() of CDialogBar. The CDialogBar::Create()
**	  calls OnInitDialog() automatically sinde MFC 7
**	- Removed many global references from raytrace and base lib
**	- Fixed ticket no. 29: The b3RenderObject::b3Recompute
**	  method checks the vertex maintainer against a null pointer.
**
**	Revision 1.1  2004/12/14 07:40:44  smork
**	- Put scene/bbox traversion methods into their own source file.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Reorganizing hierarchy from plain             **
**                        BBox stream.                                  **
**                                                                      **
*************************************************************************/

void b3Scene::b3Reorg()
{
	b3Base<b3Item>  depot;
	b3Base<b3Item> *base;
	b3Link<b3Item> *first;
	b3_count        level;

	base  =  b3GetBBoxHead();
	depot = *base;
	base->b3InitBase(CLASS_BBOX);
	if ((first = depot.First) != null)
	{
		level = first->b3GetType();
		b3BBox::b3Reorg(&depot,base,level,1);
	}
	b3BBox::b3Recount(base);
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
		new_level = bbox->b3GetType();
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

/*************************************************************************
**                                                                      **
**                        Update depth level into bboxes                **
**                                                                      **
*************************************************************************/

void b3Scene::b3Recount()
{
	b3BBox::b3Recount(b3GetBBoxHead());
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

/*************************************************************************
**                                                                      **
**                        Counting bboxes                               **
**                                                                      **
*************************************************************************/

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

/*************************************************************************
**                                                                      **
**                        Allocate vertices of bboxes/shapes            **
**                                                                      **
*************************************************************************/

// FIXME: This should renamed into b3SetupVertexMemory
void b3Scene::b3SetupVertexMemory(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;

	context->glVertexCount = 0;
	context->glPolyCount   = 0;
	context->glGridCount   = 0;
	context->glTextureSize = 0;
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3SetupVertexMemory(context);
		bbox->b3AddCount(context);
	}
}

void b3BBox::b3SetupVertexMemory(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3Shape *shape;

	b3RenderObject::b3SetupVertexMemory(context);

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3SetupVertexMemory(context);
		bbox->b3AddCount(context);
	}
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3SetupVertexMemory(context);
		shape->b3AddCount(context);
	}
}

/*************************************************************************
**                                                                      **
**                        Update vertices and indices                   **
**                                                                      **
*************************************************************************/

void b3Scene::b3Update()
{
	b3PrintF(B3LOG_FULL,"    Updating geometry...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3UpdateThread, null, false);
}

b3_bool b3Scene::b3UpdateThread(b3BBox *bbox,void *ptr)
{
	bbox->b3UpdateBBox();
	return true;
}

void b3BBox::b3UpdateBBox()
{
	b3Item  *item;
	b3Shape *shape;

	b3RenderObject::b3Update();
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Update();
	}
}

/*************************************************************************
**                                                                      **
**                        Compute object dimensions                     **
**                                                                      **
*************************************************************************/

b3_bool b3Scene::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3_bool  result = false;

	b3Update();

	b3PrintF(B3LOG_FULL,"    Updating geometry dimensions...\n");
	b3Vector::b3InitBound(lower,upper);
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(lower,upper,m_BBoxOverSize);
	}
	return result;
}

b3_bool b3BBox::b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance)
{
	b3Item    *item;
	b3BBox    *bbox;
	b3Shape   *shape;
	b3_vector  subLower;
	b3_vector  subUpper;
	b3_bool    result = false;

	b3Vector::b3InitBound(&subLower,&subUpper);
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape   = (b3Shape *)item;
		result |= shape->b3ComputeBounds(&subLower,&subUpper);
	}

	if (result)
	{
		// Use fresh data
		b3Vector::b3Scale(b3Vector::b3Sub(&subUpper,&subLower,&m_DimSize),tolerance * 0.5);
		b3Vector::b3SetMinimum(&m_DimSize,b3Scene::epsilon);
		b3Vector::b3Sub(&m_DimSize,&subLower);
		b3Vector::b3Add(&m_DimSize,&subUpper);
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox    = (b3BBox *)item;
		result |= bbox->b3ComputeBounds(&subLower,&subUpper,tolerance);
	}

	// Check lower bound
	b3Vector::b3CheckLowerBound(lower,&subLower);

	// Check upper bound
	b3Vector::b3CheckUpperBound(upper,&subUpper);

	// Compute bounds of this BBox
	if (result)
	{
		b3Vector::b3Sub(&subUpper,&subLower,&m_DimSize);
		m_DimBase   = subLower;

		b3Recompute();
		b3RenderObject::b3Update();
	}

	return result;
}

/*************************************************************************
**                                                                      **
**                        Update materials                              **
**                                                                      **
*************************************************************************/

void b3Scene::b3UpdateMaterial()
{
	b3PrintF(B3LOG_FULL,"    Updating materials...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3UpdateMaterialThread);
}

b3_bool b3Scene::b3UpdateMaterialThread(b3BBox *bbox,void *ptr)
{
	b3Item  *item;
	b3Shape *shape;

	bbox->b3UpdateMaterial();
	B3_FOR_BASE(bbox->b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3UpdateMaterial();
	}
	return true;
}

/*************************************************************************
**                                                                      **
**                        Mark materials for recomputing                **
**                                                                      **
*************************************************************************/

void b3Scene::b3RecomputeMaterial()
{
	b3PrintF(B3LOG_FULL,"    Invalidating materials...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3RecomputeMaterialThread);
}

b3_bool b3Scene::b3RecomputeMaterialThread(b3BBox *bbox,void *ptr)
{
	b3Item  *item;
	b3Shape *shape;

	bbox->b3RecomputeMaterial();
	B3_FOR_BASE(bbox->b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3RecomputeMaterial();
	}
	return true;
}

/*************************************************************************
**                                                                      **
**                        Draw scene                                    **
**                                                                      **
*************************************************************************/

void b3Scene::b3Draw(b3RenderContext *context)
{
	b3Item *item;
	b3BBox *bbox;

	b3PrintT("OpenGL - Drawing start");
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw(context);
	}
	b3PrintT("OpenGL - Drawing end");
}

void b3BBox::b3Draw(b3RenderContext *context)
{
	b3Item  *item;
	b3BBox  *bbox;
	b3Shape *shape;

	// Draw this
	b3RenderObject::b3Draw(context);

	// Draw our shapes
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Draw(context);
	}

	// Draw subsequent BBoxes
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Draw(context);
	}
}

/*************************************************************************
**                                                                      **
**                        Transformation                                **
**                                                                      **
*************************************************************************/

void b3Scene::b3Transform(
	b3_matrix *transformation,
	b3_bool    is_affine,
	b3_bool    force_action)
{
	if (!b3Matrix::b3IsUnitMatrix(transformation))
	{
		b3Item *item;
		b3BBox *bbox;

		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			bbox->b3Transform(transformation,is_affine,force_action);
		}
	}
}

b3_bool b3BBox::b3Transform(
	b3_matrix *transformation,
	b3_bool    is_affine,
	b3_bool    force_action)
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
			shape->b3Transform(transformation,is_affine);
			transformed = true;
		}
	}

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if(bbox->b3Transform(transformation,is_affine,force_action))
		{
			transformed = true;
		}
	}

	if (force_action || b3IsActive())
	{
		b3Matrix::b3MMul(&m_Matrix,transformation,&m_Matrix);
		transformed = true;
	}

	return transformed;
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

void b3BBox::b3Activate(b3_bool activate,b3_bool recurse)
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

	if (recurse)
	{
		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			bbox->b3Activate(activate);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Find BBox where bbox belongs to               **
**                                                                      **
*************************************************************************/

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

/*************************************************************************
**                                                                      **
**                        Collect all (de-)activated BBoxes             **
**                                                                      **
*************************************************************************/

void b3Scene::b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation)
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Base<b3Item> *base;

	array->b3Clear();

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		bbox   = (b3BBox *)item;
		bbox->b3CollectActiveBBoxes(array,activation);
	}
}

void b3BBox::b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation)
{
	b3Item         *item;
	b3BBox         *bbox;
	b3Base<b3Item> *base;

	if (b3IsActive() == activation)
	{
		array->b3Add(this);
	}

	base = b3GetBBoxHead();
	B3_FOR_BASE(base,item)
	{
		bbox   = (b3BBox *)item;
		bbox->b3CollectActiveBBoxes(array,activation);
	}
}

/*************************************************************************
**                                                                      **
**                        Find BBox where shape belongs to              **
**                                                                      **
*************************************************************************/

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

/*************************************************************************
**                                                                      **
**                        Recompute vertices from bbox node             **
*                         up to root bbox                               **
**                                                                      **
*************************************************************************/

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
