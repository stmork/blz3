/*
**
**	$Filename:	AppRenderView.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for OpenGL Documents (scenes/objects)
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
#include "AppRenderView.h"
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
**	Revision 1.10  2002/02/03 21:42:30  sm
**	- Added measurement printing. The measure itself is missing yet.
**	  The support is done in b3RenderView and CAppRenderView.
**	- Added support for units in b3ModellerInfo
**	- Cleaned up some accelerators. Now arrow keys are working
**	  again. The del key is working correctly inside edit controls again.
**
**	Revision 1.9  2002/01/31 19:30:31  sm
**	- Some OpenGL print optimizations done.
**	- Object renaming inside hierarchy tree added.
**	
**	Revision 1.8  2002/01/31 11:50:53  sm
**	- Now we can print OpenGL scenes (Note: We have to do basic
**	  initialization prior to render a scene. Then we can see the scene
**	  on paper)
**	
**	Revision 1.7  2002/01/30 19:46:41  sm
**	- Trying to print in debug mode (and want to see anything)
**	
**	Revision 1.6  2002/01/25 16:34:46  sm
**	- Added printer support (not running yet)
**	
**	Revision 1.5  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.4  2002/01/16 16:17:12  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.3  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.2  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.1  2002/01/13 19:24:12  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppRenderView implementation                 **
**                                                                      **
*************************************************************************/

static PIXELFORMATDESCRIPTOR window_pixelformat =
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

static PIXELFORMATDESCRIPTOR print_pixelformat =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL,
	PFD_TYPE_RGBA,
	24,
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

IMPLEMENT_DYNCREATE(CAppRenderView, CScrollView)

BEGIN_MESSAGE_MAP(CAppRenderView, CScrollView)
	//{{AFX_MSG_MAP(CAppRenderView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_BACK, OnViewBack)
	ON_COMMAND(ID_VIEW_SMALLER, OnViewSmaller)
	ON_COMMAND(ID_VIEW_SELECT, OnViewSelect)
	ON_COMMAND(ID_VIEW_BIGGER, OnViewBigger)
	ON_COMMAND(ID_VIEW_ORIGINAL, OnViewOptimal)
	ON_COMMAND(ID_VIEW_POP, OnViewPop)
	ON_COMMAND(ID_VIEW_MOVE_RIGHT, OnViewMoveRight)
	ON_COMMAND(ID_VIEW_MOVE_LEFT, OnViewMoveLeft)
	ON_COMMAND(ID_VIEW_MOVE_UP, OnViewMoveUp)
	ON_COMMAND(ID_VIEW_MOVE_DOWN, OnViewMoveDown)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOP, OnUpdateViewTop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FRONT, OnUpdateViewFront)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RIGHT, OnUpdateViewRight)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LEFT, OnUpdateViewLeft)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BACK, OnUpdateViewBack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLER, OnUpdateViewSmaller)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECT, OnUpdateViewSelect)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BIGGER, OnUpdateViewBigger)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORIGINAL, OnUpdateViewOptimal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POP, OnUpdateViewPop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_RIGHT, OnUpdateViewMove)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_LEFT, OnUpdateViewMove)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_UP, OnUpdateViewMove)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE_DOWN, OnUpdateViewMove)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/*************************************************************************
**                                                                      **
**                        CAppRenderView construction/destruction       **
**                                                                      **
*************************************************************************/

CAppRenderView::CAppRenderView()
{
	// TODO: add construction code here
	b3_index i;

	m_prtGC     = null;
	m_prtDC     = null;
	m_prtBitmap = null;
	m_prtScale  = 20; // Meaning 1:100 measure
	m_PreviousMode =
	m_SelectMode   = B3_OBJECT_SELECT;
	for (i = 0;i < B3_MODE_MAX;i++)
	{
		m_Action[i] = null;
	}
}

CAppRenderView::~CAppRenderView()
{
}

