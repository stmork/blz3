/*
**
**	$Filename:	b3ScrollView.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - view preparation routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include <math.h>
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Tracker.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**
**	Revision 1.7  2002/08/08 15:14:22  sm
**	- Some problems concerning b3Mem::b3Realloc fixed.
**	- Further error messages added.
**	
**	Revision 1.6  2002/01/17 19:17:03  sm
**	- Fixed ILBM to other unfiltered scaling
**	
**	Revision 1.5  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.4  2001/09/23 15:37:15  sm
**	- Introducing raytracing for Lines III. There is much work
**	  for a b3Display-CScrollView.
**	
**	Revision 1.3  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**	Revision 1.2  2001/07/08 12:56:43  sm
**	- Now displaying Mandelbrot set. It's like switching all light on together.
**	
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3ScrollView                    **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CB3ScrollView, CScrollView)

BEGIN_MESSAGE_MAP(CB3ScrollView, CScrollView)
	//{{AFX_MSG_MAP(CB3ScrollView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3View construction/destruction

CB3ScrollView::CB3ScrollView()
{
	CWinApp *app = AfxGetApp();

	// TODO: add construction code here	  
	b3SetMagnifying(false);
	m_InitialUpdate  = false;
	m_Panning        = false;
	m_UseCursor      = false;
	m_ViewScan       = true;
	m_Mode           = B3_VIEWMODE_ORIGINAL;
	m_xScale         = 1.0;
	m_yScale         = 1.0;
	m_PaletteDisplay = false;

	// set initial window dimensions
	m_ScaleGrey      = false;

	try
	{
		m_View    = new CB3View;
		m_TxScale = new b3Tx;
	}
	catch (...)
	{
		throw;
	}
	m_Print      = null;
	m_Cursor     = null;
	m_CurMagnify = app->LoadStandardCursor(IDC_CROSS);

	b3PrintF (B3LOG_FULL,"### CLASS: CB3ScrollView instantiated\n");
}

CB3ScrollView::~CB3ScrollView()
{
	m_MutexScaling.b3Lock();
	delete m_TxScale;
	m_MutexScaling.b3Unlock();

	delete m_View;
	b3PrintF (B3LOG_FULL,"### CLASS: CB3ScrollView destroyed\n");
}

int CB3ScrollView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDC *dc;
	int  result;

	result = CScrollView::OnCreate(lpCreateStruct);
	if (result == 0)
	{
		dc = GetDC();
		if (dc != null)
		{
			m_PaletteDisplay = (dc->GetDeviceCaps(RASTERCAPS) & RC_PALETTE) != 0;
			ReleaseDC(dc);
		}
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3ScrollView diagnostics        **
**                                                                      **
*************************************************************************/

#ifdef _DEBUG

void CB3ScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CB3ScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDocument *CB3ScrollView::GetDocument() // non-debug version is inline
{
	return m_pDocument;
}

#endif

b3Document *CB3ScrollView::b3GetDocument() // non-debug version is inline
{
	CString message;

	message.Format(
		"Die Dokumenten-Klasse für CB3ScrollView\n"
		"konnte nicht ermittelt werden!\n\n"
		"Der Fehler tritt in Datei %s in Zeile %ld auf. Bitte\n"
		"kontaktieren Sie die Bizzard III Entwicklungsgruppe\n"
		"zur Beseitigung des Fehlers!",__FILE__,__LINE__);
	b3PrintF (B3LOG_NORMAL,"FATAL ERROR:\n%s",message);
	b3Runtime::b3MessageBox(message,B3_MSGBOX_ERROR);

	// To prevent compiler warning about unreachable
	// code. This construct confuses the compiler but
	// it recognizes the exit()-function!?!
	if (true)
	{
		exit (EXIT_FAILURE);
	}
	return null;
}

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3View view parameter handling  **
**                                                                      **
*************************************************************************/

// You can override this to get a notification
// when the view mode has changed
void CB3ScrollView::b3ViewParamChanged()
{
}

void CB3ScrollView::b3Activate(b3_bool was_deactive)
{
	if (m_PaletteDisplay)
	{
		Invalidate();
	}
}

