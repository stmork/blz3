/*
**
**	$Filename:	b3ExampleScene.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - constructing simple scenes
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        b3ExampleScene implementation                 **
**                                                                      **
*************************************************************************/

inline void b3ExampleScene::b3SetCameraName(b3CameraPart *camera,int id)
{
	CString name;

	name.LoadString(id);
	camera->b3SetName(name);
}

inline void b3ExampleScene::b3SetLightName(b3Light *light,int id)
{
	CString name;

	name.LoadString(id);
	light->b3SetName(name);
}

inline void b3ExampleScene::b3SetObjectName(b3BBox *bbox,int id)
{
	CString name;

	name.LoadString(id);
	bbox->b3SetName(name);
}
