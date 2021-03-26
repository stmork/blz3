/*
**
**	$Filename:	b3RenderLight.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        b3RenderLight Implementation                  **
**                                                                      **
*************************************************************************/

b3_vector b3RenderLight::m_LightDirection
{
	0, 0, -1
};

b3RenderLight::b3RenderLight()
{
	m_LightMode = B3_LIGHT_SIMPLE;
	m_Scene     = nullptr;
}

void b3RenderLight::b3SetLightMode(b3_light_mode mode)
{
	m_LightMode = mode;
}

void b3RenderLight::b3SetScene(b3Scene * scene)
{
	m_Scene = scene;
}

void b3RenderLight::b3SetupLight(b3RenderContext * context)
{
#ifdef BLZ3_USE_OPENGL
	b3Color   ambient;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, ">b3RenderLight::b3SetupLight() # %d\n", m_LightMode);
#endif

	switch (m_LightMode)
	{
	case B3_LIGHT_SIMPLE:
	default:
		context->b3LightDefault();
		break;

	case B3_LIGHT_SCENE:
	case B3_LIGHT_SCENE_SPOT:
		B3_ASSERT(m_Scene != nullptr);
		ambient.b3Init(
			m_Scene->m_ShadowBrightness,
			m_Scene->m_ShadowBrightness,
			m_Scene->m_ShadowBrightness);

		context->b3LightNum();
		context->b3LightReset(ambient);

		B3_FOR_TYPED_BASE(b3Light, m_Scene->b3GetLightHead(), light)
		{
			if (light->b3IsActive())
			{
				b3_render_light_info info;

				// Color
				b3RenderContext::b3PkdColorToGL(B3_BLACK,    info.gl_ambient);
				b3RenderContext::b3ColorToGL(light->m_Color, info.gl_diffuse);
				b3RenderContext::b3PkdColorToGL(B3_WHITE,    info.gl_specular);

				// Geometry
				b3RenderContext::b3VectorToGL(&light->m_Position, info.gl_position);

				if ((m_LightMode == B3_LIGHT_SCENE_SPOT) && (light->m_SpotActive))
				{
					b3RenderContext::b3VectorToGL(&light->m_Direction, info.gl_direction);
					info.gl_spot_cutoff = 90;
					info.gl_spot_exp    = light->b3ComputeSpotExponent();
				}
				else
				{
					b3RenderContext::b3VectorToGL(&m_LightDirection, info.gl_direction);
					info.gl_spot_cutoff = 180;
					info.gl_spot_exp    =  10;
				}

				//Attenuation
				info.gl_Ac = 0;
				info.gl_Al = 4.0 / light->m_Distance;
				info.gl_Aq = 0;

				context->b3LightAdd(&info);
			}
		}
		break;
	}

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "<b3RenderLight::b3SetupLight() # %d\n", m_LightMode);
#endif
#endif
}
