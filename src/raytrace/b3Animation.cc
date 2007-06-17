/*
**
**	$Filename:	b3Animation.cc $
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
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Animation::b3Register()
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
**                        b3Animation implementation                    **
**                                                                      **
*************************************************************************/

/* This routine sets the actual element for special use in the geometry */
/* editor. This has got no effect on computing the animation. */
/* -------------------------------------------------------------------- */
/* AnimRoot: the root of all animation tracks */
/* Element:  now the actual track */

void b3Animation::b3SetAnimElement (b3AnimElement *Element)
{
	m_Element = Element;
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

	b3Vector::b3Init(center);
	B3_FOR_BASE(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;

		// the element to reset
		if (Anim == Element)
		{
			return;
		}

		// compute rotation center if necessary
		if ((Anim->m_Flags & flagmask) == flagmask)
		{
			tClipped = b3Math::b3Limit(t,Anim->m_Start,Anim->m_End);
			Anim->b3GetPosition (center,tClipped);
		}
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
			t = b3Math::b3Limit(m_Neutral,Anim->m_Start,Anim->m_End);
			b3Matrix::b3Unit (&Anim->m_NeutralInverse);
			Anim->b3ComputeTransformationMatrix (this,&resetMatrix,t);
			b3Matrix::b3Inverse (&resetMatrix,&Anim->m_NeutralInverse);
			return;
		}

		// compute rotation center if necessary
		if ((Anim->m_Flags & flagmask) == flagmask)
		{
			t = b3Math::b3Limit (m_Neutral,Anim->m_Start,Anim->m_End);
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

	// reset animation, compute time range and number of tracks
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
		b3Matrix::b3Inverse  (&Anim->m_Actual,&resetMatrix);
		b3ApplyTransformation(Global,Anim,&resetMatrix,m_Neutral);
		b3Matrix::b3Unit (&Anim->m_Actual);
	}

	// compute number of frames
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

void b3Scene::b3ResetAnimation()
{
	b3Animation *animation = b3GetAnimation();

	if (animation != null)
	{
		b3PrintF(B3LOG_DEBUG,"Resetting animation...\n");
		b3Update();
		animation->b3ResetAnimation(this);
		b3Animate(b3Activation::B3_ANIM_DISABLED);
	}
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

	t = b3Math::b3Round(t,b3AnimElement::epsilon);
	b3PrintF(B3LOG_FULL,"<-- TIME POINT %3.3f -->\n",t);
	m_Time         = t; /* this is the time point you wish */
	m_FrameIndex   = b3AnimFrameIndex(t);
	b3Vector::b3Init(&m_AnimCenter);

	// reset tracks backwards
	B3_FOR_BASE_BACK(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;
		if (Anim->m_Flags & ANIMFLAGF_ACTIVE)
		{
			if (b3Matrix::b3Inverse  (&Anim->m_Actual,&resetMatrix))
			{
				b3ApplyTransformation(Global,Anim,&resetMatrix,m_Neutral);
			}
			b3Matrix::b3Unit (&Anim->m_Actual);
		}
	}

	// now: Animate the elements
	B3_FOR_BASE(b3GetAnimElementHead(),item)
	{
		Anim = (b3AnimElement *)item;
		Anim->m_Center = m_AnimCenter;
		if (Anim->m_Flags & ANIMFLAGF_ACTIVE)
		{
			tClipped = b3Math::b3Limit(t,Anim->m_Start,Anim->m_End);
			Anim->b3ComputeTransformationMatrix (this,&Anim->m_Actual,tClipped);
			b3ApplyTransformation (Global,Anim,&Anim->m_Actual,t);

			// set center position
			if (Anim->m_Flags & ANIMFLAGF_CENTER)
			{
				Anim->b3GetPosition(&m_AnimCenter,tClipped);
			}
		}
	}

	Global->b3ComputeBounds(&lower,&upper);
}

void b3Scene::b3SetAnimation(b3_f64 t)
{
	b3Animation *animation = b3GetAnimation();

	if (animation != null)
	{
		animation->b3SetAnimation(this,t);
		b3UpdateCamera();
		b3Animate(b3Activation::B3_ANIM_DISABLED);
	}
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
