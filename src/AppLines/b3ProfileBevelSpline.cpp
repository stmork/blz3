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

#include "AppLinesInclude.h"

#include "b3ProfileBevelSpline.h"
#include "DlgProfileBevelSpline.h"

/*************************************************************************
**                                                                      **
**                        b3Profile implementation                      **
**                                                                      **
*************************************************************************/

b3ProfileBevelSpline b3ProfileBevelSpline::m_RegisterProfile;

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
	va_list    args;
	b3_f64     xEdge;
	b3_f64     yEdge;
	b3_f64     oblique;
	b3_vector *c = spline->m_Controls;
	int        i;

	B3_ASSERT(c != null);

	va_start(args,spline);
	xEdge   = va_arg(args,b3_f64) * 0.5;
	yEdge   = va_arg(args,b3_f64) * 0.5;
	oblique = va_arg(args,b3_f64);
	va_end(args);

	spline->b3InitCurve(2,12,true);
	spline->b3ClearControls();

	c[0].x = xEdge;
	c[0].y = yEdge - oblique * 2;
	c[1].x = xEdge;
	c[1].y = yEdge;
	c[2].x = xEdge - oblique * 2;
	c[2].y = yEdge;

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

b3_bool b3ProfileBevelSpline::b3ComputeShape(b3Spline *spline,b3Shape *base_shape,...)
{
	va_list        args;
	b3SplineShape *shape = (b3SplineShape *)base_shape;
	b3_index       index,x,y;
	b3_f64         z;
	b3_f64         height;
	b3_count       yDegree;
	b3_count       yControls;
	b3_vector     *c = spline->m_Controls;

	B3_ASSERT(c != null);

	va_start(args,base_shape);
	height    = va_arg(args,b3_f64);
	yDegree   = va_arg(args,b3_count);
	yControls = va_arg(args,b3_count);
	va_end(args);

	shape->b3Init(spline->m_Degree, yDegree, spline->m_ControlNum, yControls);

	// Init control points
	for (y = 0;y < shape->m_Spline[1].m_ControlNum;y++)
	{
		index = y * shape->m_Spline[1].m_Offset;
		z     = b3Spline::b3ArcLengthParameter(y, yDegree, yControls, height);
		for (x = 0;x < shape->m_Spline[0].m_ControlNum;x++)
		{
			shape->m_Controls[index + x]   = c[x];
			shape->m_Controls[index + x].z = z;
		}
	}
	return true;
}
