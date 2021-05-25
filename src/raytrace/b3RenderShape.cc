/*
**
**	$Filename:	b3RenderShape.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Raytracing shapes
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

#include "blz3/image/b3Sampler.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

const b3_gl_line b3Shape::m_BoxGrids[]
{
	{ 0, 1 }
	,
	{ 1, 2 },
	{ 2, 3 },
	{ 3, 0 },
	{ 7, 6 },
	{ 6, 5 },
	{ 5, 4 },
	{ 4, 7 },
	{ 0, 7 },
	{ 1, 6 },
	{ 2, 5 },
	{ 3, 4 }
};

const b3_gl_polygon b3Shape::m_BoxPolygons[]
{
	{  6, 7, 5 }
	, // top
	{  4, 5, 7 },
	{  0, 1, 3 }, // bottom
	{  2, 3, 1 },
	{ 13, 12, 10 }, // back
	{ 11, 10, 12 },
	{  9, 8, 14 }, // front
	{ 15, 14, 8 },
	{ 16, 19, 23 }, // right
	{ 20, 23, 19 },
	{ 18, 17, 21 }, // left
	{ 22, 21, 17 }
};

const b3_f32 b3Shape::m_BoxTexcoord[]
{
	0, 0,  1, 0,  1, 1,  0, 1,  0, 1, 1, 1,  1, 0,  0, 0,
	0, 0,  1, 0,  1, 1,  0, 1,  0, 1, 1, 1,  1, 0,  0, 0,
	0, 0,  1, 0,  1, 1,  0, 1,  0, 1, 1, 1,  1, 0,  0, 0
};

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_count b3ShapeRenderContext::m_SubDiv = 16;
b3_f64   b3ShapeRenderContext::m_Sin[B3_MAX_RENDER_SUBDIV + 1];
b3_f64   b3ShapeRenderContext::m_Cos[B3_MAX_RENDER_SUBDIV + 1];

b3ShapeRenderContext::b3ShapeRenderContext(const b3_count new_subdiv)
{
	b3InitSubdiv(new_subdiv);
}

void b3ShapeRenderContext::b3InitSubdiv(const b3_count new_subdiv)
{
	b3_gl_line   *  gPtr;
	b3_gl_polygon * pPtr;
	b3_index        a, i;
	b3_f64          aux;

	m_SubDiv = (new_subdiv > B3_MAX_RENDER_SUBDIV ? B3_MAX_RENDER_SUBDIV : new_subdiv);
	if (m_SubDiv < 8)
	{
		m_SubDiv = 8;
	}

	for (i = 0; i <= m_SubDiv; i++)
	{
		aux    = i * M_PI * 2 / m_SubDiv;
		m_Sin[i] = sin(aux);
		m_Cos[i] = cos(aux);
	}

	b3Free(m_CylinderIndices);
	b3Free(m_CylinderPolygons);
	b3Free(m_ConeIndices);
	b3Free(m_ConePolygons);

	m_CylinderIndices  = b3TypedAlloc<b3_gl_line>((m_SubDiv + 1) * 3);
	m_CylinderPolygons = b3TypedAlloc<b3_gl_polygon>((m_SubDiv + 1) * 2);
	if ((m_CylinderIndices != nullptr) && (m_CylinderPolygons != nullptr))
	{
		gPtr = m_CylinderIndices;
		pPtr = m_CylinderPolygons;
		a = 0;
		for (i = 0; i <= m_SubDiv; i++)
		{
			// Concatinate vertices in this ascending order:
			//
			//   (c)
			// 1-----3
			// |
			// |(a)
			// |
			// 0-----2
			//   (b)

			// (a)
			B3_GL_LINIT(gPtr, a,  a + 1); // (a)
			B3_GL_LINIT(gPtr, a,  a + 2); // (b)
			B3_GL_LINIT(gPtr, a + 1, a + 3); // (c)

			B3_GL_PINIT(pPtr, a,  a + 2, a + 1);
			B3_GL_PINIT(pPtr, a + 3, a + 1, a + 2);

			a += 2;
		}
	}
	else
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}

	m_ConeIndices  = b3TypedAlloc<b3_gl_line>((m_SubDiv + 1) * 2);
	m_ConePolygons = b3TypedAlloc<b3_gl_polygon>((m_SubDiv + 1) * 1);
	if ((m_ConeIndices != nullptr) && (m_ConePolygons != nullptr))
	{
		gPtr = m_ConeIndices;
		pPtr = m_ConePolygons;
		for (i = 0; i <= m_SubDiv; i++)
		{
			B3_GL_LINIT(gPtr, 0,  i + 1);
			B3_GL_LINIT(gPtr, i + 1, i + 2);

			B3_GL_PINIT(pPtr, i + 1, i + 2, 0);
		}
	}
	else
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}
}

b3_gl_line * b3ShapeRenderContext::b3GetCylinderIndices() const
{
	return m_CylinderIndices;
}

b3_gl_polygon * b3ShapeRenderContext::b3GetCylinderPolygons() const
{
	return m_CylinderPolygons;
}

b3_gl_line * b3ShapeRenderContext::b3GetConeIndices() const
{
	return m_ConeIndices;
}

b3_gl_polygon * b3ShapeRenderContext::b3GetConePolygons() const
{
	return m_ConePolygons;
}

/*************************************************************************
**                                                                      **
**                        b3ShapeRenderObject Implementation            **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeBound(b3_stencil_limit * limit) const
{
	b3_stencil_bound  info;

	// Get outer limits
	b3GetStencilBoundInfo(&info);
	limit->x1 = info.xInfo.min;
	limit->y1 = info.yInfo.min;
	limit->x2 = info.xInfo.max;
	limit->y2 = info.yInfo.max;

	// Do any stencil make the limits closer?
	B3_FOR_TYPED_BASE(b3Condition, b3GetConditionHead(), cond)
	{
		cond->b3ComputeBound(limit);
	}
}

/*************************************************************************
**                                                                      **
**                        Retrieving material properties                **
**                                                                      **
*************************************************************************/

