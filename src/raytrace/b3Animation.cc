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

#include "blz3/raytrace/b3Animation.h"
#include "blz3/raytrace/b3Special.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.23  2004/08/20 08:09:27  sm
**      - Optimized animation a little bit.
**
**      Revision 1.22  2004/08/19 10:12:23  sm
**      - Test if one animation run is enough - but is not.
**      - Using gluUnProject for ticket no. 7.
**
**      Revision 1.21  2004/06/19 12:11:01  sm
**      - Fixed animation problem by using a thrid dress vector
**        which points up. This is a hack because rotating vectors
**        inside the xy plane are undefined.
**      - Added material/bump save support.
**
**      Revision 1.20  2004/06/18 14:49:05  sm
**      - Some probes concerning the anim rotation problem. Should I use
**        quaternions?
**
**      Revision 1.19  2004/05/30 20:25:00  sm
**      - Set paging size in supersampling dialog to 1 instead of 10.
**      - Added support for debugging super sampling.
**      - The object preview uses the shading model of its owning world.
**      - Fixed animation problem when using rotating elements on
**        time bounds because of rounding problems. Now using
**        b3_f32 for time points.
**
**      Revision 1.18  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.17  2003/07/12 17:44:47  sm
**      - Cleaned up raytracing b3Item registration
**
**      Revision 1.16  2003/02/18 16:52:57  sm
**      - Fixed no name error on new scenes (ticket no. 4).
**      - Introduced new b3Matrix class and renamed methods.
**
**      Revision 1.15  2003/02/17 16:57:46  sm
**      - Inlining head pointer computation.
**
**      Revision 1.14  2002/08/21 10:16:40  sm
**      - Made some changes to the Un*x OpenGL renderer:
**        o Added animations
**        o Added camera switching
**        o Corrected far clipping plane computation.
**      - Configure script tidied up.
**
**      Revision 1.13  2002/08/19 16:50:39  sm
**      - Now having animation running, running, running...
**      - Activation handling modified to reflect animation
**        and user transformation actions.
**      - Made some architectual redesigns.
**
**      Revision 1.12  2002/08/18 13:05:17  sm
**      - First try to animate. We have to relink the control points which
**        are stored in separate Blizzard classes to the b3AnimElement
**        class.
**
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
