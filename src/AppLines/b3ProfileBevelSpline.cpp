/*
**
**	$Filename:	b3ProfileBevelSpline.cpp $
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
#include "b3ProfileBevelSpline.h"
#include "DlgProfileBevelSpline.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3Profile implementation                      **
**                                                                      **
*************************************************************************/

b3ProfileBevelSpline static_profile_bevel_spline;

b3ProfileBevelSpline::b3ProfileBevelSpline()
{
}

b3_bool b3ProfileBevelSpline::b3Create()
{
	m_Dlg = new CDlgProfileBevelSpline;
	m_Title.LoadString(IDS_PROFILE_BEVEL_SPLINE);
	return m_Dlg != null;
}

b3_bool b3ProfileBevelSpline::b3MatchClassType(b3_u32 class_type)
{
	return class_type == SPLINES_CYL;
}

int b3ProfileBevelSpline::b3AddImage(CImageList *images)
{
	return images->Add(AfxGetApp()->LoadIcon(IDI_PROFILE_BEVEL_SPLINE));
}

b3_bool b3ProfileBevelSpline::b3ComputeProfile(b3Spline *spline,...)
{
	va_list   args;
	b3_f64    xEdge;
	b3_f64    yEdge;
	b3_f64    oblique;
	b3_vector *c = spline->controls;
	int        i;

	va_start(args,spline);
	xEdge   = va_arg(args,b3_f64) * 0.5;
	yEdge   = va_arg(args,b3_f64) * 0.5;
	oblique = va_arg(args,b3_f64);
	va_end(args);

	spline->b3InitCurve(2,12,true);
	spline->b3ClearControls();

	c[0].x = -xEdge;
	c[0].y = oblique * 2 - yEdge;
	c[1].x = -xEdge;
	c[1].y = -yEdge;
	c[2].x = oblique * 2 - xEdge;
	c[2].y = -yEdge;

	// mirror horizontally
	for (i = 0;i < 3;i++)
	{
		c[5 - i].x = -c[i].x;
		c[5 - i].y =  c[i].y;
	}

	// mirror vertically
	for (i = 0;i < 6;i++)
	{
		c[11 - i].x =  c[i].x;
		c[11 - i].y = -c[i].y;
	}
	return true;
}