void CAppRenderView::b3ListPixelFormats(HDC dc)
{
	int                   max,i;
	PIXELFORMATDESCRIPTOR format;
	CString               flags;

	b3PrintF(B3LOG_DEBUG,"-------------------------------\n");
	b3PrintF(B3LOG_DEBUG,"Known OpenGL pixel formats:\n");
	b3PrintF(B3LOG_DEBUG,"(index) (flags) (pixeltype) (color bits) (depth) (accum bits):\n");
	format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	max = DescribePixelFormat(dc,1,0,NULL);
	for (i = 1;i <= max;i++)
	{
		DescribePixelFormat(dc,i,format.nSize,&format);
		flags = "";
		if (format.dwFlags & PFD_DOUBLEBUFFER)        flags += " PFD_DOUBLEBUFFER";
		if (format.dwFlags & PFD_STEREO)              flags += " PFD_STEREO";
		if (format.dwFlags & PFD_DRAW_TO_WINDOW)      flags += " PFD_DRAW_TO_WINDOW";
		if (format.dwFlags & PFD_DRAW_TO_BITMAP)      flags += " PFD_DRAW_TO_BITMAP";
		if (format.dwFlags & PFD_SUPPORT_GDI)         flags += " PFD_SUPPORT_GDI";
		if (format.dwFlags & PFD_SUPPORT_OPENGL)      flags += " PFD_SUPPORT_OPENGL";
		if (format.dwFlags & PFD_GENERIC_FORMAT)      flags += " PFD_GENERIC_FORMAT";
		if (format.dwFlags & PFD_NEED_PALETTE)        flags += " PFD_NEED_PALETTE";
		if (format.dwFlags & PFD_NEED_SYSTEM_PALETTE) flags += " PFD_NEED_SYSTEM_PALETTE";
		if (format.dwFlags & PFD_SWAP_EXCHANGE)       flags += " PFD_SWAP_EXCHANGE";
		if (format.dwFlags & PFD_SWAP_COPY)           flags += " PFD_SWAP_COPY";
		if (format.dwFlags & PFD_SWAP_LAYER_BUFFERS)  flags += " PFD_SWAP_LAYER_BUFFERS";
		if (format.dwFlags & PFD_GENERIC_ACCELERATED) flags += " PFD_GENERIC_ACCELERATED";
		if (format.dwFlags & PFD_SUPPORT_DIRECTDRAW)  flags += " PFD_SUPPORT_DIRECTDRAW";
		b3PrintF(B3LOG_DEBUG,
			"%3x: %8lx %8lx %2ld %2ld %2ld%s\n",i,
			format.dwFlags,format.iPixelType,
			format.cColorBits,format.cDepthBits,format.cAccumBits,flags);
	}
	b3PrintF(B3LOG_DEBUG,"-------------------------------\n");
}

int CAppRenderView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_glDC = GetDC()->GetSafeHdc();
#ifdef _DEBUG
	b3ListPixelFormats(m_glDC);
#endif
	m_glPixelFormatIndex = ChoosePixelFormat(m_glDC,&window_pixelformat);
	SetPixelFormat(m_glDC,m_glPixelFormatIndex,&window_pixelformat);
	m_glGC = wglCreateContext(m_glDC);

	return 0;
}

void CAppRenderView::OnDestroy() 
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
	CB3GetLinesApp()->b3SelectRenderContext(m_glDC,NULL);
	wglDeleteContext(m_glGC);
}

BOOL CAppRenderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= CS_OWNDC;
	cs.style |= WS_MAXIMIZE;
	return CScrollView::PreCreateWindow(cs);
}

void CAppRenderView::b3UnsetMagnification()
{
	if (m_SelectMode == B3_SELECT_MAGNIFICATION)
	{
		m_SelectMode = m_PreviousMode;
	}
}

void CAppRenderView::b3SetMagnification()
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

/*************************************************************************
**                                                                      **
**                        CAppRenderView drawing                        **
**                                                                      **
*************************************************************************/

void CAppRenderView::OnInitialUpdate()
{
	// Do necessary Blizzard III stuff!
	CAppRenderDoc *pDoc = GetDocument();

	wglMakeCurrent(m_glDC,m_glGC);
	pDoc->m_Context.b3Init();
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	m_CameraVolume.b3AllocVertices(&pDoc->m_Context);

	CScrollView::OnInitialUpdate();
}

void CAppRenderView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAppRenderDoc *pDoc         = GetDocument();
	b3_bool        doInvalidate = false;

	if (lHint & B3_UPDATE_CAMERA)
	{
		m_RenderView.b3SetCamera(m_Camera);
		m_CameraVolume.b3Update(m_Camera);
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

	if (lHint & B3_UPDATE_FULCRUM)
	{
		CMainFrame *main = CB3GetMainFrame();

		pDoc->m_Fulcrum.b3Update(pDoc->b3GetFulcrum());
		main->b3UpdateFulcrum();
		doInvalidate = true;
	}

	if (doInvalidate)
	{
		Invalidate();
	}
}

void CAppRenderView::b3Update(b3_u32 update_mask)
{
	OnUpdate(this,update_mask,NULL);
}

void CAppRenderView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

