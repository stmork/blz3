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
#include "MainFrm.h"
#include "b3Action.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.14  2001/09/05 15:21:34  sm
**	- Now object moving/rotating on perspective view.
**
**	Revision 1.13  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**	
**	Revision 1.12  2001/08/21 14:24:14  sm
**	- New selecting code added.
**	
**	Revision 1.11  2001/08/20 19:35:08  sm
**	- Index correction introduced (This is a hack!)
**	- Some OpenGL cleanups
**	
**	Revision 1.10  2001/08/20 14:16:48  sm
**	- Putting data into cmaera and light combobox.
**	- Selecting camera and light.
**	
**	Revision 1.9  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**	
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
	ON_COMMAND(ID_OBJ_SELECT, OnObjSelect)
	ON_COMMAND(ID_OBJ_MOVE, OnObjMove)
	ON_COMMAND(ID_OBJ_ROTATE, OnObjRotate)
	ON_COMMAND(ID_OBJ_SCALE, OnObjScale)
	ON_COMMAND(ID_CAM_MOVE, OnCamMove)
	ON_COMMAND(ID_CAM_TURN, OnCamTurn)
	ON_COMMAND(ID_CAM_ROTATE, OnCamRotate)
	ON_COMMAND(ID_CAM_VIEW, OnCamView)
	ON_CBN_SELCHANGE(ID_CAM_SELECT, OnCamSelect)
	ON_COMMAND(ID_LIGHT_TURN, OnLightTurn)
	ON_CBN_SELCHANGE(ID_LIGHT_SELECT, OnLightSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJ_SELECT, OnUpdateObjSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE, OnUpdateObjMove)
	ON_UPDATE_COMMAND_UI(ID_OBJ_ROTATE, OnUpdateObjRotate)
	ON_UPDATE_COMMAND_UI(ID_OBJ_SCALE, OnUpdateObjScale)
	ON_UPDATE_COMMAND_UI(ID_CAM_MOVE, OnUpdateCamMove)
	ON_UPDATE_COMMAND_UI(ID_CAM_TURN, OnUpdateCamTurn)
	ON_UPDATE_COMMAND_UI(ID_CAM_ROTATE, OnUpdateCamRotate)
	ON_UPDATE_COMMAND_UI(ID_CAM_VIEW, OnUpdateCamView)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_TURN, OnUpdateLightTurn)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
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
	m_PreviousMode =
	m_SelectMode   = B3_OBJECT_SELECT;
}

CAppLinesView::~CAppLinesView()
{
}

void CAppLinesView::b3UnsetMagnification()
{
	if (m_SelectMode == B3_SELECT_MAGNIFICATION)
	{
		m_SelectMode = m_PreviousMode;
	}
}

void CAppLinesView::b3SetMagnification()
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


	return 0;
}

void CAppLinesView::OnDestroy() 
{
	b3_index i;

	for (i = 0;i < B3_MODE_MAX;i++)
	{
		delete m_Action[i];
	}
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	wglMakeCurrent(m_DC,NULL);
	wglDeleteContext(m_GC);
}

void CAppLinesView::OnInitialUpdate()
{
	// Do necessary Blizzard III stuff!
	CAppLinesDoc *pDoc = GetDocument();

	wglMakeCurrent(m_DC,m_GC);
	pDoc->m_Context.b3Init();
	m_CameraVolume.b3AllocVertices(&pDoc->m_Context);
	m_Scene      = pDoc->m_Scene;
	B3_ASSERT(m_Scene != null);
	m_Camera     = m_Scene->b3GetCamera(true);
	m_Light      = m_Scene->b3GetLight(true);
	m_RenderView.b3SetViewMode(B3_VIEW_3D);

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
	CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	OnUpdate(this,B3_UPDATE_ALL,0);
}

void CAppLinesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAppLinesDoc *pDoc         = GetDocument();
	b3_bool       doInvalidate = false;

	if (lHint & B3_UPDATE_CAMERA)
	{
		m_RenderView.b3SetCamera(m_Camera);
		m_CameraVolume.b3Update(m_Camera);
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
		pDoc->m_Fulcrum.b3Update(pDoc->b3GetFulcrum());
		doInvalidate = true;
	}

	if (doInvalidate)
	{
		Invalidate();
	}
}

