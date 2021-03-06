/*
**
**	$Filename:	b3ControlUnits.h $
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

#ifndef B3_SYSTEM_CONTROLUNITS_H
#define B3_SYSTEM_CONTROLUNITS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/b3Config.h"

class b3ControlUnits
{
public:
	enum b3_unit
	{
		B3_UNIT_ABS = 0,
		B3_UNIT_DEGREE,
		B3_UNIT_PERCENT,
		B3_UNIT_PERMILLE,
		B3_UNIT_IOR,
		B3_UNIT_SPECULAR_EXP,
		B3_UNIT_BUMP
	};

private:
	static b3_f64 m_UnitScale[];
	b3_unit       m_Unit;

protected:
	b3ControlUnits()
	{
		m_Unit = B3_UNIT_ABS;
	}

	inline b3_unit b3GetUnit()
	{
		return m_Unit;
	}

public:
	virtual void b3SetUnit(b3_unit unit)
	{
		m_Unit = unit;
	}

	inline b3_f64   b3GetUnitScale()
	{
		return m_UnitScale[m_Unit];
	}
};

#endif