void b3Shape::b3GetDiffuseColor(b3Color & color) const
{
	b3_ray       ray;
	b3_surface   surface;

	color.b3Init(0.1f, 0.5f, 1.0f, 0.0f);
	surface.m_Incoming = &ray;
	B3_FOR_TYPED_BASE(b3Material, b3GetMaterialHead(), material)
	{
		if (material->b3GetSurfaceValues(&surface))
		{
			color = surface.m_Diffuse;
			return;
		}
	}
}

b3_f64 b3Shape::b3GetColors(
	b3Color & ambient,
	b3Color & diffuse,
	b3Color & specular) const
{
	b3_ray       ray;
	b3_surface   surface;

	b3Vector::b3Init(&ray.ipoint);
	b3Vector::b3Init(&ray.polar.m_BBoxOriginal);
	b3Vector::b3Init(&ray.polar.m_BoxPolar);
	b3Vector::b3Init(&ray.polar.m_ObjectPolar);
	b3Vector::b3Init(&ray.polar.m_Polar);
	ray.polar.m_NormalIndex = 0;

	surface.m_Incoming = &ray;
	ray.Q = 1;
	b3Vector::b3Init(&surface.m_Incoming->normal, 0, 0, 1);
	B3_FOR_TYPED_BASE(b3Material, b3GetMaterialHead(), material)
	{
		if (material->b3GetSurfaceValues(&surface))
		{
			ambient  = surface.m_Ambient;
			diffuse  = surface.m_Diffuse;
			specular = surface.m_Specular;
			return surface.m_SpecularExp;
		}
	}

	return b3RenderObject::b3GetColors(ambient, diffuse, specular);
}

bool b3Shape::b3GetChess(
	b3Color & black,
	b3Color & white,
	b3_res  & xRepeat,
	b3_res  & yRepeat) const
{
	b3Item * item;
	bool     result = false;

	item   = b3GetMaterialHead()->First;
	if (item != nullptr)
	{
		result = item->b3GetClassType() == CHESS;
		if (result)
		{
			b3MatChess * chess = (b3MatChess *)item;
			black   = chess->m_Material[0].m_Diffuse;
			white   = chess->m_Material[1].m_Diffuse;
			xRepeat = chess->m_xTimes;
			yRepeat = chess->m_yTimes;
		}
	}
	return result;
}

b3Tx * b3Shape::b3GetTexture(
	b3_f64 & xTrans,
	b3_f64 & yTrans,
	b3_f64 & xScale,
	b3_f64 & yScale) const
{
	b3Item * item;
	b3Tx  *  tx = nullptr;

	item = b3GetMaterialHead()->First;
	if ((item != nullptr) && (item->b3GetClassType() == TEXTURE) && (item->Succ == nullptr))
	{
		b3MatTexture   *  mat = (b3MatTexture *)item;
		b3_stencil_limit  limit;
		b3_f64            xLocSize;
		b3_f64            yLocSize;

		tx     = mat->m_Texture;
		B3_ASSERT(tx != nullptr);

		b3ComputeBound(&limit);
		xLocSize = limit.x2 - limit.x1;
		yLocSize = limit.y2 - limit.y1;

		xTrans = (mat->m_xStart - limit.x1) / xLocSize;
		yTrans = (mat->m_yStart - limit.y1) / yLocSize;
		xScale =  mat->m_xScale / xLocSize;
		yScale =  mat->m_yScale / yLocSize;

		b3PrintF(B3LOG_FULL, "b3ShapeRenderObject::b3GetTexture(%2.2f,%2.2f,%2.2f,%2.2f) = %s\n",
			xTrans, yTrans, xScale, yScale, tx->b3Name());
	}
	return tx;
}

class b3RenderImageSampler : public b3Sampler
{
	const b3Shape * m_Shape;
	b3Tx      *     m_Tx;

public:
	b3RenderImageSampler(const b3Shape * shape, b3Tx * tx) : m_Shape(shape)
	{
		B3_ASSERT(tx->b3IsTrueColor());

		m_Tx    = tx;
		m_xMax  = tx->xSize;
		m_yMax  = tx->ySize;
		m_Data  = tx->b3GetTrueColorData();
	}

private:
	b3SampleInfo * b3SampleInit(const b3_count CPUs) override
	{
		b3SampleInfo * info = new b3SampleInfo[CPUs];
		b3_loop        i;
		b3_res         yStart, yEnd;
		b3_pkd_color * data = m_Data;

		yStart = 0;
		for (i = 0; i < CPUs; i++)
		{
			yEnd = m_yMax * (i + 1) / CPUs;
			info[i].m_Sampler = this;
			info[i].m_Ptr     = m_Shape;
			info[i].m_xMax    = m_xMax;
			info[i].m_yMax    = m_yMax;
			info[i].m_yStart  = yStart;
			info[i].m_yEnd    = yEnd;
			info[i].m_Data    = &data[yStart * m_xMax];
			yStart = yEnd;
		}
		return info;
	}

