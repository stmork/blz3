/*
**
**	$Filename:	b3ProfileBevelStumpSpline.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Profile template
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_PROFILE_BEVELSTUMP_SPLINE_H
#define B3_PROFILE_BEVELSTUMP_SPLINE_H

#include "b3Profile.h"
#include "DlgProfileBevelStumpSpline.h"

class b3ProfileBevelStumpSpline : public b3Profile
{
	static b3ProfileBevelStumpSpline  m_RegisterProfile;

public:
	b3ProfileBevelStumpSpline();
	virtual b3_bool                b3Create();
	virtual b3_bool                b3MatchClassType(b3_u32 class_type);
	virtual int                    b3AddImage(CImageList * images);
	virtual b3_bool                b3ComputeProfile(b3Spline * spline, ...);
	virtual b3_bool                b3ComputeShape(b3Spline * spline, b3Shape * shape, ...);
};

#endif
