/*
**
**	$Filename:	AppObjectView.cpp $
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
#include "AppObjectView.h"
#include "MainFrm.h"
#include "b3Action.h"
#include <sys/timeb.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**
**	Revision 1.2  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.1  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppObjectView implementation                 **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppObjectView, CAppRenderView)

BEGIN_MESSAGE_MAP(CAppObjectView, CAppRenderView)
	//{{AFX_MSG_MAP(CAppObjectView)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView construction/destruction

CAppObjectView::CAppObjectView()
{
	// TODO: add construction code here
	b3_index i;

	m_PreviousMode =
	m_SelectMode   = B3_OBJECT_SELECT;
	for (i = 0;i < B3_MODE_MAX;i++)
	{
		m_Action[i] = null;
	}
}

CAppObjectView::~CAppObjectView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView drawing

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView diagnostics

#ifdef _DEBUG
void CAppObjectView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppObjectView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppObjectDoc* CAppObjectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppObjectDoc)));
	return (CAppObjectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView message handlers

void CAppObjectView::OnInitialUpdate()
{
	// Do necessary Blizzard III stuff!
	CAppRenderView::OnInitialUpdate();
}

void CAppObjectView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAppObjectDoc *pDoc         = GetDocument();
	b3_bool        doInvalidate = false;

	if (doInvalidate)
	{
		Invalidate();
	}

	CAppRenderView::OnUpdate(pSender,lHint,pHint);
}

void CAppObjectView::OnPaint() 
{
	// We have already an HDC, you remember?
	// So we don't need OnDraw();
	CAppObjectDoc *pDoc = GetDocument();
	CRect          rect;
	CPoint         pos;
	struct _timeb  start,stop;
	long           sDiff,mDiff;

	_ftime(&start);

	// Init Drawing
	wglMakeCurrent(m_DC,m_GC);
	pDoc->m_Context.b3StartDrawing();

	pos = GetScrollPosition();
	GetClientRect(&rect);

	// Setup view first
	m_RenderView.b3UpdateView(0,0,rect.Width(),rect.Height());

	// Then draw objects
	pDoc->m_BBox->b3Draw();
	pDoc->b3DrawFulcrum();
	_ftime(&stop);

	// Done...
	SwapBuffers(m_DC);
	ValidateRect(NULL);

	mDiff = stop.millitm - start.millitm;
	sDiff = stop.time    - start.time;
	if (mDiff < 0)
	{
		mDiff += 1000;
		sDiff -=    1;
	}
	mDiff += (sDiff * 1000);
	sDiff  = 0;

	if (mDiff > 0)
	{
		CMainFrame *main = CB3GetMainFrame();
	
		main->b3SetPerformance(this,mDiff,pDoc->m_Context.glPolyCount);
	}
}

void CAppObjectView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CB3App     *app  = CB3GetApp();
	CMainFrame *main = CB3GetMainFrame();

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"CAppObjectView::OnActivateView(%s,%p,%p)\n",
		bActivate ? "TRUE " : "FALSE",
		pActivateView, pDeactiveView);
#endif
	
	if (bActivate)
	{
/*
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		main->b3UpdateLightBox(m_Scene,m_Light);
		main->b3UpdateModellerInfo(GetDocument());
		m_Scene->b3SetCamera(m_Camera,true);
*/
		app->b3SetData();
	}
	else
	{
		main->b3Clear();
		main->b3UpdateModellerInfo();
		app->b3GetData();
	}
}

void CAppObjectView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CAppObjectDoc *pDoc = GetDocument();

	CScrollView::OnMouseMove(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchMouseMove(point.x,point.y);
}

void CAppObjectView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CAppObjectDoc *pDoc = GetDocument();

	CScrollView::OnLButtonDown(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchLButtonDown(point.x,point.y,nFlags);
}

void CAppObjectView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CAppObjectDoc *pDoc = GetDocument();

	CScrollView::OnLButtonUp(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchLButtonUp(point.x,point.y);
}

void CAppObjectView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CAppObjectDoc *pDoc = GetDocument();

	CScrollView::OnRButtonDown(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchRButtonDown(point.x,point.y,nFlags);
}

void CAppObjectView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CAppObjectDoc *pDoc = GetDocument();

	CScrollView::OnRButtonUp(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchRButtonUp(point.x,point.y);
}
