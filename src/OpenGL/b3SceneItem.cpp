/*
**
**	$Filename:	b3SceneItem.cpp $
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

#include "b3SceneItem.h"

QB3SceneItem::QB3SceneItem(b3Scene * scene) :
	QB3AbstractItem(scene, B3_TYPE_SCENE)
{
	static const QIcon world_icon(":/treeview/World");

	setIcon(world_icon);
	setText(scene->b3GetName());
	setSelectable(false);
}
