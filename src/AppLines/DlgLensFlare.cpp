/*
**
**	$Filename:	DlgLensFlare.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3LensFlare properties
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
#include "DlgLensFlare.h"

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
**                        CDlgLensFlare implementation                  **
**                                                                      **
*************************************************************************/

CDlgLensFlare::CDlgLensFlare(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgLensFlare::IDD)
{
	//{{AFX_DATA_INIT(CDlgLensFlare)
	m_ActLensFlare = FALSE;
	m_LensFlareDistance = _T("");
	//}}AFX_DATA_INIT
}


void CDlgLensFlare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLensFlare)
	DDX_Control(pDX, IDC_LF_COLOR, m_LensFlareColorCtrl);
	DDX_Check(pDX, IDC_ACT_LENS_FLARE, m_ActLensFlare);
	DDX_Text(pDX, IDC_LF_VALUE, m_LensFlareDistance);
	DDV_MaxChars(pDX, m_LensFlareDistance, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLensFlare, CDialog)
	//{{AFX_MSG_MAP(CDlgLensFlare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLensFlare message handlers

BOOL CDlgLensFlare::OnInitDialog() 
{
	m_ActLensFlare = m_LensFlare->b3IsActive();
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LensFlareColorCtrl.b3SetColor(b3Color::b3GetColor(&m_LensFlare->m_Color));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLensFlare::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
}
