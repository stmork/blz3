/*
**
**	$Filename:	PageMaterial.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden material properties
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

#include "PageBump.h"

/*************************************************************************
**                                                                      **
**                        CPageBump implementation                      **
**                                                                      **
*************************************************************************/

CPageBump::CPageBump() : CB3PropertyPage(CPageBump::IDD)
{
	//{{AFX_DATA_INIT(CPageBump)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_AmplitudeCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_BUMP);
}

CPageBump::~CPageBump()
{
}

void CPageBump::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageBump)
	DDX_Control(pDX, IDC_SPIN_AMPLITUDE, m_AmplitudeCtrl);
	//}}AFX_DATA_MAP
	m_AmplitudeCtrl.b3DDX(pDX,m_Bump->m_Amplitude);
}


BEGIN_MESSAGE_MAP(CPageBump, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageBump)
	ON_EN_KILLFOCUS(IDC_EDIT_AMPLITUDE, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_AMPLITUDE, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageBump message handlers

void CPageBump::b3PreInitDialog()
{
}

void CPageBump::b3PostInitDialog()
{
}
