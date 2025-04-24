/*
**
**	$Filename:	b3Shape.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
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

#include "blz3/raytrace/b3Bump.h"
#include "blz3/raytrace/b3Condition.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Shape::b3Register()
{
	b3PrintF(B3LOG_DEBUG, "Registering shapes...\n");
	b3Item::b3Register(&b3Sphere::b3StaticInit,           &b3Sphere::b3StaticInit,           SPHERE);
	b3Item::b3Register(&b3Area::b3StaticInit,             &b3Area::b3StaticInit,             AREA);
	b3Item::b3Register(&b3Disk::b3StaticInit,             &b3Disk::b3StaticInit,             DISK);
	b3Item::b3Register(&b3Cylinder::b3StaticInit,         &b3Cylinder::b3StaticInit,         CYLINDER);
	b3Item::b3Register(&b3Cone::b3StaticInit,             &b3Cone::b3StaticInit,             CONE);
	b3Item::b3Register(&b3Ellipsoid::b3StaticInit,        &b3Ellipsoid::b3StaticInit,        ELLIPSOID);
	b3Item::b3Register(&b3Box::b3StaticInit,              &b3Box::b3StaticInit,              BOX);
	b3Item::b3Register(&b3Torus::b3StaticInit,            &b3Torus::b3StaticInit,            TORUS);
	b3Item::b3Register(&b3Triangles::b3StaticInit,        &b3Triangles::b3StaticInit,        TRIANGLES);
	b3Item::b3Register(&b3SplineRotShape::b3StaticInit,   &b3SplineRotShape::b3StaticInit,   SPLINE_ROT);
	b3Item::b3Register(&b3SplineArea::b3StaticInit,       &b3SplineArea::b3StaticInit,       SPLINES_AREA);
	b3Item::b3Register(&b3SplineCylinder::b3StaticInit,   &b3SplineCylinder::b3StaticInit,   SPLINES_CYL);
	b3Item::b3Register(&b3SplineRing::b3StaticInit,       &b3SplineRing::b3StaticInit,       SPLINES_RING);
	b3Item::b3Register(&b3CSGSphere::b3StaticInit,        &b3CSGSphere::b3StaticInit,        CSG_SPHERE);
	b3Item::b3Register(&b3CSGCylinder::b3StaticInit,      &b3CSGCylinder::b3StaticInit,      CSG_CYLINDER);
	b3Item::b3Register(&b3CSGCone::b3StaticInit,          &b3CSGCone::b3StaticInit,          CSG_CONE);
	b3Item::b3Register(&b3CSGEllipsoid::b3StaticInit,     &b3CSGEllipsoid::b3StaticInit,     CSG_ELLIPSOID);
	b3Item::b3Register(&b3CSGBox::b3StaticInit,           &b3CSGBox::b3StaticInit,           CSG_BOX);
	b3Item::b3Register(&b3CSGTorus::b3StaticInit,         &b3CSGTorus::b3StaticInit,         CSG_TORUS);
}

/*************************************************************************
**                                                                      **
**                        b3Activation base class                       **
**                                                                      **
*************************************************************************/

b3Activation::b3Activation()
{
	b3Activate(false);
	b3Animate(B3_ANIM_DISABLED);
}

bool b3Activation::b3IsActive() const
{
	switch (m_AnimActive)
	{
	case B3_ANIM_DISABLED:
		return m_Active;

	case B3_ANIM_DEACTIVE:
		return false;

	case B3_ANIM_ACTIVE:
		return true;
	}
	return false;
}

void b3Activation::b3Activate(bool activate)
{
	m_Active = activate;
}

void b3Activation::b3Animate(bool activate)
{
	m_AnimActive = activate ? B3_ANIM_ACTIVE : B3_ANIM_DEACTIVE;
}

void b3Activation::b3Animate(b3_anim_activation activate)
{
	m_AnimActive = activate;
}

/*************************************************************************
**                                                                      **
**                        shape base class                              **
**                                                                      **
*************************************************************************/

b3Shape::b3Shape(
	const b3_size class_size,
	const b3_u32  class_type) : b3Item(class_size, class_type)
{
	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BUMP);
	m_Heads[1].b3InitBase(CLASS_CONDITION);
	m_Heads[2].b3InitBase(CLASS_MATERIAL);
}

b3Shape::b3Shape(const b3_u32 class_type) : b3Item(sizeof(b3Shape), class_type)
{
	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BUMP);
	m_Heads[1].b3InitBase(CLASS_CONDITION);
	m_Heads[2].b3InitBase(CLASS_MATERIAL);
}

