/*
**
**	$Filename:	PageWood.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood properties
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_PAGEWOOD_H__C0FD05BB_6D78_4648_8B70_E272FA5B0ADD__INCLUDED_)
#define AFX_PAGEWOOD_H__C0FD05BB_6D78_4648_8B70_E272FA5B0ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageWood.h : header file
//

#include "blz3/system/b3PropertyPage.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3FloatSliderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPageWood dialog

class CPageWood : public CB3PropertyPage
{
// Construction
public:
	CPageWood();
	~CPageWood();

	b3Wood *m_Wood;

// Dialog Data
	//{{AFX_DATA(CPageWood)
	enum { IDD = IDD_PAGE_WOOD };
	CB3FloatSliderCtrl	m_RotZCtrl;
	CB3FloatSliderCtrl	m_RotYCtrl;
	CB3FloatSpinButtonCtrl	m_RingyCtrl;
	CB3FloatSpinButtonCtrl	m_GrainyCtrl;
	CB3FloatSpinButtonCtrl	m_GrainFrequencyCtrl;
	CB3FloatSpinButtonCtrl	m_RingSpacingCtrl;
	CB3FloatSpinButtonCtrl	m_RingNoiseFrequencyCtrl;
	CB3FloatSpinButtonCtrl	m_RingNoiseCtrl;
	CB3FloatSpinButtonCtrl	m_RingFrequencyCtrl;
	CB3FloatSpinButtonCtrl	m_TrunkWobbleCtrl;
	CB3FloatSpinButtonCtrl	m_TrunkWobbleFrequencyCtrl;
	CB3FloatSpinButtonCtrl	m_AngularWobbleCtrl;
	CB3FloatSpinButtonCtrl	m_AngularWobbleFrequencyCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageWood)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageWood)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEWOOD_H__C0FD05BB_6D78_4648_8B70_E272FA5B0ADD__INCLUDED_)
