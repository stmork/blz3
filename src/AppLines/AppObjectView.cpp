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
**	Revision 1.1  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**
**
*/

/*************************************************************************
**                                                                      **
**                        CAppObjectView implementation                  **
**                                                                      **
*************************************************************************/

static PIXELFORMATDESCRIPTOR pixelformat =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,0,
	0,0,0,0,0,
	32,
	0,
	0,
	0,
	0,
	0,0,0
};

IMPLEMENT_DYNCREATE(CAppObjectView, CScrollView)

BEGIN_MESSAGE_MAP(CAppObjectView, CScrollView)
	//{{AFX_MSG_MAP(CAppObjectView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_BACK, OnViewBack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOP, OnUpdateViewTop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FRONT, OnUpdateViewFront)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RIGHT, OnUpdateViewRight)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LEFT, OnUpdateViewLeft)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BACK, OnUpdateViewBack)
	ON_COMMAND(ID_VIEW_SMALLER, OnViewSmaller)
	ON_COMMAND(ID_VIEW_SELECT, OnViewSelect)
	ON_COMMAND(ID_VIEW_BIGGER, OnViewBigger)
	ON_COMMAND(ID_VIEW_ORIGINAL, OnViewOptimal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLER, OnUpdateViewSmaller)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECT, OnUpdateViewSelect)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BIGGER, OnUpdateViewBigger)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORIGINAL, OnUpdateViewOptimal)
	ON_COMMAND(ID_VIEW_MOVE_RIGHT, OnViewMoveRight)
	ON_COMMAND(ID_VIEW_MOVE_LEFT, OnViewMoveLeft)
	ON_COMMAND(ID_VIEW_MOVE_UP, OnViewMoveUp)
	ON_COMMAND(ID_VIEW_MOVE_DOWN, OnViewMoveDown)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_RIGHT, OnUpdateViewMove)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VIEW_POP, OnViewPop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POP, OnUpdateViewPop)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
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

void CAppObjectView::b3UnsetMagnification()
{
	if (m_SelectMode == B3_SELECT_MAGNIFICATION)
	{
		m_SelectMode = m_PreviousMode;
	}
}

void CAppObjectView::b3SetMagnification()
{
	if (m_SelectMode != B3_SELECT_MAGNIFICATION)
	{
		m_PreviousMode = m_SelectMode;
		m_SelectMode   = B3_SELECT_MAGNIFICATION;
	}
	else
	{
		m_SelectMode = m_PreviousMode;
	}
}

BOOL CAppObjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= CS_OWNDC;
	cs.style |= WS_MAXIMIZE;
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView drawing

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView printing

BOOL CAppObjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAppObjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAppObjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

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

int CAppObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_DC = GetDC()->m_hDC;
#ifdef _DEBUG
	int                   max,i;
	PIXELFORMATDESCRIPTOR format;

	format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	max = DescribePixelFormat(m_DC,1,0,NULL);
	for (i = 1;i <= max;i++)
	{
		DescribePixelFormat(m_DC,i,format.nSize,&format);
		b3PrintF(B3LOG_DEBUG,"%3ld: %8lx %8lx %2ld %2ld %2ld\n",i,
			format.dwFlags,format.iPixelType,
			format.cColorBits,format.cDepthBits,format.cAccumBits);
	}
#endif
	m_PixelFormatIndex = ChoosePixelFormat(m_DC,&pixelformat);
	SetPixelFormat(m_DC,m_PixelFormatIndex,&pixelformat);
	m_GC = wglCreateContext(m_DC);


	return 0;
}

void CAppObjectView::OnDestroy() 
{
	b3_index i;

	for (i = 0;i < B3_MODE_MAX;i++)
	{
		if (m_Action[i] != null)
		{
			delete m_Action[i];
		}
	}
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	wglMakeCurrent(m_DC,NULL);
	wglDeleteContext(m_GC);
}

