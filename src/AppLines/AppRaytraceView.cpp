/*
**
**	$Filename:	AppRaytraceView.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part
**
**	(C) Copyright 2001  Steffen A. Mork
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
#include "AppLinesDoc.h"
#include "AppRaytraceView.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/09/23 15:37:15  sm
**	- Introducing raytracing for Lines III. There is much work
**	  for a b3Display-CScrollView.
**
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppRaytraceView implementation               **
**                                                                      **
*************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView

IMPLEMENT_DYNCREATE(CAppRaytraceView, CB3ScrollView)

BEGIN_MESSAGE_MAP(CAppRaytraceView, CB3ScrollView)
	//{{AFX_MSG_MAP(CAppRaytraceView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView construction/destruction

CAppRaytraceView::CAppRaytraceView()
{
	// TODO: add construction code here

}

CAppRaytraceView::~CAppRaytraceView()
{
}

BOOL CAppRaytraceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAppMandelView drawing

void CAppRaytraceView::OnDraw(CDC* pDC)
{
	CAppLinesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CB3ScrollView::OnDraw(pDC);
}

void CAppRaytraceView::OnInitialUpdate()
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

b3Document *CAppRaytraceView::b3GetDocument()
{
	CAppLinesDoc *pDoc;

	if (m_pDocument->IsKindOf(RUNTIME_CLASS(CAppLinesDoc)))
	{
		pDoc = (CAppLinesDoc *)m_pDocument;
		return (b3Document*)pDoc;
	}

	return CB3ScrollView::b3GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceView diagnostics

#ifdef _DEBUG
void CAppRaytraceView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppRaytraceView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppLinesDoc* CAppRaytraceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppLinesDoc)));
	return (CAppLinesDoc*)m_pDocument;
}
#endif //_DEBUG
