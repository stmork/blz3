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

#include "AppLinesDoc.h"

#include "blz3/raytrace/b3RenderView.h"
#include "b3CameraVolume.h"

#define B3_UPDATE_VIEW      1
#define B3_UPDATE_CAMERA    2
#define B3_UPDATE_GEOMETRY  4
#define B3_UPDATE_LIGHT     8
#define B3_UPDATE_FULCRUM  16

#define B3_UPDATE_ALL  (B3_UPDATE_GEOMETRY|B3_UPDATE_VIEW|B3_UPDATE_CAMERA)

typedef enum b3SelectMode
{
	B3_SELECT_NOTHING = -1,
	B3_SELECT_MAGNIFICATION = 0,
	B3_OBJECT_SELECT,
	B3_OBJECT_MOVE,
	B3_OBJECT_ROTATE,
	B3_OBJECT_SCALE,
	B3_CAMERA_MOVE,
	B3_CAMERA_TURN,
	B3_CAMERA_ROTATE,
	B3_CAMERA_VIEW,
	B3_LIGHT_TURN,
	B3_MODE_MAX
} b3_select_mode;

class CB3Action;

class CAppLinesView : public CScrollView
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
	b3Scene        *m_Scene;
	b3RenderView    m_RenderView;
	b3CameraPart   *m_Camera;
	b3CameraVolume  m_CameraVolume;
	b3Light        *m_Light;

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
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
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
	afx_msg void OnViewPop();
	afx_msg void OnUpdateViewPop(CCmdUI* pCmdUI);
	afx_msg void OnObjSelect();
	afx_msg void OnObjMove();
	afx_msg void OnObjRotate();
	afx_msg void OnObjScale();
	afx_msg void OnCamMove();
	afx_msg void OnCamTurn();
	afx_msg void OnCamRotate();
	afx_msg void OnCamView();
	afx_msg void OnCamSelect();
	afx_msg void OnLightTurn();
	afx_msg void OnLightSelect();
	afx_msg void OnUpdateObjSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjScale(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamTurn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLightTurn(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewToFulcrum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void b3DrawRect(b3_coord x1,b3_coord y1,b3_coord x2,b3_coord y2);
	void b3Update(b3_u32 update_mask);

protected:
	void b3SetMagnification();
	void b3UnsetMagnification();

	friend class CB3Action;
	friend class CB3MoveAction;
	friend class CB3CameraRotateAction;

	friend class CB3ActionMagnify;
	friend class CB3ActionObjectMove;
	friend class CB3ActionObjectRotate;
	friend class CB3ActionObjectScale;
	friend class CB3ActionCameraMove;
	friend class CB3ActionCameraTurn;
	friend class CB3ActionCameraRotate;
};

#ifndef _DEBUG  // debug version in AppLinesView.cpp
inline CAppLinesDoc* CAppLinesView::GetDocument()
   { return (CAppLinesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESVIEW_H)
