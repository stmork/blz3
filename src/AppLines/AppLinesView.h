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

#include "blz3/raytrace/b3RenderView.h"

#define B3_UPDATE_VIEW     1
#define B3_UPDATE_CAMERA   2
#define B3_UPDATE_GEOMETRY 4

#define B3_UPDATE_ALL  (B3_UPDATE_GEOMETRY|B3_UPDATE_VIEW|B3_UPDATE_CAMERA)

class CAppLinesView : public CScrollView
{
	HDC    m_DC;
	HGLRC  m_GC;
	int    m_PixelFormatIndex;
	b3RenderView m_RenderView;
	b3_bool      m_Selecting;
	CPoint       m_SelectStart;
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
	afx_msg void OnViewPerspective();
	afx_msg void OnViewTop();
	afx_msg void OnViewFront();
	afx_msg void OnViewRight();
	afx_msg void OnViewLeft();
	afx_msg void OnViewBack();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewFront(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewBack(CCmdUI* pCmdUI);
	afx_msg void OnViewAntialias();
	afx_msg void OnUpdateViewAntialias(CCmdUI* pCmdUI);
	afx_msg void OnViewSmaller();
	afx_msg void OnViewSelect();
	afx_msg void OnViewBigger();
	afx_msg void OnViewOptimal();
	afx_msg void OnUpdateViewSmaller(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewBigger(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOptimal(CCmdUI* pCmdUI);
	afx_msg void OnViewMoveRight();
	afx_msg void OnViewMoveLeft();
	afx_msg void OnViewMoveTop();
	afx_msg void OnViewMoveBottom();
	afx_msg void OnUpdateViewMoveRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMoveLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMoveTop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMoveBottom(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
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