void CB3ScrollView::b3GetViewParam(CB3ViewParam *param,CWnd *client)
{
	CFrameWnd      *parent;
	CRect           rect;
	CPoint          point;
	b3_window_mode  win_mode = VIEW_NORMAL;

	// Copy global of Blizzard III values
	point = GetScrollPosition();
	param->m_Mode      = m_Mode;
	param->m_Filtered  = m_ScaleGrey;
	param->m_xImgPos   = point.x;
	param->m_yImgPos   = point.y;
	param->m_xImgScale = m_xScale;
	param->m_yImgScale = m_yScale;

	// Do special on parent frame
	parent = GetParentFrame();
	if (parent != null)
	{
		// Get specific window values
		if (parent->IsIconic())
		{
			win_mode = VIEW_MINIMIZED;
		}
		if (parent->IsZoomed())
		{
			win_mode = VIEW_MAXIMIZED;
		}
		if (win_mode == VIEW_NORMAL)
		{
			parent->GetWindowRect(&rect);
			if (client != null)
			{
				client->ScreenToClient(&rect);
			}
			param->m_xWinPos  = rect.left;
			param->m_yWinPos  = rect.top;
			param->m_xWinSize = rect.Width();
			param->m_yWinSize = rect.Height();
		}
		param->m_WinMode = win_mode;
	}
}

void CB3ScrollView::b3SetViewParam(CB3ViewParam *param)
{
	CFrameWnd   *parent;
	CRect        rect;
	CPoint       point;
	CWaitCursor  wait_for_Michael_Hoelscher;

	// Copy global Blizzard III values
	m_ScaleGrey	= param->m_Filtered;
	m_Mode      = param->m_Mode;
	m_xScale    = param->m_xImgScale;
	m_yScale    = param->m_yImgScale;
	point.x     = param->m_xImgPos;
	point.y     = param->m_yImgPos;

	// Do special on parent frame
	parent = GetParentFrame();
	if (parent != null)
	{
		switch (param->m_WinMode)
		{
		case VIEW_MINIMIZED:
			parent->ShowWindow(SW_SHOWMINIMIZED);
			break;

		case VIEW_MAXIMIZED:
			parent->ShowWindow(SW_SHOWMAXIMIZED);
			break;

		default:
		case VIEW_NORMAL:
			// Set new dimensions and repaint later
			parent->MoveWindow(
				param->m_xWinPos, param->m_yWinPos,
				param->m_xWinSize,param->m_yWinSize);
			parent->ShowWindow(SW_SHOWNORMAL);
			break;
		}
	}

	// Magnification is already set. Now scroll...
	ScrollToPosition(point);
	OnUpdate(this,B3_UPDATE_TX,null);
}

/*************************************************************************
**                                                                      **
**                        Blizzard CB3ScrollView drawings               **
**                                                                      **
*************************************************************************/

void CB3ScrollView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
	m_View->b3DrawView(pDC);
}
					
