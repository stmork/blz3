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

#include "b3SystemInclude.h"
#include "blz3/system/b3PropertyPage.h"

/*************************************************************************
**                                                                      **
**                        CB3PropertyPage implementation                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3PropertyPage, CPropertyPage)

CB3PropertyPage::CB3PropertyPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
	//{{AFX_DATA_INIT(CB3PropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_PropertySheet = null;
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
	BOOL result = FALSE;

	try
	{
		b3PreInitDialog();
		CPropertyPage::OnInitDialog();
		b3PostInitDialog();
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
	CWnd *parent = GetParentOwner();

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
	m_psp.pszTitle  = m_Caption; 
	m_psp.dwFlags  |= PSP_USETITLE; 
}
