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
**	Revision 1.5  2004/12/22 21:36:37  sm
**	- Changed development environment to Visual C++ .net 2003
**
**	Revision 1.4  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**	
**	Revision 1.3  2004/05/05 16:32:26  sm
**	- Fixing following bugs:
**	  o #19 because of variable shadowing
**	  o #20 is no bug due to a KILLFOCUS event. This
**	    is still a perfomrmance problem.
**	  o #17 fixed. Now we use b3Scene::b3GetName() in
**	    combination with the b3Scene filename.
**	
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
	m_Checksum = m_Item->b3Checksum();
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
	CWnd *refresh = GetDlgItem(IDC_REFRESH);

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
