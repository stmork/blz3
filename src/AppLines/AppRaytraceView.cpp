/*
**
**	$Filename:	AppRaytraceView.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for raytracing visualization
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "stdafx.h"
#include "AppLines.h"
#include "MainFrm.h"

#include "AppRaytraceDoc.h"
#include "AppRaytraceView.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2002/08/14 16:48:49  sm
**	- The last view mode/filter mode for image views are stored in
**	  registry
**	- b3ExtractExt searches from right instead from left.
**	- Made some cleanup inside CB3ScrollView
**
**	Revision 1.7  2002/01/31 11:50:53  sm
**	- Now we can print OpenGL scenes (Note: We have to do basic
**	  initialization prior to render a scene. Then we can see the scene
**	  on paper)
**	
**	Revision 1.6  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.5  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.4  2001/12/28 15:17:44  sm
**	- Added clipboard-copy to raytraced view
**	- Added printing to raytraced view
**	- Much minor UI tuning done:
**	  o added accelerators
**	  o open maximized window
**	  o fixed some UpdateUI methods
**	  o changed exception handling in CB3ScrollView and CB3BitmapDxB
**	
**	Revision 1.3  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.2  2001/09/30 15:46:06  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
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

IMPLEMENT_DYNCREATE(CAppRaytraceView, CB3ScrollView)

BEGIN_MESSAGE_MAP(CAppRaytraceView, CB3ScrollView)
	//{{AFX_MSG_MAP(CAppRaytraceView)
	ON_COMMAND(ID_B3_UNFILTERED, OnUnfiltered)
	ON_COMMAND(ID_B3_FILTERED, OnFiltered)
	ON_COMMAND(ID_B3_ORIGINAL, OnOriginal)
	ON_COMMAND(ID_B3_WIDTH, OnWidth)
	ON_COMMAND(ID_B3_HEIGHT, OnHeight)
	ON_COMMAND(ID_B3_FULL, OnFull)
	ON_COMMAND(ID_B3_MORE, OnMore)
	ON_COMMAND(ID_B3_LESS, OnLess)
	ON_COMMAND(ID_B3_MAGNIFY, OnMagnify)
	ON_UPDATE_COMMAND_UI(ID_B3_UNFILTERED, OnUpdateUnfiltered)
	ON_UPDATE_COMMAND_UI(ID_B3_FILTERED, OnUpdateFiltered)
	ON_UPDATE_COMMAND_UI(ID_B3_ORIGINAL, OnUpdateOriginal)
	ON_UPDATE_COMMAND_UI(ID_B3_WIDTH, OnUpdateWidth)
	ON_UPDATE_COMMAND_UI(ID_B3_HEIGHT, OnUpdateHeight)
	ON_UPDATE_COMMAND_UI(ID_B3_FULL, OnUpdateFull)
	ON_UPDATE_COMMAND_UI(ID_B3_MORE, OnUpdateMore)
	ON_UPDATE_COMMAND_UI(ID_B3_LESS, OnUpdateLess)
	ON_UPDATE_COMMAND_UI(ID_B3_MAGNIFY, OnUpdateMagnify)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrintable)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrintable)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrintable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceView construction/destruction

CAppRaytraceView::CAppRaytraceView()
{
	// TODO: add construction code here
	CB3App *app = CB3GetApp();

	m_Filtered =                  app->GetProfileInt(app->b3ClientName(),"view.filter",m_Filtered) != 0;
	m_Mode     = (b3_display_mode)app->GetProfileInt(app->b3ClientName(),"view.mode",  m_Mode);
	m_Cursor = app->LoadCursor(IDC_PANNING);
}

CAppRaytraceView::~CAppRaytraceView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceView drawing

void CAppRaytraceView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CB3App     *app  = CB3GetApp();
	CMainFrame *main = CB3GetMainFrame();

	CB3ScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"CAppRaytraceView::OnActivateView(%s,%p,%p)\n",
		bActivate ? "TRUE " : "FALSE",
		pActivateView, pDeactiveView);
#endif
	
	if (bActivate)
	{
		main->b3Clear();
		GetDocument()->b3ActivateDoc();
		app->b3SetData();
	}
	else
	{
//		main->b3Clear();
//		main->b3UpdateModellerInfo();
		app->b3GetData();
	}
}

void CAppRaytraceView::OnDraw(CDC* pDC)
{
	CAppRaytraceDoc* pDoc = GetDocument();
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
	CAppRaytraceDoc *pDoc;

	if (m_pDocument->IsKindOf(RUNTIME_CLASS(CAppRaytraceDoc)))
	{
		pDoc = (CAppRaytraceDoc *)m_pDocument;
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

CAppRaytraceDoc* CAppRaytraceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppRaytraceDoc)));
	return (CAppRaytraceDoc*)m_pDocument;
}
#endif //_DEBUG

void CAppRaytraceView::b3BestFit()
{
	OnFull();
}

BOOL CAppRaytraceView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	pInfo->m_pPD->m_pd.Flags |= (PD_NOPAGENUMS | PD_NOSELECTION);
	return CB3ScrollView::OnPreparePrinting(pInfo);
}

void CAppRaytraceView::OnUpdatePrintable(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->b3IsRaytracing());
}

void CAppRaytraceView::b3ViewParamChanged()
{
	CB3App *app = CB3GetApp();

	app->WriteProfileInt(app->b3ClientName(),"view.filter",m_Filtered);
	app->WriteProfileInt(app->b3ClientName(),"view.mode",  m_Mode);
}

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceView message handlers

void CAppRaytraceView::OnUnfiltered() 
{
	// TODO: Add your command handler code here
	b3SetFilterMode(false);
}

void CAppRaytraceView::OnFiltered() 
{
	// TODO: Add your command handler code here
	b3SetFilterMode(true);
}

void CAppRaytraceView::OnOriginal() 
{
	// TODO: Add your command handler code here
	b3SetViewMode(B3_VIEWMODE_ORIGINAL);
}

void CAppRaytraceView::OnWidth() 
{
	// TODO: Add your command handler code here
	b3SetViewMode(B3_VIEWMODE_FIT_WIDTH);
}

void CAppRaytraceView::OnHeight() 
{
	// TODO: Add your command handler code here
	b3SetViewMode(B3_VIEWMODE_FIT_HEIGHT);
}

void CAppRaytraceView::OnFull() 
{
	// TODO: Add your command handler code here
	b3SetViewMode(B3_VIEWMODE_BESTFIT);
}

void CAppRaytraceView::OnMore() 
{
	// TODO: Add your command handler code here
	b3MagnifyMore();
}

void CAppRaytraceView::OnLess() 
{
	// TODO: Add your command handler code here
	b3MagnifyLess();
}

void CAppRaytraceView::OnMagnify() 
{
	// TODO: Add your command handler code here
	b3SetMagnifying();
}

void CAppRaytraceView::OnUpdateUnfiltered(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (!m_Filtered);
}

void CAppRaytraceView::OnUpdateFiltered(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Filtered);
}

void CAppRaytraceView::OnUpdateOriginal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_ORIGINAL);
}

void CAppRaytraceView::OnUpdateWidth(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_FIT_WIDTH);
}

void CAppRaytraceView::OnUpdateHeight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_FIT_HEIGHT);
}

void CAppRaytraceView::OnUpdateFull(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (m_Mode == B3_VIEWMODE_BESTFIT);
}

void CAppRaytraceView::OnUpdateMore(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Document *docBase = b3GetDocument();

	pCmdUI->Enable(docBase->m_Tx != null);
}

void CAppRaytraceView::OnUpdateLess(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Document *docBase = b3GetDocument();

	pCmdUI->Enable(docBase->m_Tx != null);
}

void CAppRaytraceView::OnUpdateMagnify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Document *docBase = b3GetDocument();

	pCmdUI->Enable(docBase->m_Tx != null);
	if (docBase->m_Tx != null)
	{
		pCmdUI->SetCheck(b3IsMagnifying());
	}
}

void CAppRaytraceView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CB3BitmapDIB  dib;
	CClientDC     dc(NULL);
	b3Document   *docBase = b3GetDocument();
	b3Tx         *tx;

	if (OpenClipboard())
	{
		tx = docBase->m_Tx;
		if  (dib.b3SetTx(tx))
		{
			HBITMAP bitmap;

			bitmap = dib.b3CreateBitmap(&dc);
			if (bitmap != null)
			{
				::EmptyClipboard();
				::SetClipboardData(CF_BITMAP,bitmap);
			}
		}
		::CloseClipboard();
	}
}

void CAppRaytraceView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!GetDocument()->b3IsRaytracing());
}
