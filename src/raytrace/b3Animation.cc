/*
**
**      $Filename:      b3Animation.cc $
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Matrix.h"

#define ANIM_STEP 0.01

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.11  2002/08/17 17:31:22  sm
**      - Introduced animation support (Puh!)
**
**      Revision 1.10  2002/08/15 13:56:43  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.9  2002/08/09 13:20:19  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.8  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.7  2002/01/01 13:50:22  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.6  2001/12/31 12:15:55  sm
**      - Fixed obsolete b3AnimElement handling
**
**      Revision 1.5  2001/12/31 11:05:17  sm
**      - Added TestData for testing Blizzard data structures for reading
**        and writing.
**      - Fixed bugs found with previous mentioned tool:
**        o Some b3AnimElement errors found to be fixed under Windows.
**        o b3TriangleShape destructor handled unchecked m_GridList pointer
**      - Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**        B3LOG_NORMAL to B3LOG_DEBUG.
**
**      Revision 1.4  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.3  2001/12/30 18:24:35  sm
**      - Added missing b3AnimControl class
**      - Some minor bug fixes done:
**        o Missed some SetModifiedFlag()
**        o b3Scene::b3SetCamera() calls added which now puts the
**          selected camera in front of the b3Special list so that Lines III
**          select it when reloading.
**
**      Revision 1.2  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
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

void b3InitAnimation::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering animation elements...\n");
	b3Item::b3Register(&b3AnimElement::b3StaticInit,&b3AnimElement::b3StaticInit,ANIM_MOVE );
	b3Item::b3Register(&b3AnimElement::b3StaticInit,&b3AnimElement::b3StaticInit,ANIM_ROTATE );
	b3Item::b3Register(&b3AnimElement::b3StaticInit,&b3AnimElement::b3StaticInit,ANIM_SCALE );
	b3Item::b3Register(&b3AnimControl::b3StaticInit,&b3AnimControl::b3StaticInit,CPOINT_3D );
	b3Item::b3Register(&b3AnimControl::b3StaticInit,&b3AnimControl::b3StaticInit,CPOINT_4D );
}

/*************************************************************************
**                                                                      **
**                        b3AnimControl implementation                  **
**                                                                      **
*************************************************************************/

b3AnimControl::b3AnimControl(b3_u32 class_type) : b3Item(sizeof (b3AnimControl),CPOINT_4D)
{
	m_Dimension = 4;
	m_Used      = 0;
	m_Max       = 0;
	m_Controls  = null;
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

	b3StoreInt  (m_Dimension);
	b3StoreCount(m_Used);
	b3StoreCount(m_Max);

	for (i = 0;i < m_Max;i++)
	{
		b3StoreVector4D(&m_Controls[i]);
	}
}

/*************************************************************************
**                                                                      **
**                        b3Animation implementation                    **
**                                                                      **
*************************************************************************/

b3Base<b3Item> *b3Animation::b3GetAnimElementHead()
{
	return &m_Heads[0];
}

/* This routine sets the actual element for special use in the geometry */
/* editor. This has got no effect on computing the animation. */
/* -------------------------------------------------------------------- */
/* AnimRoot: the root of all animation tracks */
/* Element:  now the actual track */

void b3Animation::b3SetAnimElement (b3AnimElement *Element)
{
	m_Element = Element;
}

