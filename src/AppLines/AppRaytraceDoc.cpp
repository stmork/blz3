/*
**
**	$Filename:	AppRaytraceDoc.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for raytracing visualization
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
**	Revision 1.11  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**
**	Revision 1.10  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
**	Revision 1.9  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.8  2001/12/28 15:17:44  sm
**	- Added clipboard-copy to raytraced view
**	- Added printing to raytraced view
**	- Much minor UI tuning done:
**	  o added accelerators
**	  o open maximized window
**	  o fixed some UpdateUI methods
**	  o changed exception handling in CB3ScrollView and CB3BitmapDxB
**	
**	Revision 1.7  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.6  2001/11/04 10:54:14  sm
**	- Redesigned b3Display for control use.
**	
**	Revision 1.5  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.4  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.3  2001/10/24 14:59:08  sm
**	- Some GIG bug fixes
**	- An image viewing bug fixed in bimg3
**	
**	Revision 1.2  2001/10/03 20:17:55  sm
**	- Minor bugfixes
**	
**	Revision 1.1  2001/09/30 15:46:06  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppRaytraceDoc implementation                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppRaytraceDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppRaytraceDoc, CDocument)
	//{{AFX_MSG_MAP(CAppRaytraceDoc)
	ON_COMMAND(ID_RAYTRACE, OnRaytrace)
	ON_UPDATE_COMMAND_UI(ID_RAYTRACE, OnUpdateRaytrace)
	ON_COMMAND(ID_FILE_SAVE, OnSaveImage)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateSaveImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc construction/destruction

CAppRaytraceDoc::CAppRaytraceDoc()
{
	// TODO: add one-time construction code here
	m_Scene     = null;
	m_RenderDoc = null;
}

CAppRaytraceDoc::~CAppRaytraceDoc()
{
}

BOOL CAppRaytraceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CAppRaytraceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized creation code here
	return m_Tx->b3LoadImage(lpszPathName) == B3_TX_OK;
}

void CAppRaytraceDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_RenderDoc != null)
	{
		m_RenderDoc->b3ClearRaytraceDoc();
	}
	CB3GetMainFrame()->b3UpdateModellerInfo();
	CDocument::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc serialization

void CAppRaytraceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc diagnostics

#ifdef _DEBUG
void CAppRaytraceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppRaytraceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc commands

void CAppRaytraceDoc::b3SetRenderDoc(CAppRenderDoc *RenderDoc)
{
	m_RenderDoc = RenderDoc;
}

b3_bool CAppRaytraceDoc::b3IsRaytracing()
{
	return (m_RenderDoc != null ? m_RenderDoc->b3IsRaytracing() : false);
}

b3Display *CAppRaytraceDoc::b3GetDisplay(b3_res xSize,b3_res ySize,const char *title)
{
	CAppRaytraceView *pView;
	POSITION          pos;

	pos   = GetFirstViewPosition();
	pView = (CAppRaytraceView *)GetNextView(pos);
	return new b3DisplayView(pView,xSize,ySize,title);
}

b3Display *CAppRaytraceDoc::b3GetDisplay(const char *title)
{
	CAppRaytraceView *pView;
	POSITION          pos;

	pos   = GetFirstViewPosition();
	pView = (CAppRaytraceView *)GetNextView(pos);
	return new b3DisplayView(pView,title);
}

void CAppRaytraceDoc::b3ActivateDoc()
{
	CMainFrame *main = CB3GetMainFrame();

	main->b3UpdateModellerInfo();
	main->m_dlgHierarchy.b3InitTree(m_RenderDoc);
}

void CAppRaytraceDoc::b3ActivateView()
{
	CAppRaytraceView *pView;
	POSITION          pos;

	pos   = GetFirstViewPosition();
	pView = (CAppRaytraceView *)GetNextView(pos);
	pView->GetParentFrame()->BringWindowToTop();
}

void CAppRaytraceDoc::OnRaytrace() 
{
	// TODO: Add your command handler code here
	if (m_RenderDoc != null)
	{
		m_RenderDoc->b3ToggleRaytrace();
	}
}

void CAppRaytraceDoc::OnUpdateRaytrace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(b3IsRaytracing());
}

void CAppRaytraceDoc::OnSaveImage() 
{
	// TODO: Add your command handler code here
	CString  suggest;
	CString  ext;
	CString  filter;
	b3Path   result;
	CWinApp *app = AfxGetApp();

	suggest = app->GetProfileString(CB3ClientString(),"Saved image filename","");
	filter.LoadString(IDS_SAVE_IMAGE_FILTER);
	if (b3SaveDialog(suggest,ext,filter,result))
	{
		app->WriteProfileString(CB3ClientString(),"Saved image filename",result);
		m_Tx->b3SaveImage(result);
	}
}

void CAppRaytraceDoc::OnUpdateSaveImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!b3IsRaytracing());
}
