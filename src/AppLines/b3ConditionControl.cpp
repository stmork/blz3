/*
**
**	$Filename:	b3ConditionControl.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "AppLines.h"
#include "b3ConditionControl.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/05/10 15:12:09  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**
**
*/

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
	if (absolute)
	{
		m_Ctrl->b3SetRange(m_Info->min,m_Info->max);
		m_Ctrl->b3SetDigits(0,m_Digits[B3_STENCIL_UNIT]);
		m_Ctrl->b3SetIncrement(m_Increments[B3_STENCIL_UNIT]);
		m_Ctrl->b3SetPos(pos);
	}
	else
	{
		m_Ctrl->b3SetRange(m_Info->min * m_Info->factor,m_Info->max * m_Info->factor);
		m_Ctrl->b3SetDigits(0,m_Digits[m_Info->unit]);
		m_Ctrl->b3SetIncrement(m_Increments[m_Info->unit]);
		m_Ctrl->b3SetPos(pos * m_Info->factor);
	}
}

b3_f64 b3ConditionControl::b3GetPos(b3_bool absolute)
{
	b3_f64 pos = m_Ctrl->b3GetPos();

	return absolute ? pos : pos / m_Info->factor;
}

