/*
**
**      $Filename:      b3AnimElement.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing Animation
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

#include "blz3/raytrace/b3Animation.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Light.h"
#include "blz3/raytrace/b3Special.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.10  2004/06/18 14:49:05  sm
**      - Some probes concerning the anim rotation problem. Should I use
**        quaternions?
**
**      Revision 1.9  2004/05/31 08:14:17  sm
**      - Added autorun infos
**      - Fixed rounding problem of animation.
**
**      Revision 1.8  2004/05/30 20:25:00  sm
**      - Set paging size in supersampling dialog to 1 instead of 10.
**      - Added support for debugging super sampling.
**      - The object preview uses the shading model of its owning world.
**      - Fixed animation problem when using rotating elements on
**        time bounds because of rounding problems. Now using
**        b3_f32 for time points.
**
**      Revision 1.7  2004/04/17 09:40:54  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.6  2003/10/15 13:12:19  sm
**      - Making 64 bit sure. I've got running the brt3 in 64 bit mode on
**        a sun ultra sparc sucessfully!!
**
**      Revision 1.5  2003/02/22 17:21:34  sm
**      - Changed some global variables into static class members:
**        o b3Scene::epsilon
**        o b3Scene::m_TexturePool et. al.
**        o b3SplineTemplate<class VECTOR>::bspline_errno
**
**      Revision 1.4  2003/02/18 16:52:57  sm
**      - Fixed no name error on new scenes (ticket no. 4).
**      - Introduced new b3Matrix class and renamed methods.
**
**      Revision 1.3  2002/08/19 16:50:39  sm
**      - Now having animation running, running, running...
**      - Activation handling modified to reflect animation
**        and user transformation actions.
**      - Made some architectual redesigns.
**
**      Revision 1.2  2002/08/18 13:05:17  sm
**      - First try to animate. We have to relink the control points which
**        are stored in separate Blizzard classes to the b3AnimElement
**        class.
**
**      Revision 1.1  2002/08/17 17:31:22  sm
**      - Introduced animation support (Puh!)
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3AnimControl implementation                  **
**                                                                      **
*************************************************************************/

b3_f64 b3AnimElement::epsilon = 0.0005;

#define ANIM_STEP (2.0 * b3AnimElement::epsilon)

b3AnimControl::b3AnimControl(b3_u32 class_type) : b3Item(sizeof (b3AnimControl),CPOINT_4D)
{
	m_Dimension = 4;
	m_Used      = 0;
	m_Max       = B3_MAX_CONTROLS;
	m_Controls  = (b3_vector4D *)b3Alloc(m_Max * sizeof(b3_vector4D));
}

b3AnimControl::b3AnimControl(b3_u32 *src) : b3Item(src)
{
	b3_index i;

	m_Dimension = b3InitInt();
	m_Used      = b3InitInt();
	m_Max       = b3InitInt();

	m_Controls  = (b3_vector4D *)b3Alloc(m_Max * sizeof(b3_vector4D));
	if (m_Controls == null)
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}
	for (i = 0;i < m_Max;i++)
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

	for (i = 0;i < m_Max;i++)
	{
		b3StoreVector4D(&m_Controls[i]);
	}
}

void b3AnimControl::b3InitNurbs(b3Nurbs &nurbs)
{
	if (nurbs.controls == null)
	{
		nurbs.control_max = m_Max;
		nurbs.controls    = m_Controls;
	}

	m_Used = nurbs.control_num;
}

/*************************************************************************
**                                                                      **
**                        b3AnimElement implementation                  **
**                                                                      **
*************************************************************************/

b3AnimElement::b3AnimElement(b3_u32 class_type) : b3Item(sizeof (b3AnimElement),class_type)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase(CLASS_VERTEX);
}

b3AnimElement::b3AnimElement(b3_u32 *src) : b3Item(src)
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
	b3InitNurbs(&m_Param,null,m_Knots);
	for (b3_index i = 0;i < m_Param.knot_max;i++)
	{
		m_Knots[i] = b3InitFloat();
	}

	b3InitString(m_Name,  B3_ANIMSTRINGLEN);
	b3InitString(m_Object,B3_BOXSTRINGLEN);
}