	void b3SampleTask(const b3SampleInfo * info) override
	{
		b3Material    *   material;
		b3_ray            ray;
		b3BBox            bbox = BBOX;
		b3_surface        surface;
		b3_stencil_limit  limit;
		b3_pkd_color   *  data = info->m_Data;
		b3_pkd_color      color;
		b3_coord          x, y;
		b3_f64            fx, fxStep;
		b3_f64            fy, fyStep;
		bool              loop;

		m_Shape->b3ComputeBound(&limit);
		fxStep = (limit.x2 - limit.x1 - 2 * b3Math::epsilon) / info->m_xMax;
		fyStep = (limit.y2 - limit.y1 - 2 * b3Math::epsilon) / info->m_yMax;

		surface.m_Incoming = &ray;
		surface.m_Incoming->Q = 1;
		ray.bbox  = &bbox;
		ray.shape = m_Shape;
		bbox.b3Prepare(nullptr);

		fy = limit.y1 + info->m_yStart * fyStep + b3Math::epsilon;
		ray.polar.m_NormalIndex = 0;
		ray.polar.m_BoxPolar.z =
			ray.polar.m_ObjectPolar.z =
				ray.polar.m_Polar.z =
					ray.polar.m_BBoxOriginal.z =
						ray.ipoint.z = 0;
		for (y = info->m_yStart; y < info->m_yEnd; y++)
		{
			ray.polar.m_BoxPolar.y =
				ray.polar.m_ObjectPolar.y =
					ray.polar.m_Polar.y =
						ray.polar.m_BBoxOriginal.y =
							ray.ipoint.y = fy;

			fx = limit.x1 + b3Math::epsilon;
			for (x = 0; x < info->m_xMax; x++)
			{
				ray.polar.m_BoxPolar.x =
					ray.polar.m_ObjectPolar.x =
						ray.polar.m_Polar.x =
							ray.polar.m_BBoxOriginal.x =
								ray.ipoint.x = fx;

				color = B3_BLACK;
				loop  = true;
				for (material  = (b3Material *)m_Shape->b3GetMaterialHead()->First;
					(material != nullptr) && loop;
					material  = (b3Material *)material->Succ)
				{
					if (material->b3GetSurfaceValues(&surface))
					{
						surface.m_Diffuse.b3SetAlpha(m_Shape->b3CheckStencil(&ray.polar) ? 0 : 1);
						color     = surface.m_Diffuse;
						loop      = false;
					}
				}
				*data++ = color;
				fx += fxStep;
			}
			fy += fyStep;
		}
	}
};

bool b3Shape::b3GetImage(b3Tx * image) const
{
	b3Item    *   item;
	bool          result = false;
	b3_u32        type;

	for (item  = b3GetMaterialHead()->First;
		(item != nullptr) && (!result);
		item  = item->Succ)
	{
		type   = item->b3GetClassType();
		result = ((type != NORMAL_MATERIAL) && (type != CHESS));
	}

	if (result)
	{
		b3PrintT(">b3GetImage()");
		b3RenderImageSampler sampler(this, image);

		sampler.b3Sample();
		b3PrintT("<b3GetImage()");
	}
	return result;
}

b3_render_mode b3Shape::b3GetRenderMode() const
{
	return b3IsActive() ? B3_RENDER_FILLED : B3_RENDER_LINE;
}

/*************************************************************************
**                                                                      **
**                        Searching for equal indices                   **
**                                                                      **
*************************************************************************/

b3_count b3Shape::b3GetIndexOverhead(
	b3_f64 xLeft,
	b3_f64 yLeft)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_count Overhead;
	b3_index xs, xe;
	b3_f64   x1, x2;

	if (m_Limit.x1 < xLeft)
	{
		m_Limit.x1 = xLeft;
	}
	if (m_Limit.y1 < yLeft)
	{
		m_Limit.y1 = yLeft;
	}
	x1 = m_Limit.x1 * SinCosSteps;
	x2 = m_Limit.x2 * SinCosSteps;
	xs = (b3_index)ceil(x1);
	xe = (b3_index)floor(x2);
	Overhead = xe - xs;
	if ((xs - x1) > b3Math::epsilon)
	{
		Overhead++;
	}
	if ((x2 - xe) > b3Math::epsilon)
	{
		Overhead++;
	}

	return ((xs > 0) || (xe < SinCosSteps)) ? -Overhead : Overhead;
}

#define no_USE_FIND_VERTEX

b3_index b3Shape::b3FindVertex(b3_index index)
{
#ifdef USE_FIND_VERTEX
	b3_vector * point = &glVertex[index].v;

	for (b3_index i = 0; i < glVertexCount; i++)
	{
		if (b3Vector::b3Distance(point, &glVertex[i].v) < epsilon)
		{
			return i;
		}
	}

	B3_ASSERT(false);
#endif
	return index;
}

