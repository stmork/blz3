/*
**
**	$Filename:	b3FloatSpinButtonCtrl.h $
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

#ifndef B3_SYSTEM_FLOATSPINBUTTONCTRL_H
#define B3_SYSTEM_FLOATSPINBUTTONCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// b3FloatSpinButtonCtrl.h : header file
//

#include "blz3/b3Config.h"

/////////////////////////////////////////////////////////////////////////////
// CB3FloatSpinButtonCtrl window

#define B3_FSBC_DEFAULT_INCREMENT 1.0
#define B3_FSBC_DEFAULT_ACCEL     5.0
#define B3_FSBC_DEFAULT_DIGITS    2

class CB3FloatSpinButtonCtrl : public CSpinButtonCtrl
{
	b3_f64 m_Increment;
	b3_f64 m_Min;
	b3_f64 m_Max;
	b3_f64 m_Pos;
	b3_f64 m_Accel;
	char   m_Format[16];

public:
	         CB3FloatSpinButtonCtrl();
	virtual ~CB3FloatSpinButtonCtrl();
	
	void     b3DDX(CDataExchange *pDX,b3_f32 &pos);
	void     b3DDX(CDataExchange *pDX,b3_f64 &pos);
	b3_f64   b3SetPos(b3_f64 pos);
	b3_f64   b3SetRange(b3_f64 min,b3_f64 max);
	void     b3SetDigits(int pre=0,int post = B3_FSBC_DEFAULT_DIGITS);
	void     b3SetIncrement(b3_f64 increment = B3_FSBC_DEFAULT_INCREMENT);
	void     b3SetAccel(b3_f64 increment = B3_FSBC_DEFAULT_ACCEL,int secs = 2);
	b3_f64   b3GetPos();
	b3_f64   b3GetAccel();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3FloatSpinButtonCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3FloatSpinButtonCtrl)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(B3_FLOATSPINBUTTONCTRL_H)