void b3AnimElement::b3Write()
{
	b3StorePtr   (null);
	b3StoreVector(&m_Center);
	b3StoreMatrix(&m_Actual);
	b3StoreMatrix(&m_NeutralInverse);
	b3StoreFloat(m_Ratio);
	b3StoreInt  (m_Empty);
	b3StoreFloat(m_Start);
	b3StoreFloat(m_End);
	b3StoreInt  (m_Flags);
	b3StoreIndex(m_TrackIndex);
	b3StoreBool (m_CurveUse);

	// Init nurbs
	b3StoreNurbs(&m_Param);
	for (b3_index i = 0;i < m_Param.knot_max;i++)
	{
		b3StoreFloat(m_Knots[i]);
	}

	b3StoreString(m_Name,  B3_ANIMSTRINGLEN);
	b3StoreString(m_Object,B3_BOXSTRINGLEN);
}

char *b3AnimElement::b3GetName()
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

void b3AnimElement::b3GetPosition(b3_vector32_4D *position,b3_f64 t)
{
	b3AnimControl *ctrl = (b3AnimControl *)m_Heads[0].First;
	b3_f64         q,qStart,ratio;
	b3_index       pos;
	b3_f64         coeffs[B3_MAX_DEGREE+1];

	if (ctrl != null)
	{
		// Init controls
		ctrl->b3InitNurbs(m_Param);

		// compute predefined values
		qStart = m_Knots[m_Param.closed ? 0 : m_Param.degree];
		q      = m_Knots[m_Param.control_num] - qStart;

		// compute position at time t
		ratio = (t - m_Start) / (m_End - m_Start);
		pos = m_Param.b3Mansfield (coeffs,qStart + q * ratio);
		m_Param.b3MansfieldVector(position,coeffs,pos,0);
	}
}

void b3AnimElement::b3GetPosition(b3_vector *position,b3_f64 t)
{
	b3_vector32_4D dummy;

	b3GetPosition(&dummy,t);
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
	b3Animation *AnimRoot,
	b3_matrix   *transform,
	b3_f64       t)
{
	b3_vector move;

	// now compute transformation
	b3GetPosition (&move,t);
	b3Matrix::b3Move (&m_NeutralInverse,transform,&move);
}

/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3AnimElement::b3AnimateRotate(
	b3Animation *AnimRoot,
	b3_matrix   *transform,
	b3_f64       tParam)
{
	b3_f64     t1,t2,t = b3Round(tParam);
	b3_bool    negate;
	b3_vector  lookTo;
	b3_vector  oldDir;
	b3_vector  oldCenter;

	t1 = b3Round(b3Math::b3Limit (t - ANIM_STEP,m_Start,m_End));
	t2 = b3Round(b3Math::b3Limit (t + ANIM_STEP,m_Start,m_End));
	if (t1 != t2)
	{
		b3GetPosition (&lookTo, t);
		if (t != t1)
		{
			AnimRoot->b3RecomputeCenter (this,&oldCenter,t1);
			b3GetPosition     (&oldDir,t1);
			negate = false;
		}
		else
		{
			AnimRoot->b3RecomputeCenter (this,&oldCenter,t2);
			b3GetPosition     (&oldDir,t2);
			negate = true;
		}

		lookTo.x -= m_Center.x;
		lookTo.y -= m_Center.y;
		lookTo.z -= m_Center.z;
		oldDir.x -= oldCenter.x;
		oldDir.y -= oldCenter.y;
		oldDir.z -= oldCenter.z;
		b3Matrix::b3Dress (&m_NeutralInverse,transform,
			&m_Center,&lookTo,&oldDir,negate);
		if (fabs(b3Matrix::b3Det4(transform)) < b3Scene::epsilon)
		{
			b3Matrix::b3Unit (transform);
		}
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
	b3Animation *AnimRoot,
	b3_matrix   *transform,
	b3_f64       t)
{
	b3_vector scale;

	b3GetPosition (&scale,t);
	scale.x -= m_Center.x;
	scale.y -= m_Center.y;
	scale.z -= m_Center.z;
	b3Matrix::b3Scale (&m_NeutralInverse,transform,&m_Center,&scale);
}

/* This routine computes the transformation matrix for one animation */
/* element depending of its transformation type and the global time point. */
/* ----------------------------------------------------------------------- */
/* Anim:      animation element */
/* transform: where to store the transformation matrix */
/* t:         time point */

void b3AnimElement::b3ComputeTransformationMatrix(
	b3Animation *AnimRoot,
	b3_matrix   *transform,
	b3_f64       t)
{
	switch (b3GetClassType())
	{
		case ANIM_MOVE :
			b3AnimateMove   (AnimRoot,transform,t);
			break;

		case ANIM_ROTATE :
			b3AnimateRotate (AnimRoot,transform,t);
			break;

		case ANIM_SCALE :
			b3AnimateScale  (AnimRoot,transform,t);
			break;
	}
}

void b3BBox::b3Animate(b3Activation::b3_anim_activation animate,b3_bool recurse)
{
	b3BBox *bbox;
	b3Shape *shape;
	b3Item  *item;

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		shape->b3Animate(animate);
	}

	if (recurse)
	{
		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			bbox->b3Animate(animate,recurse);
		}
	}
}