void b3Shape::b3ComputeQuadricNormals(bool normalize)
{
#ifdef USE_FIND_VERTEX
	b3_vector normal;
	b3_vector xDir, yDir;
	b3_index  i, k, v1, v2, v3, n1, n2, n3, start, end;

	// Clear normals
	b3GetVertexRange(start, end);
	for (i = start; i < end; i++)
	{
		b3Vector::b3Init(&glVertex[i].n);
	}

	for (i = k = 0; i < glPolyCount; i++)
	{
		n1 = b3FindVertex(v1 = glPolygons[k++]);
		n2 = b3FindVertex(v2 = glPolygons[k++]);
		n3 = b3FindVertex(v3 = glPolygons[k++]);

		b3Vector::b3Sub(&glVertex[n2].v, &glVertex[n1].v, &xDir);
		b3Vector::b3Sub(&glVertex[n3].v, &glVertex[n1].v, &yDir);
		b3Vector::b3CrossProduct(&xDir, &yDir, &normal);
		if (b3Vector::b3Normalize(&normal) > epsilon)
		{
			b3Vector::b3Add(&normal, &glVertex[v1].n);
			b3Vector::b3Add(&normal, &glVertex[v2].n);
			b3Vector::b3Add(&normal, &glVertex[v3].n);

			if (n1 != v1)
			{
				b3Vector::b3Add(&normal, &glVertex[n1].n);
			}
			if (n2 != v2)
			{
				b3Vector::b3Add(&normal, &glVertex[n2].n);
			}
			if (n3 != v3)
			{
				b3Vector::b3Add(&normal, &glVertex[n3].n);
			}
		}
	}

	// Normalize
	if (normalize)
	{
		for (i = 0; i < glVertexCount; i++)
		{
			b3Vector::b3Normalize(&glVertex[i].n);
		}
	}
#else
	b3RenderObject::b3ComputeNormals(normalize);
#endif
}

