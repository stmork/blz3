/*
**
**	$Filename:	b3CameraItem.h $
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

#ifndef QB3CAMERAITEM_H
#define QB3CAMERAITEM_H

#include "b3AbstractItem.h"

#include <blz3/raytrace/b3Scene.h>

Q_DECLARE_METATYPE(b3CameraPart *)

class QB3CameraItem : public QB3AbstractItem<b3CameraPart>
{
public:
	explicit QB3CameraItem(b3CameraPart * camera);

	bool check() override;
};

#endif // QB3CAMERAITEM_H
