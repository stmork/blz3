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

#if !defined(AFX_B3FLOATEDIT_H__9ECAE69B_DC04_11D5_89F4_00E018258F1A__INCLUDED_)
#define AFX_B3FLOATEDIT_H__9ECAE69B_DC04_11D5_89F4_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3floatedit.h : header file
//

#include "blz3/b3Config.h"

/////////////////////////////////////////////////////////////////////////////
// CB3FloatEdit window

class CB3FloatEdit : public CEdit
{
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
	b3_f64 m_Value;
	CB3FloatEdit();

// Attributes
public:

// Operations
public:
	b3_bool b3Check();
	void    b3SetMin(b3_f64 min);
	void    b3SetMax(b3_f64 max);
	void    b3DisableBound();
	void    b3SetDigits(int pre,int post);
	void    b3SetValue(b3_f64 value);

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
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3FLOATEDIT_H__9ECAE69B_DC04_11D5_89F4_00E018258F1A__INCLUDED_)
