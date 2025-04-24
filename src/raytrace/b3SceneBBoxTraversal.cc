/*
**
**	$Filename:	b3SceneBBoxTraversal.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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
**                        Reorganizing hierarchy from plain             **
**                        BBox stream.                                  **
**                                                                      **
*************************************************************************/

void b3Scene::b3Reorg()
{
	b3Base<b3Item>         depot;
	b3Base<b3Item>    *    base;
	const b3Link<b3Item> * first;
	b3_count               level;

	base  =  b3GetBBoxHead();
	depot = *base;
	base->b3InitBase(CLASS_BBOX);
	if ((first = depot.First) != nullptr)
	{
		level = first->b3GetType();
		b3BBox::b3Reorg(&depot, base, level, 1);
	}
	b3BBox::b3Recount(base);
}

void b3BBox::b3Reorg(
	b3Base<b3Item> * depot,
	b3Base<b3Item> * base,
	b3_count         level,
	b3_count         rec,
	b3Item     *     insert_after)
{
	b3_count         new_level;
	b3BBox     *     bbox;
	b3Base<b3Item> * sub_base = nullptr;

	while ((bbox = (b3BBox *)depot->First) != nullptr)
	{
		new_level = bbox->b3GetType();
		if (new_level < level)
		{
			return;
		}
		if (new_level == level)
		{
			depot->b3Remove(bbox);
			insert_after != nullptr ? base->b3Insert(insert_after, bbox) : base->b3Append(bbox);
			sub_base = bbox->b3GetBBoxHead();
		}
		else
		{
			B3_ASSERT(sub_base != nullptr);
			b3Reorg(depot, sub_base, new_level, rec + 1);
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

void b3BBox::b3Recount(b3Base<b3Item> * base, b3_count level)
{
	B3_FOR_TYPED_BASE(b3BBox, base, bbox)
	{
		b3Recount(bbox->b3GetBBoxHead(), level + 1);
		bbox->ClassType &= 0xffff0000;
		bbox->ClassType |= level;
	}
}

/*************************************************************************
**                                                                      **
**                        Counting bboxes                               **
**                                                                      **
*************************************************************************/

b3_count b3Scene::b3GetBBoxCount() const
{
	b3_count  count = 0;

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		count += bbox->b3Count();
	}
	return count;
}

b3_count b3BBox::b3Count() const
{
	b3_count count = 1;

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		count += bbox->b3Count();
	}
	return count;
}

/*************************************************************************
**                                                                      **
**                        Allocate vertices of bboxes/shapes            **
**                                                                      **
*************************************************************************/

void b3Scene::b3SetupVertexMemory(b3RenderContext * context)
{
	context->glVertexCount = 0;
	context->glPolyCount   = 0;
	context->glGridCount   = 0;
	context->glTextureSize = 0;

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3SetupVertexMemory(context);
		bbox->b3AddCount(context);
	}
}

void b3BBox::b3SetupVertexMemory(b3RenderContext * context)
{
	b3RenderObject::b3SetupVertexMemory(context);

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3SetupVertexMemory(context);
		bbox->b3AddCount(context);
	}
	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		shape->b3SetupVertexMemory(context);
		shape->b3AddCount(context);
	}
}

void b3Scene::b3FreeVertexMemory()
{
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3FreeVertexMemory();
	}
}

/*************************************************************************
**                                                                      **
**                        Update vertices and indices                   **
**                                                                      **
*************************************************************************/

void b3Scene::b3Update()
{
	b3PrintF(B3LOG_FULL, "    Updating geometry...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3UpdateThread, nullptr, false);
}

bool b3Scene::b3UpdateThread(b3BBox * bbox, void * ptr B3_UNUSED)
{
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "      Updating object <%s>\n", bbox->b3GetName());
#endif
	bbox->b3UpdateBBox();
	return true;
}

