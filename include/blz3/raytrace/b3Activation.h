/*
**
**	$Filename:	b3Activation.h $ 
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
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

class B3_PLUGIN b3Activation
{
public:
	enum b3_anim_activation
	{
		B3_ANIM_DISABLED,
		B3_ANIM_DEACTIVE,
		B3_ANIM_ACTIVE
	};

private:
	b3_bool            m_Active;
	b3_anim_activation m_AnimActive;

public:
	        b3Activation();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate = true);
	void    b3Animate(b3_bool activate = true);
	void    b3Animate(b3_anim_activation activate);
};

#endif