void CAppLinesView::b3Update(b3_u32 update_mask)
{
	OnUpdate(this,update_mask,NULL);
}

void CAppLinesView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

BOOL CAppLinesView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
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
	CRect         rect;
	CPoint        pos;

	// Init Drawing
	wglMakeCurrent(m_DC,m_GC);
	pDoc->m_Context.b3StartDrawing();

	pos = GetScrollPosition();
	GetClientRect(&rect);

	// Setup view first
	m_RenderView.b3UpdateView(0,0,rect.Width(),rect.Height());

	// Then draw objects
	m_Scene->b3Draw();
	if (!m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_CameraVolume.b3Draw();
	}
	pDoc->b3DrawFulcrum();

	// Done...
	SwapBuffers(m_DC);
	ValidateRect(NULL);
}

void CAppLinesView::b3DrawRect(
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

void CAppLinesView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchMouseMove(point.x,point.y);
}

void CAppLinesView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonDown(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchLButtonDown(point.x,point.y,nFlags);
}

void CAppLinesView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonUp(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchLButtonUp(point.x,point.y);
}

void CAppLinesView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnRButtonDown(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchRButtonDown(point.x,point.y,nFlags);
}

void CAppLinesView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnRButtonUp(nFlags, point);
	m_Action[m_SelectMode]->b3DispatchRButtonUp(point.x,point.y);
}

void CAppLinesView::OnViewPerspective() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewTop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_TOP);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewFront() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_FRONT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_RIGHT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_LEFT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppLinesView::OnViewBack() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_BACK);
	b3UnsetMagnification();
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
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewBigger() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(0.8);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewSelect() 
{
	// TODO: Add your command handler code here
	b3SetMagnification();
}

void CAppLinesView::OnViewPop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3PopView();
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppLinesView::OnViewOptimal() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Original();
	b3UnsetMagnification();
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

void CAppLinesView::OnObjSelect() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_SELECT;
}

void CAppLinesView::OnObjMove() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_MOVE;
}

void CAppLinesView::OnObjRotate() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_ROTATE;
}

void CAppLinesView::OnObjScale() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_SCALE;
}

void CAppLinesView::OnCamMove() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_MOVE;
}

void CAppLinesView::OnCamTurn() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_TURN;
}

void CAppLinesView::OnCamRotate() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_ROTATE;
}

void CAppLinesView::OnCamView() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_VIEW;
}

void CAppLinesView::OnCamSelect() 
{
	// TODO: Add your command handler code here
	CMainFrame *main;

	main = (CMainFrame *)b3GetApp()->m_pMainWnd;
	m_Camera = main->b3GetSelectedCamera();
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnLightTurn() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_LIGHT_TURN;
}

void CAppLinesView::OnLightSelect() 
{
	// TODO: Add your command handler code here
	CMainFrame *main;

	main = (CMainFrame *)b3GetApp()->m_pMainWnd;
	m_Light = main->b3GetSelectedLight();
	OnUpdate(this,B3_UPDATE_LIGHT,NULL);
}

void CAppLinesView::OnUpdateObjSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_SELECT);
}

void CAppLinesView::OnUpdateObjMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_MOVE);
}

void CAppLinesView::OnUpdateObjRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_ROTATE);
}

void CAppLinesView::OnUpdateObjScale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_SCALE);
}

void CAppLinesView::OnUpdateCamMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_MOVE);
}

void CAppLinesView::OnUpdateCamTurn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_TURN);
}

void CAppLinesView::OnUpdateCamRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_ROTATE);
}

void CAppLinesView::OnUpdateCamView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_VIEW);
}

void CAppLinesView::OnUpdateLightTurn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_LIGHT_TURN);
}

void CAppLinesView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
// TODO: Add your specialized code here and/or call the base class
	CMainFrame *main;

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	main = (CMainFrame *)b3GetApp()->m_pMainWnd;
	if (bActivate)
	{
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		main->b3UpdateLightBox(m_Scene,m_Light);
	}
	else
	{
		main->b3Clear();
	}
}
