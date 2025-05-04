/*
**
**	$Filename:	b3BBox.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing bounding boxes
**
**	(C) Copyright 2001  Steffen A. Mork
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
#include "b3ReadCOB.h"
#include "b3ReadTGF.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

const b3_gl_line b3BBox::m_BBoxIndices[12 * 2]
{
	{ 0, 1 },
	{ 1, 2 },
	{ 2, 3 },
	{ 3, 0 },
	{ 4, 5 },
	{ 5, 6 },
	{ 6, 7 },
	{ 7, 4 },
	{ 0, 4 },
	{ 1, 5 },
	{ 2, 6 },
	{ 3, 7 }
};

b3Color  b3BBox::m_BBoxGridColor{ 0.4f, 0.4f, 0.4f, 0.0f };
bool     b3BBox::m_GridVisible = true;

b3_count b3BBox::m_Visible;
b3_count b3BBox::m_PartiallyVisible;
b3_count b3BBox::m_Invisible;

void b3BBox::b3RegisterBBoxes()
{
	b3PrintF(B3LOG_DEBUG, "Registering bounding boxes...\n");
	b3Item::b3Register(&b3BBox::b3StaticInit, &b3BBox::b3StaticInit, CLASS_BBOX, true);
}

b3BBox::b3BBox(const b3_u32 class_type) : b3Item(sizeof(b3BBox), class_type)
{
	b3AllocHeads(2);
	m_Heads[0].b3InitBase(CLASS_SHAPE);
	m_Heads[1].b3InitBase(CLASS_BBOX);

	b3Matrix::b3Unit(&m_Matrix);
	b3Matrix::b3Unit(&m_Inverse);
	m_Type       = 0;
	b3Vector::b3Init(&m_DimBase, -1.0, -1.0, -1.0);
	b3Vector::b3Init(&m_DimSize,  2.0,  2.0,  2.0);
	m_BoxName[0] = 0;
	m_BoxURL[0]  = 0;

	m_CSGIntersectionCount = 0;
	m_ShapeCount           = 0;
	m_Visibility           = B3_BBOX_UNKNOWN;
}

b3BBox::b3BBox(const b3_u32 * src) : b3Item(src)
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
		b3InitString(m_BoxName, diff);
		m_BoxURL[0] = 0;
	}
	else
	{
		b3InitString(m_BoxName, B3_BOXSTRINGLEN);
		b3InitString(m_BoxURL,  B3_BOXSTRINGLEN);
	}

	m_CSGIntersectionCount = 0;
	m_ShapeCount           = 0;
	m_Visibility           = B3_BBOX_UNKNOWN;
}

void b3BBox::b3Write()
{
	b3StoreInt(m_Type);
	b3StoreVector(&m_DimBase);
	b3StoreVector(&m_DimSize);
	b3StoreNull(); // This is Custom
	b3StoreMatrix(&m_Matrix);

	b3StoreString(m_BoxName, B3_BOXSTRINGLEN);
	b3StoreString(m_BoxURL,  B3_BOXSTRINGLEN);
}

bool b3BBox::b3PrepareBBox(b3_scene_preparation * scene_prep, bool recursive)
{
	b3Matrix::b3Inverse(&m_Matrix, &m_Inverse);
	m_ShapeCount = 0;

	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		if (!shape->b3Prepare(scene_prep))
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
		B3_FOR_TYPED_BASE(b3CSGShape, b3GetShapeHead(), csgShape)
		{
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
				m_ShapeCount, m_CSGIntersectionCount);

			return false;
		}
	}

	if (recursive)
	{
		B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
		{
			if (!bbox->b3PrepareBBox(scene_prep, recursive))
			{
				return false;
			}
		}
	}

	return true;
}

const char * b3BBox::b3GetName() const
{
	return m_BoxName;
}

/*************************************************************************
**                                                                      **
**                        Rendering                                     **
**                                                                      **
*************************************************************************/

void b3BBox::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount B3_UNUSED,
	b3_count     &    polyCount B3_UNUSED)
{
	vertCount = 8;
}

void b3BBox::b3AllocVertexMemory(b3RenderContext * context)
{
	if (b3VectorBufferObjects::b3AllowVBO())
	{
		b3RenderObject::b3AllocVertexMemory(context);
	}
	else
	{
		glVertexElements->b3SetVertices(m_BBoxVertex);
		glVertexElements->b3SetCount(8);
		glVertexElements->b3SetCustom(true);
	}
	glGridElements->b3SetGrids(const_cast<b3_gl_line *>(m_BBoxIndices));
	glGridElements->b3SetCount(12);
	glGridElements->b3SetCustom(true);

	glPolygonElements->b3SetPolygons(nullptr);
	glPolygonElements->b3SetCount(0);
}

void b3BBox::b3FreeVertexMemory()
{
	B3_FOR_TYPED_BASE(b3Shape, b3GetShapeHead(), shape)
	{
		shape->b3FreeVertexMemory();
	}

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3FreeVertexMemory();
	}
}

