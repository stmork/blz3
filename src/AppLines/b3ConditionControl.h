/*
**
**	$Filename:	b3ConditionControl.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 707 $
**	$Date: 2004-05-10 17:12:09 +0200 (Mo, 10 Mai 2004) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Updates controls depending of used shape.
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef CONDITION_CONTROL_H
#define CONDITION_CONTROL_H

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/raytrace/b3Condition.h"

enum b3_condition_ctrl
{
	B3_COND_CTRL_START,
	B3_COND_CTRL_DIR,
	B3_COND_CTRL_LEN,
	B3_COND_CTRL_END
};

class b3ConditionControl
{
	static b3_f64 m_Increments[3];
	static b3_f64 m_Accels[3];
	static int    m_Digits[3];

	CB3FloatSpinButtonCtrl *m_Ctrl;
	CString                *m_Legend;
	b3_condition_ctrl       m_Mode;
	b3_stencil_bound_info  *m_Info;

public:
	void   b3Init(CB3FloatSpinButtonCtrl *ctrl,CString *legend,b3_stencil_bound_info *info,b3_condition_ctrl mode);
	void   b3SetPos(b3_bool absolute,b3_f64 pos);
	b3_f64 b3GetPos(b3_bool absolute);
};

#endif
