/*
**
**	$Filename:	CDlgSearchPathList.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Search path handling dialog
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "DlgSearchPathList.h"
#include "blz3/system/b3Dir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*************************************************************************
**                                                                      **
**                        Blizzatd III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**
**	Revision 1.1  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgSearchPathList dialog                     **
**                                                                      **
*************************************************************************/

CDlgSearchPathList::CDlgSearchPathList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchPathList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSearchPathList)
	//}}AFX_DATA_INIT
	m_IsList = true;
}


void CDlgSearchPathList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSearchPathList)
	DDX_Control(pDX, IDC_PATH_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSearchPathList, CDialog)
	//{{AFX_MSG_MAP(CDlgSearchPathList)
	ON_BN_CLICKED(IDC_PATH_FIRST, OnPathFirst)
	ON_BN_CLICKED(IDC_PATH_PREV, OnPathPrev)
	ON_BN_CLICKED(IDC_PATH_SUCC, OnPathSucc)
	ON_BN_CLICKED(IDC_PATH_LAST, OnPathLast)
	ON_BN_CLICKED(IDC_PATH_NEW, OnPathNew)
	ON_BN_CLICKED(IDC_PATH_DELETE, OnPathDelete)
	ON_CBN_SELCHANGE(IDC_PATH_LIST, OnSelchangePath)
	ON_CBN_KILLFOCUS(IDC_PATH_LIST, OnKillfocusPath)
	ON_BN_CLICKED(IDC_PATH_EDIT, OnPathEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectList message handlers

BOOL CDlgSearchPathList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_List.LimitText(B3_FILESTRINGLEN);
	m_ListBase = &m_SearchPath->m_SearchPath;
	if ((m_ListBase->First == null) && (m_IsList))
	{
		// In list boxes we need at least one element
		m_Index  = -1;
		m_PathEntry = null;
		OnPathNew();
	}
	else
	{
		b3RefreshList();
		b3SetSelection(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSearchPathList::b3RefreshList()
{
	b3PathEntry *entry;
	char        *string;
	int          index;

	m_List.ResetContent();
	B3_FOR_BASE(m_ListBase,entry)
	{
		string = (char *)((b3Path *)entry);
		index  = m_List.AddString(string);
		m_List.SetItemDataPtr(index,entry);
	}
}

void CDlgSearchPathList::b3SetSelection(int index)
{
	int count;

	count = m_List.GetCount();
	if (count > 0)
	{
		if ((index >= count) || (index < 0))
		{
			index = count - 1;
		}
		m_Index  = m_List.SetCurSel(index);
		m_PathEntry = (b3PathEntry *)m_List.GetItemDataPtr(m_Index);
	}
	else
	{
		m_Index  = -1;
		m_PathEntry = null;
	}
	b3UpdateUI();
}

void CDlgSearchPathList::b3UpdateUI()
{
	CWnd *wndFirst;
	CWnd *wndPrev;
	CWnd *wndSucc;
	CWnd *wndLast;
	CWnd *wndDelete;

	wndFirst  = GetDlgItem(IDC_PATH_FIRST);
	wndPrev   = GetDlgItem(IDC_PATH_PREV);
	wndSucc   = GetDlgItem(IDC_PATH_SUCC);
	wndLast   = GetDlgItem(IDC_PATH_LAST);
	wndDelete = GetDlgItem(IDC_PATH_DELETE);

	if (m_PathEntry != null)
	{
		wndFirst->EnableWindow(m_PathEntry       != m_ListBase->First);
		wndPrev->EnableWindow (m_PathEntry->Prev != null);
		wndSucc->EnableWindow (m_PathEntry->Succ != null);
		wndLast->EnableWindow (m_PathEntry       != m_ListBase->Last);
		wndDelete->EnableWindow(m_IsList ? m_List.GetCount() > 1 : TRUE);
	}
	else
	{
		wndFirst->EnableWindow(FALSE);
		wndPrev->EnableWindow(FALSE);
		wndSucc->EnableWindow(FALSE);
		wndLast->EnableWindow(FALSE);
		wndDelete->EnableWindow(FALSE);
	}
}

void CDlgSearchPathList::OnPathFirst() 
{
	// TODO: Add your control notification handler code here
	if (m_PathEntry != null)
	{
		m_ListBase->b3Remove(m_PathEntry);
		m_ListBase->b3First(m_PathEntry);
		b3RefreshList();
		b3SetSelection(0);
	}
}

void CDlgSearchPathList::OnPathPrev() 
{
	// TODO: Add your control notification handler code here
	b3PathEntry *prev;

	if (m_PathEntry != null)
	{
		prev = m_PathEntry->Prev;
		m_ListBase->b3Remove(m_PathEntry);
		m_ListBase->b3Insert(prev->Prev,m_PathEntry);
		b3RefreshList();
		b3SetSelection(m_Index - 1);
	}
}

void CDlgSearchPathList::OnPathSucc() 
{
	// TODO: Add your control notification handler code here
	b3PathEntry *succ;

	if (m_PathEntry != null)
	{
		succ = m_PathEntry->Succ;
		m_ListBase->b3Remove(m_PathEntry);
		m_ListBase->b3Insert(succ,m_PathEntry);
		b3RefreshList();
		b3SetSelection(m_Index + 1);
	}
}

void CDlgSearchPathList::OnPathLast() 
{
	// TODO: Add your control notification handler code here
	if (m_PathEntry != null)
	{
		m_ListBase->b3Remove(m_PathEntry);
		m_ListBase->b3Append(m_PathEntry);
		b3RefreshList();
		b3SetSelection(-1);
	}
}

void CDlgSearchPathList::OnPathNew() 
{
	// TODO: Add your control notification handler code here
	b3PathEntry *new_entry;

	// Create new list entry
	new_entry = new b3PathEntry((const char *)((b3Path *)m_PathEntry));
	m_ListBase->b3Insert(m_PathEntry,new_entry);

	// Update combobox
	m_Index  = m_List.InsertString(m_Index + 1,(const char *)((b3Path *)new_entry));
	m_List.SetItemDataPtr(m_Index,new_entry);
	m_PathEntry = new_entry;

	b3SetSelection(m_Index);
}

void CDlgSearchPathList::OnPathDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_PathEntry != null)
	{
		m_ListBase->b3Remove(m_PathEntry);
		m_List.DeleteString (m_Index);
		delete m_PathEntry;

		b3SetSelection(m_Index);
	}
}

void CDlgSearchPathList::OnPathEdit() 
{
	// TODO: Add your control notification handler code here
	CString  path   = (const char *)((b3Path *)m_PathEntry);
	b3Folder folder = this;

	folder.b3SelectFolder(path,IDS_SELECT_TEXTURE_PATH);
	strcpy((char *)((b3Path *)m_PathEntry),path);
	b3RefreshList();
	b3SetSelection(0);
}

void CDlgSearchPathList::OnSelchangePath() 
{
	// TODO: Add your control notification handler code here
	m_Index  = m_List.GetCurSel();
	if (m_Index >= 0)
	{
		m_PathEntry = (b3PathEntry *)m_List.GetItemDataPtr(m_Index);
		if ((unsigned long)m_PathEntry == 0xffffffff)
		{
			m_PathEntry = null;
		}
	}
	else
	{
		m_PathEntry = null;
	}
	b3UpdateUI();
}

void CDlgSearchPathList::OnKillfocusPath() 
{
	// TODO: Add your control notification handler code here
	CString        title;

	if (m_PathEntry != null)
	{
		m_List.GetWindowText(title);
		m_List.DeleteString  (m_Index);
		m_List.InsertString  (m_Index,title);
		m_List.SetItemDataPtr(m_Index,m_PathEntry);
		strcpy((char *)((b3Path *)m_PathEntry),title);			  
		b3SetSelection(m_Index);
	}
}
