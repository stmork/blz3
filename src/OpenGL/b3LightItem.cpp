/*
**
**	$Filename:	b3LightItem.cpp $
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

QB3LightItem::QB3LightItem(b3Light * light) : QB3AbstractItem(light, B3_TYPE_LIGHT)
{
	setCheckable(true);
	setCheckState(light->b3IsActive() ? Qt::Checked : Qt::Unchecked);
	setText(QString::fromLatin1(light->b3GetName()));
}

bool QB3LightItem::check()
{
	const bool checked = checkState() == Qt::Checked;
	b3Light  * light   = *this;

	light->m_LightActive = checked;
	return checked;
}
