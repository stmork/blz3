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
#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	m_CreateMode = 0;
	//}}AFX_DATA_INIT
	m_Shape        = null;
	m_ShapeCreator = null;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProfile message handlers

int CDlgSelectProfile::b3Edit(b3Item *item,b3_bool create)
{
	CDlgSelectProfile    dlg;
	CDlgCreateTriangles  dlg_triangles;
	CB3ShapeDialog      *page = null;
	CB3ShapeDialog      *page_to_delete = null;
	int                  result;

	if (create)
	{
		dlg.m_Shape = (b3Shape *)item;
		if (dlg.DoModal() == IDOK)
		{
			switch(dlg.m_CreateMode)
			{
			case 0:
				// This page is dynamically created
				page_to_delete = page = dlg.m_ShapeCreator;
				break;

			case 1:
				switch(item->b3GetClassType())
				{
				case TRIANGLES:
					page = &dlg_triangles;
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
	result = CB3ShapeDialog::b3Edit(page,item,create);
	if (page_to_delete != null)
	{
		delete page_to_delete;
	}
	return result;
}

BOOL CDlgSelectProfile::OnInitDialog() 
{
	b3Profile *profile;
	b3_u32     class_type = m_Shape->b3GetClassType();
	LVITEM     item;
	int        i = 0;

	CDialog::OnInitDialog();
	m_ImageList.Create(64,64,ILC_COLOR8,30,8);
	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ListCtrl.SetIconSpacing(128,96);

	// TODO: Add extra initialization here
	memset(&item,0,sizeof(item));
	item.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
	LinesProfileBase.b3Sort(b3Profile::b3Compare);
	B3_FOR_BASE(&LinesProfileBase,profile)
	{
		if (profile->b3MatchClassType(class_type))
		{
			profile->b3AddImage(&m_ImageList);
			item.iImage  = i;
			item.lParam  = (LPARAM)profile;
			item.state   = 0; // Preselection see CB3ImageList
			item.pszText = (char *)profile->b3GetTitle();
			m_ListCtrl.InsertItem(&item);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectProfile::OnOK() 
{
	// TODO: Add extra validation here
	b3Profile           *profile = null;
	POSITION             pos;
	int                  index;

	CDialog::OnOK();
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
				m_ShapeCreator = profile->b3GetCreateDialog();
			}
		}
		break;
	}
}
