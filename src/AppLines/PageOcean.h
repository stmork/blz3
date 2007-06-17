/*
**
**	$Filename:	BumpOceanWave $
**	$Release:	Dortmund 2006 $
**	$Revision: 1466 $
**	$Date: 2006-10-21 11:46:30 +0200 (Sa, 21 Okt 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Ozeanwellen
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_PAGE_OCEANWAVE_H
#define B3_PAGE_OCEANWAVE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpOceanWave.h : header file
//

#include "blz3/system/b3PropertyPage.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "b3OceanWaveCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAuthor dialog

class CPageOcean : public CB3PropertyPage
{
// Construction
public:
	b3BumpOcean  *m_Ocean;
	CPageOcean();   // standard constructor
// Dialog Data
	//{{AFX_DATA(CPageOcean)
	enum { IDD = IDD_PAGE_OCEAN };
	CB3FloatEdit           m_A;
	CB3FloatSpinButtonCtrl m_T;
	CB3FloatEdit           m_Size;
	CB3FloatEdit           m_L;
	CB3IntSpinButtonCtrl   m_Dim;
	CB3FloatEdit           m_Wx;
	CB3FloatEdit           m_Wy;
	CB3OceanWaveCtrl       m_OceanWaveCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageOcean)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageOcean)
	virtual void  OnOK();
	afx_msg void  OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

#endif
