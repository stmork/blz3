/*
**
**	$Filename:	AppRaytraceDoc.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPRAYTRACEDOC_H)
#define APPRAYTRACEDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/system/b3Document.h"
#include "blz3/raytrace/b3Raytrace.h"

#include "AppLinesDoc.h"

class CAppRaytraceDoc : public CDocument, public b3Document
{
	b3Scene      *m_Scene;
	CAppLinesDoc *m_LinesDoc;
protected: // create from serialization only
	CAppRaytraceDoc();
	DECLARE_DYNCREATE(CAppRaytraceDoc)

// Attributes
public:

// Operations
public:
	void b3SetLinesDoc(CAppLinesDoc *LinesDoc);
	b3Display *b3GetDisplay(const char *title);
	b3Display *b3GetDisplay(b3_res xSize,b3_res ySize,const char *title);
	void       b3ActivateView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppRaytraceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppRaytraceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppRaytraceDoc)
	afx_msg void OnRaytrace();
	afx_msg void OnUpdateRaytrace(CCmdUI* pCmdUI);
	afx_msg void OnSaveImage();
	afx_msg void OnUpdateSaveImage(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPRAYTRACEDOC_H)
