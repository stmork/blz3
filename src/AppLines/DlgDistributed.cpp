/*
**
**	$Filename:	DlgDistributed.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Distributed properties
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
#include "DlgDistributed.h"

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
**                        CDlgDistributed implementation                **
**                                                                      **
*************************************************************************/

CDlgDistributed::CDlgDistributed(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgDistributed::IDD)
{
	//{{AFX_DATA_INIT(CDlgDistributed)
	m_ActDistributed = FALSE;
	m_ActMotionBlur = FALSE;
	m_PixelFilter = -1;
	m_TimeFilter = -1;
	m_SamplingMethod = -1;
	//}}AFX_DATA_INIT
}


void CDlgDistributed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDistributed)
	DDX_Control(pDX, IDC_SPF, m_SamplesPerFrameCtrl);
	DDX_Control(pDX, IDC_SPP, m_SamplesPerPixelControl);
	DDX_Check(pDX, IDC_ACT_DISTRIBUTED, m_ActDistributed);
	DDX_Check(pDX, IDC_ACT_MOTION_BLUR, m_ActMotionBlur);
	DDX_Radio(pDX, IDC_PFLTR_BOX, m_PixelFilter);
	DDX_Radio(pDX, IDC_TFLTR_BOX, m_TimeFilter);
	DDX_Radio(pDX, IDC_SMP_REGULAR, m_SamplingMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDistributed, CDialog)
	//{{AFX_MSG_MAP(CDlgDistributed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDistributed message handlers

BOOL CDlgDistributed::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgDistributed::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDistributed::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
}
