/*
**
**	$Filename:	b3DashPen.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Creating a dashed CPen
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3DashPen.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.1  2002/02/13 20:13:13  sm
**	- Added dashed line pattern support in class CB3DashPen
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3DashPen implementation                     **
**                                                                      **
*************************************************************************/

CB3DashPen::CB3DashPen()
{
}

CB3DashPen::CB3DashPen(
	COLORREF  color,
	b3_dash   pattern,
	DWORD    *stipple,
	b3_count  len)
{
	b3CreateDashPen(color,pattern,stipple,len);
}

b3_bool CB3DashPen::b3CreateDashPen(
	COLORREF  color,
	b3_dash   pattern,
	DWORD    *stipple,
	b3_count  len)
{
	LOGBRUSH logbrush;
	b3_count count = 0;

	switch (pattern)
	{
	case B3_DASH_DOT_DOT:
		m_Stipple[4] = 0;
		m_Stipple[5] = 3;
		count += 2;
	case B3_DASH_DOT:
		m_Stipple[2] = 0;
		m_Stipple[3] = 3;
		count += 2;
	case B3_DASH:
		m_Stipple[0] = 1;
		m_Stipple[1] = 3;
		stipple = m_Stipple;
		len = count + 2;
		break;

	case B3_DASH_CUSTOM:
		break;
	}

	// Create brush entry for color
	logbrush.lbColor = color;
	logbrush.lbHatch = 0;
	logbrush.lbStyle = BS_SOLID;

	// Create pen from brush and pattern (I don't know what is Windows is doing here...)
	return CreatePen(PS_USERSTYLE|PS_SOLID|PS_GEOMETRIC,1,&logbrush,len,stipple);
}
