/*
**
**	$Filename:	b3RenderLight.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Rendering OpenGL lights
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_RENDERLIGHT_H
#define B3_RAYTRACE_RENDERLIGHT_H

#include "blz3/base/b3Render.h"

enum b3_light_mode
{
	B3_LIGHT_SIMPLE,
	B3_LIGHT_SCENE,
	B3_LIGHT_SCENE_SPOT
};

class b3Scene;

class B3_PLUGIN b3RenderLight
{
	       b3_light_mode  m_LightMode;
	       b3Scene       *m_Scene;

	static b3_vector      m_LightDirection;

public:
	     b3RenderLight();
	void b3SetupLight(b3RenderContext *context);
	void b3SetScene(b3Scene *scene);
	void b3SetLightMode(b3_light_mode mode);
};

#endif