void b3BBox::b3UpdateBBox()
{
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "      Updating object <%s>", b3GetName());
#endif
	b3RenderObject::b3Update();
	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL, ".");
#endif
		shape->b3Update();
	}
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "\n");
#endif
}

/*************************************************************************
**                                                                      **
**                        Compute object dimensions                     **
**                                                                      **
*************************************************************************/

bool b3Scene::b3ComputeBounds(b3_vector * lower, b3_vector * upper)
{
	bool     result = false;

	b3Update();

	b3PrintF(B3LOG_FULL, "    Updating geometry dimensions...\n");
	b3Vector::b3InitBound(lower, upper);

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		result |= bbox->b3ComputeBounds(lower, upper, m_BBoxOverSize);
	}
	return result;
}

bool b3BBox::b3ComputeBounds(b3_vector * lower, b3_vector * upper, b3_f64 tolerance)
{
	b3_vector  subLower;
	b3_vector  subUpper;
	bool       result = false;

	b3Vector::b3InitBound(&subLower, &subUpper);
	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		result |= shape->b3ComputeBounds(&subLower, &subUpper);
	}

	if (result)
	{
		// Use fresh data
		b3Vector::b3Scale(b3Vector::b3Sub(&subUpper, &subLower, &m_DimSize), tolerance * 0.5);
		b3Vector::b3SetMinimum(&m_DimSize, b3Math::epsilon);
		b3Vector::b3Sub(&m_DimSize, &subLower);
		b3Vector::b3Add(&m_DimSize, &subUpper);
	}

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		result |= bbox->b3ComputeBounds(&subLower, &subUpper, tolerance);
	}

	// Check lower bound
	b3Vector::b3CheckLowerBound(lower, &subLower);

	// Check upper bound
	b3Vector::b3CheckUpperBound(upper, &subUpper);

	// Compute bounds of this BBox
	if (result)
	{
		b3Vector::b3Sub(&subUpper, &subLower, &m_DimSize);
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
	b3PrintF(B3LOG_FULL, "    Updating materials...\n");

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3UpdateMaterial();
	}
}

void b3BBox::b3UpdateMaterial()
{
	b3RenderObject::b3UpdateMaterial();

	B3_FOR_TYPED_BASE(b3Shape, b3GetBBoxHead(), shape)
	{
		shape->b3UpdateMaterial();
	}

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3UpdateMaterial();
	}
}

/*************************************************************************
**                                                                      **
**                        Mark materials for recomputing                **
**                                                                      **
*************************************************************************/

void b3Scene::b3RecomputeMaterial()
{
	b3PrintF(B3LOG_FULL, "    Invalidating materials...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	m_PrepareInfo.b3Prepare(b3RecomputeMaterialThread);
}

bool b3Scene::b3RecomputeMaterialThread(b3BBox * bbox, void * ptr B3_UNUSED)
{
	bbox->b3RecomputeMaterial();
	B3_FOR_TYPED_BASE(b3Shape, bbox->b3GetShapeHead(), shape)
	{
		shape->b3RecomputeMaterial();
	}
	return true;
}

/*************************************************************************
**                                                                      **
**                        Draw scene                                    **
**                                                                      **
*************************************************************************/

void b3Scene::b3Draw(b3RenderContext * context)
{
	b3PrintT("OpenGL - Drawing start");
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3Draw(context);
	}
	b3PrintT("OpenGL - Drawing end");
}

void b3BBox::b3Draw(b3RenderContext * context)
{
	// Draw this
	b3RenderObject::b3Draw(context);

	// Draw our shapes
	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		shape->b3Draw(context);
	}

	// Draw subsequent BBoxes
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3Draw(context);
	}
}

/*************************************************************************
**                                                                      **
**                        Transformation                                **
**                                                                      **
*************************************************************************/