void b3Scene::b3Animate(b3Activation::b3_anim_activation animate)
{
	b3BBox *bbox;
	b3Item  *item;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3Animate(animate,true);
	}
}

void b3AnimElement::b3SelectObjects (b3BBox *bbox)
{
	while (bbox != null)
	{
		if (stricmp(bbox->b3GetName(),m_Object) == 0)
		{
			bbox->b3Animate(b3Activation::B3_ANIM_ACTIVE,(m_Flags & ANIMFLAGF_RECURSIVE) != 0);
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

b3_bool b3AnimElement::b3SelectAnimElement (b3Scene *scene)
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

void b3Animation::b3ApplyTransformation (
	b3Scene       *Global,
	b3AnimElement *Anim,
	b3_matrix     *transform,
	b3_f64         t)
{
	b3Light      *Light;
	b3CameraPart *Camera;
	b3_f64        twirl,width,height,focal;
	b3_vector     diff;

	if (Anim->m_Flags & ANIMFLAGF_LIGHT)
	{
		Light = Global->b3GetLightByName(Anim->m_Object);
		if (Light != null)
		{
			b3PrintF(B3LOG_FULL,"  ANIM light %s\n",Anim->m_Object);
			Anim->b3GetPosition (&Light->m_Position,
				b3Math::b3Limit(t,Anim->m_Start,Anim->m_End));
			if ((Light->m_SpotActive) &&
			    ( Anim->b3GetClassType() == ANIM_ROTATE))
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
		if ((Camera != null) && (t >= Anim->m_Start))
		{
			twirl  = Camera->b3GetTwirl ();
			focal  = Camera->b3GetFocalLength();
			width  = b3Vector::b3Length (&Camera->m_Width);
			height = b3Vector::b3Length (&Camera->m_Height);
			diff   = Camera->m_EyePoint;
			Anim->b3GetPosition (&Camera->m_EyePoint,
				b3Math::b3Limit(t,Anim->m_Start,Anim->m_End));

			if (Anim->b3GetClassType() == ANIM_ROTATE)
			{
				b3PrintF(B3LOG_FULL,"  ANIM camera %s (rotate)\n",Anim->m_Object);
				Camera->b3Orientate(&Camera->m_EyePoint,&Anim->m_Center,focal,width,height);
				Camera->b3SetTwirl(twirl);
			}
			
			if (Anim->b3GetClassType() == ANIM_MOVE)
			{
				b3PrintF(B3LOG_FULL,"  ANIM camera %s (move)\n",Anim->m_Object);
				Camera->m_ViewPoint.x += (diff.x - Camera->m_EyePoint.x);
				Camera->m_ViewPoint.y += (diff.y - Camera->m_EyePoint.y);
				Camera->m_ViewPoint.z += (diff.z - Camera->m_EyePoint.z);
			}
		}
	}

	if (Anim->m_Flags & ANIMFLAGF_OBJECT)
	{
		b3PrintF(B3LOG_FULL,"  ANIM object %s (move)\n",Anim->m_Object);
		Anim->b3SelectAnimElement (Global);
		Global->b3Transform (transform,false);
	}
}