// Return TRUE if up-scaling  
b3_bool CB3ScrollView::b3SetSize(b3Tx *m_Tx,CSize &sizeTotal)
{
	CSize  sizeDiff;
	CSize  sizeArrow;
	CRect  rect;
	b3_f64 factor;
	b3_f64 srcScale,dstScale;

	// Setup viewable area
	GetClientRect(&rect);
	sizeDiff.cx = rect.right  - rect.left;
	sizeDiff.cy = rect.bottom - rect.top;

	if (m_Tx == null)
	{
#ifdef _DEBUG
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # SetScrollSizes(): total: (%ld,%ld)\n",
			sizeTotal.cx,sizeTotal.cy);
#endif
		SetScrollSizes(MM_TEXT,sizeTotal);
		return false;
	}

	// Setup scale factors
	switch (m_Mode)
	{
	case B3_VIEWMODE_FIT_WIDTH :
		m_xScale =
		m_yScale = (double)sizeDiff.cx / (double)m_Tx->xSize;
		break;
	case B3_VIEWMODE_FIT_HEIGHT :
		m_xScale =
		m_yScale = (double)sizeDiff.cy / (double)m_Tx->ySize;
		break;
	case B3_VIEWMODE_FIT_BEST:
		srcScale = (double)m_Tx->ySize / (double)m_Tx->xSize;
		dstScale = (double)sizeDiff.cy / (double)sizeDiff.cx;
		if (srcScale > dstScale)
		{
			m_xScale =
			m_yScale = (double)sizeDiff.cy / (double)m_Tx->ySize;
		}
		else
		{
			m_xScale =
			m_yScale = (double)sizeDiff.cx / (double)m_Tx->xSize;
		}
		break;
	case B3_VIEWMODE_ORIGINAL :
		m_xScale = 1.0;
		m_yScale = 1.0;
		break;
	case B3_VIEWMODE_STRETCH_BEST :
		m_xScale = (double)sizeDiff.cx / (double)m_Tx->xSize;
		m_yScale = (double)sizeDiff.cy / (double)m_Tx->ySize;
		break;
	case B3_VIEWMODE_SCALE :
		// Scale is computed in OnScaleMore()/OnScaleLess()
		break;
	}

	// Check if magnification ist too high!
	if ((m_Tx->xSize * m_xScale) > 6399)
	{
		factor = m_xScale / (6399.0 / (double)m_Tx->xSize);

		m_xScale /= factor;
		m_yScale /= factor;
	}
	if ((m_Tx->ySize * m_yScale) > 6399)
	{
		factor = m_yScale / (6399.0 / (double)m_Tx->ySize);

		m_xScale /= factor;
		m_yScale /= factor;
	}

	// Setup total view size
	sizeTotal.cx = (long)(m_Tx->xSize * m_xScale);
	sizeTotal.cy = (long)(m_Tx->ySize * m_yScale);

	// Setup arrow skip amount
	sizeArrow.cx = sizeDiff.cx >> 3;
	sizeArrow.cy = sizeDiff.cy >> 3;
	SetScrollSizes(MM_TEXT, sizeTotal, sizeDiff, sizeArrow);
#ifdef _DEBUG
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # SetScrollSizes():"
			" total: (%ld,%ld) page: (%ld,%ld) line: (%ld,%ld)\n",
			sizeTotal.cx,sizeTotal.cy,
			sizeDiff.cx, sizeDiff.cy,
			sizeArrow.cx,sizeArrow.cy);
#endif

	return (m_xScale >= 1.0) && (m_yScale >= 1.0);
}

void CB3ScrollView::OnInitialUpdate()
{
	b3Document *pDoc = b3GetDocument();
	CSize       sizeTotal(0,0);

	m_InitialUpdate = true;
	CScrollView::OnInitialUpdate();
	b3SetSize(pDoc->m_Tx,sizeTotal);
	OnUpdate(this,B3_UPDATE_TX,null);
}

void CB3ScrollView::b3UpdateSize(LPARAM lHint)
{
	b3Document *pDoc = b3GetDocument();
	CRect       rect;

	// This is necessary because the initial image
	// shouldn't be too big!
	if ((pDoc->m_Tx != null) && (lHint & B3_UPDATE_SIZE))
	{
		b3Tx   *tx = pDoc->m_Tx;
		b3_res  width,height;

		if (tx->b3IsLoaded())
		{
			GetClientRect (&rect);
			width  = rect.Width();
			height = rect.Height();
			if ((width >= tx->xSize) || (height >= tx->ySize))
			{
				m_Mode = B3_VIEWMODE_ORIGINAL;
			}
		}
	}
}

