/*
**
**	$Filename:	AppLinesView.cpp $
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
#include "AppLinesView.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2001/08/14 13:34:39  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**
**	Revision 1.7  2001/08/13 15:05:01  sm
**	- Now we can scale and move around with stacked views.
**	
**	Revision 1.6  2001/08/12 19:47:47  sm
**	- Now having correct orthogonal projection incl. aspect ratio
**	
**	Revision 1.5  2001/08/11 19:59:15  sm
**	- Added orthogonal projection
**	
**	Revision 1.4  2001/08/11 16:29:07  sm
**	- Nasty UnCR done
**	- Compiling but not running OpenGL under Unix
**	
**	Revision 1.3  2001/08/11 15:59:58  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**	
**	Revision 1.2  2001/08/10 18:28:58  sm
**	- Some bug fixing
**	- Update functions per view inserted. Now with redraw when loading.
**	
**	Revision 1.1  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesView implementation                  **
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

IMPLEMENT_DYNCREATE(CAppLinesView, CScrollView)

BEGIN_MESSAGE_MAP(CAppLinesView, CScrollView)
	//{{AFX_MSG_MAP(CAppLinesView)
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
	ON_COMMAND(ID_VIEW_ANTIALIAS, OnViewAntialias)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANTIALIAS, OnUpdateViewAntialias)
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
	ON_COMMAND(ID_VIEW_MOVE_TOP, OnViewMoveTop)
	ON_COMMAND(ID_VIEW_MOVE_BOTTOM, OnViewMoveBottom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_RIGHT, OnUpdateViewMoveRight)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_LEFT, OnUpdateViewMoveLeft)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_TOP, OnUpdateViewMoveTop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_BOTTOM, OnUpdateViewMoveBottom)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VIEW_POP, OnViewPop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POP, OnUpdateViewPop)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView construction/destruction

CAppLinesView::CAppLinesView()
{
	// TODO: add construction code here
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	m_SelectMode = B3_SELECT_NOTHING;
	m_Selecting  = false;
}

CAppLinesView::~CAppLinesView()
{
}

BOOL CAppLinesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= CS_OWNDC;
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView drawing

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView printing

BOOL CAppLinesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAppLinesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAppLinesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView diagnostics

#ifdef _DEBUG
void CAppLinesView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppLinesView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppLinesDoc* CAppLinesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppLinesDoc)));
	return (CAppLinesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView message handlers

int CAppLinesView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void CAppLinesView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	wglMakeCurrent(m_DC,NULL);
	wglDeleteContext(m_GC);
}

void CAppLinesView::OnDraw(CDC* pDC)
{
	CAppLinesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CAppLinesView::OnPaint() 
{
	// We have already an HDC, you remember?
	// So we don't need OnDraw();
	CAppLinesDoc *pDoc = GetDocument();
	b3Scene      *scene = pDoc->b3GetScene();

	if (scene != null)
	{
		CRect  rect;
		CPoint pos;

		// Init Drawing
		wglMakeCurrent(m_DC,m_GC);
		pDoc->m_Context.b3StartDrawing();

		pos = GetScrollPosition();
		GetClientRect(&rect);
//		m_RenderView.b3UpdateView(pos.x,750 - pos.y - rect.Height(),1000,750);
		m_RenderView.b3UpdateView(0,0,rect.Width(),rect.Height());
		scene->b3Draw();

		// Done...
		SwapBuffers(m_DC);
		ValidateRect(NULL);
	}
}

BOOL CAppLinesView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
}

void CAppLinesView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	OnUpdate(this,B3_UPDATE_ALL,0);
}

void CAppLinesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAppLinesDoc *pDoc  = GetDocument();
	b3Scene      *scene = pDoc->b3GetScene();
	b3_bool       doInvalidate = false;

	if ((lHint & B3_UPDATE_CAMERA) && (scene != null))
	{
		m_RenderView.b3SetCamera(scene);
		doInvalidate = true;
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

	if (doInvalidate)
	{
		Invalidate();
	}
}

void CAppLinesView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonDown(nFlags, point);
	switch(m_SelectMode)
	{
	case B3_SELECT_MAGNIFICATION:
		SetCapture();
		m_SelectStart = point;
		m_SelectAct   = point;
		m_Selecting   = true;
		break;
	}
}

void CAppLinesView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnMouseMove(nFlags, point);

	if ((m_Selecting) && (m_SelectStart != point)) switch(m_SelectMode)
	{
	case B3_SELECT_MAGNIFICATION:
		CDC *dc = CDC::FromHandle(m_DC);
		CPen red(PS_SOLID,1,RGB(0xff,0x11,0x44));

		dc->SetROP2(R2_NOTXORPEN);
		dc->SelectObject(&red);
		dc->MoveTo(m_SelectStart);
		dc->LineTo(m_SelectStart.x,m_SelectAct.y);
		dc->LineTo(m_SelectAct);
		dc->LineTo(m_SelectAct.x,m_SelectStart.y);
		dc->LineTo(m_SelectStart);

		m_SelectAct = point;

		dc->MoveTo(m_SelectStart);
		dc->LineTo(m_SelectStart.x,m_SelectAct.y);
		dc->LineTo(m_SelectAct);
		dc->LineTo(m_SelectAct.x,m_SelectStart.y);
		dc->LineTo(m_SelectStart);
		break;
	}
}

void CAppLinesView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_Selecting) switch(m_SelectMode)
	{
	case B3_SELECT_MAGNIFICATION:
		CRect  rect;
		b3_f64 xSize,ySize;

		GetClientRect(&rect);
		xSize = rect.Width();
		ySize = rect.Height();
		m_RenderView.b3Select(
			m_SelectStart.x / xSize,
			m_SelectStart.y / ySize,
			point.x / xSize,
			point.y / ySize);
		m_Selecting = false;
		::ReleaseCapture();
		OnUpdate(this,B3_UPDATE_VIEW,NULL);
		m_SelectMode = m_PreviousMode;
		break;
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CAppLinesView::OnViewPerspective() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewTop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_TOP);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewFront() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_FRONT);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_RIGHT);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_LEFT);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewBack() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_BACK);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnUpdateViewTop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_TOP));
}

void CAppLinesView::OnUpdateViewFront(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_FRONT));
}

void CAppLinesView::OnUpdateViewRight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_RIGHT));
}

void CAppLinesView::OnUpdateViewLeft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_LEFT));
}

void CAppLinesView::OnUpdateViewBack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_BACK));
}

void CAppLinesView::OnViewAntialias() 
{
	// TODO: Add your command handler code here
	m_RenderView.m_AntiAliased = !m_RenderView.m_AntiAliased;
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnUpdateViewAntialias(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_RenderView.m_AntiAliased);
}

void CAppLinesView::OnViewSmaller() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(1.25);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewBigger() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(0.8);
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewSelect() 
{
	// TODO: Add your command handler code here
	m_PreviousMode = m_SelectMode;
	m_SelectMode   = B3_SELECT_MAGNIFICATION;
}

void CAppLinesView::OnViewPop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3PopView();
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewOptimal() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Original();
	m_SelectMode = m_PreviousMode;
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnUpdateViewSmaller(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnUpdateViewBigger(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnUpdateViewSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
	pCmdUI->SetCheck(m_SelectMode == B3_SELECT_MAGNIFICATION);
}

void CAppLinesView::OnUpdateViewPop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_RenderView.b3ViewStackNotEmpty());
}

void CAppLinesView::OnUpdateViewOptimal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnViewMoveRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.2,0.0);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewMoveLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(-0.2,0.0);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewMoveTop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.0,0.2);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewMoveBottom() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.0,-0.2);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnUpdateViewMoveRight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnUpdateViewMoveLeft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnUpdateViewMoveTop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppLinesView::OnUpdateViewMoveBottom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}
