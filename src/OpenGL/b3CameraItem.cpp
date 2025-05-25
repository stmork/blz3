/*
**
**	$Filename:	b3CameraItem.cpp $
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

#include "b3CameraItem.h"

QB3CameraItem::QB3CameraItem(b3CameraPart * camera) :
	QB3AbstractItem(camera, B3_TYPE_CAMERA)
{
	setCheckable(true);
	setCheckState(camera->b3IsActive() ? Qt::Checked : Qt::Unchecked);
	setText(QString::fromUtf8(camera->b3GetName()));
}

bool QB3CameraItem::check()
{
	const bool     checked = checkState() == Qt::Checked;
	b3CameraPart * camera  = *this;

	camera->b3Activate(checked);
	return checked;
}