void CB3ScrollView::b3UpdateTx(LPARAM lHint)
{
	b3Document *pDoc = b3GetDocument();
	b3_bool     DoRescale;
	CSize       sizeTotal(0,0);
	CRect       rect;

	if (lHint & B3_UPDATE_TX)
	{
		b3SetSize(pDoc->m_Tx,sizeTotal);

		// OK, scale and force showing the new page
		if (pDoc->m_Tx != null)
		{
			b3_res  depth;
			b3_bool ScaleBigger;

			ScaleBigger = b3SetSize(pDoc->m_Tx,sizeTotal);

			if ((lHint & B3_UPDATE_PREVENT_RESCALE) == 0)
			{
				pDoc->b3Lock("OnUpdateTx");

				// Compute color depth
				if (!pDoc->m_Tx->b3IsTrueColor())
				{
					// We have a palette...
					if (m_ScaleGrey)
					{
						if (ScaleBigger)
						{
							depth = pDoc->m_Tx->depth;
						}
						else
						{
							depth = 8;
						}
					}
					else
					{
						depth = pDoc->m_Tx->depth;
					}
				}
				else
				{
					depth = 24;
				}

				// Now get scaled copy of image
				if (lHint & B3_UPDATE_CHECK_SCALING)
				{
					m_MutexScaling.b3Lock();
					DoRescale =
						((m_TxScale->xSize != (b3_res)sizeTotal.cx) ||
						 (m_TxScale->ySize != (b3_res)sizeTotal.cy) ||
						 (m_TxScale->depth != depth) ||
						 (m_Filtered       != m_ScaleGrey));
					m_MutexScaling.b3Unlock();
				}
				else
				{
					DoRescale = true;
				}

				if (DoRescale)
				{
					b3PrintF (B3LOG_FULL,
						"### CLASS: b3View # b3Lock()   - OnUpdateTx [scaling]\n");
					m_MutexScaling.b3Lock();									
					try
					{
						if (m_TxScale->b3AllocTx(sizeTotal.cx,sizeTotal.cy,depth))
						{
							if ((!m_ScaleGrey) || (ScaleBigger))
							{
								if (m_Mode == B3_VIEWMODE_ORIGINAL)
								{
									m_TxScale->b3Copy(pDoc->m_Tx);
								}
								else
								{
									m_TxScale->b3Scale(pDoc->m_Tx);
								}
							}
							else
							{
								m_TxScale->b3ScaleToGrey(pDoc->m_Tx);
							}
							m_Filtered = m_ScaleGrey;
							b3ViewParamChanged();
						}
					}
					catch (b3TxException &t)
					{
						b3PrintF (B3LOG_NORMAL,
							"### CLASS: b3View # Texture exception occured: %s!\n",
							t.b3GetErrorMsg());
						m_MutexScaling.b3Unlock();
						throw;
					}
					catch(...)
					{
						b3PrintF (B3LOG_NORMAL,
							"### CLASS: b3View # Unknown exception occured!\n");
						m_MutexScaling.b3Unlock();
						throw;
					}
					m_MutexScaling.b3Unlock();
					b3PrintF (B3LOG_FULL,
						"### CLASS: b3View # b3Unlock() - OnUpdateTx [scaling]\n");
				}
				else
				{
#ifdef _DEBUG
					b3PrintF (B3LOG_FULL,"### CLASS: b3View # Doesn't need rescale...\n");
#endif
				}
				pDoc->b3Unlock("OnUpdateTx");
			}

			// Update view
			b3PrintF (B3LOG_FULL,"### CLASS: b3View # b3Lock()   - OnUpdateTx [scaling]\n");
			m_MutexScaling.b3Lock();
			m_View->b3UpdateView (m_TxScale,this);
			m_MutexScaling.b3Unlock();
			b3PrintF (B3LOG_FULL,"### CLASS: b3View # b3Unlock() - OnUpdateTx [scaling]\n");
		}
	}
}

void CB3ScrollView::b3Update(LPARAM lHint)
{
	if (lHint & (B3_UPDATE_TX | B3_UPDATE_TRACKER2))
	{
		InvalidateRect (null);
	}
}

void CB3ScrollView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CDocument *pDoc = GetDocument();

	b3SetMagnifying(false);
	if ((pDoc == null) || (m_ViewScan == null) || (!IsKindOf(RUNTIME_CLASS(CB3ScrollView))))
	{
		return;
	}

	if (m_InitialUpdate)
	{
#ifdef _DEBUG
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # OnUpdate(%02lx)%s%s%s%s%s%s%s%s\n",
			lHint,
			lHint & B3_UPDATE_TX              ? " [B3_UPDATE_TX]" : "",
			lHint & B3_UPDATE_TRACKER1        ? " [B3_UPDATE_TRACKER1]" : "",
			lHint & B3_UPDATE_TRACKER2        ? " [B3_UPDATE_TRACKER2]" : "",
			lHint & B3_UPDATE_CHECK_SCALING   ? " [B3_UPDATE_CHECK_SCALING]" : "",
			lHint & B3_UPDATE_PREVENT_RESCALE ? " [B3_UPDATE_PREVENT_RESCALE]" : "",
			lHint & B3_UPDATE_TRACKER_LIST    ? " [B3_UPDATE_TRACKER_LIST]" : "",
			lHint & B3_UPDATE_SIZE            ? " [B3_UPDATE_SIZE]" : "",
			lHint & B3_UPDATE_TOOLTIP         ? " [B3_UPDATE_TOOLTIP]" : "");
