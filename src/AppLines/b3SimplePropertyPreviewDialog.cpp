/*
**
**	$Filename:	b3SimplePropertyPreviewDialog.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview and property sheet
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

#include "b3SimplePropertyPreviewDialog.h"
#include "AppLines.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/04/24 20:15:52  sm
**	- Further slide material dialog development
**
**	Revision 1.1  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3SimplePropertyPreviewDialog                **
**                                    implementation                    **
**                                                                      **
*************************************************************************/

CB3SimplePropertyPreviewDialog::CB3SimplePropertyPreviewDialog(int dlgId,CWnd* pParent /*=NULL*/)
	: CDialog(dlgId, pParent)
{
	//{{AFX_DATA_INIT(CB3SimplePropertyPreviewDialog)
	m_AutoRefresh = TRUE;
	//}}AFX_DATA_INIT
}


void CB3SimplePropertyPreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3SimplePropertyPreviewDialog)
	DDX_Check(pDX, IDC_AUTO_REFRESH, m_AutoRefresh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3SimplePropertyPreviewDialog, CDialog)
	//{{AFX_MSG_MAP(CB3SimplePropertyPreviewDialog)
	ON_MESSAGE(WM_USER,OnPreview)
	ON_BN_CLICKED(IDC_AUTO_REFRESH, OnAutoRefresh)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePropertyPreviewDialog message handlers

BOOL CB3SimplePropertyPreviewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3InitDialog();
	CB3PropertyPage::b3InitPropertySheet(this,m_PropertySheet,IDC_PROPERTY);

	b3UpdateUI();
	OnAutoRefresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3SimplePropertyPreviewDialog::OnEdit()
{
	UpdateData();
    b3Preview();
}

void CB3SimplePropertyPreviewDialog::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CB3SimplePropertyPreviewDialog::OnPreview(WPARAM wParam,LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	b3Preview();
}

void CB3SimplePropertyPreviewDialog::b3Preview()
{
	if (m_AutoRefresh)
	{
		b3UpdateUI();
	}
}

void CB3SimplePropertyPreviewDialog::OnAutoRefresh() 
{
	// TODO: Add your control notification handler code here
	CWnd *refresh = GetDlgItem(IDC_REFRESH);

	UpdateData();
	refresh->EnableWindow(!m_AutoRefresh);
}

void CB3SimplePropertyPreviewDialog::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
}
