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
**	Revision 1.4  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**
**	Revision 1.3  2004/04/24 20:15:51  sm
**	- Further slide material dialog development
**	
**	Revision 1.2  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
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

CPageOakPlank::CPageOakPlank() : CB3PropertyPage(CPageOakPlank::IDD)
{
	//{{AFX_DATA_INIT(CPageOakPlank)
	//}}AFX_DATA_INIT
	m_xScaleCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_xScaleCtrl.b3SetRange(0.0,10.0);

	m_yScaleCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_yScaleCtrl.b3SetRange(0.0,10.0);
	
	m_xOffsetCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_xOffsetCtrl.b3SetRange(0.0,10.0);
	
	m_xTimesCtrl.b3SetRange(1,10);
	m_yTimesCtrl.b3SetRange(1,10);
}

CPageOakPlank::~CPageOakPlank()
{
}

void CPageOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
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


BEGIN_MESSAGE_MAP(CPageOakPlank, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageOakPlank)
	ON_EN_KILLFOCUS(IDC_SPIN_XSCALE, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_YSCALE, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_XOFFSET,OnEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_XTIMES, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_YTIMES, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XSCALE, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_YSCALE, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XOFFSET,OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XTIMES, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_YTIMES, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageOakPlank message handlers
