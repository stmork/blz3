/*
**
**	$Filename:	b3ConditionControl.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"
#include "b3ConditionControl.h"

/*************************************************************************
**                                                                      **
**                        CDlgCreateStencil implementation              **
**                                                                      **
*************************************************************************/

b3_f64 b3ConditionControl::m_Increments[3] =
{
	0.1,1.0,1.0
};

b3_f64 b3ConditionControl::m_Accels[3] =
{
	0.25,5.0,15.0
};

int b3ConditionControl::m_Digits[3] =
{
	3,2,1
};

void b3ConditionControl::b3Init(
	CB3FloatSpinButtonCtrl *ctrl,
	CString                *legend,
	b3_stencil_bound_info  *info,
	b3_condition_ctrl       mode)
{
	m_Ctrl   = ctrl;
	m_Legend = legend;
	m_Mode   = mode;
	m_Info   = info;

	switch(m_Info->unit)
	{
	case B3_STENCIL_UNIT:
		switch(m_Mode)
		{
		case B3_COND_CTRL_START:
			m_Legend->LoadString(IDS_STENCIL_START_UNIT);
			break;
		case B3_COND_CTRL_DIR:
			m_Legend->LoadString(IDS_STENCIL_DIR_UNIT);
			break;
		case B3_COND_CTRL_LEN:
			m_Legend->LoadString(IDS_STENCIL_LEN_UNIT);
			break;
		case B3_COND_CTRL_END:
			m_Legend->LoadString(IDS_STENCIL_END_UNIT);
			break;
		}
		break;

	case B3_STENCIL_LENGTH:
		switch(m_Mode)
		{
		case B3_COND_CTRL_START:
			m_Legend->LoadString(IDS_STENCIL_START_LENGTH);
			break;
		case B3_COND_CTRL_DIR:
			m_Legend->LoadString(IDS_STENCIL_DIR_LENGTH);
			break;
		case B3_COND_CTRL_LEN:
			m_Legend->LoadString(IDS_STENCIL_LEN_LENGTH);
			break;
		case B3_COND_CTRL_END:
			m_Legend->LoadString(IDS_STENCIL_END_LENGTH);
			break;
		}
		break;

	case B3_STENCIL_ANGLE:
		switch(m_Mode)
		{
		case B3_COND_CTRL_START:
			m_Legend->LoadString(IDS_STENCIL_START_ANGLE);
			break;
		case B3_COND_CTRL_DIR:
			m_Legend->LoadString(IDS_STENCIL_DIR_ANGLE);
			break;
		case B3_COND_CTRL_LEN:
			m_Legend->LoadString(IDS_STENCIL_LEN_ANGLE);
			break;
		case B3_COND_CTRL_END:
			m_Legend->LoadString(IDS_STENCIL_END_ANGLE);
			break;
		}
		break;
	}
}

void b3ConditionControl::b3SetPos(
	b3_bool absolute,
	b3_f64  pos)
{
	b3_f64 min,max;

	switch (m_Mode)
	{
	case B3_COND_CTRL_START:
	case B3_COND_CTRL_END:
		min = m_Info->min;
		max = m_Info->max;
		break;

	case B3_COND_CTRL_LEN:
		min =    0;
		max =  100;
		break;

	case B3_COND_CTRL_DIR:
	default:
		min =  -100;
		max =   100;
		break;
	}

	if (absolute)
	{
		m_Ctrl->b3SetRange(min,max);
		m_Ctrl->b3SetDigits(0,m_Digits[B3_STENCIL_UNIT]);
		m_Ctrl->b3SetIncrement(m_Increments[B3_STENCIL_UNIT]);
		m_Ctrl->b3SetPos(pos);
	}
	else
	{
		m_Ctrl->b3SetRange(min * m_Info->factor,max * m_Info->factor);
		m_Ctrl->b3SetDigits(0,m_Digits[m_Info->unit]);
		m_Ctrl->b3SetIncrement(m_Increments[m_Info->unit]);
		m_Ctrl->b3SetPos(pos * m_Info->factor);
	}
}

b3_f64 b3ConditionControl::b3GetPos(b3_bool absolute)
{
	b3_f64 pos;

	if (absolute)
	{
		pos = m_Ctrl->b3GetPos();
	}
	else
	{
		pos = m_Ctrl->b3GetPos() / m_Info->factor;
	}

	return pos;
}
