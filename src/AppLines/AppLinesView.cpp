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

void CAppLinesView::OnDraw(CDC* pDC)
{
	CAppLinesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CAppLinesView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

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


void CAppLinesView::OnPaint() 
{
	// We have already an HDC, you remember?
	// So we don't need OnDraw();
	CAppLinesDoc *pDoc = GetDocument();
	b3Scene      *scene = pDoc->b3GetScene();

	if (scene != null)
	{
		wglMakeCurrent(m_DC,m_GC);

		glClearColor(0.7f,0.7f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnableClientState(GL_VERTEX_ARRAY);

		scene->b3Draw();
		SwapBuffers(m_DC);
		ValidateRect(NULL);
	}
}

void CAppLinesView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CAppLinesDoc *pDoc  = GetDocument();
	b3Scene      *scene = pDoc->b3GetScene();

	if (scene != null)
	{
		wglMakeCurrent(m_DC,m_GC);
		scene->b3SetView(cx,cy);
	}
}

BOOL CAppLinesView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return FALSE;
}
