/*
**
**	$Filename:	b3ControlUnits.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Control units
**
**	(C) Copyright 2004  Steffen A. Mork
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
#include "blz3/system/b3ControlUnits.h"

/*************************************************************************
**                                                                      **
**                        CB3FloatSpinButtonCtrl implementation         **
**                                                                      **
*************************************************************************/

b3_f64 b3ControlUnits::m_UnitScale[] =
{
	1.0,
	180.0 / M_PI,
	100.0,
	1000.0,
	1.0,
	1.0,
	1.0
};
