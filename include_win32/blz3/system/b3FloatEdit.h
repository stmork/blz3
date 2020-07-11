/*
**
**	$Filename:	b3FloatEdit.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Editing a floating point number
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_FLOATEDIT_H
#define B3_SYSTEM_FLOATEDIT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3floatedit.h : header file
//

#include "blz3/b3Config.h"

#define B3_FE_MIN DBL_MIN
#define B3_FE_MAX DBL_MAX

/////////////////////////////////////////////////////////////////////////////
// CB3FloatEdit window

class B3_PLUGIN CB3FloatEdit : public CEdit
{
	DECLARE_DYNAMIC(CB3FloatEdit)

	// Construction
	char    m_Format[16];
	char    m_Buffer[32];
	int     m_PreDigits;
	int     m_PostDigits;
	b3_f64  m_Min;
	b3_f64  m_Max;
	b3_bool m_MinActive;
	b3_bool m_MaxActive;
public:
	b3_f64  m_Value;
	CB3FloatEdit();

	// Attributes
public:

	// Operations
public:
	void    b3DDX(CDataExchange * pDX, b3_f32 & pos);
	void    b3DDX(CDataExchange * pDX, b3_f64 & pos);
	b3_f64  b3SetMin(b3_f64 min);
	b3_f64  b3SetMax(b3_f64 max);
	b3_f64  b3SetRange(b3_f64 min, b3_f64 max);
	void    b3DisableBound();
	void    b3SetDigits(int pre, int post);
	b3_f64  b3SetPos(b3_f64 value);
	b3_f64  b3GetPos();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3FloatEdit)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CB3FloatEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3FloatEdit)
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	b3_f64 b3Check();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(B3_SYSTEM_FLOATEDIT_H)