b3_bool CAppRenderView::b3GetDimension(
	b3_f64 &xSize,
	b3_f64 &ySize,
	b3_f64 &unit)
{
	unit = 1.0;
	return m_RenderView.b3GetDimension(xSize,ySize);
}

BOOL CAppRenderView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CAppRenderView::OnDraw(CDC* pDC)
{
	CAppRenderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CAppRenderView::b3Draw(
	b3_res xSize,
	b3_res ySize,
	b3_f64 xOffset,
	b3_f64 yOffset)
{
}

void CAppRenderView::OnPaint() 
{
	// We have already an HDC, you remember?
	// So we don't need OnDraw();
	CRect          rect;
	struct _timeb  start,stop;
	long           sDiff,mDiff;

	// Init Drawing
	CB3GetLinesApp()->b3SelectRenderContext(m_glDC,m_glGC);
	GetClientRect(&rect);
	GetDocument()->m_Context.b3SetBGColor(0.8,0.8,0.8);

	_ftime(&start);
	b3Draw(rect.Width(),rect.Height());
	_ftime(&stop);

	// Done...
	SwapBuffers(m_glDC);
	ValidateRect(NULL);

	// Compute time spent for drawing
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
		CMainFrame *main = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	
		main->b3SetPerformance(this,mDiff,GetDocument()->m_Context.glPolyCount);
	}
}

void CAppRenderView::b3DrawRect(
	b3_coord x1,
	b3_coord y1,
	b3_coord x2,
	b3_coord y2)
{
	CDC *dc = CDC::FromHandle(m_glDC);
	CPen red(PS_SOLID,1,RGB(0xff,0x11,0x44));

	dc->SetROP2(R2_NOTXORPEN);
	dc->SelectObject(&red);
	dc->MoveTo(x1,y1);
	dc->LineTo(x2,y1);
	dc->LineTo(x2,y2);
	dc->LineTo(x1,y2);
	dc->LineTo(x1,y1);
}

b3_bool CAppRenderView::b3IsMouseActionAllowed()
{
	return true;
}

/*************************************************************************
**                                                                      **
**                        CAppRenderView printing                       **
**                                                                      **
*************************************************************************/

BOOL CAppRenderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	CAppLinesApp *app = CB3GetLinesApp();
	CRect         rect;
	PRINTDLG     *prtDlg;
	DEVMODE      *DevMode;

	// Set printer orientation by images dimensions
	prtDlg = &pInfo->m_pPD->m_pd;
	if (app->GetPrinterDeviceDefaults(prtDlg))
	{
		DevMode = (DEVMODE *)::GlobalLock(prtDlg->hDevMode);
		if (DevMode != null)
		{
			if (b3GetDimension(m_prtWidth,m_prtHeight,m_prtUnit))
			{
				DevMode->dmOrientation =
					(short)(m_prtHeight > m_prtWidth ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE);
			}
			else
			{
				GetClientRect(&rect);
				DevMode->dmOrientation =
					(short)(rect.Height() > rect.Width() ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE);
			}
			::GlobalUnlock(prtDlg->hDevMode);
		}
	}
	prtDlg->Flags |= (PD_NOPAGENUMS | PD_NOSELECTION);

	return DoPreparePrinting(pInfo);
}

