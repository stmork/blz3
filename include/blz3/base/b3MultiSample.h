/*
**
**	$Filename:	b3MultiSample.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision: 1450 $
**	$Date: 2006-06-28 19:48:22 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Containter for OpenGL multisampling
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_MULTISAMPLE_H
#define B3_BASE_MULTISAMPLE_H

#include "blz3/b3Config.h"

/**
 * This class handles the existance and the usablility
 * of vector buffer objects.
 */
class B3_PLUGIN b3MultiSample
{
	static        b3_bool glHasMS;

public:
	/**
	 * This method checks for the existance of multi sampling in the given OpenGL extension string.
	 *
	 * @param extensions The OpenGL extension string.
	 */
	static        void    b3Init(const char *extensions);

	/**
	 * This method enables or disables multi sampling.
	 *
	 * @param enable A flag for enabling multi sampling.
	 */
	static        void    b3Enable(b3_bool enable = true);

	/**
	 * This method determines if multi sampling is enabled.
	 *
	 * @return True if multi sampling is enabled.
	 */
	static        b3_bool b3IsEnabled();

	/**
	 * This method returns true if multi sampling is available.
	 *
	 * @return True if multi sampling is available.
	 */
	static inline b3_bool b3HasMS()
	{
		return glHasMS;
	}
};

#endif
