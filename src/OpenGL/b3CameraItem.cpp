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

#include "b3CameraItem.h"

QB3CameraItem::QB3CameraItem(b3CameraPart * camera) : m_Camera(camera)
{
	setCheckable(true);
	setCheckState(camera->b3IsActive() ? Qt::Checked : Qt::Unchecked);
	setText(camera->b3GetName());
}
