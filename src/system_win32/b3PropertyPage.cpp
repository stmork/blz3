/*
**
**	$Filename:	b3PropertyPage.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for property page dialog
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

#include "blz3/system/b3PropertyPage.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2004/05/06 18:13:52  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**
**	Revision 1.3  2004/04/24 20:15:52  sm
**	- Further slide material dialog development
**	
**	Revision 1.2  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	Revision 1.1  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3PropertyPage implementation                **
**                                                                      **
*************************************************************************/

CB3PropertyPage::CB3PropertyPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
	//{{AFX_DATA_INIT(CB3PropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_PropertySheet = null;
	m_psp.pszTitle  = m_Caption; 
	m_psp.dwFlags  |= PSP_USETITLE; 

}

CB3PropertyPage::~CB3PropertyPage()
{
}

void CB3PropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3PropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3PropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CB3PropertyPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3PropertyPage message handlers

BOOL CB3PropertyPage::OnInitDialog() 
{
	if (m_Caption.IsEmpty())
	{
		GetWindowText(m_Caption);
	}
	m_psp.pszTitle = m_Caption; 

	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3PropertyPage::OnEdit()
{
	UpdateData();
	b3UpdateUI();
}

void CB3PropertyPage::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CB3PropertyPage::b3UpdateUI()
{
	CWnd *parent = GetParent();

	if (parent != null)
	{
		parent->SendMessage(WM_USER);
	}
}

void CB3PropertyPage::b3InitPropertySheet(CDialog *dlg,CPropertySheet &sheet,int id)
{
	CRect  rect;
	CWnd  *dlgItem = dlg->GetDlgItem(id);

	sheet.Create(dlg, WS_CHILD | WS_VISIBLE, 0);
	sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	sheet.ModifyStyle( 0, WS_TABSTOP );

	dlgItem->GetWindowRect( &rect );
	dlg->ScreenToClient( &rect );
	sheet.SetWindowPos( NULL, rect.left-7, rect.top-7, 0, 0, 
			SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
}

void CB3PropertyPage::b3AddToSheet(CPropertySheet *sheet)
{
	m_PropertySheet = sheet;
	m_PropertySheet->AddPage(this);
}

void CB3PropertyPage::b3SetCaption(int id)
{
	m_Caption.LoadString(id);
	m_psp.pszTitle = m_Caption; 
}
