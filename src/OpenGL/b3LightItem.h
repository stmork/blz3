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

#pragma once

#ifndef QB3LIGHTITEM_H
#define QB3LIGHTITEM_H

#include <QStandardItem>

#include <blz3/raytrace/b3Light.h>

class QB3LightItem : public QStandardItem
{
public:
	explicit QB3LightItem(b3Light * light);

private:
	b3Light * m_Light;
};

#endif // QB3LIGHTITEM_H