void b3Scene::b3Transform(
	b3_matrix * transformation,
	bool       is_affine,
	bool       force_action)
{
	if (!b3Matrix::b3IsUnitMatrix(transformation))
	{
		B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
		{
			bbox->b3Transform(transformation, is_affine, force_action);
		}
	}
}

bool b3BBox::b3Transform(
	b3_matrix * transformation,
	bool        is_affine,
	bool        force_action)
{
	bool  transformed = false;

	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		if (force_action || shape->b3IsActive())
		{
			shape->b3Transform(transformation, is_affine);
			transformed = true;
		}
	}

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		if (bbox->b3Transform(transformation, is_affine, force_action))
		{
			transformed = true;
		}
	}

	if (force_action || b3IsActive())
	{
		b3Matrix::b3MMul(&m_Matrix, transformation, &m_Matrix);
		transformed = true;
	}

	return transformed;
}

/*************************************************************************
**                                                                      **
**                        Activation                                    **
**                                                                      **
*************************************************************************/

void b3Scene::b3Activate(bool activate)
{
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3Activate(activate);
	}
}

void b3BBox::b3Activate(bool activate, bool recurse)
{
	if (activate)
	{
		m_Type |=   BBF_ACTIVE;
	}
	else
	{
		m_Type &= (~BBF_ACTIVE);
	}

	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		shape->b3Activate(activate);
	}

	if (recurse)
	{
		B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
		{
			bbox->b3Activate(activate, recurse);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Find BBox where bbox belongs to               **
**                                                                      **
*************************************************************************/

b3Base<b3Item> * b3Scene::b3FindBBoxHead(b3BBox * bbox) const
{
	b3Base<b3Item> * base = b3GetBBoxHead();

	B3_FOR_TYPED_BASE(b3BBox, base, inc_bbox)
	{
		if (inc_bbox == bbox)
		{
			return base;
		}

		b3Base<b3Item> * result = inc_bbox->b3FindBBoxHead(bbox);
		if (result != nullptr)
		{
			return result;
		}
	}
	return nullptr;
}

b3Base<b3Item> * b3BBox::b3FindBBoxHead(b3BBox * bbox)
{
	b3Base<b3Item> * base = b3GetBBoxHead();

	B3_FOR_TYPED_BASE(b3BBox, base, item)
	{
		if (item == bbox)
		{
			return base;
		}

		b3BBox     *     inc_bbox = static_cast<b3BBox *>(item);
		b3Base<b3Item> * result   = inc_bbox->b3FindBBoxHead(bbox);
		if (result != nullptr)
		{
			return result;
		}
	}
	return nullptr;
}

/*************************************************************************
**                                                                      **
**                        Collect all (de-)activated BBoxes             **
**                                                                      **
*************************************************************************/

void b3Scene::b3CollectActiveBBoxes(b3Array<b3BBox *> * array, bool activation)
{
	array->b3Clear();

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3CollectActiveBBoxes(array, activation);
	}
}

void b3BBox::b3CollectActiveBBoxes(b3Array<b3BBox *> * array, bool activation)
{
	if (b3IsActive() == activation)
	{
		array->b3Add(this);
	}

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3CollectActiveBBoxes(array, activation);
	}
}

/*************************************************************************
**                                                                      **
**                        Find BBox where shape belongs to              **
**                                                                      **
*************************************************************************/

b3BBox * b3Scene::b3FindParentBBox(b3Shape * shape) const
{
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		b3BBox * result = bbox->b3FindParentBBox(shape);

		if (result != nullptr)
		{
			return result;
		}
	}
	return nullptr;
}

b3BBox * b3BBox::b3FindParentBBox(b3Shape * shape)
{
	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), item)
	{
		if (item == shape)
		{
			return this;
		}
	}

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		b3BBox * result = bbox->b3FindParentBBox(shape);

		if (result != nullptr)
		{
			return result;
		}
	}
	return nullptr;
}

/*************************************************************************
**                                                                      **
**                        Recompute vertices from bbox node             **
*                         up to root bbox                               **
**                                                                      **
*************************************************************************/