void CAppRenderView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	BITMAPINFO  info;
	void       *buffer = null;
	int         PixelFormatIndex = -1;
	b3_res      mmWidth,mmHeight;
	b3_size     limit = 32;
	b3_size     denom;

	CScrollView::OnBeginPrinting(pDC, pInfo);

	// Get pixel dimension of one page
	m_prtPageWidth   = pDC->GetDeviceCaps(HORZRES);
	m_prtPageHeight  = pDC->GetDeviceCaps(VERTRES);

	// Use at least 1/3 size to give the printer driver the chance to dither appropriately.
	for (denom = 3;((m_prtPageWidth * m_prtPageHeight * 3) / (denom * 1024 * 1024)) > limit;denom++);
	m_prtPageWidth  /= denom;
	m_prtPageHeight /= denom;

	if (m_RenderView.b3IsViewMode(B3_VIEW_3D) || m_RenderView.b3ViewStackNotEmpty())
	{
		m_prtCountWidth  = 1;
		m_prtCountHeight = 1;
		m_prtLogWidth    = m_prtPageWidth;
		m_prtLogHeight   = m_prtPageHeight;
		m_prtLogOffsetX  = 0;
		m_prtLogOffsetY  = 0;
	}
	else
	{
		// Compute full area dimension in mm
		m_prtHardCopyWidth  = m_prtWidth  * m_prtUnit / m_prtScale;
		m_prtHardCopyHeight = m_prtHeight * m_prtUnit / m_prtScale;

		// Get page size in mm
		mmWidth          = pDC->GetDeviceCaps(HORZSIZE);
		mmHeight         = pDC->GetDeviceCaps(VERTSIZE);

		// Compute complete scene dimension in bitmap coordinates (for tiling)
		m_prtLogWidth    = (b3_f64)m_prtPageWidth  * m_prtHardCopyWidth  / mmWidth;
		m_prtLogHeight   = (b3_f64)m_prtPageHeight * m_prtHardCopyHeight / mmHeight;

		// Compute page width/height in scene coordinates (for tiling)
		m_prtLogOffsetX  = mmWidth  * m_prtScale / m_prtUnit;
		m_prtLogOffsetY  = mmHeight * m_prtScale / m_prtUnit;

		// Compute page count in any direction
		m_prtCountWidth  = (m_prtHardCopyWidth  + mmWidth  - 1) / mmWidth;
		m_prtCountHeight = (m_prtHardCopyHeight + mmHeight - 1) / mmHeight;
		B3_ASSERT((m_prtCountWidth > 0) && (m_prtCountHeight > 0));
	}
	pInfo->SetMaxPage(m_prtCountWidth * m_prtCountHeight);

	memset (&info,0,sizeof(info));
	info.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth    = m_prtPageWidth;
	info.bmiHeader.biHeight   = m_prtPageHeight;
	info.bmiHeader.biPlanes   =  1;
	info.bmiHeader.biBitCount = 24;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSizeImage   = info.bmiHeader.biWidth * info.bmiHeader.biHeight * 3;

	// Prepare memory DC (with classic WinAPI)
	m_prtDC        = ::CreateCompatibleDC(NULL);
	m_prtBitmap    = ::CreateDIBSection(m_prtDC,&info, DIB_RGB_COLORS,&buffer,NULL,0);
	m_prtOldBitmap = ::SelectObject(m_prtDC,m_prtBitmap);

	// Prepare pixel format and create OpenGL render context
#ifdef _DEBUG
	b3ListPixelFormats(m_glDC);
#endif

	PixelFormatIndex = ChoosePixelFormat(m_prtDC,&print_pixelformat);
	SetPixelFormat(m_prtDC,PixelFormatIndex,&print_pixelformat);
	m_prtGC = wglCreateContext(m_prtDC);
	CB3GetLinesApp()->b3SelectRenderContext(m_prtDC,m_prtGC);

	// Make initialization to this (dummy) Blizzad III context
	b3RenderContext::b3Init();
}

void CAppRenderView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWaitCursor wait;

	// Do it!
	CB3GetLinesApp()->b3SelectRenderContext(m_prtDC,m_prtGC);
	GetDocument()->m_Context.b3SetBGColor(1,1,1);

	// The pages are enumerated from [1..max]
	// The offset 0/0 maps to the lower left.
	b3Draw(
		m_prtLogWidth,
		m_prtLogHeight,
		m_prtLogOffsetX * ((pInfo->m_nCurPage - 1) % m_prtCountWidth),
		m_prtLogOffsetY * ((pInfo->GetMaxPage() - pInfo->m_nCurPage) / m_prtCountWidth));
	glFinish();

	// Done and copy into destination DC
	pDC->SetMapMode(MM_TEXT);
	pDC->StretchBlt(
		0,0,pDC->GetDeviceCaps(HORZRES),pDC->GetDeviceCaps(VERTRES),
		CDC::FromHandle(m_prtDC),
		0,0,m_prtPageWidth,m_prtPageHeight,SRCCOPY);

	// Make this valid
	ValidateRect(NULL);
}

void CAppRenderView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CScrollView::OnEndPrinting(pDC, pInfo);
	
	if (m_prtGC != 0)
	{
		CB3GetLinesApp()->b3SelectRenderContext(NULL,NULL);
		wglDeleteContext(m_prtGC);
		m_prtGC = 0;

		if (m_prtDC != 0)
		{
			if (m_prtOldBitmap != 0)
			{
				::SelectObject(m_prtDC,m_prtOldBitmap);
			}
			::DeleteDC(m_prtDC);
			m_prtDC = 0;

			if (m_prtBitmap != 0)
			{
				::DeleteObject(m_prtBitmap);
				m_prtBitmap = 0;
			}
		}
	}
}

/*************************************************************************
**                                                                      **
**                        CAppRenderView diagnostics                    **
**                                                                      **
*************************************************************************/

