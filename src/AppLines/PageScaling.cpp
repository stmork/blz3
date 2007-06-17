/*
**
**	$Filename:	PageScaling.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1492 $
**	$Date: 2007-06-07 11:33:07 +0200 (Do, 07 Jun 2007) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "PageScaling.h"

/*************************************************************************
**                                                                      **
**                        CPageScaling implementation                   **
**                                                                      **
*************************************************************************/


CPageScaling::CPageScaling() : CB3PropertyPage(CPageScaling::IDD)
{
	m_Scaling = null;
	//{{AFX_DATA_INIT(CPageScaling)
	m_ScaleMode = -1;
	//}}AFX_DATA_INIT
}

CPageScaling::~CPageScaling()
{
}

void CPageScaling::DoDataExchange(CDataExchange* pDX)
{
	if (!pDX->m_bSaveAndValidate)
	{
		m_ScaleMode = m_Scaling->b3GetScaleMode();
	}
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageScaling)
	DDX_Control(pDX, IDC_SCALE_X,   m_xScaleCtrl);
	DDX_Control(pDX, IDC_SCALE_Y,   m_yScaleCtrl);
	DDX_Control(pDX, IDC_SCALE_Z,   m_zScaleCtrl);
	DDX_Radio(pDX,   IDC_SCALE_BOX_POLAR, m_ScaleMode);
	//}}AFX_DATA_MAP
	m_ScaleCtrl.b3DDX(pDX);
	if (pDX->m_bSaveAndValidate)
	{
		m_Scaling->b3SetScaleMode(m_ScaleMode);
	}
}


BEGIN_MESSAGE_MAP(CPageScaling, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageScaling)
	ON_EN_KILLFOCUS(IDC_SCALE_X, OnEdit)
	ON_EN_KILLFOCUS(IDC_SCALE_Y, OnEdit)
	ON_EN_KILLFOCUS(IDC_SCALE_Z, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_POLAR, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_OBJECT_POLAR, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_BOX_POLAR, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_IPOINT, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_IPOINT_ORIGINAL, OnEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageScaling message handlers

void CPageScaling::b3PreInitDialog()
{
	m_ScaleCtrl.b3Init(&m_Scaling->m_Scale,&m_xScaleCtrl,&m_yScaleCtrl,&m_zScaleCtrl);
}

void CPageScaling::b3PostInitDialog()
{
	m_xScaleCtrl.b3SetRange(0.0001,10000);
	m_yScaleCtrl.b3SetRange(0.0001,10000);
	m_zScaleCtrl.b3SetRange(0.0001,10000);
}
