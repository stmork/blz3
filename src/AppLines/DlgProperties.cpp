/*
**
**	$Filename:	DlgProperties.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Scene properties
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
#include "DlgProperties.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/08 15:53:37  sm
**	- Cleaned up makefiles for Un*x
**	- New dialog for print buffer size.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgProperties implementation                 **
**                                                                      **
*************************************************************************/

CDlgProperties::CDlgProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProperties)
	m_PrtBuffer = CB3GetLinesApp()->m_PrintBufferSize;
	//}}AFX_DATA_INIT
}


void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperties)
	DDX_Control(pDX, IDC_PRT_BUFFER_SPIN, m_PrtBufferCtrl);
	DDX_Text(pDX, IDC_PRT_BUFFER, m_PrtBuffer);
	DDV_MinMaxInt(pDX, m_PrtBuffer, 1, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProperties, CDialog)
	//{{AFX_MSG_MAP(CDlgProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties message handlers

BOOL CDlgProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PrtBufferCtrl.SetRange(1,128);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProperties::OnOK() 
{
	// TODO: Add extra validation here
	CAppLinesApp * app = CB3GetLinesApp();

	CDialog::OnOK();
	app->m_PrintBufferSize = m_PrtBuffer;
	app->WriteProfileInt(app->b3ClientName(),"print buffer size",m_PrtBuffer);
}