/*************************************************************************
**                                                                      **
**                        Sphere computation                            **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeSphereVertices(
	b3_vector  & Base,
	b3_vector  & Dir)
{
	b3_gl_vertex * Vector;
	b3_index      i, j;
	b3_count      Circles;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_f64        cx, sx, cy, sy, a;
	b3_f64        LocalSin[B3_MAX_RENDER_SUBDIV + 1], LocalCos[B3_MAX_RENDER_SUBDIV + 1];
	b3_f32        Rad;
	b3_vector     Aux, Dir1, Dir2, Dir3;

	Vector = *glVertexElements;
	Aux    = Base;
	Dir1.x = Rad = b3Vector::b3Length(&Dir);
	Dir1.y = 0;
	Dir1.z = 0;
	Dir2.x = 0;
	Dir2.y = Rad;
	Dir2.z = 0;
	Dir3.x = 0;
	Dir3.y = 0;
	Dir3.z = Rad;
	Circles = (SinCosSteps >> 1) + 1;
	m_xVertices   = SinCosSteps + 1;
	m_yVertices   = Circles;

	a = 2.0 / SinCosSteps;
	for (j = 0 ; j < Circles; j++)
	{
		LocalCos[j] = Rad = -cos(j * a * M_PI);
		LocalSin[j] = sqrt(1.0 - Rad * Rad);
	}

	for (i = 0; i <= SinCosSteps; i++)
	{
		cx = b3ShapeRenderContext::m_Cos[i];
		sx = b3ShapeRenderContext::m_Sin[i];
		for (j = 0; j < Circles; j++)
		{
			cy = LocalCos[j];
			sy = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + cx * sy * Dir1.x + sx * sy * Dir2.x + cy * Dir3.x;
			Vector->v.y = Base.y + cx * sy * Dir1.y + sx * sy * Dir2.y + cy * Dir3.y;
			Vector->v.z = Base.z + cx * sy * Dir1.z + sx * sy * Dir2.z + cy * Dir3.z;
			Vector++;
		}
	}
}

void b3Shape::b3ComputeSphereNormals(b3_vector & base, bool normalize)
{
	b3_gl_vertex * glVertex = *glVertexElements;
	b3_index i;

	for (i = 0; i < glVertexElements->b3GetCount(); i++)
	{
		if (normalize)
		{
			b3_f64 x, y, z, r;

			x = base.x - glVertex[i].v.x;
			y = base.y - glVertex[i].v.y;
			z = base.z - glVertex[i].v.z;
			r = 1.0 / sqrt(x * x + y * y + z * z);

			glVertex[i].n.x = x * r;
			glVertex[i].n.y = y * r;
			glVertex[i].n.z = z * r;
		}
		else
		{
			glVertex[i].n.x = base.x - glVertex[i].v.x;
			glVertex[i].n.y = base.y - glVertex[i].v.y;
			glVertex[i].n.z = base.z - glVertex[i].v.z;
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Cylinder computation                          **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeCylinderVertices(
	b3_vector  & Base,
	b3_vector  & Dir1,
	b3_vector  & Dir2,
	b3_vector  & Dir3)
{
	b3_gl_vertex * Vector      = *glVertexElements;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_f64        sx, sy, b, h, start, end;
	b3_index      i;
	b3_count      iMax;
	b3_vector     Bottom;
	b3_count      glVertexCount = 0;

	h        = m_Limit.y2 - m_Limit.y1;
	b        = m_Limit.y1;
	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;
	start    = m_Limit.x1 * SinCosSteps;
	end      = m_Limit.x2 * SinCosSteps;
	i        = (b3_index)ceil(start);
	iMax     = (b3_count)floor(end);
	m_xVertices    = 0;
	m_yVertices    = 2;

	if ((i - start) > b3Math::epsilon)
	{
		b  = m_Limit.x1 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->t.s = 0;
		Vector->t.t = 0;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->t.s = 0;
		Vector->t.t = 1;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		m_xVertices++;
	}

	for (; i <= iMax; i++)
	{
		b3_f64 s = ((double)i / SinCosSteps) / (m_Limit.x2 - m_Limit.x1) - m_Limit.x1;

		sx = b3ShapeRenderContext::m_Cos[i % SinCosSteps];
		sy = b3ShapeRenderContext::m_Sin[i % SinCosSteps];

		Vector->t.s = s;
		Vector->t.t = 0;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->t.s = s;
		Vector->t.t = 1;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;

		glVertexCount += 2;
		m_xVertices++;
	}

	if ((end - iMax) > b3Math::epsilon)
	{
		b = m_Limit.x2 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->t.s = 1;
		Vector->t.t = 0;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->t.s = 1;
		Vector->t.t = 1;
		Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;

		glVertexCount += 2;
		m_xVertices++;
	}

	glVertexElements->b3SetCount(glVertexCount);
}

void b3Shape::b3ComputeCylinderIndices()
{
	b3_count Overhead;
	b3_count glGridCount;

	b3ComputeBound(&m_Limit);
	Overhead = b3GetIndexOverhead(0.0, 0.0);
	if (Overhead < 0)
	{
		glGridCount = 1;
		Overhead = -Overhead;
	}
	else
	{
		glGridCount = 0;
	}

	glGridElements->b3SetCount(glGridCount + Overhead * 3);
	glPolygonElements->b3SetCount(Overhead * 2);
}

/*************************************************************************
**                                                                      **
**                        Cone computation                              **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeConeVertices(
	b3_vector  & Base,
	b3_vector  & Dir1,
	b3_vector  & Dir2,
	b3_vector  & Dir3)
{
	b3_gl_vertex * Vector      = *glVertexElements;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_f64        sx, sy, b, h, d, a, start, end;
	b3_index      i;
	b3_count      iMax;
	b3_vector     Bottom;
	b3_count      glVertexCount = 0;

	d        = m_Limit.y2 - m_Limit.y1;
	b        = m_Limit.y1;
	h        = m_Limit.y2;
	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;

	start  = m_Limit.x1 * SinCosSteps;
	end    = m_Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	m_xVertices  = 0;
	m_yVertices  = 1;

	if (m_Limit.y2 < 1)
	{
		m_yVertices++;
		if ((i - start) > b3Math::epsilon)
		{
			a = m_Limit.x1 * M_PI * 2;
			sx = (1 - b) * cos(a);
			sy = (1 - b) * sin(a);
			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1 - h) * cos(a);
			sy = (1 - h) * sin(a);
			Vector->t.s = 0;
			Vector->t.t = 1;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;
			Vector++;

			glVertexCount += 2;
			m_xVertices++;
		}

		for (; i <= iMax; i++)
		{
			b3_f64 s = ((double)i / SinCosSteps) / (m_Limit.x2 - m_Limit.x1) - m_Limit.x1;

			sx = (1 - b) * b3ShapeRenderContext::m_Cos[i % SinCosSteps];
			sy = (1 - b) * b3ShapeRenderContext::m_Sin[i % SinCosSteps];
			Vector->t.s = s;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1 - h) * b3ShapeRenderContext::m_Cos[i % SinCosSteps];
			sy = (1 - h) * b3ShapeRenderContext::m_Sin[i % SinCosSteps];
			Vector->t.s = s;
			Vector->t.t = 1;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;
			Vector++;

			glVertexCount += 2;
			m_xVertices++;
		}

		if ((end - iMax) > b3Math::epsilon)
		{
			a  = m_Limit.x2 * M_PI * 2;

			sx = (1 - b) * cos(a);
			sy = (1 - b) * sin(a);
			Vector->t.s = 1;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1 - h) * cos(a);
			sy = (1 - h) * sin(a);
			Vector->t.s = 1;
			Vector->t.t = 1;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x + d * Dir3.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y + d * Dir3.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z + d * Dir3.z;

			glVertexCount += 2;
			m_xVertices++;
		}
	}
	else
	{
		Vector->t.s = 0.5;
		Vector->t.t = 1;
		Vector->v.x = Base.x + Dir3.x;
		Vector->v.y = Base.y + Dir3.y;
		Vector->v.z = Base.z + Dir3.z;
		Vector++;
		glVertexCount++;

		if ((i - start) > b3Math::epsilon)
		{
			a  = m_Limit.x1 * M_PI * 2;
			sx = (1 - b) * cos(a);
			sy = (1 - b) * sin(a);

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			glVertexCount++;
			m_xVertices++;
		}

		for (; i <= iMax; i++)
		{
			sx = (1 - b) * b3ShapeRenderContext::m_Cos[i % SinCosSteps];
			sy = (1 - b) * b3ShapeRenderContext::m_Sin[i % SinCosSteps];

			Vector->t.s = ((double)i / SinCosSteps) / (m_Limit.x2 - m_Limit.x1) - m_Limit.x1;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			glVertexCount++;
			m_xVertices++;
		}

		if ((end - iMax) > b3Math::epsilon)
		{
			a  = m_Limit.x2 * M_PI * 2;
			sx = (1 - b) * cos(a);
			sy = (1 - b) * sin(a);

			Vector->t.s = 1;
			Vector->t.t = 0;
			Vector->v.x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->v.y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->v.z = Bottom.z + sx * Dir1.z + sy * Dir2.z;

			glVertexCount++;
			m_xVertices++;
		}
	}
	glVertexElements->b3SetCount(glVertexCount);
}

void b3Shape::b3ComputeConeIndices()
{
	b3_count Overhead;
	b3_count glGridCount;
	b3_count glPolyCount;

	b3ComputeBound(&m_Limit);
	Overhead = b3GetIndexOverhead(0.0, 0.0);
	if (Overhead < 0)
	{
		glGridCount = 1;
		Overhead  = -Overhead;
	}
	else
	{
		glGridCount = 0;
	}
	if (m_Limit.y2 < 1)
	{
		glGridElements->b3SetGrids(GridsCyl);
		glPolygonElements->b3SetPolygons(PolysCyl);
		glGridCount += Overhead * 3;
		glPolyCount  = Overhead * 2;
	}
	else
	{
		glGridElements->b3SetGrids(GridsCone);
		glPolygonElements->b3SetPolygons(PolysCone);
		glGridCount += Overhead * 2;
		glPolyCount  = Overhead;
	}

	glGridElements->b3SetCount(glGridCount);
	glPolygonElements->b3SetCount(glPolyCount);
}

/*************************************************************************
**                                                                      **
**                        Ellipsoid computation                         **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeEllipsoidVertices(
	b3_vector  & Base,
	b3_vector  & Dir1,
	b3_vector  & Dir2,
	b3_vector  & Dir3)
{
	b3_gl_vertex * Vector      = *glVertexElements;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_index      i, j;
	b3_count      iMax, Circles = 0;
	b3_f64        RadX, RadY, sx, sy;
	b3_f64        LocalSin[B3_MAX_RENDER_SUBDIV + 1], LocalCos[B3_MAX_RENDER_SUBDIV + 1];
	b3_f32        start, end, a;
	b3_count      glVertexCount = 0;

	start  = (m_Limit.y1 + 1) * SinCosSteps * 0.25;
	end    = (m_Limit.y2 + 1) * SinCosSteps * 0.25;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > b3Math::epsilon)	/* underflow */
	{
		LocalSin[Circles] = m_Limit.y1;
		Circles++;
	}
	a = 4.0 / SinCosSteps;
	for (j = i; j <= iMax; j++)		/* integers */
	{
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > b3Math::epsilon)	/* Overflow */
	{
		LocalSin[Circles] = m_Limit.y2;
		Circles++;
	}

	for (j = 0; j < Circles; j++)		/* Calculate Values */
	{
		LocalSin[j] = a = sin(LocalSin[j] * M_PI * 0.5);
		LocalCos[j] = sqrt(1.0 - a * a);
	}

	start  = m_Limit.x1 * SinCosSteps;
	end    = m_Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	m_xVertices = 0;
	m_yVertices = Circles;

	if ((i - start) > b3Math::epsilon)
	{
		a  = m_Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j = 0; j < Circles; j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		m_xVertices++;
	}

	for (; i <= iMax; i++)
	{
		sx = b3ShapeRenderContext::m_Cos[i];
		sy = b3ShapeRenderContext::m_Sin[i];

		for (j = 0; j < Circles; j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		m_xVertices++;
	}

	if ((end - iMax) > b3Math::epsilon)
	{
		a  = m_Limit.x2 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j = 0; j < Circles; j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = 0;
			Vector->v.x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->v.y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->v.z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		glVertexCount += Circles;
		m_xVertices++;
	}

	glVertexElements->b3SetCount(glVertexCount);
}

