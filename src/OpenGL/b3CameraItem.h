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

#ifndef QB3CAMERAITEM_H
#define QB3CAMERAITEM_H

#include <QStandardItem>

#include <blz3/raytrace/b3Scene.h>

class QB3CameraItem : public QStandardItem
{
public:
	explicit QB3CameraItem(b3CameraPart * camera);

private:
	b3CameraPart * m_Camera = nullptr;
};

#endif // QB3CAMERAITEM_H
