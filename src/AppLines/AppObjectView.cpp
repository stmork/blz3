/*
**
**	$Filename:	AppObjectView.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for objects
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
**	Revision 1.6  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**
**	Revision 1.5  2002/01/16 16:17:12  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.4  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView construction/destruction

CAppObjectView::CAppObjectView()
{
	// TODO: add construction code here
	b3_index i;

	m_PreviousMode =
	m_SelectMode   = B3_SHAPE_MOVE;
	for (i = 0;i < B3_MODE_MAX;i++)
	{
		m_Action[i] = null;
	}
	m_Camera = new b3CameraPart(CAMERA);
	m_xAngle = 225 * M_PI / 180;
	m_yAngle =  30 * M_PI / 180;
}

CAppObjectView::~CAppObjectView()
{
	delete m_Camera;
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
	CAppObjectDoc *pDoc         = GetDocument();

	m_Action[B3_SELECT_MAGNIFICATION] = new CB3ActionMagnify(this);
	m_Action[B3_SHAPE_MOVE]           = new CB3ActionShapeMove(this);
	m_Action[B3_SHAPE_ROTATE_POINT]   = new CB3ActionShapeRotatePoint(this);
	m_Action[B3_SHAPE_ROTATE_AXIS]    = new CB3ActionShapeRotateAxis(this);
	m_Action[B3_SHAPE_SCALE]          = new CB3ActionShapeScale(this);
	m_Action[B3_SHAPE_MIRROR_POINT]   = new CB3ActionShapeMirrorPoint(this);
	m_Action[B3_SHAPE_MIRROR_AXIS]    = new CB3ActionShapeMirrorAxis(this);
	m_Action[B3_SHAPE_MIRROR_AREA]    = new CB3ActionShapeMirrorArea(this);

	CAppRenderView::OnInitialUpdate();

	// TODO: calculate the total size of this view
//	OnUpdate(this,B3_UPDATE_ALL,0);
}

void CAppObjectView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	b3_bool     doInvalidate = false;
	b3_vector   center;

	if (lHint & B3_UPDATE_OBJECT)
	{
		m_BBox = GetDocument()->m_BBox;
	}

	if ((lHint & B3_UPDATE_CAMERA) && (m_BBox != null))
	{
		center.x = m_BBox->m_DimBase.x + 0.5 * m_BBox->m_DimSize.x;
		center.y = m_BBox->m_DimBase.y + 0.5 * m_BBox->m_DimSize.y;
		center.z = m_BBox->m_DimBase.z + 0.5 * m_BBox->m_DimSize.z;
		m_Camera->b3Overview(&center,&m_BBox->m_DimSize,m_xAngle,m_yAngle);
		doInvalidate = true;
	}

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