void b3Shape::b3GetEllipsoidIndexCount(b3_count & gridCount, b3_count & polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_index ys, ye;
	b3_f64   y1, y2;

	b3ComputeBound(&m_Limit);
	m_Overhead = b3GetIndexOverhead(0.0, -1.0);
	if (m_Overhead < 0)
	{
		m_EndLine  = true;
		m_Overhead = -m_Overhead;
	}
	else
	{
		m_EndLine = false;
	}

	y1 = (m_Limit.y1 + 1) * SinCosSteps * 0.25;
	y2 = (m_Limit.y2 + 1) * SinCosSteps * 0.25;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	m_Heights = ye - ys;
	if ((ys - y1) > b3Math::epsilon)
	{
		m_Heights++;
	}
	if ((y2 - ye) > b3Math::epsilon)
	{
		m_Heights++;
	}

	m_Widths = m_Heights - 1;
	if ((SinCosSteps * 0.5 - y2) > b3Math::epsilon)
	{
		m_Widths++;
	}
	if (y1  > b3Math::epsilon)
	{
		m_Widths++;
	}

	gridCount = m_Heights * 2;
	polyCount = m_Heights * 2;
	if (y1 <= b3Math::epsilon)
	{
		polyCount--;
		gridCount--;
	}
	if ((SinCosSteps * 0.5 - y2) > b3Math::epsilon)
	{
		gridCount++;
	}
	gridCount *= m_Overhead;
	polyCount *= m_Overhead;
	if (m_EndLine)
	{
		gridCount += m_Heights;
	}
}

