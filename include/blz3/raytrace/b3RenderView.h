/*
**
**	$Filename:	b3Raytrace.h $ 
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing Structure Definitions
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_RENDERVIEW_H
#define B3_RAYTRACE_RENDERVIEW_H

#include "blz3/b3Config.h"

typedef enum b3ViewMode
{
	B3_VIEW_3D = 1,
	B3_VIEW_TOP,
	B3_VIEW_FRONT,
	B3_VIEW_RIGHT,
	B3_VIEW_LEFT,
	B3_VIEW_BACK
} b3_view_mode;

class b3RenderView
{
	b3_view_mode m_ViewMode;
	b3_vector    m_EyePoint;
	b3_vector    m_ViewPoint;
	b3_vector    m_Width;
	b3_vector    m_Height;

public:
	b3_bool          m_AntiAliased;

public:
	        b3RenderView();
	void    b3SetViewMode(b3_view_mode mode);
	b3_bool b3IsViewMode(b3_view_mode mode);
	void    b3SetCamera(b3CameraPart *camera);
	void    b3SetCamera(b3Scene *scene);
	void    b3UpdateView(b3_res xSize,b3_res ySize);
};

#endif
