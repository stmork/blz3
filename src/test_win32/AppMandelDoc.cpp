// AppMandelDoc.cpp : implementation of the CAppMandelDoc class
//

#include "stdafx.h"
#include "AppMandel.h"
#include "MainFrm.h"

#include "AppMandelDoc.h"
#include "AppMandelView.h"

#include "blz3/b3Main.h"
#include "blz3/system/b3ViewParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc

IMPLEMENT_DYNCREATE(CAppMandelDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppMandelDoc, CDocument)
	//{{AFX_MSG_MAP(CAppMandelDoc)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_COMPUTE, OnCompute)
	ON_UPDATE_COMMAND_UI(ID_COMPUTE, OnUpdateMandel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc construction/destruction

CAppMandelDoc::CAppMandelDoc()
{
	// TODO: add one-time construction code here
	m_MandelThread = new b3Thread("Mandelbrot computing...");
	OnReset();
}

CAppMandelDoc::~CAppMandelDoc()
{
	delete m_MandelThread;
}

BOOL CAppMandelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc serialization

void CAppMandelDoc::Serialize(CArchive& ar)
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
// CAppMandelDoc diagnostics

#ifdef _DEBUG
void CAppMandelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppMandelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc commands

b3_u32 CAppMandelDoc::b3ComputingThread(void *ptr)
{
	CAppMandelDoc *pDoc = (CAppMandelDoc *)ptr;
	CWinApp       *app  = AfxGetApp();
	b3_u32         result;

	result = _Blizzard3Main(pDoc->m_size,pDoc->m_args);
	app->m_pMainWnd->PostMessage(WM_B3_UPDATE_UI);
	return result;
}

void CAppMandelDoc::OnCompute() 
{
	// TODO: Add your command handler code here
	if (!m_MandelThread->b3IsRunning())
	{
		CWinApp        *app  = AfxGetApp();
		CFrameWnd      *main = (CFrameWnd *)app->m_pMainWnd;
		CAppMandelView *view = (CAppMandelView *)main->GetActiveView();
		CRect           rect;
		CB3ViewParam    vParam("mandel params");	
		b3_f64          rNew,rDiff,rWidth, rStart,rEnd;
		b3_f64          iNew,iDiff,iHeight,iStart,iEnd;

		view->b3GetViewParam(&vParam);
		m_args[0] = (char *)AfxGetAppName();

		rDiff = m_rEnd - m_rStart;
		iDiff = m_iEnd - m_iStart;
		rNew  = rDiff  / vParam.m_xImgScale;
		iNew  = iDiff  / vParam.m_yImgScale;

		if ((rNew != 0.0) && (iNew != 0.0))
		{
			view->GetClientRect(&rect);

			rWidth    = (b3_f64)m_Tx->xSize * vParam.m_xImgScale;
			iHeight   = (b3_f64)m_Tx->ySize * vParam.m_yImgScale;
			rStart    = (b3_f64)(vParam.m_xImgPos + rect.left)   / rWidth;
			rEnd      = (b3_f64)(vParam.m_xImgPos + rect.right)  / rWidth;
			iStart    = (b3_f64)(vParam.m_yImgPos + rect.top)    / iHeight;
			iEnd      = (b3_f64)(vParam.m_yImgPos + rect.bottom) / iHeight;
			m_rEnd    = m_rStart + rEnd * rDiff;
			m_iEnd    = m_iStart + iEnd * iDiff;
			m_rStart += rStart * rDiff;
			m_iStart += iStart * iDiff;
			view->b3BestFit();
		}

		sprintf(m_text[0],"%f",m_rStart);
		sprintf(m_text[1],"%f",m_rEnd);
		sprintf(m_text[2],"%f",m_iStart);
		sprintf(m_text[3],"%f",m_iEnd);

		m_args[1] = m_text[0];
		m_args[2] = m_text[1];
		m_args[3] = m_text[2];
		m_args[4] = m_text[3];
		m_args[5] = "256";
		m_size    = sizeof(m_args) / sizeof(char *);

		// TODO: code your application's behavior here.
		m_MandelThread->b3Start(&b3ComputingThread,this);
	}
}

void CAppMandelDoc::OnReset() 
{
	// TODO: Add your command handler code here
	m_rStart = -1.0;
	m_rEnd   =  2.2;
	m_iStart = -1.0;
	m_iEnd   =  1.0;
}

void CAppMandelDoc::OnUpdateMandel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_MandelThread->b3IsRunning());
	
}
