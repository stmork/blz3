/*
**
**	$Filename:	AppObjectDoc.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for objects
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
#include "AppObjectDoc.h"
#include "AppObjectView.h"
#include "MainFrm.h"

#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2002/01/16 17:01:08  sm
**	- Some minor fixes done
**
**	Revision 1.6  2002/01/16 16:17:12  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.5  2002/01/15 16:17:31  sm
**	- Checked OLE support
**	- Checked icons
**	- Added two bwd files which create icons
**	
**	Revision 1.4  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.3  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.2  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.1  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesDoc implementation                   **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppObjectDoc, CAppRenderDoc)

BEGIN_MESSAGE_MAP(CAppObjectDoc, CAppRenderDoc)
	//{{AFX_MSG_MAP(CAppObjectDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAppObjectDoc, CAppRenderDoc)
	//{{AFX_DISPATCH_MAP(CAppObjectDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAppObject to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {72D6951A-8984-11D5-A54F-0050BF4EB3F3}
static const IID IID_IAppObject =
{ 0x72d6951a, 0x8984, 0x11d5, { 0xa5, 0x4f, 0x0, 0x50, 0xbf, 0x4e, 0xb3, 0xf3 } };

BEGIN_INTERFACE_MAP(CAppObjectDoc, CAppRenderDoc)
	INTERFACE_PART(CAppObjectDoc, IID_IAppObject, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc construction/destruction

CAppObjectDoc::CAppObjectDoc()
{
	// TODO: add one-time construction code here
	m_BBox         = null;
	m_Raytracer    = new b3Thread("Raytracing master thread");
	m_Info         = new b3ModellerInfo(LINES_INFO);
	b3MatrixUnit(&m_OriginalPosition);
	EnableAutomation();

	AfxOleLockApp();
}

CAppObjectDoc::~CAppObjectDoc()
{
	AfxOleUnlockApp();
	delete m_Raytracer;
	delete m_Info;
}

BOOL CAppObjectDoc::OnNewDocument()
{
	BOOL        result = FALSE;

	if (!CDocument::OnNewDocument())
	{
		return result;
	}

	// TODO: Add your specialized creation code here
	return result;
}

BOOL CAppObjectDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CMainFrame     *main = CB3GetMainFrame();
	CString         message;
	BOOL            result = FALSE;
	b3Base<b3Item>  base;
	b3_count        level;
	b3_matrix       inverse;

	if (!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}

	try
	{
		message.Format(IDS_DOC_READ,lpszPathName);
		main->b3SetStatusMessage(message);

		m_World.b3Read(lpszPathName);
		main->b3SetStatusMessage(IDS_DOC_REORG);
		m_BBox = (b3BBox *)m_World.b3GetFirst();
		level  = m_BBox->b3GetClassType() & 0xffff;
		b3BBox::b3Reorg(m_World.b3GetHead(),&base,level,1);
		b3BBox::b3Recount(m_BBox->b3GetBBoxHead());

		m_OriginalPosition = m_BBox->m_Matrix;
		b3MatrixInv(&m_BBox->m_Matrix,&inverse);
		m_BBox->b3Transform(&inverse,true);
		main->b3SetStatusMessage(IDS_DOC_PREPARE);
		m_BBox->b3Prepare();

		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_BBox->b3AllocVertices(&m_Context);

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();

		UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
//		m_DlgHierarchy->b3InitTree(this,true);
//		m_DlgHierarchy->b3SelectBBox(bbox);
		result = TRUE;
	}
	catch(b3WorldException *e)
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error code %d\n",e->b3GetError());
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
	}
	return result;
}

BOOL CAppObjectDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL     result = false;
	return result;
}

void CAppObjectDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_BBox->b3Transform(&m_OriginalPosition,true);
	delete m_BBox;
	m_BBox = null;
	CDocument::OnCloseDocument();
}



/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc serialization

void CAppObjectDoc::Serialize(CArchive& ar)
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
// CAppLinesDoc diagnostics

#ifdef _DEBUG
void CAppObjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppObjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/*************************************************************************
**                                                                      **
**                        General scene commands                        **
**                                                                      **
*************************************************************************/

void CAppObjectDoc::b3ComputeBounds()
{
	m_Lower.x =  FLT_MAX;
	m_Lower.y =  FLT_MAX;
	m_Lower.z =  FLT_MAX;
	m_Upper.x = -FLT_MAX;
	m_Upper.y = -FLT_MAX;
	m_Upper.z = -FLT_MAX;

	m_BBox->b3ComputeBounds(&m_Lower,&m_Upper,0.02);
}
