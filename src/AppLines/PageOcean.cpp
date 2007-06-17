/*
**
**	$Filename:	BumpOceanWave $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Ozeanwellen
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"
#include "PageOcean.h"

/*************************************************************************
**                                                                      **
**                        BumpOceanWave implementation                  **
**                                                                      **
*************************************************************************/

CPageOcean::CPageOcean()
	: CB3PropertyPage(CPageOcean::IDD)
{
	//{{AFX_DATA_INIT(CPageOcean)
	//}}AFX_DATA_INIT
}

void CPageOcean::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageOcean)
	DDX_Control(pDX, IDC_B3OCEANWAVE_A, m_A);
	DDX_Control(pDX, IDC_B3OCEANWAVE_T_SPIN, m_T);
	DDX_Control(pDX, IDC_B3OCEANWAVE_SIZE, m_Size);
	DDX_Control(pDX, IDC_B3OCEANWAVE_L, m_L);
	DDX_Control(pDX, IDC_B3OCEANWAVE_DIM_SPIN, m_Dim);
	DDX_Control(pDX, IDC_B3OCEANWAVE_WX, m_Wx);
	DDX_Control(pDX, IDC_B3OCEANWAVE_WY, m_Wy);
	DDX_Control(pDX, IDC_OCEANWAVE, m_OceanWaveCtrl);
	//}}AFX_DATA_MAP

	m_A.b3DDX(pDX, m_Ocean->m_A);
	m_T.b3DDX(pDX, m_Ocean->m_T);
	m_Size.b3DDX(pDX, m_Ocean->m_GridSize);
	m_L.b3DDX(pDX, m_Ocean->m_l);
	m_Dim.b3DDX(pDX, m_Ocean->m_Dim);
	m_Wx.b3DDX(pDX, m_Ocean->m_Wx);
	m_Wy.b3DDX(pDX, m_Ocean->m_Wy);
}

BEGIN_MESSAGE_MAP(CPageOcean, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageOcean)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_A, OnEdit)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_T, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_B3OCEANWAVE_T_SPIN, OnSpin)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_SIZE, OnEdit)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_L, OnEdit)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_DIM, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_B3OCEANWAVE_DIM_SPIN, OnSpin)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_WX, OnEdit)
	ON_EN_KILLFOCUS(IDC_B3OCEANWAVE_WY, OnEdit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*************************************************************************
**                                                                      **
**                        Message handlers                              **
**                                                                      **
*************************************************************************/

void CPageOcean::b3PreInitDialog()
{
	m_T.b3SetRange(1.0, 100.0);
	m_Dim.b3SetRange(4, 12);
}

void CPageOcean::b3PostInitDialog()
{
	m_A.b3SetPos(m_Ocean->m_A);
	m_T.b3SetPos(m_Ocean->m_T);
	m_Size.b3SetPos(m_Ocean->m_GridSize);
	m_L.b3SetPos(m_Ocean->m_l);
	m_Dim.b3SetPos(m_Ocean->m_Dim);
	m_Wx.b3SetPos(m_Ocean->m_Wx);
	m_Wy.b3SetPos(m_Ocean->m_Wy);
	m_OceanWaveCtrl.b3SetOcean(m_Ocean);
}

void CPageOcean::b3UpdateUI()
{
	m_OceanWaveCtrl.b3Update();
}

void CPageOcean::OnOK()
{
	CB3PropertyPage::OnOK();
}

void CPageOcean::OnDestroy()
{
	CB3PropertyPage::OnDestroy();
}
