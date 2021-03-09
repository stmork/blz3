/*
**
**	$Filename:	b3AnimElement.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing Animation
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        b3AnimControl implementation                  **
**                                                                      **
*************************************************************************/

b3_f64 b3AnimElement::epsilon = 1.0 / 2048;

#define ANIM_STEP (2.0 * b3AnimElement::epsilon)

b3AnimControl::b3AnimControl(b3_u32 class_type B3_UNUSED) :
	b3Item(sizeof(b3AnimControl), CPOINT_4D)
{
	m_Dimension = 4;
	m_Used      = 0;
	m_Max       = B3_MAX_CONTROLS;
	m_Controls  = (b3_vector4D *)b3Alloc(m_Max * sizeof(b3_vector4D));
}

b3AnimControl::b3AnimControl(b3_u32 * src) : b3Item(src)
{
	b3_index i;

	m_Dimension = b3InitInt();
	m_Used      = b3InitInt();
	m_Max       = b3InitInt();

	m_Controls  = (b3_vector4D *)b3Alloc(m_Max * sizeof(b3_vector4D));
	if (m_Controls == nullptr)
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}
	for (i = 0; i < m_Max; i++)
	{
		m_Controls[i].x = b3InitFloat();
		m_Controls[i].y = b3InitFloat();
		m_Controls[i].z = b3InitFloat();
		m_Controls[i].w = (m_Dimension == 4 ? b3InitFloat() : 1.0f);
	}

	// Force homogenous coordinates
	ClassType   = CPOINT_4D;
	m_Dimension = 4;
}

void b3AnimControl::b3Write()
{
	b3_index i;

	b3StoreCount(m_Dimension);
	b3StoreCount(m_Used);
	b3StoreCount(m_Max);

	for (i = 0; i < m_Max; i++)
	{
		b3StoreVector4D(&m_Controls[i]);
	}
}

void b3AnimControl::b3InitNurbs(b3Nurbs & nurbs)
{
	if (nurbs.m_Controls == nullptr)
	{
		nurbs.m_ControlMax = m_Max;
		nurbs.m_Controls   = m_Controls;
	}

	m_Used = nurbs.m_ControlNum;
}

/*************************************************************************
**                                                                      **
**                        b3AnimElement implementation                  **
**                                                                      **
*************************************************************************/

b3AnimElement::b3AnimElement(b3_u32 class_type) : b3Item(sizeof(b3AnimElement), class_type)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_VERTEX);
}

b3AnimElement::b3AnimElement(b3_u32 * src) : b3Item(src)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_VERTEX);

	b3InitNOP();
	if (m_ItemSize > 0x1ec)
	{
		b3InitVector(&m_Center);
		b3InitMatrix(&m_Actual);
		b3InitMatrix(&m_NeutralInverse);
	}
	else
	{
		b3Vector::b3Init(&m_Center);
		b3InitMatrix(&m_Actual);
		b3Matrix::b3Unit(&m_NeutralInverse);
	}
	m_Ratio      = b3InitFloat();
	m_Empty      = b3InitInt();
	m_Start      = b3InitFloat();
	m_End        = b3InitFloat();
	m_Flags      = b3InitInt();
	m_TrackIndex = b3InitInt();
	m_CurveUse   = (m_ItemSize > 0x1ec ? b3InitInt() : 0);

	// Init nurbs
	b3InitNurbs(&m_Param, nullptr, m_Knots);
	for (b3_index i = 0; i < m_Param.m_KnotMax; i++)
	{
		m_Knots[i] = b3InitFloat();
	}

	b3InitString(m_Name,  B3_ANIMSTRINGLEN);
	b3InitString(m_Object, B3_BOXSTRINGLEN);
}

void b3AnimElement::b3Write()
{
	b3StorePtr(nullptr);
	b3StoreVector(&m_Center);
	b3StoreMatrix(&m_Actual);
	b3StoreMatrix(&m_NeutralInverse);
	b3StoreFloat(m_Ratio);
	b3StoreInt(m_Empty);
	b3StoreFloat(m_Start);
	b3StoreFloat(m_End);
	b3StoreInt(m_Flags);
	b3StoreIndex(m_TrackIndex);
	b3StoreBool(m_CurveUse);

	// Init nurbs
	b3StoreNurbs(&m_Param);
	for (b3_index i = 0; i < m_Param.m_KnotMax; i++)
	{
		b3StoreFloat(m_Knots[i]);
	}

	b3StoreString(m_Name,  B3_ANIMSTRINGLEN);
	b3StoreString(m_Object, B3_BOXSTRINGLEN);
}

char * b3AnimElement::b3GetName()
{
	return m_Name;
}

