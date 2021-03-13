/*
**
**	$Filename:	qrender.cpp $
**	$Release:	Dortmund 2001 - 2021 $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "b3LightItem.h"

QB3LightItem::QB3LightItem(b3Light * light) : m_Light(light)
{
	setCheckable(true);
	setCheckState(m_Light->b3IsActive() ? Qt::Checked : Qt::Unchecked);
	setText(m_Light->b3GetName());
}
