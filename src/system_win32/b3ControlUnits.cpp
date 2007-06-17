/*
**
**	$Filename:	b3ControlUnits.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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
