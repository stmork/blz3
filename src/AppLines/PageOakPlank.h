/*
**
**	$Filename:	PageOakPlank.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Oak plank properties
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_PAGEOAKPLANK_H__6C6EB5DC_9FC0_4D43_8DF2_A49ED898D843__INCLUDED_)
#define AFX_PAGEOAKPLANK_H__6C6EB5DC_9FC0_4D43_8DF2_A49ED898D843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageOakPlank.h : header file
//

#include "blz3/system/b3PropertyPage.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/base/b3Wood.h"

/////////////////////////////////////////////////////////////////////////////
// CPageOakPlank dialog

class CPageOakPlank : public CB3PropertyPage
{
	// Construction
public:
	b3OakPlank * m_OakPlank;
	CPageOakPlank();
	~CPageOakPlank();

	// Dialog Data
	//{{AFX_DATA(CPageOakPlank)
	enum { IDD = IDD_PAGE_OAKPLANK };
	CB3IntSpinButtonCtrl	m_xTimesCtrl;
	CB3IntSpinButtonCtrl	m_yTimesCtrl;
	CB3FloatSpinButtonCtrl	m_xScaleCtrl;
	CB3FloatSpinButtonCtrl	m_yScaleCtrl;
	CB3FloatSpinButtonCtrl	m_xOffsetCtrl;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageOakPlank)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageOakPlank)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEOAKPLANK_H__6C6EB5DC_9FC0_4D43_8DF2_A49ED898D843__INCLUDED_)
