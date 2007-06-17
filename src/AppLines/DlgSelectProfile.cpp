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

#include "AppLinesInclude.h"

#include "DlgSelectProfile.h"
#include "DlgCreateTriangles.h"
#include "DlgCreateRotShape.h"
#include "DlgCreateSplineShape.h"
#include "DlgEditTriangles.h"
#include "DlgEditRotShape.h"
#include "DlgEditSplineShape.h"
#include "b3Profile.h"

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
	CDlgCreateTriangles    dlg_create_triangles;
	CDlgCreateRotShape     dlg_create_rotshape;
	CDlgCreateSplineShape  dlg_create_spline_shape;
	CDlgEditTriangles      dlg_edit_triangles;
	CDlgEditRotShape       dlg_edit_rotshape;
	CDlgEditSplineShape    dlg_edit_spline_shape;
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
					page = &dlg_create_triangles;
					break;

				case SPLINE_ROT:
					page = &dlg_create_rotshape;
					break;

				case SPLINES_AREA:
				case SPLINES_CYL:
				case SPLINES_RING:
					page = &dlg_create_spline_shape;
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
			page = &dlg_edit_triangles;
			break;

		case SPLINE_ROT:
			page = &dlg_edit_rotshape;
			break;

		case SPLINES_AREA:
		case SPLINES_CYL:
		case SPLINES_RING:
			page = &dlg_edit_spline_shape;
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
	b3Profile::m_LinesProfileBase.b3Sort(b3Profile::b3Sort);
	B3_FOR_BASE(&b3Profile::m_LinesProfileBase,profile)
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
