/*
**
**	$Filename:	b3Profile.h $
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

#ifndef B3_PROFILE_H
#define B3_PROFILE_H

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Array.h"

class CB3ProfileShapeDialog;

class b3Profile : public b3Link<b3Profile>
{
public:
	b3_bool            m_Closed;
	b3Array<b3_vector> m_Controls;

public:
	                               b3Profile();
	virtual b3_bool                b3MatchClassType(b3_u32 class_type);
	virtual const char            *b3GetTitle();
	virtual void                   b3AddImage(CImageList *images);
	virtual CB3ProfileShapeDialog *b3GetCreateDialog();

	static  int                    b3Compare(b3Profile *a,b3Profile *b,void *ptr);
};

extern b3Base<b3Profile> LinesProfileBase;

#endif
