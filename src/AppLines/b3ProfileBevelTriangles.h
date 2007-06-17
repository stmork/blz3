/*
**
**	$Filename:	b3ProfileBevelTriangles.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 1053 $
**	$Date: 2005-01-23 21:57:22 +0100 (So, 23 Jan 2005) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Triangels with beveled edges
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_PROFILE_BEVEL_TRIANGLES_H
#define B3_PROFILE_BEVEL_TRIANGLES_H

#include "b3Profile.h"
#include "DlgProfileBevelTriangles.h"

class b3ProfileBevelTriangles : public b3Profile
{
	static b3ProfileBevelTriangles m_RegisterProfile;

public:
	                               b3ProfileBevelTriangles();
	virtual b3_bool                b3Create();
	virtual b3_bool                b3MatchClassType(b3_u32 class_type);
	virtual int                    b3AddImage(CImageList *images);
	virtual b3_bool                b3ComputeProfile(b3Spline *spline,...);
	virtual b3_bool                b3ComputeShape(  b3Spline *spline,b3Shape *shape,...);
};

#endif
