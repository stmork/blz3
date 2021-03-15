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

#include <QStandardItem>

#include <blz3/raytrace/b3Scene.h>

Q_DECLARE_METATYPE(b3CameraPart *)

class QB3CameraItem : public QStandardItem
{
public:
	explicit QB3CameraItem(b3CameraPart * camera);

	inline operator b3CameraPart * () const
	{
		return data().value<b3CameraPart *>();
	}

	bool check();
};

#endif // QB3CAMERAITEM_H
