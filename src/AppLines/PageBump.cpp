/*
**
**	$Filename:	PageMaterial.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden material properties
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
#include "PageBump.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2004/04/24 20:15:51  sm
**	- Further slide material dialog development
**
**	Revision 1.4  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**	Revision 1.3  2004/04/23 16:51:09  sm
**	- Color renaming finished.
**	- Bug #18 fixed: The bump amplitude is read out correctly now.
**	
**	Revision 1.2  2004/04/23 08:23:14  sm
**	- Adjusted wood dialog.
**	- Updated copyright in splash window.
**	
**	Revision 1.1  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CPageBump implementation                      **
**                                                                      **
*************************************************************************/

CPageBump::CPageBump() : CB3PropertyPage(CPageBump::IDD)
{
	//{{AFX_DATA_INIT(CPageBump)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_AmplitudeCtrl.b3SetRange(0.0,1.0);
	m_AmplitudeCtrl.b3SetIncrement(0.0001);
	m_AmplitudeCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERMILLE);
}

CPageBump::~CPageBump()
{
}

void CPageBump::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageBump)
	DDX_Control(pDX, IDC_SPIN_AMPLITUDE, m_AmplitudeCtrl);
	//}}AFX_DATA_MAP
	m_AmplitudeCtrl.b3DDX(pDX,m_Bump->m_Amplitude);
}


BEGIN_MESSAGE_MAP(CPageBump, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageBump)
	ON_EN_KILLFOCUS(IDC_EDIT_AMPLITUDE, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_AMPLITUDE, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageBump message handlers
