/*
**
**	$Filename:	AppRaytraceView.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPRAYTRACEVIEW_H)
#define APPRAYTRACEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/system/b3ScrollView.h"

class CAppRaytraceView : public CB3ScrollView
{
public: // create from serialization only
	CAppRaytraceView();
	DECLARE_DYNCREATE(CAppRaytraceView)

// Attributes
public:
	CAppLinesDoc* GetDocument();

// Operations
public:
	void b3BestFit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppRaytraceView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppRaytraceView();
	virtual b3Document *b3GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAppRaytraceView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AppMandelView.cpp
inline CAppLinesDoc* CAppRaytraceView::GetDocument()
   { return (CAppLinesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPMANDELVIEW_H__DF6683BF_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
