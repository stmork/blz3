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

#ifndef B3_SYSTEM_DASHPEN_H
#define B3_SYSTEM_DASHPEN_H

#include "blz3/b3Config.h"

enum b3_dash
{
	B3_DASH,
	B3_DASH_DOT,
	B3_DASH_DOT_DOT,
	B3_DASH_CUSTOM
};

class CB3DashPen : public CPen
{
	DWORD m_Stipple[6];

public:
	        CB3DashPen();
	        CB3DashPen(     COLORREF color,b3_dash dash = B3_DASH,DWORD *stipple = null,b3_count stipple_len = 0);
	b3_bool b3CreateDashPen(COLORREF color,b3_dash dash = B3_DASH,DWORD *stipple = null,b3_count stipple_len = 0);
};

#endif
