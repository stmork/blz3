/*
**
**	$Filename:	b3ProfileBevelStumpSpline.cpp $
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

#include "b3ProfileBevelStumpSpline.h"
#include "DlgProfileBevelStumpSpline.h"

/*************************************************************************
**                                                                      **
**                        b3Profile implementation                      **
**                                                                      **
*************************************************************************/

b3ProfileBevelStumpSpline b3ProfileBevelStumpSpline::m_RegisterProfile;

b3ProfileBevelStumpSpline::b3ProfileBevelStumpSpline()
{
}

b3_bool b3ProfileBevelStumpSpline::b3Create()
{
	m_Dlg = new CDlgProfileBevelStumpSpline;
	m_Title.LoadString(IDS_PROFILE_BEVELSTUMP_SPLINE);
	return m_Dlg != null;
}

b3_bool b3ProfileBevelStumpSpline::b3MatchClassType(b3_u32 class_type)
{
	return class_type == SPLINES_CYL;
}

int b3ProfileBevelStumpSpline::b3AddImage(CImageList *images)
{
	return images->Add(AfxGetApp()->LoadIcon(IDI_PROFILE_BEVELSTUMP_SPLINE));
}

b3_bool b3ProfileBevelStumpSpline::b3ComputeProfile(b3Spline *spline,...)
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

b3_bool b3ProfileBevelStumpSpline::b3ComputeShape(b3Spline *spline,b3Shape *base_shape,...)
{
	va_list        args;
	b3SplineShape *shape = (b3SplineShape *)base_shape;
	b3_index       bIndex,tIndex,x,y;
	b3_f64         z;
	b3_f64         xEdge;
	b3_f64         yEdge;
	b3_f64         height;
	b3_f64         oblique;
	b3_vector     *c = spline->m_Controls;

	B3_ASSERT(c != null);

	va_start(args,base_shape);
	xEdge   = va_arg(args,b3_f64);
	yEdge   = va_arg(args,b3_f64);
	height  = va_arg(args,b3_f64);
	oblique = va_arg(args,b3_f64);
	va_end(args);

	shape->b3Init(spline->m_Degree, 2, spline->m_ControlNum,6);

	// Init bottom half of control points
	for (y = 0;y < 3;y++)
	{
		bIndex = y * shape->m_Spline[1].m_Offset;
		z     = (y < 2 ? 0 : oblique * 2);
		for (x = 0;x < shape->m_Spline[0].m_ControlNum;x++)
		{
			if (y == 0)
			{
				shape->m_Controls[bIndex + x].x = B3_SIGN(c[x].x) * (xEdge * 0.5 - oblique);
				shape->m_Controls[bIndex + x].y = B3_SIGN(c[x].y) * (yEdge * 0.5 - oblique);
			}
			else
			{
				shape->m_Controls[bIndex + x] = c[x];
			}
			shape->m_Controls[bIndex + x].z = z;
		}
	}

	// Mirror bottom to top
	for (y = 0;y < 3;y++)
	{
		bIndex =      y  * shape->m_Spline[1].m_Offset;
		tIndex = (5 - y) * shape->m_Spline[1].m_Offset;
		z      = shape->m_Controls[bIndex].z;
		for (x = 0;x < shape->m_Spline[0].m_ControlNum;x++)
		{
			shape->m_Controls[tIndex + x] =   shape->m_Controls[bIndex + x];
			shape->m_Controls[tIndex + x].z = height - z;
		}
	}

	return true;
}
