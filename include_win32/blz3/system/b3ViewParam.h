/*
**
**	$Filename:	b3ViewParam.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - View parameter handling
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_VIEWPARAM_H
#define B3_SYSTEM_VIEWPARAM_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"

enum b3_view_mode
{
	B3_VIEWMODE_FIT_BEST     = 0,
	B3_VIEWMODE_FIT_WIDTH    = 1,
	B3_VIEWMODE_FIT_HEIGHT   = 2,
	B3_VIEWMODE_ORIGINAL     = 3,
	B3_VIEWMODE_STRETCH_BEST = 4,
	B3_VIEWMODE_SCALE        = 5,
	B3_VIEWMODE_BESTFIT      = B3_VIEWMODE_FIT_BEST
};

#define VIEW_PARAM_NAME_LEN 32

// Window modes
enum b3_window_mode
{
	VIEW_MINIMIZED = -1,
	VIEW_NORMAL    =  0,
	VIEW_MAXIMIZED =  1
};

class CB3ViewParam : public b3Link<CB3ViewParam>
{
	char name[VIEW_PARAM_NAME_LEN];
public:
	// Direct view mode
	b3_view_mode    m_Mode;
	b3_bool         m_Filtered;

	// Part of image
	b3_coord        m_xImgPos,m_yImgPos;
	b3_f64          m_xImgScale,m_yImgScale;

	// Position of window
	b3_coord        m_xWinPos,m_yWinPos;
	b3_res          m_xWinSize,m_yWinSize;
	b3_window_mode  m_WinMode;
public:
	                CB3ViewParam(const char *param_name);
	char           *b3GetName();
};

class CB3ViewParamBase
{
	b3Base<CB3ViewParam> m_Params;
public:
	               ~CB3ViewParamBase();
	CB3ViewParam   *b3GetViewParam(const char *param_name,b3_bool &new_one);
	void            b3DeleteParam(CB3ViewParam *param,b3_bool delete_reg_entry=false);
	CB3ViewParam   *b3GetFirstParam();
	b3_bool         b3LoadParams();
	b3_bool         b3SaveParams();
};

#endif
