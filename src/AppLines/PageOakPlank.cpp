/*
**
**	$Filename:	PageOakPlank.cpp $
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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "PageOakPlank.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CPageOakPlank implementation                  **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CPageOakPlank, CPropertyPage)

CPageOakPlank::CPageOakPlank() : CPropertyPage(CPageOakPlank::IDD)
{
	//{{AFX_DATA_INIT(CPageOakPlank)
	//}}AFX_DATA_INIT
	m_xScaleCtrl.b3SetRange(0.0,10.0);
	m_xScaleCtrl.b3SetIncrement(0.01);
	m_xScaleCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);

	m_yScaleCtrl.b3SetRange(0.0,10.0);
	m_yScaleCtrl.b3SetIncrement(0.01);
	m_yScaleCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	
	m_xOffsetCtrl.b3SetRange(0.0,10.0);
	m_xOffsetCtrl.b3SetIncrement(0.01);
	m_xOffsetCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	
	m_xTimesCtrl.b3SetRange(1,10);
	m_yTimesCtrl.b3SetRange(1,10);
}

CPageOakPlank::~CPageOakPlank()
{
}

void CPageOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageOakPlank)
	DDX_Control(pDX, IDC_SPIN_XSCALE, m_xScaleCtrl);
	DDX_Control(pDX, IDC_SPIN_YSCALE, m_yScaleCtrl);
	DDX_Control(pDX, IDC_SPIN_XOFFSET, m_xOffsetCtrl);
	DDX_Control(pDX, IDC_SPIN_XTIMES, m_xTimesCtrl);
	DDX_Control(pDX, IDC_SPIN_YTIMES, m_yTimesCtrl);
	//}}AFX_DATA_MAP
	m_xScaleCtrl.b3DDX(pDX,m_OakPlank->m_xScale);
	m_yScaleCtrl.b3DDX(pDX,m_OakPlank->m_yScale);
	m_xOffsetCtrl.b3DDX(pDX,m_OakPlank->m_xOffset);
	m_xTimesCtrl.b3DDX(pDX,m_OakPlank->m_xTimes);
	m_yTimesCtrl.b3DDX(pDX,m_OakPlank->m_yTimes);
}


BEGIN_MESSAGE_MAP(CPageOakPlank, CPropertyPage)
	//{{AFX_MSG_MAP(CPageOakPlank)
	ON_EN_KILLFOCUS(IDC_SPIN_XSCALE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_YSCALE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_XOFFSET, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_XTIMES, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_YTIMES, OnSurfaceEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XSCALE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_YSCALE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XOFFSET, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XTIMES, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_YTIMES, OnSurfaceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageOakPlank message handlers

void CPageOakPlank::OnSurfaceEdit()
{
	UpdateData();
	b3UpdateUI();
}

void CPageOakPlank::OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnSurfaceEdit();
	*pResult = 0;
}

void CPageOakPlank::b3UpdateUI()
{
	CWnd *parent = GetParent();

	if (parent != null)
	{
		parent->SendMessage(WM_USER);
	}
}
