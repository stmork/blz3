/*
**
**	$Filename:	DlgSuperSampling.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3SuperSampling properties
**
**	(C) Copyright 2001  Steffen A. Mork
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
#include "DlgSuperSampling.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/11/25 12:25:31  sm
**	- Completing some dialogs:
**	  o super sampling
**	  o distributed raytracing
**
**	Revision 1.2  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.1  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgSuperSampling implementation              **
**                                                                      **
*************************************************************************/

CDlgSuperSampling::CDlgSuperSampling(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgSuperSampling::IDD)
{
	//{{AFX_DATA_INIT(CDlgSuperSampling)
	m_ActSuperSampling = FALSE;
	m_GreenLegend = _T("");
	m_BlueLegend = _T("");
	m_RedLegend = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSuperSampling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuperSampling)
	DDX_Control(pDX, IDC_SS4_RED, m_RedCtrl);
	DDX_Control(pDX, IDC_SS4_GREEN, m_GreenCtrl);
	DDX_Control(pDX, IDC_SS4_BLUE, m_BlueCtrl);
	DDX_Check(pDX, IDC_SS4_ACTIVE, m_ActSuperSampling);
	DDX_Text(pDX, IDC_SS4_GREEN_LEGEND, m_GreenLegend);
	DDX_Text(pDX, IDC_SS4_BLUE_LEGEND, m_BlueLegend);
	DDX_Text(pDX, IDC_SS4_RED_LEGEND, m_RedLegend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuperSampling, CDialog)
	//{{AFX_MSG_MAP(CDlgSuperSampling)
	ON_BN_CLICKED(IDC_SS4_ACTIVE, OnActivation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuperSampling message handlers

BOOL CDlgSuperSampling::OnInitDialog() 
{
	m_LastRed   = floor(m_SuperSample->m_Limit.r * 100.0 + 0.5);
	m_LastGreen = floor(m_SuperSample->m_Limit.g * 100.0 + 0.5);
	m_LastBlue  = floor(m_SuperSample->m_Limit.b * 100.0 + 0.5);

	m_ActSuperSampling = m_SuperSample->b3IsActive();
	m_RedLegend.Format(IDS_SS4_RED_LEGEND,m_LastRed);
	m_GreenLegend.Format(IDS_SS4_GREEN_LEGEND,m_LastGreen);
	m_BlueLegend.Format(IDS_SS4_BLUE_LEGEND,m_LastBlue);

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_RedCtrl.SetRangeMin (   0);
	m_RedCtrl.SetRangeMax ( 100);
	m_RedCtrl.SetPageSize (  10);
	m_RedCtrl.SetTicFreq  (  10);
	m_RedCtrl.SetPos      (m_LastRed);
	
	m_GreenCtrl.SetRangeMin (   0);
	m_GreenCtrl.SetRangeMax ( 100);
	m_GreenCtrl.SetPageSize (  10);
	m_GreenCtrl.SetTicFreq  (  10);
	m_GreenCtrl.SetPos      (m_LastGreen);
	
	m_BlueCtrl.SetRangeMin (   0);
	m_BlueCtrl.SetRangeMax ( 100);
	m_BlueCtrl.SetPageSize (  10);
	m_BlueCtrl.SetTicFreq  (  10);
	m_BlueCtrl.SetPos      (m_LastBlue);
	
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSuperSampling::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int  red,green,blue;
	bool update = false;

	red = m_RedCtrl.GetPos();
	if (red != m_LastRed)
	{
		m_LastRed = red;
		m_RedLegend.Format(IDS_SS4_RED_LEGEND,m_LastRed);
		update = true;
	}

	green = m_GreenCtrl.GetPos();
	if (green != m_LastGreen)
	{
		m_LastGreen = green;
		m_GreenLegend.Format(IDS_SS4_GREEN_LEGEND,m_LastGreen);
		update = true;
	}

	blue = m_BlueCtrl.GetPos();
	if (blue != m_LastBlue)
	{
		m_LastBlue = blue;
		m_BlueLegend.Format(IDS_SS4_BLUE_LEGEND,m_LastBlue);
		update = true;
	}

	if (update)
	{
		UpdateData(FALSE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSuperSampling::OnActivation() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RedCtrl.EnableWindow(m_ActSuperSampling);
	m_GreenCtrl.EnableWindow(m_ActSuperSampling);
	m_BlueCtrl.EnableWindow(m_ActSuperSampling);
}

void CDlgSuperSampling::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
	m_SuperSample->m_Limit.r = m_RedCtrl.GetPos()   * 0.01f;
	m_SuperSample->m_Limit.g = m_GreenCtrl.GetPos() * 0.01f;
	m_SuperSample->m_Limit.b = m_BlueCtrl.GetPos()  * 0.01f;
	m_SuperSample->b3Activate(m_ActSuperSampling);
}
