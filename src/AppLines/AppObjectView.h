/*
**
**	$Filename:	AppObjectView.h $
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

#if !defined(APPOBJECTVIEW_H)
#define APPOBJECTVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppObjectDoc.h"
#include "blz3/raytrace/b3RenderView.h"
#include "b3CameraVolume.h"
#include "b3View.h"

class CB3Action;

class CAppObjectView : public CScrollView
{
	HDC             m_DC;
	HGLRC           m_GC;
	int             m_PixelFormatIndex;
	b3_select_mode  m_PreviousMode;
	b3_select_mode  m_SelectMode;
	CPoint          m_SelectStart;
	CPoint          m_SelectAct;
	CB3Action      *m_Action[B3_MODE_MAX];

protected:
	b3RenderView    m_RenderView;

protected: // create from serialization only
	CAppObjectView();
	DECLARE_DYNCREATE(CAppObjectView)

// Attributes
public:
	CAppObjectDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppObjectView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppObjectView();
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
	afx_msg void OnViewSmaller();
	afx_msg void OnViewSelect();
	afx_msg void OnViewBigger();
	afx_msg void OnViewOptimal();
	afx_msg void OnViewPop();
	afx_msg void OnUpdateViewSmaller(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewBigger(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOptimal(CCmdUI* pCmdUI);
	afx_msg void OnViewMoveRight();
	afx_msg void OnViewMoveLeft();
	afx_msg void OnViewMoveUp();
	afx_msg void OnViewMoveDown();
	afx_msg void OnUpdateViewPop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMove(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void b3DrawRect(b3_coord x1,b3_coord y1,b3_coord x2,b3_coord y2);
	void b3Update(b3_u32 update_mask);

protected:
	void b3SetMagnification();
	void b3UnsetMagnification();
};

#ifndef _DEBUG  // debug version in AppLinesView.cpp
inline CAppObjectDoc* CAppObjectView::GetDocument()
   { return (CAppObjectDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESVIEW_H)
