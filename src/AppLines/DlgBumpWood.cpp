/*
**
**	$Filename:	DlgBumpWood.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood bump map
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

#include "AppLines.h"
#include "DlgBumpWood.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2004/04/19 09:00:52  sm
**	- Added bump sampler.
**	- Reactivated bump sampler in bump dialogs.
**
**	Revision 1.3  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**	
**	Revision 1.2  2004/04/17 17:18:33  sm
**	- Made some include adjustments
**	- Added oakplank bump as dialog
**	- Fixed b3BumpWood and b3BumpOakPlank
**	  bump computation
**	
**	Revision 1.1  2004/04/11 18:56:08  sm
**	- Forgotten these files.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpWood::CDlgBumpWood(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBumpWood::IDD, pParent)
{
	m_Bump            = (b3BumpWood *)item;
	m_PageBump.m_Bump = m_Bump;
	m_PageWood.m_Wood = m_Bump;
	//{{AFX_DATA_INIT(CDlgBumpWood)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBumpWood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpWood)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpWood, CDialog)
	//{{AFX_MSG_MAP(CDlgBumpWood)
	ON_MESSAGE(WM_USER,OnPreviewBump)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWood message handlers

void CDlgBumpWood::b3Register()
{
	b3Loader::b3AddClassType(BUMP_WOOD,IDS_BUMP_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgBumpWood::b3Edit(b3Item *item)
{
	CDlgBumpWood dlg(item);

	dlg.DoModal();
	return true;
}

BOOL CDlgBumpWood::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl);
	m_BumpSampler->b3SetBump(m_Bump);

	m_PropertySheet.AddPage(&m_PageBump);
	m_PropertySheet.AddPage(&m_PageWood);

	m_PropertySheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_PropertySheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_PropertySheet.ModifyStyle( 0, WS_TABSTOP );

	CRect rcSheet;
	GetDlgItem( IDC_PROPERTY )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );
	m_PropertySheet.SetWindowPos( NULL, rcSheet.left-7, rcSheet.top-7, 0, 0, 
			SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );

	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBumpWood::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpSampler);
}

void CDlgBumpWood::OnPreviewBump() 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
}