void b3Shape::b3ComputeEllipsoidIndices()
{
	b3_count       SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_index       i, j, s;
	b3_gl_line  *  gPtr = *glGridElements;
	b3_gl_polygon * pPtr = *glPolygonElements;
	b3_f64         y1, y2;

	s = 0;
	y1 = (m_Limit.y1 + 1) * SinCosSteps * 0.25;
	y2 = (m_Limit.y2 + 1) * SinCosSteps * 0.25;

	for (i = 0; i < m_Overhead; i++)
	{
		for (j = 0; j < m_Heights; j++)
		{
			// This marks a longitude
			B3_GL_LINIT(gPtr, s + j, s + j + 1);
		}

		// North pole
		if (y1 <= b3Math::epsilon)
		{
			// NOTE: j = 0 substitution
			B3_GL_PINIT(pPtr, s + m_Heights + 2, s + m_Heights + 1, s + 1);
			j = 1;
		}
		else
		{
			j = 0;
		}

		while (j < m_Heights)
		{
			B3_GL_LINIT(gPtr, s + j, s + j + m_Heights + 1);

			B3_GL_PINIT(pPtr, s + j,                s + j + 1,            s + j + m_Heights + 1);
			B3_GL_PINIT(pPtr, s + j + m_Heights + 2, s + j + m_Heights + 1, s + j + 1);
			j++;
		}

		if ((SinCosSteps * 0.5 - y2) > b3Math::epsilon)
		{
			B3_GL_LINIT(gPtr, s + j, s + j + m_Heights + 1);

			j++;
		}
		s += (m_Heights + 1);
	}

	if (m_EndLine)
	{
		for (j = 0; j < m_Heights; j++)
		{
			B3_GL_LINIT(gPtr, s + j, s + j + 1);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Box computation                               **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeBoxVertices(
	b3_vector  & Base,
	b3_vector  & Dir1,
	b3_vector  & Dir2,
	b3_vector  & Dir3)
{
	const b3_f32  *  tex_coord = m_BoxTexcoord;
	b3_vector  Aux;
	b3_index   i;
	b3_gl_vertex * glVertex = *glVertexElements;

	glVertex[0].t.s = *tex_coord++;
	glVertex[0].t.t = *tex_coord++;
	glVertex[0].v.x = Aux.x = Base.x;
	glVertex[0].v.y = Aux.y = Base.y;
	glVertex[0].v.z = Aux.z = Base.z;

	glVertex[1].t.s = *tex_coord++;
	glVertex[1].t.t = *tex_coord++;
	glVertex[1].v.x = (Aux.x += Dir1.x);
	glVertex[1].v.y = (Aux.y += Dir1.y);
	glVertex[1].v.z = (Aux.z += Dir1.z);

	glVertex[2].t.s = *tex_coord++;
	glVertex[2].t.t = *tex_coord++;
	glVertex[2].v.x = (Aux.x += Dir2.x);
	glVertex[2].v.y = (Aux.y += Dir2.y);
	glVertex[2].v.z = (Aux.z += Dir2.z);

	glVertex[3].t.s = *tex_coord++;
	glVertex[3].t.t = *tex_coord++;
	glVertex[3].v.x = (Aux.x -= Dir1.x);
	glVertex[3].v.y = (Aux.y -= Dir1.y);
	glVertex[3].v.z = (Aux.z -= Dir1.z);

	glVertex[4].t.s = *tex_coord++;
	glVertex[4].t.t = *tex_coord++;
	glVertex[4].v.x = (Aux.x += Dir3.x);
	glVertex[4].v.y = (Aux.y += Dir3.y);
	glVertex[4].v.z = (Aux.z += Dir3.z);

	glVertex[5].t.s = *tex_coord++;
	glVertex[5].t.t = *tex_coord++;
	glVertex[5].v.x = (Aux.x += Dir1.x);
	glVertex[5].v.y = (Aux.y += Dir1.y);
	glVertex[5].v.z = (Aux.z += Dir1.z);

	glVertex[6].t.s = *tex_coord++;
	glVertex[6].t.t = *tex_coord++;
	glVertex[6].v.x = (Aux.x -= Dir2.x);
	glVertex[6].v.y = (Aux.y -= Dir2.y);
	glVertex[6].v.z = (Aux.z -= Dir2.z);

	glVertex[7].t.s = *tex_coord++;
	glVertex[7].t.t = *tex_coord++;
	glVertex[7].v.x = (Aux.x -= Dir1.x);
	glVertex[7].v.y = (Aux.y -= Dir1.y);
	glVertex[7].v.z = (Aux.z -= Dir1.z);

	m_xVertices = m_yVertices = 1;

	for (i = 0; i < 8; i++)
	{
		glVertex[i +  8] = glVertex[i + 16] = glVertex[i];
	}
}

void b3Shape::b3ComputeBoxIndices()
{
	glGridElements->b3SetGrids((b3_gl_line *)m_BoxGrids);
	glPolygonElements->b3SetPolygons((b3_gl_polygon *)m_BoxPolygons);
}

/*************************************************************************
**                                                                      **
**                        Torus computation                             **
**                                                                      **
*************************************************************************/

void b3Shape::b3ComputeTorusVertices(
	b3_vector  & Base,
	b3_vector  & Dir1,
	b3_vector  & Dir2,
	b3_vector  & Dir3,
	b3_f64       aRad,
	b3_f64       bRad)
{
	b3_gl_vertex * Vector;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_f64        start, end, a;
	b3_f64        sx, RadX, LocalSin[B3_MAX_RENDER_SUBDIV + 1];
	b3_f64        sy, RadY, LocalCos[B3_MAX_RENDER_SUBDIV + 1];
	b3_f64        relTex[B3_MAX_RENDER_SUBDIV + 1];
	b3_index      i, j;
	b3_count      iMax, Circles = 0;
	b3_vector     Aux;
	b3_count      glVertexCount = 0;

	Vector = *glVertexElements;
	start  = m_Limit.y1 * SinCosSteps;
	end    = m_Limit.y2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > b3Math::epsilon)	/* underflow */
	{
		relTex[Circles]   = 0;
		LocalSin[Circles] = m_Limit.y1;
		Circles++;
	}
	a = 1.0 / SinCosSteps;
	for (j = i; j <= iMax; j++)		/* integers */
	{
		relTex[Circles]   = ((double)j / SinCosSteps) / (m_Limit.x2 - m_Limit.x1) - m_Limit.x1;
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > b3Math::epsilon)	/* Overflow */
	{
		relTex[Circles]   = 1;
		LocalSin[Circles] = m_Limit.y2;
		Circles++;
	}
	m_xVertices = 0;
	m_yVertices = Circles;

	for (j = 0; j < Circles; j++)		/* Calculate Values */
	{
		LocalCos[j] = cos(LocalSin[j] * M_PI * 2) * bRad;
		LocalSin[j] = sin(LocalSin[j] * M_PI * 2) * bRad;
	}

	start  = m_Limit.x1 * SinCosSteps;
	end    = m_Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);

	if ((i - start) > b3Math::epsilon)
	{
		a     = m_Limit.x1 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0; j < Circles; j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 0;
			Vector->t.t = relTex[j];
			Vector->n.x = Aux.x - (Vector->v.x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x);
			Vector->n.y = Aux.y - (Vector->v.y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y);
			Vector->n.z = Aux.z - (Vector->v.z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z);
			Vector++;
		}
		glVertexCount += Circles;
		m_xVertices++;
	}

	for (; i <= iMax; i++)
	{
		sx    = b3ShapeRenderContext::m_Cos[i];
		sy    = b3ShapeRenderContext::m_Sin[i];
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0; j < Circles; j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = ((double)i / SinCosSteps) / (m_Limit.x2 - m_Limit.x1) - m_Limit.x1;
			Vector->t.t = relTex[j];
			Vector->n.x = Aux.x - (Vector->v.x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x);
			Vector->n.y = Aux.y - (Vector->v.y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y);
			Vector->n.z = Aux.z - (Vector->v.z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z);
			Vector++;
		}
		glVertexCount += Circles;
		m_xVertices++;
	}

	if ((end - iMax) > b3Math::epsilon)
	{
		a     = m_Limit.x2 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j = 0; j < Circles; j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->t.s = 1;
			Vector->t.t = relTex[j];
			Vector->n.x = Aux.x - (Vector->v.x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x);
			Vector->n.y = Aux.y - (Vector->v.y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y);
			Vector->n.z = Aux.z - (Vector->v.z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z);
			Vector++;
		}
		glVertexCount += Circles;
		m_xVertices++;
	}

	glVertexElements->b3SetCount(glVertexCount);
}

