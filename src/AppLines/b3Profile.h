/*
**
**	$Filename:	b3Profile.h $
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

#ifndef B3_PROFILE_H
#define B3_PROFILE_H

#include "blz3/base/b3Array.h"
#include "blz3/base/b3Compare.h"
#include "blz3/base/b3Spline.h"
#include "b3ProfileShapeDialog.h"

class b3Profile : public b3Link<b3Profile>, public b3Comparator<b3Profile>
{
protected:
	CB3ProfileShapeDialog    *m_Dlg;
	CString                   m_Title;

public:
	static b3Base<b3Profile>  m_LinesProfileBase;

	b3_bool                   m_Closed;
	b3Array<b3_vector>        m_Controls;

public:
	                               b3Profile();
	virtual                       ~b3Profile();
	virtual b3_bool                b3Create();
	virtual b3_bool                b3MatchClassType(b3_u32 class_type);
	virtual int                    b3AddImage(CImageList *images);
	virtual b3_bool                b3ComputeProfile(b3Spline *spline,...);
	virtual b3_bool                b3ComputeShape(  b3Spline *spline,b3Shape *shape,...);
	        const char            *b3GetTitle();
	        CB3ProfileShapeDialog *b3GetCreateDialog();

	        int                    b3Cmp(b3Profile *b);
};

#endif
