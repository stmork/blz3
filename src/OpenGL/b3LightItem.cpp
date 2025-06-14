/*
**
**	$Filename:	b3LightItem.cpp $
**	$Release:	Dortmund 2001 - 2025 $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001 - 2025  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "b3LightItem.h"

QB3LightItem::QB3LightItem(b3Light * light) : QB3AbstractItem(light, B3_TYPE_LIGHT)
{
	setCheckable(true);
	setCheckState(light->b3IsActive() ? Qt::Checked : Qt::Unchecked);
	setText(QString::fromUtf8(light->b3GetName()));
}

bool QB3LightItem::check()
{
	const bool checked = checkState() == Qt::Checked;
	b3Light  * light   = *this;

	light->m_LightActive = checked;
	return checked;
}