#endif

		b3UpdateSize(lHint);
		b3UpdateTx(lHint);
		b3Update(lHint);
	}
}

BOOL CB3ScrollView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush background(GetSysColor(COLOR_WINDOW));

	FillOutsideRect(pDC,&background);
	return TRUE;
}

/*************************************************************************
**                                                                      **
**                        Blizzard III printing routines                **
**                                                                      **
*************************************************************************/

BOOL CB3ScrollView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	CWinApp    *app = AfxGetApp();
	PRINTDLG   *prtDlg;
	DEVMODE    *DevMode;
	b3Document *pDoc = b3GetDocument();
	b3Tx       *tx;

	// Set printer orientation by images dimensions
	prtDlg = &pInfo->m_pPD->m_pd;
	if (app->GetPrinterDeviceDefaults(prtDlg))
	{
		DevMode = (DEVMODE *)::GlobalLock(prtDlg->hDevMode);
		if (DevMode != null)
		{
			tx                     = pDoc->m_Tx;
			DevMode->dmOrientation =
				(short)(tx->ySize > tx->xSize ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE);
			::GlobalUnlock(prtDlg->hDevMode);
		}
	}

	return DoPreparePrinting(pInfo);
}

void CB3ScrollView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CScrollView::OnPrepareDC(pDC, pInfo);
}


void CB3ScrollView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	b3Document *pDoc = b3GetDocument();
	b3Tx       *tx;
	b3_count    strips = 16,strip_height;

	CView::OnBeginPrinting(pDC, pInfo);

	// OK, let's do our job!
	tx           = pDoc->m_Tx;
	strip_height = (tx->ySize + strips - 1) / strips;
	m_Print = new CB3Print (
		tx,				// Source image to print
		strips,			// Number of strips
		strip_height,	// Strip height of original image
		pDC,pInfo,true);
}

void CB3ScrollView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_Print->b3PrintPage(pDC,pInfo);
}

void CB3ScrollView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: add cleanup after printing
	if (m_Print)
	{
		delete m_Print;
		m_Print = null;
	}
	CView::OnEndPrinting(pDC, pInfo);
}

void CB3ScrollView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_Print)
	{
		delete m_Print;
		m_Print = null;
	}
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3View diagnostics              **
**                                                                      **
*************************************************************************/

void CB3ScrollView::b3ShowView() 
{
	// TODO: Add your command handler code here
	m_ViewScan = !m_ViewScan;
	b3ViewParamChanged();
}

void CB3ScrollView::b3ScaleBW() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	b3_bool     oldScaling = m_ScaleGrey;

	m_ScaleGrey = false;
	if (oldScaling ^ m_ScaleGrey)
	{
		OnUpdate(this,B3_UPDATE_TX,null);
		b3ViewParamChanged();
	}
}

void CB3ScrollView::b3ScaleGrey() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	b3_bool     oldScaling = m_ScaleGrey;

	m_ScaleGrey = true;
	if (oldScaling ^ m_ScaleGrey)
	{
		OnUpdate(this,B3_UPDATE_TX,null);
		b3ViewParamChanged();
	}
}

void CB3ScrollView::b3FilterMode(b3_bool filter_enable)
{
	m_ScaleGrey = filter_enable;
}

void CB3ScrollView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
#ifdef _DEBUG
	b3PrintF (B3LOG_FULL,"### CLASS: b3View # OnSize(%d,%d)\n",cx,cy);
#endif
	OnUpdate(this,B3_UPDATE_TX|B3_UPDATE_CHECK_SCALING,null);
}

void CB3ScrollView::OnRefresh(CView *pSender,LPARAM lHint,CObject *pHint)
{
#ifdef _DEBUG
	b3PrintF (B3LOG_FULL,"### CLASS: b3View # OnRefresh(%02lx)\n",lHint);
#endif
	OnUpdate(pSender,lHint,pHint);
}