/* This routine computes the point on a curve at a specified time point. */
/* The time point must be valid to compute the curve point at the time */
/* point. So use ClipValue() to clip at the time bounds. */
/* --------------------------------------------------------------------- */
/* Element:  where the NURBS curve is specified */
/* position: where to store the curve point */
/* t:        the time point of the curve point */

void b3AnimElement::b3GetPosition(b3_vector32_4D * position, b3_f64 t)
{
	b3AnimControl * ctrl = (b3AnimControl *)m_Heads[0].First;
	b3_f64         q, qStart, ratio;
	b3_index       pos;
	b3_f64         coeffs[B3_MAX_DEGREE + 1];

	if (ctrl != nullptr)
	{
		// Init controls
		ctrl->b3InitNurbs(m_Param);

		// compute predefined values
		qStart = m_Knots[m_Param.m_Closed ? 0 : m_Param.m_Degree];
		q      = m_Knots[m_Param.m_ControlNum] - qStart;

		// compute position at time t
		ratio = (t - m_Start) / (m_End - m_Start);
		pos = m_Param.b3Mansfield(coeffs, qStart + q * ratio);
		m_Param.b3MansfieldVector(position, coeffs, pos, 0);
	}
}

void b3AnimElement::b3GetPosition(b3_vector * position, b3_f64 t)
{
	b3_vector32_4D dummy;

	b3GetPosition(&dummy, t);
	position->x = dummy.x;
	position->y = dummy.y;
	position->z = dummy.z;
}



/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3AnimElement::b3AnimateMove(
	b3Animation * AnimRoot B3_UNUSED,
	b3_matrix  *  transform,
	b3_f64        t)
{
	b3_vector move;

	// now compute transformation
	b3GetPosition(&move, t);
	b3Matrix::b3Move(&m_NeutralInverse, transform, &move);
}

/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3AnimElement::b3AnimateRotate(
	b3Animation * AnimRoot,
	b3_matrix  * transform,
	b3_f64       t)
{
	b3_f64     t1, t2, tRef;
	b3_vector  lookTo;
	b3_vector  refDir;
	b3_vector  refCenter;
	b3_bool    future;

	t1 = b3Math::b3Round(b3Math::b3Limit(t - ANIM_STEP, m_Start, m_End), epsilon);
	t2 = b3Math::b3Round(b3Math::b3Limit(t + ANIM_STEP, m_Start, m_End), epsilon);
	if (t1 != t2)
	{
		// Compute present orientation
		b3GetPosition(&lookTo, t);
		lookTo.x -= m_Center.x;
		lookTo.y -= m_Center.y;
		lookTo.z -= m_Center.z;

		// Which reference to use.
		future = (t == t1);

		// Use future or past reference time?
		tRef = future ? t2 : t1;

		// Compute reference
		AnimRoot->b3RecomputeCenter(this, &refCenter, tRef);
		b3GetPosition(&refDir, tRef);

		refDir.x -= refCenter.x;
		refDir.y -= refCenter.y;
		refDir.z -= refCenter.z;

		// Orientate
		b3Matrix::b3Dress(&m_NeutralInverse, transform,
			&m_Center, &lookTo, &refDir, future);
	}
	else
	{
		*transform = m_NeutralInverse;
	}
}

/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3AnimElement::b3AnimateScale(
	b3Animation * AnimRoot B3_UNUSED,
	b3_matrix  *  transform,
	b3_f64        t)
{
	b3_vector scale;

	b3GetPosition(&scale, t);
	scale.x -= m_Center.x;
	scale.y -= m_Center.y;
	scale.z -= m_Center.z;
	b3Matrix::b3Scale(&m_NeutralInverse, transform, &m_Center, &scale);
}

/* This routine computes the transformation matrix for one animation */
/* element depending of its transformation type and the global time point. */
/* ----------------------------------------------------------------------- */
/* Anim:      animation element */
/* transform: where to store the transformation matrix */
/* t:         time point */

void b3AnimElement::b3ComputeTransformationMatrix(
	b3Animation * AnimRoot,
	b3_matrix  * transform,
	b3_f64       t)
{
	switch (b3GetClassType())
	{
	case ANIM_MOVE :
		b3AnimateMove(AnimRoot, transform, t);
		break;

	case ANIM_ROTATE :
		b3AnimateRotate(AnimRoot, transform, t);
		break;

	case ANIM_SCALE :
		b3AnimateScale(AnimRoot, transform, t);
		break;
	}
}

void b3BBox::b3Animate(b3Activation::b3_anim_activation animate, b3_bool recurse)
{
	b3BBox * bbox;
	b3Shape * shape;
	b3Item * item;

	B3_FOR_BASE(b3GetShapeHead(), item)
	{
		shape = (b3Shape *)item;
		shape->b3Animate(animate);
	}

	if (recurse)
	{
		B3_FOR_BASE(b3GetBBoxHead(), item)
		{
			bbox = (b3BBox *)item;
			bbox->b3Animate(animate, recurse);
		}
	}
}

