// DlgHierarchy.cpp : implementation file
//

#include "blz3/b3Config.h"
#include "AppLines.h"
#include "AppLinesView.h"
#include "DlgHierarchy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHierarchy dialog


CDlgHierarchy::CDlgHierarchy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHierarchy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHierarchy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgHierarchy::~CDlgHierarchy()
{
}


void CDlgHierarchy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHierarchy)
	DDX_Control(pDX, IDC_HIERARCHY, m_Hierarchy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHierarchy, CDialog)
	//{{AFX_MSG_MAP(CDlgHierarchy)
	ON_BN_CLICKED(IDC_ACTIVATE, OnActivate)
	ON_BN_CLICKED(IDC_DEACTIVATE, OnDeactivate)
	ON_BN_CLICKED(IDC_ALL_DEACTIVATE, OnAllDeactivate)
	ON_BN_CLICKED(IDC_ALL_ACTIVATE, OnAllActivate)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_HIERARCHY, OnEndLabelEditHierarchy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHierarchy message handlers

void CDlgHierarchy::AddBBoxes (
	b3BBox    *BBox,
	HTREEITEM  parent)
{
	TV_INSERTSTRUCT item;
	HTREEITEM       newItem;
	long            imgNum;
	b3BBox         *sub;

	while (BBox != null)
	{
		imgNum = 0;
		sub = (b3BBox *)BBox->b3GetBBoxHead()->First;
		if (sub                           != null) imgNum += 1;
		if (BBox->b3GetShapeHead()->First != null) imgNum += 2;

		item.hParent      = parent;
		item.hInsertAfter = TVI_LAST;
		item.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		item.item.pszText        = BBox->BoxName;
		item.item.cchTextMax     = B3_BOXSTRINGLEN;
		item.item.lParam         = (long)BBox;
		item.item.iImage         = imgNum;
		item.item.iSelectedImage = imgNum + 5;

		newItem = m_Hierarchy.InsertItem (&item);
		if (sub != null)
		{
			AddBBoxes (sub,newItem);
		}

		BBox = (b3BBox *)BBox->Succ;
	}
}

void CDlgHierarchy::InitTree()
{
	TV_INSERTSTRUCT item;
	long            num = m_Scene->b3GetBBoxCount();

	b3Free();

	m_TreeItems = (HTREEITEM *)b3Alloc(sizeof(HTREEITEM) * num);
	if (m_TreeItems == null) return;

	m_ImageList.Create(16,16,ILC_COLOR8,5,5);
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_EMPTY));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SHAPES));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SHAPES_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_WORLD));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_EMPTY));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_S_SHAPES));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_S_SHAPES_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_WORLD));

	m_Hierarchy.SetImageList (&m_ImageList,TVSIL_NORMAL);

	item.hParent      = TVI_ROOT;
	item.hInsertAfter = TVI_FIRST;
	item.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	item.item.pszText = "Welt";
	item.item.lParam  = 0;
	item.item.iImage         = 4;
	item.item.iSelectedImage = 9;
	m_TreeItems[0]    = m_Hierarchy.InsertItem (&item);

	AddBBoxes (m_Scene->b3GetFirstBBox(),m_TreeItems[0]);
}

BOOL CDlgHierarchy::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Scene = m_Doc->m_Scene;
	InitTree ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHierarchy::OnActivate() 
{
	// TODO: Add your control notification handler code here
	b3BBox    *BBox;
	HTREEITEM  item = m_Hierarchy.GetSelectedItem();
	HTREEITEM  root = m_Hierarchy.GetRootItem();

	BBox = (b3BBox *)m_Hierarchy.GetItemData(item);
	if (BBox == null) BBox = m_Scene->b3GetFirstBBox();
	BBox->b3Activate();
	m_Doc->SetModifiedFlag();
	m_Doc->UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CDlgHierarchy::OnDeactivate() 
{
	// TODO: Add your control notification handler code here
	b3BBox    *BBox;
	HTREEITEM  item = m_Hierarchy.GetSelectedItem();
	HTREEITEM  root = m_Hierarchy.GetRootItem();

	BBox = (b3BBox *)m_Hierarchy.GetItemData(item);
	if (BBox == null) BBox = m_Scene->b3GetFirstBBox();
	BBox->b3Activate(false);
	m_Doc->SetModifiedFlag();
	m_Doc->UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CDlgHierarchy::OnAllDeactivate() 
{
	// TODO: Add your control notification handler code here
	m_Scene->b3Activate(false);
	m_Doc->SetModifiedFlag();
	m_Doc->UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CDlgHierarchy::OnAllActivate() 
{
	// TODO: Add your control notification handler code here
	m_Scene->b3Activate();
	m_Doc->UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CDlgHierarchy::OnEndLabelEditHierarchy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO     *pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	b3BBox     *BBox;
	// TODO: Add your control notification handler code here
	
	if (BBox->BoxName,pTVDispInfo->item.pszText)
	{
		BBox = (b3BBox *)pTVDispInfo->item.lParam;
		strcpy (BBox->BoxName,pTVDispInfo->item.pszText);
		m_Doc->SetModifiedFlag();
		m_Doc->UpdateAllViews(null,B3_UPDATE_VIEW);
		*pResult = 1;
	}
	else *pResult = 0;
}
