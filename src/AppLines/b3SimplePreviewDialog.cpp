/*
**
**	$Filename:	b3SimplePreviewDialog.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview sheet
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

#include "b3SimplePreviewDialog.h"
#include "AppLines.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**
**	Revision 1.1  2004/04/25 10:34:51  sm
**	- Completed Cook/Torrance dialog
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3SimplePreviewDialog implementation         **
**                                                                      **
*************************************************************************/

CB3SimplePreviewDialog::CB3SimplePreviewDialog(int dlgId,CWnd* pParent /*=NULL*/)
	: CDialog(dlgId, pParent)
{
	CB3App *app = CB3GetApp();

	//{{AFX_DATA_INIT(CB3SimplePropertyPreviewDialog)
	m_AutoRefresh = TRUE;
	//}}AFX_DATA_INIT
    m_RegKeyAutoRefresh.Format("material.dlg %d.auto refresh",dlgId);
	m_AutoRefresh = app->GetProfileInt(CB3ClientString(),m_RegKeyAutoRefresh,m_AutoRefresh);
}

CB3SimplePreviewDialog::~CB3SimplePreviewDialog()
{
	AfxGetApp()->WriteProfileInt(CB3ClientString(),m_RegKeyAutoRefresh,m_AutoRefresh);
}

void CB3SimplePreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3SimplePreviewDialog)
	DDX_Check(pDX, IDC_AUTO_REFRESH, m_AutoRefresh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3SimplePreviewDialog, CDialog)
	//{{AFX_MSG_MAP(CB3SimplePreviewDialog)
	ON_MESSAGE(WM_USER,OnPreview)
	ON_BN_CLICKED(IDC_AUTO_REFRESH, OnAutoRefresh)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePreviewDialog message handlers

BOOL CB3SimplePreviewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3InitDialog();

	b3UpdateUI();
	OnAutoRefresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3SimplePreviewDialog::OnEdit()
{
	UpdateData();
    b3Preview();
}

void CB3SimplePreviewDialog::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CB3SimplePreviewDialog::OnPreview(WPARAM wParam,LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	b3Preview();
}

void CB3SimplePreviewDialog::b3Preview()
{
	if (m_AutoRefresh)
	{
		b3UpdateUI();
	}
}

void CB3SimplePreviewDialog::OnAutoRefresh() 
{
	// TODO: Add your control notification handler code here
	CWnd *refresh = GetDlgItem(IDC_REFRESH);

	if (refresh != null)
	{
		UpdateData();
		refresh->EnableWindow(!m_AutoRefresh);
	}
}

void CB3SimplePreviewDialog::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
}
