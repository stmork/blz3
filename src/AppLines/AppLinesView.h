/*
**
**	$Filename:	AppLinesView.h $
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

#if !defined(APPLINESVIEW_H)
#define APPLINESVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define B3_UPDATE_DRAW 1
#define B3_UPDATE_VIEW 2
#define B3_UPDATE_ALL  (B3_UPDATE_DRAW|B3_UPDATE_VIEW)

class CAppLinesView : public CScrollView
{
	HDC    m_DC;
	HGLRC  m_GC;
	int    m_PixelFormatIndex;
protected: // create from serialization only
	CAppLinesView();
	DECLARE_DYNCREATE(CAppLinesView)

// Attributes
public:
	CAppLinesDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppLinesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAppLinesView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AppLinesView.cpp
inline CAppLinesDoc* CAppLinesView::GetDocument()
   { return (CAppLinesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESVIEW_H)