b3Shape::b3Shape(const b3_u32 * src) : b3Item(src)
{
	b3InitVector(); // This is the normal
	b3InitVector(); // This is Polar.Polar
	b3InitVector(); // This is Polar.ObjectPolar
	b3InitVector(); // This is Polar.BoxPolar
	b3InitNOP();    // This is Custom
}

void b3Shape::b3InitActivation()
{
	b3Activate(B3_PARSE_INDEX_VALID ? b3InitBool() : false);
}

void b3Shape::b3Write()
{
	b3StoreVector(); // This is the normal
	b3StoreVector(); // This is Polar.Polar
	b3StoreVector(); // This is Polar.ObjectPolar
	b3StoreVector(); // This is Polar.BoxPolar
	b3StoreNull();   // This is Custom

	// Trick! Store custom shape data...
	b3StoreShape();

	// and append activation
	b3StoreBool(b3IsActive());
}

void b3Shape::b3StoreShape()
{
}

bool b3Shape::b3Prepare(b3_preparation_info * prep_info)
{
	B3_FOR_TYPED_BASE(b3Condition, b3GetConditionHead(), cond)
	{
		if (!cond->b3Prepare(prep_info))
		{
			return false;
		}
	}

	B3_FOR_TYPED_BASE(b3Bump, b3GetBumpHead(), bump)
	{
		if (!bump->b3Prepare(prep_info))
		{
			return false;
		}
	}

	B3_FOR_TYPED_BASE(b3Material, b3GetMaterialHead(), material)
	{
		if (!material->b3Prepare(prep_info))
		{
			return false;
		}
	}

	return true;
}

void b3Shape::b3BumpNormal(b3_ray * ray) const
{
	bool     deriv_computed = false;
	bool     deriv_ok       = false;

	b3Normal(ray);
	B3_FOR_TYPED_BASE(b3Bump, b3GetBumpHead(), bump)
	{
		if (bump->b3NeedDeriv())
		{
			if (!deriv_computed)
			{
				deriv_ok       = b3NormalDeriv(ray);
				deriv_computed = true;
			}
			if (deriv_ok)
			{
				bump->b3BumpNormal(ray);
			}
		}
		else
		{
			bump->b3BumpNormal(ray);
		}
	}

	b3Vector::b3Normalize(&ray->normal);
}

void b3Shape::b3SetupPicking(b3PickInfo * info B3_UNUSED)
{
}

void b3Shape::b3SetupGrid(b3PickInfo * info B3_UNUSED)
{
}

b3Material * b3Shape::b3GetSurfaceValues(b3_surface * surface) const
{
	B3_FOR_TYPED_BASE(b3Material, b3GetMaterialHead(), material)
	{
		if (material->b3GetSurfaceValues(surface))
		{
			return material;
		}
	}
	surface->m_Diffuse.b3Init(0.1f, 0.5f, 1.0f, 0.0f);
	surface->m_Ambient.b3Init(0.05f, 0.25f, 0.5f, 0.0f);
	surface->m_Specular.b3Init(0.1f, 0.1f, 0.1f, 0.0f);

	surface->m_Reflection  =      0.0f;
	surface->m_Refraction  =      0.0f;
	surface->m_Ior         =      1.0f;
	surface->m_SpecularExp = 100000.0f;

	return nullptr;
}

bool b3Shape::b3CheckStencil(b3_polar * polar B3_UNUSED) const
{
	return true;
}

void b3Shape::b3GetStencilBoundInfo(b3_stencil_bound * info) const
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_UNIT;
	info->yInfo.unit   = B3_STENCIL_UNIT;
	info->xInfo.factor = 1;
	info->yInfo.factor = 1;
}

void b3Shape::b3Transform(
	b3_matrix * transformation B3_UNUSED,
	bool        is_affine B3_UNUSED)
{
	b3PrintF(B3LOG_NORMAL, "b3Shape::b3Transform() not overloaded!\n");
	B3_ASSERT(true);
}

/*************************************************************************
**                                                                      **
**                        Simple shapes (non CSG)                       **
**                                                                      **
*************************************************************************/

b3SimpleShape::b3SimpleShape(
	const b3_size class_size,
	const b3_u32  class_type) : b3Shape(class_size, class_type)
{
}

b3SimpleShape::b3SimpleShape(const b3_u32 class_type) : b3Shape(sizeof(b3SimpleShape), class_type)
{
}

b3SimpleShape::b3SimpleShape(const b3_u32 * src) : b3Shape(src)
{
}