void CAppObjectView::OnInitialUpdate()
{
	// Do necessary Blizzard III stuff!
	CAppObjectDoc *pDoc = GetDocument();

	wglMakeCurrent(m_DC,m_GC);
	pDoc->m_Context.b3Init();
	B3_ASSERT(pDoc->m_BBox != null);
	m_RenderView.b3SetViewMode(B3_VIEW_3D);

/*
	m_Action[B3_SELECT_MAGNIFICATION] = new CB3ActionMagnify(this);
	m_Action[B3_OBJECT_SELECT]        = new CB3ActionObjectSelect(this);
	m_Action[B3_OBJECT_MOVE]          = new CB3ActionObjectMove(this);
	m_Action[B3_OBJECT_ROTATE]        = new CB3ActionObjectRotate(this);
	m_Action[B3_OBJECT_SCALE]         = new CB3ActionObjectScale(this);
	m_Action[B3_CAMERA_MOVE]          = new CB3ActionCameraMove(this);
	m_Action[B3_CAMERA_TURN]          = new CB3ActionCameraTurn(this);
	m_Action[B3_CAMERA_ROTATE]        = new CB3ActionCameraRotate(this);
	m_Action[B3_CAMERA_VIEW]          = new CB3ActionCameraView(this);
	m_Action[B3_LIGHT_TURN]           = new CB3ActionLightTurn(this);
*/
	CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	OnUpdate(this,B3_UPDATE_ALL,0);
}

void CAppObjectView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAppObjectDoc *pDoc         = GetDocument();
	b3_bool        doInvalidate = false;

	if (lHint & B3_UPDATE_CAMERA)
	{
//		m_RenderView.b3SetCamera(m_Camera);
//		m_CameraVolume.b3Update(m_Camera);
		doInvalidate = true;
	}
	if (lHint & B3_UPDATE_LIGHT)
	{
	}
	if (lHint & B3_UPDATE_GEOMETRY)
	{
		m_RenderView.b3SetBounds(&pDoc->m_Lower,&pDoc->m_Upper);
		doInvalidate = true;
	}
	if (lHint & B3_UPDATE_VIEW)
	{
		CRect rect;
		CSize size;

		if (m_RenderView.b3IsViewMode(B3_VIEW_3D) && false)
		{
			GetClientRect(&rect);
			size = rect.Size();
		}
		else
		{
			size.cx = 10;
			size.cy = 10;
		}
		SetScrollSizes(MM_TEXT, size);
		doInvalidate = true;
	}

	if (lHint & B3_UPDATE_FULCRUM)
	{
		CMainFrame *main = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		pDoc->m_Fulcrum.b3Update(pDoc->b3GetFulcrum());
		main->b3UpdateFulcrum();
		doInvalidate = true;
	}

	if (doInvalidate)
	{
		Invalidate();
	}
}

void CAppObjectView::b3Update(b3_u32 update_mask)
{
	OnUpdate(this,update_mask,NULL);
}

void CAppObjectView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

BOOL CAppObjectView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CAppObjectView::OnDraw(CDC* pDC)
{
	CAppObjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
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

void CAppObjectView::b3DrawRect(
	b3_coord x1,
	b3_coord y1,
	b3_coord x2,
	b3_coord y2)
{
	CDC *dc = CDC::FromHandle(m_DC);
	CPen red(PS_SOLID,1,RGB(0xff,0x11,0x44));

	dc->SetROP2(R2_NOTXORPEN);
	dc->SelectObject(&red);
	dc->MoveTo(x1,y1);
	dc->LineTo(x2,y1);
	dc->LineTo(x2,y2);
	dc->LineTo(x1,y2);
	dc->LineTo(x1,y1);
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

void CAppObjectView::OnViewPerspective() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppObjectView::OnViewTop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_TOP);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppObjectView::OnViewFront() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_FRONT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppObjectView::OnViewRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_RIGHT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppObjectView::OnViewLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_LEFT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppObjectView::OnViewBack() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_BACK);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppObjectView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppObjectView::OnUpdateViewTop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_TOP));
}

void CAppObjectView::OnUpdateViewFront(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_FRONT));
}

void CAppObjectView::OnUpdateViewRight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_RIGHT));
}

void CAppObjectView::OnUpdateViewLeft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_LEFT));
}

void CAppObjectView::OnUpdateViewBack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_BACK));
}

void CAppObjectView::OnViewSmaller() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(1.25);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnViewBigger() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(0.8);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnViewSelect() 
{
	// TODO: Add your command handler code here
	b3SetMagnification();
}

void CAppObjectView::OnViewPop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3PopView();
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnViewOptimal() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Original();
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnUpdateViewSmaller(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppObjectView::OnUpdateViewBigger(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppObjectView::OnUpdateViewSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
	pCmdUI->SetCheck(m_SelectMode == B3_SELECT_MAGNIFICATION);
}

void CAppObjectView::OnUpdateViewPop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_RenderView.b3ViewStackNotEmpty());
}

void CAppObjectView::OnUpdateViewOptimal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppObjectView::OnViewMoveRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.2,0.0);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnViewMoveLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(-0.2,0.0);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnViewMoveUp() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.0,0.2);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnViewMoveDown() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.0,-0.2);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppObjectView::OnUpdateViewMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}