void CB3ScrollView::b3SetMagnifying(b3_bool DoMagnify)
{
	m_DoMagnify = DoMagnify;
}

b3_bool CB3ScrollView::b3IsMagnifying()
{
	return m_DoMagnify;
}

void CB3ScrollView::b3ViewMode(b3_display_mode mode)
{
	// TODO: Add your command handler code here
	if (m_Mode != mode)
	{
		CWaitCursor wait_for_Werner_Schaefer;

		m_Mode = mode;
		OnUpdate(this,B3_UPDATE_TX,null);
		b3ViewParamChanged();
	}
}

b3_bool CB3ScrollView::b3CorrectScaleCenter(CPoint &position,b3_f64 factor)
{
	CRect       rect;
	b3Document *pDoc = b3GetDocument();
	b3Tx       *tx;
	b3_f64      xImgSize,xWndSize,xPart;
	b3_f64      yImgSize,yWndSize,yPart;
	b3_f64      scale;

	// Get actual values
	position = GetScrollPosition();
	GetClientRect(&rect);

	// Compute image values to be compared against
	// window values (all in double -> more precision)
	tx       = pDoc->m_Tx;
	if (tx == null)
	{
		return false;
	}

	xImgSize = tx->xSize * m_xScale;
	yImgSize = tx->ySize * m_yScale;
	xWndSize = rect.Width();
	yWndSize = rect.Height();

	// Scale factor which scroll position must move
	// (coords in actual system)
	scale    = (factor - 1.0) * 0.5;

	// Compute x
	if (xImgSize > xWndSize)
	{
		xPart = position.x * factor + scale * xWndSize;
	}
	else
	{
		xPart = 0;
	}

	// Compute y
	if (yImgSize > yWndSize)
	{
		yPart = position.y * factor + scale * yWndSize;
	}
	else
	{
		yPart = 0;
	}
	position.x = (long)floor(xPart + 0.5);
	position.y = (long)floor(yPart + 0.5);

	return true;
}

void CB3ScrollView::b3MagnifyMore() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	CPoint      position;
	b3_f64      factor = 1.25;

	if (b3CorrectScaleCenter(position,factor))
	{
		m_Mode    = B3_VIEWMODE_SCALE;
		m_xScale *= factor;
		m_yScale *= factor;
		OnUpdate(this,B3_UPDATE_TX,null);
		ScrollToPosition(position);
		b3ViewParamChanged();
	}
}

void CB3ScrollView::b3MagnifyLess() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	CPoint      position;
	b3_f64      factor = 0.8;

	b3CorrectScaleCenter(position,factor);
	m_Mode    = B3_VIEWMODE_SCALE;
	m_xScale *= factor;
	m_yScale *= factor;
	OnUpdate(this,B3_UPDATE_TX,null);
	ScrollToPosition(position);
	b3ViewParamChanged();
}

void CB3ScrollView::b3Scale(double xScale,double yScale) 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	CPoint      position;
	double      factor = (xScale + yScale) * 0.5;

	b3CorrectScaleCenter(position,factor);
	m_Mode   = B3_VIEWMODE_SCALE;
	m_xScale = xScale;
	m_yScale = yScale;
	OnUpdate(this,B3_UPDATE_TX,null);
	ScrollToPosition(position);
	b3ViewParamChanged();
}

void CB3ScrollView::b3ViewRubber() 
{
	// TODO: Add your command handler code here
	b3SetMagnifying();
}

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3View mouse actions            **
**                                                                      **
*************************************************************************/