bool b3SimpleShape::b3CheckStencil(b3_polar * polar) const
{
	bool         result = true;

	B3_FOR_TYPED_BASE(b3Condition, b3GetConditionHead(), cond)
	{
		result = cond->b3Conditionate(result, cond->b3CheckStencil(polar));
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        Base class for area shapes                    **
**                                                                      **
*************************************************************************/

b3Shape2::b3Shape2(
	const b3_size class_size,
	const b3_u32  class_type) : b3SimpleShape(class_size, class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1, 50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0, 50, 0);
}

b3Shape2::b3Shape2(const b3_u32 class_type) : b3SimpleShape(sizeof(b3Shape2), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1, 50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0, 50, 0);
}

b3Shape2::b3Shape2(const b3_u32 * src) : b3SimpleShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitFloat(); // This is m_NormalLength
	b3InitActivation();
}

void b3Shape2::b3StoreShape()
{
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreFloat(m_NormalLength);
}

bool b3Shape2::b3Prepare(b3_preparation_info * prep_info)
{
	b3Vector::b3CrossProduct(&m_Dir1, &m_Dir2, &m_Normal);
	m_NormalLength = b3Vector::b3Length(&m_Normal);
	return b3Shape::b3Prepare(prep_info);
}

void b3Shape2::b3Transform(b3_matrix * transformation, bool is_affine)
{
	b3Matrix::b3VMul(transformation, &m_Base, &m_Base, true);
	b3Matrix::b3VMul(transformation, &m_Dir1, &m_Dir1, false);
	b3Matrix::b3VMul(transformation, &m_Dir2, &m_Dir2, false);
	b3TransformVertices(transformation, is_affine);
}

void b3Shape2::b3SetupPicking(b3PickInfo * info)
{
	info->b3AddPickPoint(&m_Base, "b");
	info->b3AddPickDir(&m_Base, &m_Dir1, "1");
	info->b3AddPickDir(&m_Base, &m_Dir2, "2");
}

/*************************************************************************
**                                                                      **
**                        Base class for simple quadric shapes          **
**                                                                      **
*************************************************************************/

b3Shape3::b3Shape3(
	const b3_size class_size,
	const b3_u32  class_type) : b3SimpleShape(class_size, class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1, 50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0, 50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0, 50);
}

b3Shape3::b3Shape3(const b3_u32 class_type) : b3SimpleShape(sizeof(b3Shape3), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1, 50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0, 50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0, 50);
}

b3Shape3::b3Shape3(const b3_u32 * src) : b3SimpleShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);
	b3InitInt();   // This is lSize
	b3InitFloat(); // This is m_Denom
	b3InitFloat(); // This is m_DirLen[0]
	b3InitFloat(); // This is m_DirLen[1]
	b3InitFloat(); // This is m_DirLen[2]
	b3InitActivation();
}

void b3Shape3::b3StoreShape()
{
	for (b3_loop i = 0; i < 3; i++)
	{
		b3_vector normal;

		b3Vector::b3Init(&normal, &m_Normals[i]);
		b3StoreVector(&normal);
	}
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreVector(&m_Dir3);
	b3StoreInt(0); // This is lSize
	b3StoreFloat(m_Denom);
	b3StoreFloat(m_DirLen[0]);
	b3StoreFloat(m_DirLen[1]);
	b3StoreFloat(m_DirLen[2]);
}

bool b3Shape3::b3Prepare(b3_preparation_info * prep_info)
{
	bool result = false;

	if (b3BaseTransformation::b3Prepare())
	{
		result = b3Shape::b3Prepare(prep_info);
	}
	return result;
}

void b3Shape3::b3GetStencilBoundInfo(b3_stencil_bound * info) const
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_ANGLE;
	info->yInfo.unit   = B3_STENCIL_LENGTH;
	info->xInfo.factor = 360;
	info->yInfo.factor = b3Vector::b3Length(&m_Dir3);;
}

void b3Shape3::b3ComputeNormals(bool normalize)
{
	b3ComputeQuadricNormals(normalize);
}

void b3Shape3::b3Transform(b3_matrix * transformation, bool is_affine)
{
	b3Matrix::b3VMul(transformation, &m_Base, &m_Base, true);
	b3Matrix::b3VMul(transformation, &m_Dir1, &m_Dir1, false);
	b3Matrix::b3VMul(transformation, &m_Dir2, &m_Dir2, false);
	b3Matrix::b3VMul(transformation, &m_Dir3, &m_Dir3, false);
	b3TransformVertices(transformation, is_affine);
}

void b3Shape3::b3SetupPicking(b3PickInfo * info)
{
	info->b3AddPickPoint(&m_Base, "b");
	info->b3AddPickDir(&m_Base, &m_Dir1, "1");
	info->b3AddPickDir(&m_Base, &m_Dir2, "2");
	info->b3AddPickDir(&m_Base, &m_Dir3, "3");
}
