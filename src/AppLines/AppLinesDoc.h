/*
**
**	$Filename:	AppLinesDoc.h $
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

#if !defined(APPLINESDOC_H)
#define APPLINESDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/raytrace/b3Raytrace.h"
#include "b3Fulcrum.h"

class CAppLinesDoc : public CDocument, public b3Document
{
protected:
	b3World  m_World;
// Attributes
public:
	b3Fulcrum             m_Fulcrum;
	b3Scene              *m_Scene;
	b3ModellerInfo       *m_Info;
	b3RenderShapeContext  m_Context;
	b3_vector             m_Lower;
	b3_vector             m_Upper;

// Operations
protected: // create from serialization only
	CAppLinesDoc();
	DECLARE_DYNCREATE(CAppLinesDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	b3_vector *b3GetFulcrum();
	void       b3DrawFulcrum();
	void       b3ComputeBounds();
	virtual ~CAppLinesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAppLinesDoc)
	afx_msg void OnHierachy();
	afx_msg void OnRaytrace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESDOC_H)
