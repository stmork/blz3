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

IMPLEMENT_DYNCREATE(CAppMandelView, CScrollView)

BEGIN_MESSAGE_MAP(CAppMandelView, CScrollView)
	//{{AFX_MSG_MAP(CAppMandelView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
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
}

void CAppMandelView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
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

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView message handlers
