/*
**
**	$Filename:	b3Activation.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for shape activation.
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_ACTIVATION_H
#define B3_RAYTRACE_ACTIVATION_H

#include "blz3/b3Config.h"

/**
 * This class provides activation state for any rendering object.
 */
class B3_PLUGIN b3Activation
{
public:
	/**
	 * This enumeration lists the animations' activation state.
	 */
	enum b3_anim_activation
	{
		B3_ANIM_DISABLED,  //!< The objects' animation is disabled.
		B3_ANIM_DEACTIVE,  //!< The objects' animation is not active.
		B3_ANIM_ACTIVE     //!< The objects' animation is active.
	};

private:
	b3_bool            m_Active;
	b3_anim_activation m_AnimActive;

public:
	/**
	 * This constructor initializes thie instance.
	 */
	b3Activation();

	/**
	 * This method returns the activation state.
	 *
	 * @return True on activation.
	 */
	b3_bool b3IsActive();

	/**
	 * This method sets the activation state.
	 *
	 * @param activate The new activation state.
	 */
	void    b3Activate(b3_bool activate = true);

	/**
	 * This method sets the animation state.
	 *
	 * @param activate The new animation state.
	 */
	void    b3Animate(b3_bool activate = true);

	/**
	 * This method sets the animation state.
	 *
	 * @param activate The new animation state.
	 * @see b3_anim_activation
	 */
	void    b3Animate(b3_anim_activation activate);
};

#endif
