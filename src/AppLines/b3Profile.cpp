/*
**
**	$Filename:	b3Profile.cpp $
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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/03/09 19:48:14  sm
**	- Added a second profile for spline cylinders.
**	- BSpline shape creation dialog added.
**	- Added some features to b3SplineTemplate class:
**	  o call b3ThroughEndControl() for open splines
**	  o optimize subdivision on b3InitCurve()
**	- Fine tuing and fixed much minor bugs.
**
**	Revision 1.2  2002/03/05 20:38:24  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.1  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3Profile implementation                      **
**                                                                      **
*************************************************************************/

b3Base<b3Profile> LinesProfileBase;

b3Profile::b3Profile() : b3Link<b3Profile>(sizeof(b3Profile))
{
	m_Dlg = null;
	LinesProfileBase.b3Append(this);
}

b3Profile::~b3Profile()
{
	if (m_Dlg != null)
	{
		delete m_Dlg;
	}
}

b3_bool b3Profile::b3Create()
{
	m_Title.LoadString(IDS_PROFILE);
	return false;
}

b3_bool b3Profile::b3MatchClassType(b3_u32 class_type)
{
	return false;
}

const char *b3Profile::b3GetTitle()
{
	return m_Title;
}

int b3Profile::b3AddImage(CImageList *images)
{
	return -1;
}

CB3ProfileShapeDialog *b3Profile::b3GetCreateDialog()
{
	B3_ASSERT(m_Dlg != null);
	m_Dlg->m_Profile = this;
	return m_Dlg;
}

int b3Profile::b3Compare(b3Profile *a,b3Profile *b,void *ptr)
{
	return stricmp(a->b3GetTitle(),b->b3GetTitle());
}

b3_bool b3Profile::b3ComputeProfile(b3Spline *spline,...)
{
	return false;
}

b3_bool b3Profile::b3ComputeShape(b3Spline *spline,b3Shape *shape,...)
{
	return false;
}
