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
	B3_VIEW_3D = 0,
	B3_VIEW_TOP,
	B3_VIEW_FRONT,
	B3_VIEW_RIGHT,
	B3_VIEW_LEFT,
	B3_VIEW_BACK,

	B3_VIEW_MAX            // Number of valid view modes
} b3_view_mode;

class b3RenderViewItem : public b3Link<b3RenderViewItem>
{
public:
	b3_vector m_Size;
	b3_vector m_Mid;
	b3_f64    m_xRelation;
	b3_f64    m_yRelation;

public:
	     b3RenderViewItem();
	     b3RenderViewItem(b3_vector *lower,b3_vector *upper);
	void b3Set(b3_vector *lower,b3_vector *upper);
};

class b3RenderView
{
	b3_view_mode              m_ViewMode;
	b3_vector                 m_EyePoint;
	b3_vector                 m_ViewPoint;
	b3_vector                 m_Width;
	b3_vector                 m_Height;

	b3_vector                 m_Lower;
	b3_vector                 m_Upper;

	b3Base<b3RenderViewItem>  m_ViewStack[B3_VIEW_MAX];
	b3Base<b3RenderViewItem>  m_Depot;
	b3RenderViewItem         *m_Actual;

public:
	b3_bool                   m_AntiAliased;
	b3_bool                   m_AspectRatio;

public:
	                  b3RenderView();
	                 ~b3RenderView();
	void              b3SetViewMode(b3_view_mode mode);
	b3_bool           b3IsViewMode(b3_view_mode mode);
	void              b3SetCamera(b3CameraPart *camera);
	void              b3SetCamera(b3Scene *scene);
	b3_bool           b3SetBounds(b3Scene *scene);
	void              b3SetBounds(b3_vector *lower,b3_vector *upper);
	void              b3PopView();
	b3_bool           b3ViewStackNotEmpty();
	void              b3Original();
	void              b3Scale(b3_f64 scale);
	void              b3Move(b3_f64 xDir,b3_f64 yDir);
	void              b3Select(b3_f64 xStart,b3_f64 yStart,b3_f64 xEnd,b3_f64 yEnd);
	void              b3UpdateView(b3_coord xPos,b3_coord yPos,b3_res xSize,b3_res ySize);

private:
	b3RenderViewItem *b3NewRenderViewItem(b3RenderViewItem *lastItem = null);
};

#endif