b3_f64 b3Animation::b3ClipValue(b3_f64 val,b3_f64 min,b3_f64 max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

/* This routine computes the curve point corresponding to the state if */
/* this animation track is not existing. All position computations */
/* use this point as a reference point. */
/* ------------------------------------------------------------------- */
/* Element: animation element */
/* neutral: curve point which represent the initial position */

void b3Animation::b3GetNeutralPosition(
	b3AnimElement *Element,
	b3_vector     *neutral)
{
	Element->b3GetPosition (neutral,m_Neutral);
}

/* This routine looks for previous tracks so that the actual track could */
/* drawn in the same track without overlapping the valid time zones. */
/* There are four conditions to put both elements into the same track: */
/* a) same animation types */
/* b) both elements use the same number of tracks */
/* c) no overlapping */
/* d) same destination object to animate */
/* --------------------------------------------------------------------- */
/* Anim: animation tracks which want to find another same track */

b3AnimElement *b3Animation::b3FindSameTrack(b3AnimElement *Anim)
{
	b3AnimElement *prev = Anim;

	while ((prev = (b3AnimElement *)prev->Prev) != null)
	{
		if ((prev->b3GetClassType() == Anim->b3GetClassType()) &&
			(prev->m_CurveUse       == Anim->m_CurveUse) &&
		    (prev->m_End            <= Anim->m_Start) &&
		    (strcmp(prev->m_Object,Anim->m_Object) == 0))
		{
			return prev;
		}
	}
	return prev;
}

void b3Animation::b3RecomputeCenter (
	b3AnimElement *Element,
	b3_vector     *center,
	b3_f64         t)
{
	b3AnimElement *Anim;
	b3Item        *item;
	b3_u32         flagmask = ANIMFLAGF_ACTIVE|ANIMFLAGF_CENTER;
	b3_f64         tClipped;

	center->x = 0;
	center->y = 0;
	center->z = 0;
	B3_FOR_BASE(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;

		/* the element to reset */
		if (Anim == Element)
		{
			return;
		}

			/* compute rotation center if necessary */
		if ((Anim->m_Flags & flagmask) == flagmask)
		{
			tClipped = b3ClipValue (t,Anim->m_Start,Anim->m_End);
			Anim->b3GetPosition (center,tClipped);
		}
	}
}



/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3Animation::b3AnimateMove(
	b3AnimElement *Element,
	b3_matrix     *transform,
	b3_f64         t)
{
	b3_vector move;

		/* now compute transformation */
	Element->b3GetPosition (&move,t);
	b3MatrixMove (&Element->m_NeutralInverse,transform,&move);
}


/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3Animation::b3AnimateRotate(
	b3AnimElement *Element,
	b3_matrix     *transform,
	b3_f64         tParam)
{
	b3_f64     t1,t2,t = tParam;
	b3_bool    negate;
	b3_vector  lookTo;
	b3_vector  oldDir;
	b3_vector  oldCenter;

	t1 = b3ClipValue (t - ANIM_STEP,Element->m_Start,Element->m_End);
	t2 = b3ClipValue (t + ANIM_STEP,Element->m_Start,Element->m_End);
	if (t1 != t2)
	{
		Element->b3GetPosition (&lookTo, t);
		if (t != t1)
		{
			b3RecomputeCenter (Element,&oldCenter,t1);
			Element->b3GetPosition     (&oldDir,t1);
			negate = false;
		}
		else
		{
			b3RecomputeCenter (Element,&oldCenter,t2);
			Element->b3GetPosition     (&oldDir,t2);
			negate = true;
		}

		lookTo.x -= Element->m_Center.x;
		lookTo.y -= Element->m_Center.y;
		lookTo.z -= Element->m_Center.z;
		oldDir.x -= oldCenter.x;
		oldDir.y -= oldCenter.y;
		oldDir.z -= oldCenter.z;
		b3MatrixDress (&Element->m_NeutralInverse,transform,
			&Element->m_Center,&lookTo,&oldDir,negate);
		if (fabs(b3Det4(transform)) < 0.0001)
		{
			b3MatrixUnit (transform);
		}
	}
	else
	{
		*transform = Element->m_NeutralInverse;
	}
}

/* This routine computes the new position point for an animation. The */
/* point is computed using a NURBS curve. */
/* ------------------------------------------------------------------ */
/* Element:   animation track */
/* transform: tramsformation matrix to compute */
/* t:         time point */

void b3Animation::b3AnimateScale(
	b3AnimElement *Element,
	b3_matrix     *transform,
	b3_f64         t)
{
	b3_vector scale;

	Element->b3GetPosition (&scale,t);
	scale.x -= Element->m_Center.x;
	scale.y -= Element->m_Center.y;
	scale.z -= Element->m_Center.z;
	b3MatrixScale (&Element->m_NeutralInverse,transform,&Element->m_Center,&scale);
}

/* This routine computes the transformation matrix for one animation */
/* element depending of its transformation type and the global time point. */
/* ----------------------------------------------------------------------- */
/* Anim:      animation element */
/* transform: where to store the transformation matrix */
/* t:         time point */

void b3Animation::b3ComputeTransformationMatrix(
 	b3AnimElement *Anim,
	b3_matrix     *transform,
	b3_f64         t)
{
	switch (Anim->b3GetClassType())
	{
		case ANIM_MOVE :
			b3AnimateMove   (Anim,transform,t);
			break;

		case ANIM_ROTATE :
			b3AnimateRotate (Anim,transform,t);
			break;

		case ANIM_SCALE :
			b3AnimateScale  (Anim,transform,t);
			break;
	}
}

/* This routine sets the activation state of a bounding box depending */
/* of the box name. */
/* ------------------------------------------------------------------ */
/* Global:  geometry structure */
/* Element: amimation track */

b3_bool b3Animation::b3SelectAnimElement (
	b3Scene       *Global,
	b3AnimElement *Element)
{
	if (Element == null)
	{
		return false;
	}

	Global->b3Activate(false);
	if (Element->m_Flags & ANIMFLAGF_OBJECT)
	{
//		Global->m_PrepareInfo.b3CollectBBoxes();
//		BTraverse (&Global->BBoxes,SelectObjects,Element);
	}

	return (Element->m_Flags & ANIMFLAGF_OBJECT) != 0;
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
		Light = Global->b3GetLight(Anim->m_Object);
		if (Light != null)
		{
			Anim->b3GetPosition (&Light->m_Position,
				b3ClipValue(t,Anim->m_Start,Anim->m_End));
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
		Camera = Global->b3GetCamera(Anim->m_Object);
		if ((Camera != null) && (t >= Anim->m_Start))
		{
			twirl  = Camera->b3GetTwirl ();
			focal  = Camera->b3GetFocalLength();
			width  = b3Vector::b3Length (&Camera->m_Width);
			height = b3Vector::b3Length (&Camera->m_Height);
			diff   = Camera->m_EyePoint;
			Anim->b3GetPosition (&Camera->m_EyePoint,
				b3ClipValue(t,Anim->m_Start,Anim->m_End));

			if (Anim->b3GetClassType() == ANIM_ROTATE)
			{
				Camera->m_ViewPoint = Anim->m_Center;
				Camera->b3SetTwirl(twirl);
			}
			
			if (Anim->b3GetClassType() == ANIM_MOVE)
			{
				Camera->m_ViewPoint.x += (diff.x - Camera->m_EyePoint.x);
				Camera->m_ViewPoint.y += (diff.y - Camera->m_EyePoint.y);
				Camera->m_ViewPoint.z += (diff.z - Camera->m_EyePoint.z);
			}
		}
	}

	if (Anim->m_Flags & ANIMFLAGF_OBJECT)
	{
		b3SelectAnimElement (Global,Anim);
		Global->b3Transform (transform,false);
	}
}

void b3Animation::b3RecomputeNeutralInverse (b3AnimElement *Element)
{
	b3AnimElement *Anim;
	b3Item        *item;
	b3_u32         flagmask = ANIMFLAGF_ACTIVE|ANIMFLAGF_CENTER;
	b3_f64         t;
	b3_matrix      resetMatrix;

	m_AnimCenter.x = 0;
	m_AnimCenter.y = 0;
	m_AnimCenter.z = 0;

	B3_FOR_BASE(b3GetAnimElementHead(),item)

	{
		// the element to reset
		Anim = (b3AnimElement *)item;
		Anim->m_Center = m_AnimCenter;
		if (Anim == Element)
		{
			t = b3ClipValue (m_Neutral,Anim->m_Start,Anim->m_End);
			b3MatrixUnit (&Anim->m_NeutralInverse);
			b3ComputeTransformationMatrix (Anim,&resetMatrix,t);
			b3MatrixInv (&resetMatrix,&Anim->m_NeutralInverse);
			return;
		}

			/* compute rotation center if necessary */
		if ((Anim->m_Flags & flagmask) == flagmask)
		{
			t = b3ClipValue (m_Neutral,Anim->m_Start,Anim->m_End);
			Anim->b3GetPosition (&m_AnimCenter,t);
		}
	}
}


/* This routine sets all track objects to the default position as if */
/* there were be no animation. */
/* ----------------------------------------------------------------- */
/* Global: geometry structure */

void b3Animation::b3ResetAnimation (b3Scene *Global)
{
	b3AnimElement *Anim;
	b3AnimElement *prev;
	b3Item        *item;
	b3_matrix      resetMatrix;

		/* reset animation, compute time range and number of tracks */
	m_Tracks = 0;
	m_Frames = 0;
	Anim  = (b3AnimElement *)b3GetAnimElementHead()->First;
	m_Start = m_End = (Anim != null ? Anim->m_Start : 0);
	while (Anim != null)
	{
		m_End     = B3_MAX(Anim->m_End,  m_End);
		m_Start   = B3_MIN(Anim->m_Start,m_Start);
		b3RecomputeNeutralInverse (Anim);
		prev = b3FindSameTrack(Anim);
		if (prev != null)
		{
			Anim->m_TrackIndex  = prev->m_TrackIndex;
		}
		else
		{
			Anim->m_TrackIndex  = m_Tracks;
			m_Tracks += Anim->m_CurveUse;
		}

		Anim  = (b3AnimElement *)Anim->Succ;
	}

	B3_FOR_BASE_BACK(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;
		b3MatrixInv  (&Anim->m_Actual,&resetMatrix);
		b3ApplyTransformation(Global,Anim,&resetMatrix,m_Neutral);
		b3MatrixUnit (&Anim->m_Actual);
	}

		/* compute number of frames */
	m_Frames =
		(b3_index)ceil (m_End   * m_FramesPerSecond) -
		(b3_index)floor(m_Start * m_FramesPerSecond);
	if (m_Frames < 1)
	{
		m_Frames = 1;
	}
	m_Time       = m_Start;
	m_FrameIndex = 0;
}


/* This routine sets the scene to the specified time point t. */
/* ---------------------------------------------------------- */
/* Global:   geometry structure */
/* AnimRoot: the animation */
/* t:        the specified time point in seconds */

void b3Animation::b3SetAnimation (b3Scene *Global,b3_f64 t)
{
	b3AnimElement *Anim;
	b3Item        *item;
	b3_vector      lower,upper;
	b3_matrix      resetMatrix;
	b3_f64         tClipped;

	m_Time         = t; /* this is the time point you wish */
	m_FrameIndex   = (b3_index)(( -m_Start) * m_FramesPerSecond);
	m_AnimCenter.x = 0;
	m_AnimCenter.y = 0;
	m_AnimCenter.z = 0;


	// reset tracks backwards
	B3_FOR_BASE_BACK(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;
		if (Anim->m_Flags & ANIMFLAGF_ACTIVE)
		{
			if (b3MatrixInv  (&Anim->m_Actual,&resetMatrix))
			{
				b3ApplyTransformation(Global,Anim,&resetMatrix,m_Neutral);
			}
			b3MatrixUnit (&Anim->m_Actual);
		}
	}

	// now: Animate the elements
	B3_FOR_BASE(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;
		Anim->m_Center = m_AnimCenter;
		if (Anim->m_Flags & ANIMFLAGF_ACTIVE)
		{
			tClipped = b3ClipValue (t,Anim->m_Start,Anim->m_End);
			b3ComputeTransformationMatrix (Anim,&Anim->m_Actual,tClipped);
			b3ApplyTransformation (Global,Anim,&Anim->m_Actual,t);

				/* set center position */
			if (Anim->m_Flags & ANIMFLAGF_CENTER)
			{
				Anim->b3GetPosition(&m_AnimCenter,tClipped);
			}
		}
	}
	Global->b3ComputeBounds(&lower,&upper);
}

b3_bool b3Animation::b3ActivateAnimation (
	b3Scene *Global,
	b3_bool  activation)
{
	b3_f64  oldTime;
	b3_bool oldActivation;

	// get old state
	oldActivation = (m_Flags & ANIMF_ON) != 0;
	oldTime       =  m_Time;
	b3ResetAnimation  (Global);

	// move to neutral time point, set activation flag
	b3SetAnimation (Global,m_Neutral);
	m_Flags &=  ~ANIMF_ON;
	if (activation)
	{
		m_Flags |= ANIMF_ON;
		b3SetAnimation (Global,oldTime);
	}

	return oldActivation;
}


/*************************************************************************
**                                                                      **
**                        some auxiliary routines                       **
**                                                                      **
*************************************************************************/


/* This routine converts an frame index starting at 0 to the time */
/* in seconds specified by the animation structure. */
/* -------------------------------------------------------------- */
/* AnimRoot: animation root structure */
/* index:    frame index */

b3_f64 b3Animation::b3AnimTimeCode (b3_index index)
{
	return m_Start + (b3_f64)index / m_FramesPerSecond;
}

/* This routine converts a time code to frame index starting at 0. */
/* -------------------------------------------------------------- */
/* AnimRoot: animation root structure */
/* t:        time code */

b3_index b3Animation::b3AnimFrameIndex (b3_f64 t)
{
	return (b3_index)((t - m_Start) * m_FramesPerSecond);
}
