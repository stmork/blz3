/*
**
**	$Filename:	PageScaling.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Scaling properties
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
#include "PageScaling.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CPageScaling implementation                   **
**                                                                      **
*************************************************************************/


CPageScaling::CPageScaling() : CB3PropertyPage(CPageScaling::IDD)
{
	//{{AFX_DATA_INIT(CPageScaling)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageScaling::~CPageScaling()
{
}

void CPageScaling::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageScaling)
	DDX_Control(pDX, IDC_SCALE_X,   m_xScaleCtrl);
	DDX_Control(pDX, IDC_SCALE_Y,   m_yScaleCtrl);
	DDX_Control(pDX, IDC_SCALE_Z,   m_zScaleCtrl);
	//}}AFX_DATA_MAP
	m_ScaleCtrl.b3DDX(pDX);
}


BEGIN_MESSAGE_MAP(CPageScaling, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageScaling)
	ON_EN_KILLFOCUS(IDC_SCALE_X, OnEdit)
	ON_EN_KILLFOCUS(IDC_SCALE_Y, OnEdit)
	ON_EN_KILLFOCUS(IDC_SCALE_Z, OnEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageScaling message handlers

BOOL CPageScaling::OnInitDialog() 
{
	m_ScaleCtrl.b3Init(&m_Scaling->m_Scale,&m_xScaleCtrl,&m_yScaleCtrl,&m_zScaleCtrl);

	CB3PropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
