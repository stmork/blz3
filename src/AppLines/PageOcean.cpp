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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
** PROTECTED REGION ID(CDlgBumpOceanWave_log) ENABLED START
**	$Log$
**	Revision 1.1  2006/04/29 11:25:48  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**
**	Revision 1.6  2006/04/19 18:20:26  sm
**	- Regeneration
**	
**	Revision 1.5  2006/04/19 07:48:06  sm
**	- Regeneration
**	
**	Revision 1.4  2006/04/18 20:47:02  sm
**	- Regeneration
**	
**	Revision 1.3  2006/04/18 15:13:16  sm
**	- Added b3Scaling profile class.
**	- Regeneration
**	
**	Revision 1.2  2006/04/18 14:32:13  sm
**	- Added support for generalization.
**	
**	Revision 1.1  2006/04/18 12:18:38  sm
**	- Added ocean library
**	
**	
** PROTECTED REGION END
*/

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
	DDX_Control(pDX, IDC_B3OCEANWAVE_DIM_SPIN, m_Dim);
	DDX_Control(pDX, IDC_B3OCEANWAVE_WX, m_Wx);
	DDX_Control(pDX, IDC_B3OCEANWAVE_WY, m_Wy);
	//}}AFX_DATA_MAP

	m_A.b3DDX(pDX, m_Ocean->m_A);
	m_T.b3DDX(pDX, m_Ocean->m_T);
	m_Size.b3DDX(pDX, m_Ocean->m_GridSize);
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

BOOL CPageOcean::OnInitDialog()
{
	m_T.b3SetRange(1.0, 100.0);
	m_Dim.b3SetRange(4, 12);
	CB3PropertyPage::OnInitDialog();
	m_A.b3SetPos(m_Ocean->m_A);
	m_T.b3SetPos(m_Ocean->m_T);
	m_Size.b3SetPos(m_Ocean->m_GridSize);
	m_Dim.b3SetPos(m_Ocean->m_Dim);
	m_Wx.b3SetPos(m_Ocean->m_Wx);
	m_Wy.b3SetPos(m_Ocean->m_Wy);
	b3UpdateUI();
	return TRUE;
}

void CPageOcean::OnOK()
{
	CB3PropertyPage::OnOK();
}

void CPageOcean::OnDestroy()
{
	CB3PropertyPage::OnDestroy();
}

void CPageOcean::b3UpdateUI()
{
// PROTECTED REGION ID(_10_5_1_15500e1_1145358477964_574357_129_updateui) ENABLED START
// PROTECTED REGION END

}

void CPageOcean::OnEdit()
{
	UpdateData();
}

void CPageOcean::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnEdit();
	*pResult = 0;
}

