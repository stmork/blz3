/*
**
**	$Filename:	b3FloatSliderCtrl.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Spin button control for float values
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_FLOATSLIDERCTRL_H
#define B3_SYSTEM_FLOATSLIDERCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// b3FloatSliderCtrl.h : header file
//

#include "blz3/b3Config.h"

/////////////////////////////////////////////////////////////////////////////
// CB3FloatSliderCtrl 

#define B3_FSBC_DEFAULT_INCREMENT 1.0
#define B3_FSBC_DEFAULT_ACCEL     5.0
#define B3_FSBC_DEFAULT_DIGITS    2

class CB3FloatSliderCtrl : public CSliderCtrl
{
	b3_f64 m_Min;
	b3_f64 m_Max;
	b3_f64 m_Pos;
	int    m_PageSize;
	int    m_TicFrequence;

public:
	         CB3FloatSliderCtrl();
	virtual ~CB3FloatSliderCtrl();
	
	void     b3DDX(CDataExchange *pDX,b3_f32 &pos);
	void     b3DDX(CDataExchange *pDX,b3_f64 &pos);
	b3_f64   b3SetPos(b3_f64 pos);
	b3_f64   b3SetRange(b3_f64 min,b3_f64 max);
	void     b3SetTicks(b3_f64 page,b3_f64 ticks);
	b3_f64   b3GetPos();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3FloatSliderCtrl)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	b3_f64 b3ConvertPos(int pos);
	int    b3ConvertPos(b3_f64 pos);
	b3_f64 b3ConvertRel(int pos);
	int    b3ConvertRel(b3_f64 pos);
};

#endif
