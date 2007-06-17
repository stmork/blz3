/*
**
**	$Filename:	b3SimplePreviewDialog.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3SystemInclude.h"
#include "blz3/system/b3SimplePreviewDialog.h"

/*************************************************************************
**                                                                      **
**                        CB3SimplePreviewDialog implementation         **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3SimplePreviewDialog, CDialog)

CB3SimplePreviewDialog::CB3SimplePreviewDialog(b3Item *item,int dlgId,CWnd* pParent /*=NULL*/)
	: CDialog(dlgId, pParent)
{
	CB3App *app = CB3GetApp();

	//{{AFX_DATA_INIT(CB3SimplePropertyPreviewDialog)
	m_AutoRefresh = TRUE;
	//}}AFX_DATA_INIT
	m_Item = item;
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
	DDX_Check(pDX, m_AutoRefreshId, m_AutoRefresh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3SimplePreviewDialog, CDialog)
	//{{AFX_MSG_MAP(CB3SimplePreviewDialog)
	ON_MESSAGE(WM_USER,OnPreview)
	ON_BN_CLICKED(m_AutoRefreshId, OnAutoRefresh)
	ON_BN_CLICKED(m_RefreshId, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePreviewDialog message handlers

BOOL CB3SimplePreviewDialog::OnInitDialog() 
{
	BOOL result = FALSE;

	try
	{
		b3PreInitDialog();
		result = CDialog::OnInitDialog();
		b3PostInitDialog();

		b3UpdateUI();
		m_Checksum = m_Item->b3Checksum();
		OnAutoRefresh();
	}
	catch(exception &exc)
	{
		b3PrintF(B3LOG_NORMAL, 
			"ERROR in CB3Dialog::OnInitDialog()!\n",
			"      %s\n", exc.what());
		result = FALSE;
	}
	return result;  // return TRUE unless you set the focus to a control
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

LRESULT CB3SimplePreviewDialog::OnPreview(WPARAM wParam,LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	b3Preview();
	return 0;
}

void CB3SimplePreviewDialog::b3Preview()
{
	b3PrintF(B3LOG_FULL,">CB3SimplePreviewDialog::b3Preview() m_AutoRefresh = %s\n",
		m_AutoRefresh ? "True" : "False");
	if (m_AutoRefresh)
	{
		b3_u32 checksum = m_Item->b3Checksum();

		if (checksum != m_Checksum)
		{
			b3UpdateUI();
			m_Checksum = checksum;
		}
	}
	b3PrintF(B3LOG_FULL,"<CB3SimplePreviewDialog::b3Preview()");
}

void CB3SimplePreviewDialog::OnAutoRefresh() 
{
	// TODO: Add your control notification handler code here
	CWnd *refresh = GetDlgItem(m_RefreshId);

	if (refresh != null)
	{
		UpdateData();
		refresh->EnableWindow(!m_AutoRefresh);
		b3Preview();
	}
}

void CB3SimplePreviewDialog::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
	m_Checksum = m_Item->b3Checksum();
}