bool b3Scene::b3BacktraceRecompute(b3BBox * search)
{
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		if (bbox == search)
		{
			bbox->b3Recompute();
			return true;
		}

		const bool result = bbox->b3BacktraceRecompute(search) != 0;

		if (result)
		{
			bbox->b3Recompute();
			return result;
		}
	}
	return false;
}

bool b3BBox::b3BacktraceRecompute(b3BBox * search)
{
	// Found?
	if (search == this)
	{
		b3Recompute();
		return true;
	}

	// Search children
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		const bool result = bbox->b3BacktraceRecompute(search);

		if (result)
		{
			bbox->b3Recompute();
			return result;
		}
	}
	return false;
}

/*************************************************************************
**                                                                      **
**                        Compute visibility of bboxes for              **
*                         selected camera                               **
**                                                                      **
*************************************************************************/

#define CLIP_LEFT    1
#define CLIP_RIGHT   2
#define CLIP_TOP     4
#define CLIP_BOTTOM  8
#define CLIP_BACK   16

void b3BBox::b3ComputeVisibility(b3CameraProjection * projection)
{
	b3_gl_vertex * glVertex = *glVertexElements;
	b3_loop        i, visible_count = 0;
	b3_u32         flag = CLIP_RIGHT | CLIP_LEFT | CLIP_BOTTOM | CLIP_TOP | CLIP_BACK;

	for (i = 0; i < 8; i++)
	{
		b3_vector in, out;
		b3_u32    mask = 0;

		in.x = glVertex[i].v.x;
		in.y = glVertex[i].v.y;
		in.z = glVertex[i].v.z;

		projection->b3Project(&in, &out);

		if (out.z <  0)
		{
			mask |= CLIP_BACK;
			if (out.x < -0)
			{
				mask |= CLIP_RIGHT;
			}
			else
			{
				mask |= CLIP_LEFT;
			}
			if (out.y <  0)
			{
				mask |= CLIP_TOP;
			}
			else
			{
				mask |= CLIP_BOTTOM;
			}
		}
		else
		{
			if (out.x < -1)
			{
				mask |= CLIP_LEFT;
			}
			else if (out.x >  1)
			{
				mask |= CLIP_RIGHT;
			}
			if (out.y < -1)
			{
				mask |= CLIP_BOTTOM;
			}
			else if (out.y >  1)
			{
				mask |= CLIP_TOP;
			}
		}

		if (mask == 0)
		{
			visible_count++;
		}

		flag &= mask;
	}

	if (visible_count == 8)
	{
		m_Visibility = B3_BBOX_VISIBLE;
		m_Visible++;
	}
	else if (flag == 0)
	{
		m_Visibility = B3_BBOX_PARTIALLY_VISIBLE;
		m_PartiallyVisible++;
	}
	else
	{
		m_Visibility = B3_BBOX_INVISIBLE;
		m_Invisible++;
	}

	if (m_Visibility != B3_BBOX_INVISIBLE)
	{
		B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
		{
			bbox->b3ComputeVisibility(projection);
		}
	}
}

void b3Scene::b3ComputeVisibility()
{
	b3CameraProjection  projection(b3GetActualCamera());

	b3PrintF(B3LOG_FULL, "  preparing visibility...\n");

	b3BBox::m_Visible = 0;
	b3BBox::m_PartiallyVisible = 0;
	b3BBox::m_Invisible = 0;

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3ComputeVisibility(&projection);
	}

	b3PrintF(B3LOG_FULL, "    Visible objects:           %5d\n", b3BBox::m_Visible);
	b3PrintF(B3LOG_FULL, "    Partially visible objects: %5d\n", b3BBox::m_PartiallyVisible);
	b3PrintF(B3LOG_FULL, "    Invisible objects:         %5d\n", b3BBox::m_Invisible);
}
