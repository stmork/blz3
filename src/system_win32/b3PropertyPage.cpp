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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.10  2006/05/27 13:32:22  sm
**	- Added CB3Dialog base class for simple dialogs.
**	- Adjusted all tool dialog base classes for better oAW MDA generation
**
**	Revision 1.9  2006/05/23 20:23:42  sm
**	- Some view/bitmap cleanups.
**	- Some more ocean wave ctrl development.
**	- Some preview property page cleanups.
**	- Changed data access methods of b3Tx.
**	
**	Revision 1.8  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.7  2004/05/12 14:13:28  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.6  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.5  2004/05/07 18:19:08  sm
**	- Added some menu entries and toolbar buttons
**	- Fixed missing default title of CB3PropertyPages
**	
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