// Start panning when pressing the left mouse button.
void CB3ScrollView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	b3Document *pDoc = b3GetDocument();
	CPoint      position;

	CScrollView::OnLButtonDown(nFlags, point);

	// Get cursor position
	m_Start    = point;

	// Get scroll reference point
	m_Position = GetScrollPosition();
	
	// Set magnification
	if ((b3IsMagnifying()) && (pDoc->m_Tx != null))
	{
		CB3Tracker selector(m_TxScale);
		CRect      client;
		CRect      rect;
		CPoint     actPos;
		b3_f64     factor;
		b3_f64     oldAspect;
		b3_f64     newAspect;
		b3_res     xSize;
		b3_res     ySize;

		b3PrintF (B3LOG_FULL,"### CLASS: b3View # Magnifying start.\n");
		::SetCursor(m_CurMagnify);
		selector.b3Create();
		selector.TrackRubberBand(this,point,FALSE);
		selector.GetTrueRect(&rect);
		if ((rect.Width() > 0) && (rect.Height() > 0))
		{
			CWaitCursor wait;

			GetClientRect(&client);

			// Compute aspect ratios of old client window an new selection
			oldAspect = (double)client.Height() / (double)client.Width();
			newAspect = (double)rect.Height()   / (double)rect.Width();

			// Select right scaling
			if (oldAspect < newAspect)
			{
				factor = (double)client.Height() / (double)rect.Height();
			}
			else
			{
				factor = (double)client.Width()  / (double)rect.Width();
			}

			// Convert destination scroll position to original value
			actPos.x = (long)((m_Position.x + rect.left) / m_xScale);
			actPos.y = (long)((m_Position.y + rect.top)  / m_yScale);

			xSize = pDoc->m_Tx->xSize;
			ySize = pDoc->m_Tx->ySize;

			// Set new scaling and its mode
			m_xScale *= factor;
			m_yScale *= factor;

			m_Mode    = B3_VIEWMODE_SCALE;
			OnUpdate(this,B3_UPDATE_TX,null);

			// Compute new scroll position using new view factor
			actPos.x = (long)(actPos.x * m_xScale);
			actPos.y = (long)(actPos.y * m_yScale);
			ScrollToPosition(actPos);
			b3ViewParamChanged();
		}
		b3SetMagnifying(false);
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # Magnifying stop.\n");
		return;
	}

	// enable panning
	m_Panning   = true;
	m_UseCursor = true;

	// Set cursor
	b3PrintF (B3LOG_FULL,"### CLASS: b3View # Panning start.\n");
	b3PrintF (B3LOG_FULL,"### CLASS: b3View # SetCursor(%8lx); [OnLButtonDown]\n",m_Cursor);
	::SetCursor(m_Cursor);
}

// What's going up when moving the mouse while dolding down
// the left mouse button
void CB3ScrollView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_Panning)
	{
		if (nFlags & MK_LBUTTON)
		{
			b3_res  xDiff,yDiff;
			RECT    rect;
			CPoint  diff,act;

			// Compute mouse movement
			diff.x  = m_Start.x    - point.x;
			diff.y  = m_Start.y    - point.y;

			// Add this movement to scroll reference point and set!
			act.x   = m_Position.x + diff.x;
			act.y   = m_Position.y + diff.y;
				
			// Clip against bounds...
			GetClientRect(&rect);
			xDiff = rect.right  - rect.left;
			yDiff = rect.bottom - rect.top;

			m_MutexScaling.b3Lock();
			if (xDiff >= m_TxScale->xSize)
			{
				act.x = 0;
			}
			if (yDiff >= m_TxScale->ySize)
			{
				act.y = 0;
			}
			m_MutexScaling.b3Unlock();

			// Scroll it!
			if (act != m_OldAct)
			{
				ScrollToPosition(act);
				m_OldAct = act;
			}
			m_UseCursor = true;
		}
		else
		{
			m_Panning = false;
			b3PrintF (B3LOG_FULL,"### CLASS: b3View # Panning stop.\n");
		}
	}

	if (b3IsMagnifying())
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # SetCursor(m_CurMagnify = %8lx); [OnMouseMove]\n",m_CurMagnify);
		::SetCursor(m_CurMagnify);
	}

	if (m_UseCursor)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # SetCursor(m_Cursor = %8lx); [OnMouseMove]\n",m_Cursor);
		::SetCursor(m_Cursor);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

// Stop panning when leaving the left mouse button
void CB3ScrollView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// disable panning
	m_Panning   = false;
	m_UseCursor = false;

	b3PrintF (B3LOG_FULL,"### CLASS: b3View # Panning stop.\n");

	// TODO: Add your message handler code here and/or call default
	// Disable panning.
	CScrollView::OnLButtonUp(nFlags, point);
}
