/*
**
**	$Filename:	AppObjectDoc.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPOBJECTDOC_H)
#define APPOBJECTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/raytrace/b3Raytrace.h"

class CDlgHierarchy;

class CAppObjectDoc : public CDocument, public b3Document
{
protected:
	b3World               m_World;
	b3Thread             *m_Raytracer;
	b3Display            *m_Display;

	// Attributes
public:
	b3Fulcrum             m_Fulcrum;
	b3BBox               *m_BBox;
	b3ModellerInfo       *m_Info;
	b3RenderShapeContext  m_Context;
	b3_vector             m_Lower;
	b3_vector             m_Upper;

// Operations
protected: // create from serialization only
	CAppObjectDoc();
	DECLARE_DYNCREATE(CAppObjectDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppObjectDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
	b3_vector *b3GetFulcrum();
	b3_vector *b3GetStepMove();
	b3_vector *b3GetStepRotate();
	void       b3DrawFulcrum();

	virtual   ~CAppObjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppLinesDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	CDlgHierarchy *m_DlgHierarchy;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESDOC_H)
