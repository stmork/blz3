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

#include "AppLinesInclude.h"

#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        b3Profile implementation                      **
**                                                                      **
*************************************************************************/

b3Base<b3Profile> b3Profile::m_LinesProfileBase;

b3Profile::b3Profile() : b3Link<b3Profile>(sizeof(b3Profile))
{
	m_Dlg = null;
	m_LinesProfileBase.b3Append(this);
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

int b3Profile::b3Cmp(b3Profile *b)
{
	return stricmp(b3GetTitle(),b->b3GetTitle());
}

b3_bool b3Profile::b3ComputeProfile(b3Spline *spline,...)
{
	return false;
}

b3_bool b3Profile::b3ComputeShape(b3Spline *spline,b3Shape *shape,...)
{
	return false;
}
