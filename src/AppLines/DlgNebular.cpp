/*
**
**	$Filename:	DlgNebular.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Nebular properties
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
#include "DlgNebular.h"

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
**                        CDlgNebular implementation                    **
**                                                                      **
*************************************************************************/

CDlgNebular::CDlgNebular(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgNebular::IDD)
{
	//{{AFX_DATA_INIT(CDlgNebular)
	m_ActNebular = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgNebular::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNebular)
	DDX_Control(pDX, IDC_NEB_COLOR, m_NebColorCtrl);
	DDX_Check(pDX, IDC_ACT_NEBULAR, m_ActNebular);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNebular, CDialog)
	//{{AFX_MSG_MAP(CDlgNebular)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNebular message handlers

BOOL CDlgNebular::OnInitDialog() 
{
	m_ActNebular = m_Nebular->b3IsActive();
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_NebColorCtrl.b3SetColor(b3Color::b3GetColor(&m_Nebular->m_NebularColor));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNebular::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
}
