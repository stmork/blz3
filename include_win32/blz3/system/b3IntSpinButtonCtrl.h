/*
**
**	$Filename:	b3IntSpinButtonCtrl.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Spin button control for int values
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_INTSPINBUTTONCTRL_H
#define B3_SYSTEM_INTSPINBUTTONCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// b3IntSpinButtonCtrl.h : header file
//

#include "blz3/b3Config.h"

/////////////////////////////////////////////////////////////////////////////
// CB3FloatSpinButtonCtrl window

#define B3_ISBC_DEFAULT_INCREMENT 1
#define B3_ISBC_DEFAULT_ACCEL     5

class B3_PLUGIN CB3IntSpinButtonCtrl : public CSpinButtonCtrl
{
	b3_s32 m_Increment;
	b3_s32 m_Min;
	b3_s32 m_Max;
	b3_s32 m_Pos;
	b3_s32 m_Accel;

public:
	         CB3IntSpinButtonCtrl();
	virtual ~CB3IntSpinButtonCtrl();
	
	b3_s32   b3SetPos(b3_s32 pos);
	b3_s32   b3SetRange(b3_s32 min,b3_s32 max);
	void     b3SetIncrement(b3_s32 increment = B3_ISBC_DEFAULT_INCREMENT);
	void     b3SetAccel(b3_s32 increment = B3_ISBC_DEFAULT_ACCEL,int secs = 2);
	void     b3DDX(CDataExchange *pDX,b3_s32 &pos);
	void     b3DDX(CDataExchange *pDX,b3_offset &pos);
	b3_s32   b3GetPos();
	b3_s32   b3GetAccel();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3IntSpinButtonCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3IntSpinButtonCtrl)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(B3_INTTSPINBUTTONCTRL_H)
