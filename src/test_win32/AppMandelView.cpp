// AppMandelView.cpp : implementation of the CAppMandelView class
//

#include "stdafx.h"
#include "AppMandel.h"

#include "AppMandelDoc.h"
#include "AppMandelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView

IMPLEMENT_DYNCREATE(CAppMandelView, CB3ScrollView)

BEGIN_MESSAGE_MAP(CAppMandelView, CB3ScrollView)
	//{{AFX_MSG_MAP(CAppMandelView)
	ON_COMMAND(ID_B3_UNFILTERED, OnUnfiltered)
	ON_COMMAND(ID_B3_FILTERED, OnFiltered)
	ON_COMMAND(ID_B3_ORIGINAL, OnOriginal)
	ON_COMMAND(ID_B3_WIDTH, OnWidth)
	ON_COMMAND(ID_B3_HEIGHT, OnHeight)
	ON_COMMAND(ID_B3_FULL, OnFull)
	ON_COMMAND(ID_B3_MORE, OnMore)
	ON_COMMAND(ID_B3_LESS, OnLess)
	ON_COMMAND(ID_B3_MAGNIFY, OnMagnify)
	ON_UPDATE_COMMAND_UI(ID_B3_UNFILTERED, OnUpdateUnfiltered)
	ON_UPDATE_COMMAND_UI(ID_B3_FILTERED, OnUpdateFiltered)
	ON_UPDATE_COMMAND_UI(ID_B3_ORIGINAL, OnUpdateOriginal)
	ON_UPDATE_COMMAND_UI(ID_B3_WIDTH, OnUpdateWidth)
	ON_UPDATE_COMMAND_UI(ID_B3_HEIGHT, OnUpdateHeight)
	ON_UPDATE_COMMAND_UI(ID_B3_FULL, OnUpdateFull)
	ON_UPDATE_COMMAND_UI(ID_B3_MORE, OnUpdateMore)
	ON_UPDATE_COMMAND_UI(ID_B3_LESS, OnUpdateLess)
	ON_UPDATE_COMMAND_UI(ID_B3_MAGNIFY, OnUpdateMagnify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView construction/destruction

CAppMandelView::CAppMandelView()
{
	// TODO: add construction code here

}

CAppMandelView::~CAppMandelView()
{
}

BOOL CAppMandelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView drawing

void CAppMandelView::OnDraw(CDC* pDC)
{
	CAppMandelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CB3ScrollView::OnDraw(pDC);
}

void CAppMandelView::OnInitialUpdate()
{
#if 0
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
#else
	CB3ScrollView::OnInitialUpdate();
#endif
}

b3Document *CAppMandelView::b3GetDocument()
{
	CAppMandelDoc *pDoc;

	if (m_pDocument->IsKindOf(RUNTIME_CLASS(CAppMandelDoc)))
	{
		pDoc = (CAppMandelDoc *)m_pDocument;
		return (b3Document*)pDoc;
	}

	return CB3ScrollView::b3GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView diagnostics

#ifdef _DEBUG
void CAppMandelView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppMandelView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppMandelDoc* CAppMandelView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppMandelDoc)));
	return (CAppMandelDoc*)m_pDocument;
}
#endif //_DEBUG

void CAppMandelView::b3BestFit()
{
	OnFull();
}

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView message handlers

void CAppMandelView::OnUnfiltered() 
{
	// TODO: Add your command handler code here
	b3ScaleBW();
}

void CAppMandelView::OnFiltered() 
{
	// TODO: Add your command handler code here
	b3ScaleGrey();
}

void CAppMandelView::OnOriginal() 
{
	// TODO: Add your command handler code here
	b3ViewMode(B3_VIEWMODE_ORIGINAL);
}

void CAppMandelView::OnWidth() 
{
	// TODO: Add your command handler code here
	b3ViewMode(B3_VIEWMODE_FIT_WIDTH);
}

void CAppMandelView::OnHeight() 
{
	// TODO: Add your command handler code here
	b3ViewMode(B3_VIEWMODE_FIT_HEIGHT);
}

void CAppMandelView::OnFull() 
{
	// TODO: Add your command handler code here
	b3ViewMode(B3_VIEWMODE_BESTFIT);
}

void CAppMandelView::OnMore() 
{
	// TODO: Add your command handler code here
	b3MagnifyMore();
}

void CAppMandelView::OnLess() 
{
	// TODO: Add your command handler code here
	b3MagnifyLess();
}

void CAppMandelView::OnMagnify() 
{
	// TODO: Add your command handler code here
	b3SetMagnifying();
}

void CAppMandelView::OnUpdateUnfiltered(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (!m_ScaleGrey);
}

void CAppMandelView::OnUpdateFiltered(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_ScaleGrey);
}

void CAppMandelView::OnUpdateOriginal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_ORIGINAL);
}

void CAppMandelView::OnUpdateWidth(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_FIT_WIDTH);
}

void CAppMandelView::OnUpdateHeight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_FIT_HEIGHT);
}

void CAppMandelView::OnUpdateFull(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_BESTFIT);
}

void CAppMandelView::OnUpdateMore(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Document *docBase = b3GetDocument();

	pCmdUI->Enable(docBase->m_Tx != null);
}

void CAppMandelView::OnUpdateLess(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Document *docBase = b3GetDocument();

	pCmdUI->Enable(docBase->m_Tx != null);
}

void CAppMandelView::OnUpdateMagnify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Document *docBase = b3GetDocument();

	pCmdUI->Enable(docBase->m_Tx != null);
	if (docBase->m_Tx != null)
	{
		pCmdUI->SetCheck(b3IsMagnifying());
	}
}