void b3Shape::b3ComputeTorusNormals()
{
	b3_gl_vertex * glVertex = *glVertexElements;

	for (int i = 0; i < glVertexElements->b3GetCount(); i++)
	{
		b3Vector::b3Normalize(&glVertex[i].n);
	}
}

void b3Shape::b3GetTorusIndexCount(b3_count & gridCount, b3_count & polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_index ys, ye;
	bool     EndCol = false;
	b3_f64   y1, y2;

	b3ComputeBound(&m_Limit);
	m_Overhead = b3GetIndexOverhead(0.0, 0.0);
	if (m_Overhead < 0)
	{
		m_EndLine  =  true;
		m_Overhead = -m_Overhead;
	}
	else
	{
		m_EndLine = false;
	}

	y1 = m_Limit.y1 * SinCosSteps;
	y2 = m_Limit.y2 * SinCosSteps;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	m_Heights = ye - ys;
	if ((ys - y1) > b3Math::epsilon)
	{
		m_Heights++;
	}
	if ((y2 - ye) > b3Math::epsilon)
	{
		m_Heights++;
	}
	if ((ys > 0) || (ye < SinCosSteps))
	{
		EndCol = true;
	}

	m_Widths = m_Heights;
	if (EndCol)
	{
		m_Widths++;
	}

	gridCount = (m_Widths + m_Heights) * m_Overhead;
	if (m_EndLine)
	{
		gridCount += m_Heights;
	}
	polyCount = m_Heights * m_Overhead * 2;
}

void b3Shape::b3ComputeTorusIndices()
{
	b3_index       i, j, s;
	b3_gl_line  *  gPtr = *glGridElements;
	b3_gl_polygon * pPtr = *glPolygonElements;

	s = 0;
	for (i = 0; i < m_Overhead; i++)
	{
		for (j = 0; j < m_Heights; j++)
		{
			B3_GL_LINIT(gPtr, s + j, s + j + 1);

			B3_GL_PINIT(pPtr, s + j,                s + j + 1,            s + j + m_Heights + 1);
			B3_GL_PINIT(pPtr, s + j + m_Heights + 2, s + j + m_Heights + 1, s + j + 1);

		}

		for (j = 0; j < m_Widths; j++)
		{
			B3_GL_LINIT(gPtr, s + j, s + j + m_Heights + 1);
		}
		s += (m_Heights + 1);
	}

	if (m_EndLine)
	{
		for (j = 0; j < m_Heights; j++)
		{
			B3_GL_LINIT(gPtr, s + j, s + j + 1);
		}
	}
}
