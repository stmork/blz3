/*
**
**	$Filename:	b3RenderLight.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

#ifndef B3_RAYTRACE_RENDERLIGHT_H
#define B3_RAYTRACE_RENDERLIGHT_H

#include "blz3/base/b3Render.h"

/**
 * This enumeration lists the possible light sources for OpenGL.
 */
enum b3_light_mode
{
	B3_LIGHT_SIMPLE,     //!< One simple light source.
	B3_LIGHT_SCENE,      //!< Light sources from raytracing scene.
	B3_LIGHT_SCENE_SPOT  //!< Light sources including spot lights.
};

class b3Scene;

/**
 * This class handles scene lighting in different lighting modes.
 */
class B3_PLUGIN b3RenderLight
{
	b3_light_mode  m_LightMode;
	b3Scene       *m_Scene;

	static b3_vector      m_LightDirection;

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3RenderLight();

	/**
	 * This method initializes the light sources of the given render context.
	 *
	 * @param context The render context.
	 * @see b3RenderContext.
	 */
	void b3SetupLight(b3RenderContext *context);

	/**
	 * This method sets the scene which contains the light sources to
	 * setup.
	 *
	 * @param scene The scene which contains the light sources.
	 */
	void b3SetScene(b3Scene *scene);

	/**
	 * This method sets the light initialization mode.
	 *
	 * @param mode The new light mode.
	 */
	void b3SetLightMode(b3_light_mode mode);
};

#endif