void b3BBox::b3ComputeVertices()
{
	b3_gl_vertex * glVertex = *glVertexElements;

	glVertex[0].v.x = m_DimBase.x;
	glVertex[0].v.y = m_DimBase.y;
	glVertex[0].v.z = m_DimBase.z;

	glVertex[1].v.x = m_DimBase.x;
	glVertex[1].v.y = m_DimBase.y;
	glVertex[1].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[2].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[2].v.y = m_DimBase.y;
	glVertex[2].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[3].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[3].v.y = m_DimBase.y;
	glVertex[3].v.z = m_DimBase.z;

	glVertex[4].v.x = m_DimBase.x;
	glVertex[4].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[4].v.z = m_DimBase.z;

	glVertex[5].v.x = m_DimBase.x;
	glVertex[5].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[5].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[6].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[6].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[6].v.z = m_DimBase.z + m_DimSize.z;

	glVertex[7].v.x = m_DimBase.x + m_DimSize.x;
	glVertex[7].v.y = m_DimBase.y + m_DimSize.y;
	glVertex[7].v.z = m_DimBase.z;

	glVertexElements->b3SetCount(8);
}

void b3BBox::b3ComputeNormals(bool normalize B3_UNUSED)
{
	// compute no normals!
}

void b3BBox::b3Update()
{
	b3UpdateBBox();

	// Update subsequent BBoxes
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3Update();
	}
}

/*************************************************************************
**                                                                      **
**                        Special transformations                       **
**                                                                      **
*************************************************************************/

void b3BBox::b3ResetTransformation()
{
	b3Matrix::b3Unit(&m_Matrix);
	b3Matrix::b3Unit(&m_Inverse);

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3ResetTransformation();
	}
}

bool b3BBox::b3Inverse(b3_matrix * original)
{
	b3_matrix inverse;
	bool      success;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "--- b3BBox::b3Inverse() ---\n");
	b3Matrix::b3Dump(&m_Matrix, "original");
#endif

	success = b3Matrix::b3Inverse(&m_Matrix, &inverse) != nullptr;
	if (success)
	{
		if (original != nullptr)
		{
			*original = m_Matrix;
		}
		b3Transform(&inverse, true, true);

		// This is not necessary but it removes
		// round errors.
		b3ResetTransformation();
	}

#ifdef _DEBUG
	b3Matrix::b3Dump(&m_Matrix, "b3BBox::m_Matrix");
	b3Matrix::b3Dump(&m_Inverse, "b3BBox::m_Inverse");
#endif

	return success;
}

bool b3BBox::b3Reverse(b3_matrix * original)
{
	bool success = b3Matrix::b3Inverse(original, &m_Inverse) != nullptr;

	if (success)
	{
		b3Transform(original, true, true);
	}

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "--- b3BBox::b3Reverse() ---\n");
	b3Matrix::b3Dump(original, "original");
	b3Matrix::b3Dump(&m_Matrix, "b3BBox::m_Matrix");
	b3Matrix::b3Dump(&m_Inverse, "b3BBox::m_Inverse");
#endif

	return success;
}

/*************************************************************************
**                                                                      **
**                        BBox state handling                           **
**                                                                      **
*************************************************************************/

bool b3BBox::b3IsActive() const
{
	return (m_Type & BBF_ACTIVE) != 0;
}

void b3BBox::b3Expand(bool expand)
{
	if (expand)
	{
		m_Type |=   BBF_EXPANDED;
	}
	else
	{
		m_Type &= (~BBF_EXPANDED);
	}
}

bool b3BBox::b3IsExpanded() const
{
	return (m_Type & BBF_EXPANDED) != 0;
}

/*************************************************************************
**                                                                      **
**                        Bbox hierarchy state handling                 **
**                                                                      **
*************************************************************************/

void b3BBox::b3Dump(b3_count level) const
{
	level = b3GetType();
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL, "Object %s (level %d)\n", m_BoxName, level);

	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3Dump(level);
	}
}

void b3BBox::b3CollectBBoxes(b3Array<b3BBoxReference> & array)
{
	b3BBoxReference  reference(this);

	array.b3Add(reference);
	B3_FOR_TYPED_BASE(b3BBox, b3GetBBoxHead(), bbox)
	{
		bbox->b3CollectBBoxes(array);
	}
}

bool b3BBox::b3FindBBox(b3Base<b3Item> * base, b3BBox * search)
{
	B3_FOR_TYPED_BASE(b3BBox, base, bbox)
	{
		if (bbox == search)
		{
			return true;
		}
		if (b3FindBBox(bbox->b3GetBBoxHead(), search))
		{
			return true;
		}
	}
	return false;
}

b3BBox * b3BBox::b3ReadCOB(const char * filename)
{
	return b3COBReader::b3ReadCOB(filename);
}

b3BBox * b3BBox::b3ReadTGF(const char * filename)
{
	return b3TGFReader::b3ReadTGFBBox(filename);
}
