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
	LinesProfileBase.b3Append(this);
}

b3_bool b3Profile::b3MatchClassType(b3_u32 class_type)
{
	return false;
}

const char *b3Profile::b3GetTitle()
{
	CString title;
	
	title.LoadString(IDS_PROFILE);
	return title;
}

void b3Profile::b3AddImage(CImageList *images)
{
}

CB3ProfileShapeDialog *b3Profile::b3GetCreateDialog()
{
	return null;
}

int b3Profile::b3Compare(b3Profile *a,b3Profile *b,void *ptr)
{
	return stricmp(a->b3GetTitle(),b->b3GetTitle());
}
