/*
**
**	$Filename:	b3ProfileBevelSpline.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 1053 $
**	$Date: 2005-01-23 21:57:22 +0100 (So, 23 Jan 2005) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Profile template
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_PROFILE_BEVEL_SPLINE_H
#define B3_PROFILE_BEVEL_SPLINE_H

#include "b3Profile.h"
#include "DlgProfileBevelSpline.h"

class b3ProfileBevelSpline : public b3Profile
{
	static b3ProfileBevelSpline    m_RegisterProfile;

public:
	                               b3ProfileBevelSpline();
	virtual b3_bool                b3Create();
	virtual b3_bool                b3MatchClassType(b3_u32 class_type);
	virtual int                    b3AddImage(CImageList *images);
	virtual b3_bool                b3ComputeProfile(b3Spline *spline,...);
	virtual b3_bool                b3ComputeShape(  b3Spline *spline,b3Shape *shape,...);
};

#endif
