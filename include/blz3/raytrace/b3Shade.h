/*
**
**	$Filename:	b3Shade.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for root scene
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_SHADE_H
#define B3_RAYTRACE_SHADE_H

#include "blz3/raytrace/b3Base.h"

class B3_PLUGIN b3Shader
{
protected:
	b3Scene                *m_Scene;
	b3Nebular              *m_Nebular;
	b3_count                m_TraceDepth;

public:
	                        b3Shader(b3Scene *scene);

	virtual void            b3Prepare();
	        b3_bool         b3Shade(b3_ray *ray,b3_count depth = 0);
	        void            b3Shade(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &result);

	inline  b3_bool         b3FindObscurer(b3_ray *ray,b3_f64 max = DBL_MAX)
	{
		return m_Scene->b3Intersect(ray,max);
	}

protected:
	virtual void            b3ShadeSurface(b3_surface &surface,b3_count depth) = 0;
	virtual void            b3ShadePostMaterial(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &material_shading,b3Color &result) = 0;
	virtual void            b3ShadeLight(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &result) = 0;

protected:
	void                    b3ComputeOutputRays(b3_surface *surface);
};

///////////////
// Phong shader
///////////////
class B3_PLUGIN b3ShaderPhong : public b3Shader
{
public:
	               b3ShaderPhong(b3Scene *scene);

protected:
	       void    b3ShadeSurface(b3_surface &surface,b3_count depth);
	       void    b3ShadePostMaterial(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &material_shading,b3Color &result);
	       void    b3ShadeLight(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &result);
};

//////////////
// Mork shader
//////////////
class B3_PLUGIN b3ShaderMork : public b3Shader
{
	b3_f64 m_ShadowFactor;

public:
	         b3ShaderMork(b3Scene *scene);

	void     b3Prepare();

protected:
	void     b3ShadeSurface(b3_surface &surface,b3_count depth);
	void     b3ShadePostMaterial(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &material_shading,b3Color &result);
	void     b3ShadeLight(b3Light *light,b3_light_info *jit,b3_surface *surface,b3Color &result);
};

#endif
