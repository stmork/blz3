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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuperSampling, CDialog)
	//{{AFX_MSG_MAP(CDlgSuperSampling)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuperSampling message handlers

BOOL CDlgSuperSampling::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSuperSampling::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSuperSampling::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
}