#ifdef _DEBUG
void CAppRenderView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppRenderView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppRenderDoc* CAppRenderView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppRenderDoc)));
	return (CAppRenderDoc*)m_pDocument;
}
#endif //_DEBUG

/*************************************************************************
**                                                                      **
**                        CAppRenderView message handlers               **
**                                                                      **
*************************************************************************/

void CAppRenderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_ADD:
		SendMessage(WM_COMMAND,MAKELONG(ID_VIEW_BIGGER,1));
		break;
	case VK_SUBTRACT:
		SendMessage(WM_COMMAND,MAKELONG(ID_VIEW_SMALLER,1));
		break;
	case VK_RIGHT:
		SendMessage(WM_COMMAND,MAKELONG(ID_VIEW_MOVE_RIGHT,1));
		break;
	case VK_LEFT:
		SendMessage(WM_COMMAND,MAKELONG(ID_VIEW_MOVE_LEFT,1));
		break;
	case VK_UP:
		SendMessage(WM_COMMAND,MAKELONG(ID_VIEW_MOVE_UP,1));
		break;
	case VK_DOWN:
		SendMessage(WM_COMMAND,MAKELONG(ID_VIEW_MOVE_DOWN,1));
		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAppRenderView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
	if ((m_Action[m_SelectMode] != null) && b3IsMouseActionAllowed())
	{
		m_Action[m_SelectMode]->b3DispatchMouseMove(point.x,point.y);
	}
}

void CAppRenderView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonDown(nFlags, point);
	if ((m_Action[m_SelectMode] != null) && b3IsMouseActionAllowed())
	{
		m_Action[m_SelectMode]->b3DispatchLButtonDown(point.x,point.y,nFlags);
	}
}

void CAppRenderView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonUp(nFlags, point);
	if ((m_Action[m_SelectMode] != null) && b3IsMouseActionAllowed())
	{
		m_Action[m_SelectMode]->b3DispatchLButtonUp(point.x,point.y);
	}
}

void CAppRenderView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnRButtonDown(nFlags, point);
	if ((m_Action[m_SelectMode] != null) && b3IsMouseActionAllowed())
	{
		m_Action[m_SelectMode]->b3DispatchRButtonDown(point.x,point.y,nFlags);
	}
}

void CAppRenderView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnRButtonUp(nFlags, point);
	if ((m_Action[m_SelectMode] != null) && b3IsMouseActionAllowed())
	{
		m_Action[m_SelectMode]->b3DispatchRButtonUp(point.x,point.y);
	}
}

void CAppRenderView::OnViewPerspective() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppRenderView::OnViewTop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_TOP);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppRenderView::OnViewFront() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_FRONT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppRenderView::OnViewRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_RIGHT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppRenderView::OnViewLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_LEFT);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppRenderView::OnViewBack() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3SetViewMode(B3_VIEW_BACK);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,0);
}

void CAppRenderView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppRenderView::OnUpdateViewTop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_TOP));
}

void CAppRenderView::OnUpdateViewFront(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_FRONT));
}

void CAppRenderView::OnUpdateViewRight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_RIGHT));
}

void CAppRenderView::OnUpdateViewLeft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_LEFT));
}

void CAppRenderView::OnUpdateViewBack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_RenderView.b3IsViewMode(B3_VIEW_BACK));
}

void CAppRenderView::OnViewSmaller() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(1.25);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnViewBigger() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Scale(0.8);
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnViewSelect() 
{
	// TODO: Add your command handler code here
	b3SetMagnification();
}

void CAppRenderView::OnViewPop() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3PopView();
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnViewOptimal() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Original();
	b3UnsetMagnification();
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnUpdateViewSmaller(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppRenderView::OnUpdateViewBigger(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppRenderView::OnUpdateViewSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
	pCmdUI->SetCheck(m_SelectMode == B3_SELECT_MAGNIFICATION);
}

void CAppRenderView::OnUpdateViewPop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_RenderView.b3ViewStackNotEmpty());
}

void CAppRenderView::OnUpdateViewOptimal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}

void CAppRenderView::OnViewMoveRight() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.2,0.0);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnViewMoveLeft() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(-0.2,0.0);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnViewMoveUp() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.0,0.2);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnViewMoveDown() 
{
	// TODO: Add your command handler code here
	m_RenderView.b3Move(0.0,-0.2);
	OnUpdate(this,B3_UPDATE_VIEW,NULL);
}

void CAppRenderView::OnUpdateViewMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}