void b3Scene::b3Animate(b3Activation::b3_anim_activation animate)
{
	b3BBox * bbox;
	b3Item * item;

	B3_FOR_BASE(b3GetBBoxHead(), item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Animate(animate, true);
	}
}

void b3AnimElement::b3SelectObjects(b3BBox * bbox)
{
	while (bbox != nullptr)
	{
		if (stricmp(bbox->b3GetName(), m_Object) == 0)
		{
			bbox->b3Animate(b3Activation::B3_ANIM_ACTIVE, (m_Flags & ANIMFLAGF_RECURSIVE) != 0);
		}
		b3SelectObjects((b3BBox *)bbox->b3GetBBoxHead()->First);
		bbox = (b3BBox *)bbox->Succ;
	}
}

/* This routine sets the activation state of a bounding box depending */
/* of the box name. */
/* ------------------------------------------------------------------ */
/* Global:  geometry structure */
/* Element: amimation track */

b3_bool b3AnimElement::b3SelectAnimElement(b3Scene * scene)
{
	scene->b3Animate(b3Activation::B3_ANIM_DEACTIVE);
	if (m_Flags & ANIMFLAGF_OBJECT)
	{
		b3SelectObjects(scene->b3GetFirstBBox());
	}

	return (m_Flags & ANIMFLAGF_OBJECT) != 0;
}


/* This routine takes a transformation matrix and applies them to an */
/* animation elements. All possible destination objects */
/* (lights, cameras and objects) are stored in the geometry structure. */
/* ------------------------------------------------------------------- */
/* Global:    geometry structure */
/* Anim:      one animation element */
/* transform: transformation matrix to apply */

void b3Animation::b3ApplyTransformation(
	b3Scene    *   Global,
	b3AnimElement * Anim,
	b3_matrix   *  transform,
	b3_f64         t)
{
	b3Light   *   Light;
	b3CameraPart * Camera;
	b3_f64        twirl, width, height, focal;
	b3_vector     diff;

	if (Anim->m_Flags & ANIMFLAGF_LIGHT)
	{
		Light = Global->b3GetLightByName(Anim->m_Object);
		if (Light != nullptr)
		{
			b3PrintF(B3LOG_FULL, "  ANIM light %s\n", Anim->m_Object);
			Anim->b3GetPosition(&Light->m_Position,
				b3Math::b3Limit(t, Anim->m_Start, Anim->m_End));
			if ((Light->m_SpotActive) &&
				(Anim->b3GetClassType() == ANIM_ROTATE))
			{
				Light->m_Direction.x = Anim->m_Center.x - Light->m_Position.x;
				Light->m_Direction.y = Anim->m_Center.y - Light->m_Position.y;
				Light->m_Direction.z = Anim->m_Center.z - Light->m_Position.z;
			}
		}
	}

	if (Anim->m_Flags & ANIMFLAGF_CAMERA)
	{
		Camera = Global->b3GetCameraByName(Anim->m_Object);
		if ((Camera != nullptr) && (t >= Anim->m_Start))
		{
			twirl  = Camera->b3GetTwirl();
			focal  = Camera->b3GetFocalLength();
			width  = b3Vector::b3Length(&Camera->m_Width);
			height = b3Vector::b3Length(&Camera->m_Height);
			diff   = Camera->m_EyePoint;
			Anim->b3GetPosition(&Camera->m_EyePoint,
				b3Math::b3Limit(t, Anim->m_Start, Anim->m_End));

			if (Anim->b3GetClassType() == ANIM_ROTATE)
			{
				b3PrintF(B3LOG_FULL, "  ANIM camera %s (rotate)\n", Anim->m_Object);
				Camera->b3Orientate(&Camera->m_EyePoint, &Anim->m_Center, focal, width, height);
				Camera->b3SetTwirl(twirl);
			}

			if (Anim->b3GetClassType() == ANIM_MOVE)
			{
				b3PrintF(B3LOG_FULL, "  ANIM camera %s (move)\n", Anim->m_Object);
				Camera->m_ViewPoint.x += (diff.x - Camera->m_EyePoint.x);
				Camera->m_ViewPoint.y += (diff.y - Camera->m_EyePoint.y);
				Camera->m_ViewPoint.z += (diff.z - Camera->m_EyePoint.z);
			}
		}
	}

	if (Anim->m_Flags & ANIMFLAGF_OBJECT)
	{
		Anim->b3SelectAnimElement(Global);

		b3PrintF(B3LOG_FULL, "  ANIM object %s (move)\n", Anim->m_Object);
		Global->b3Transform(transform, Anim->b3GetClassType() != ANIM_SCALE);
	}
}
