/*
**
**	$Filename:	DlgSelectProfile.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select profile for triangle or B spline shape
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "DlgSelectProfile.h"
#include "DlgCreateTriangles.h"
#include "DlgCreateRotShape.h"
#include "DlgCreateSplineShape.h"
#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**
**	Revision 1.8  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.7  2002/07/30 21:46:24  sm
**	- More powerful pixel format selection.
**	- Added b3Comparator class for sorting.
**	
**	Revision 1.6  2002/05/10 15:24:23  sm
**	- Corrected some exceptions in b3Tx
**	- Added double click support in list controls when creating
**	  a new shape.
**	- Some minor fixes done.
**	
**	Revision 1.5  2002/03/11 13:48:54  sm
**	- Cleaned up dialog titles
**	- Fixed some texture bugs concerning palette copying.
**	- Added a triangles profile.
**	
**	Revision 1.4  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**	Revision 1.3  2002/03/09 19:48:14  sm
**	- Added a second profile for spline cylinders.
**	- BSpline shape creation dialog added.
**	- Added some features to b3SplineTemplate class:
**	  o call b3ThroughEndControl() for open splines
**	  o optimize subdivision on b3InitCurve()
**	- Fine tuing and fixed much minor bugs.
**	
**	Revision 1.2  2002/03/05 20:38:24  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.1  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgSelectProfile implementation              **
**                                                                      **
*************************************************************************/

CDlgSelectProfile::CDlgSelectProfile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectProfile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectProfile)
	m_CreateMode = 1;
	//}}AFX_DATA_INIT
	m_Shape        = null;
	m_ShapeCreator = null;
	m_Count        = 0;
	m_CreateMode   = AfxGetApp()->GetProfileInt(CB3ClientString(),"profile select mode",m_CreateMode);
}


void CDlgSelectProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectProfile)
	DDX_Control(pDX, IDC_PROFILELIST, m_ListCtrl);
	DDX_Radio(pDX, IDC_CREATE_PROFILE, m_CreateMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectProfile, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectProfile)
	ON_NOTIFY(NM_DBLCLK, IDC_PROFILELIST, OnProfileDoubleClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProfile message handlers

int CDlgSelectProfile::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgSelectProfile      dlg;
	CDlgCreateTriangles    dlg_triangles;
	CDlgCreateRotShape     dlg_rot_shape;
	CDlgCreateSplineShape  dlg_spline_shape;
	CB3ShapeDialog        *page = null;
	int                    result;

	if (create)
	{
		dlg.m_Shape = (b3Shape *)item;
		if (dlg.DoModal() == IDOK)
		{
			switch(dlg.m_CreateMode)
			{
			case 0:
				page = dlg.m_ShapeCreator;
				break;

			case 1:
				switch(item->b3GetClassType())
				{
				case TRIANGLES:
					page = &dlg_triangles;
					break;

				case SPLINE_ROT:
					page = &dlg_rot_shape;
					break;

				case SPLINES_AREA:
				case SPLINES_CYL:
				case SPLINES_RING:
					page = &dlg_spline_shape;
					break;
				}
				break;
			}
		}
	}
	else
	{
		switch (item->b3GetClassType())
		{
		case TRIANGLES:
			page = null;
			break;

		case SPLINE_ROT:
			page = null;
			break;

		case SPLINES_AREA:
		case SPLINES_CYL:
		case SPLINES_RING:
			page = null;
			break;
		}
	}

	// Customize shape values
	result = CB3ShapeDialog::b3Edit(page,shading_class_type, item,create);
	return result;
}

BOOL CDlgSelectProfile::OnInitDialog() 
{
	b3Profile *profile;
	b3_u32     class_type = m_Shape->b3GetClassType();
	LVITEM     item;
	int        index,selected;
	int        pos = 0;

	CDialog::OnInitDialog();
	m_ImageList.Create(64,64,ILC_COLOR8,30,8);
	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ListCtrl.SetIconSpacing(128,96);

	// TODO: Add extra initialization here
	m_Section.Format("profile selected [%08x]",m_Shape->b3GetClassType());
	selected = AfxGetApp()->GetProfileInt(CB3ClientString(),m_Section,0);
	
	// Init list control
	memset(&item,0,sizeof(item));
	item.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
	LinesProfileBase.b3Sort(b3Profile::b3Sort);
	B3_FOR_BASE(&LinesProfileBase,profile)
	{
		if (profile->b3MatchClassType(class_type))
		{
			index = profile->b3AddImage(&m_ImageList);
			if (index >= 0)
			{
				item.iImage  = index;
				item.iItem   = pos++;
				item.lParam  = (LPARAM)profile;
				item.state   = (index == selected ? item.stateMask : 0); // Preselection see CB3ImageList
				item.pszText = (char *)profile->b3GetTitle();
				if (m_ListCtrl.InsertItem(&item) != -1)
				{
					// Everything went good!
					m_Count++;
				}
			}
		}
	}

	// Disable profile list and make only default creation available.
	if (m_Count == 0)
	{
		// Force default creation mode
		m_CreateMode = 1;
		UpdateData(FALSE);
		GetDlgItem(IDC_CREATE_PROFILE)->EnableWindow(FALSE);

		// Disable profile list
		m_ListCtrl.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectProfile::OnOK() 
{
	// TODO: Add extra validation here
	b3Profile *profile = null;
	POSITION   pos;
	int        index;

	CDialog::OnOK();
	AfxGetApp()->WriteProfileInt(CB3ClientString(),"profile select mode",m_CreateMode);
	
	switch(m_CreateMode)
	{
	case 0:
		pos = m_ListCtrl.GetFirstSelectedItemPosition();
		if (pos != 0)
		{
			index   = m_ListCtrl.GetNextSelectedItem(pos);
			profile = (b3Profile *)m_ListCtrl.GetItemData(index);
			if (profile != null)
			{
				AfxGetApp()->WriteProfileInt(CB3ClientString(),m_Section,index);
				m_ShapeCreator = profile->b3GetCreateDialog();
			}
		}
		break;

	case 1:

		break;
	}
}

void CDlgSelectProfile::OnProfileDoubleClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_CreateMode = 0;
	UpdateData(FALSE);
	OnOK();
	*pResult = 0;
}
