/*
**
**	$Filename:	AppRaytraceDoc.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"
#include "b3SelectTexture.h"

/*************************************************************************
**                                                                      **
**                        CAppRaytraceDoc implementation                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppRaytraceDoc, CB3LinesDocument)

BEGIN_MESSAGE_MAP(CAppRaytraceDoc, CB3LinesDocument)
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

const char *CAppRaytraceDoc::b3GetDocumentName()
{
	return "_Lines_III_Image_Document";
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
	BOOL result = false;

	try
	{
		result = m_Tx->b3LoadImage(lpszPathName,true) == B3_TX_OK;
	}
	catch(b3ExceptionBase &e)
	{
		b3PrintF(B3LOG_NORMAL,"ERROR: Opening image %s (code: %d)\n",
			result ? "True" : "False",e.b3GetError());
		B3_MSG_ERROR(e);
	}
	return result;
}

void CAppRaytraceDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_RenderDoc != null)
	{
		m_RenderDoc->b3ClearRaytraceDoc();
	}
	CB3GetMainFrame()->b3UpdateModellerInfo();
	CB3LinesDocument::OnCloseDocument();
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
	b3DisplayView    *display;
	POSITION          pos;

	pos     = GetFirstViewPosition();
	pView   = (CAppRaytraceView *)GetNextView(pos);
	display = new b3DisplayView(pView,xSize,ySize,title);
	display->b3SetRowRefreshCount(CB3GetLinesApp()->m_RowRefreshCount);
	return display;
}

b3Display *CAppRaytraceDoc::b3GetDisplay(const char *title)
{
	CAppRaytraceView *pView;
	b3DisplayView    *display;
	POSITION          pos;

	pos     = GetFirstViewPosition();
	pView   = (CAppRaytraceView *)GetNextView(pos);
	display = new b3DisplayView(pView,title);
	display->b3SetRowRefreshCount(CB3GetLinesApp()->m_RowRefreshCount);
	return display;
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

void CAppRaytraceDoc::b3UpdateTitle(const char *title)
{
	b3Path fullname;
	b3Path path,name;

	CB3SelectSaveTexture::b3GetLastFilename((char *)fullname);
	if (strlen(title) > 0)
	{
		fullname.b3SplitFileName(path,null);
		name.b3LinkFileName(path,title);
		m_Tx->b3Name(name);
	}
	else
	{
		m_Tx->b3Name(fullname);
	}
}

void CAppRaytraceDoc::OnSaveImage() 
{
	// TODO: Add your command handler code here
	b3Path   result = "";
	CWinApp *app    = AfxGetApp();

	if (CB3SelectSaveTexture::b3Select(result,m_Tx->b3Name()))
	{
		m_Tx->b3SaveImage(result);
	}
}

void CAppRaytraceDoc::OnUpdateSaveImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!b3IsRaytracing());
}
