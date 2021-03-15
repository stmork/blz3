/*
**
**	$Filename:	b3SceneItem.h $
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

#ifndef QB3SCENEITEM_H
#define QB3SCENEITEM_H

#include "b3AbstractItem.h"

#include <blz3/raytrace/b3Scene.h>

Q_DECLARE_METATYPE(b3Scene *)

class QB3SceneItem : public QB3AbstractItem<b3Scene>
{
public:
	explicit QB3SceneItem(b3Scene * scene);
};

#endif // QB3SCENEITEM_H
