/*
**
**	$Filename:	AppRenderView.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for OpenGL Documents (scenes/objects)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPRENDERVIEW_H)
#define APPRENDERVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppRenderDoc.h"
#include "blz3/system/b3DashPen.h"
#include "blz3/system/b3PixelFormat.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/raytrace/b3RenderLight.h"
#include "blz3/base/b3Array.h"
#include "b3CameraVolume.h"

#define B3_UPDATE_VIEW        1
#define B3_UPDATE_CAMERA      2
#define B3_UPDATE_GEOMETRY    4
#define B3_UPDATE_LIGHT       8
#define B3_UPDATE_FULCRUM    16
#define B3_UPDATE_PICK      256
#define B3_UPDATE_LIGHTVIEW 512


#define B3_UPDATE_ALL  (B3_UPDATE_GEOMETRY|B3_UPDATE_VIEW|B3_UPDATE_CAMERA|B3_UPDATE_LIGHT)

enum b3_select_mode
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
	B3_SHAPE_MOVE,
	B3_SHAPE_ROTATE_POINT,
	B3_SHAPE_ROTATE_AXIS,
	B3_SHAPE_SCALE,
	B3_SHAPE_MIRROR_POINT,
	B3_SHAPE_MIRROR_AXIS,
	B3_SHAPE_MIRROR_AREA,
	B3_MODE_MAX
};

class CB3Action;

class CAppRenderView : public CScrollView, public CB3PixelFormat
{
	// OpenGL printing values
	HGLRC           m_prtGC;
	HDC             m_prtDC;
	HBITMAP         m_prtBitmap;
	HGDIOBJ         m_prtOldBitmap;
	b3_f64          m_prtScale;
	b3_f64          m_prtUnit;
	b3_f64          m_prtWidth;
	b3_f64          m_prtHeight;
	b3_res          m_prtPageWidth;
	b3_res          m_prtPageHeight;
	b3_res          m_prtLogWidth;
	b3_res          m_prtLogHeight;
	b3_res          m_prtLogOffsetX;
	b3_res          m_prtLogOffsetY;
	b3_res          m_prtHardCopyWidth;
	b3_res          m_prtHardCopyHeight;
	b3_count        m_prtCountWidth;
	b3_count        m_prtCountHeight;
	b3_res          m_prtLineWidth;
	b3_res          m_prtLineHeight;
	b3_count        m_prtLineNum;
	b3_bool         m_DoubleBuffered;

protected:
	CB3DashPen      m_RedDash;

	// Acting modes
	b3_select_mode  m_PreviousMode;
	b3_select_mode  m_SelectMode;
	CPoint          m_SelectStart;
	CPoint          m_SelectAct;
	CB3Action   *   m_Action[B3_MODE_MAX];

	// camera data
	b3RenderView    m_RenderView;
	b3RenderLight   m_RenderLight;
	b3CameraPart  * m_Camera;
	b3CameraVolume  m_CameraVolume;

protected: // create from serialization only
	CAppRenderView();
	DECLARE_DYNCREATE(CAppRenderView)

	// Attributes
public:
	CAppRenderDoc * GetDocument();

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppRenderView)
public:
	virtual void OnDraw(CDC * pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView * pSender, LPARAM lHint, CObject * pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo * pInfo);
	virtual void OnBeginPrinting(CDC * pDC, CPrintInfo * pInfo);
	virtual void OnEndPrinting(CDC * pDC, CPrintInfo * pInfo);
	virtual void OnPrint(CDC * pDC, CPrintInfo * pInfo);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CAppRenderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext & dc) const;
#endif

protected:
	virtual b3_bool   b3IsMouseActionAllowed();
	virtual b3_bool   b3GetDimension(b3_f64 & xSize, b3_f64 & ySize, b3_f64 & unit, b3_f64 & measure);
	virtual void      b3Draw(b3_res xSize, b3_res ySize, b3_f64 xOffset = 0.0, b3_f64 yOffset = 0.0);
	virtual void      b3DrawDC(HDC hDC, b3_res xSize, b3_res ySize, b3_f64 xOffset = 0.0, b3_f64 yOffset = 0.0);
	virtual void      b3DrawLegend(CDC * pDC);
	virtual void      b3UpdateLight();
	void      b3DrawText(CDC * pDC, const char * text);

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppRenderView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewPerspective();
	afx_msg void OnViewTop();
	afx_msg void OnViewFront();
	afx_msg void OnViewRight();
	afx_msg void OnViewLeft();
	afx_msg void OnViewBack();
	afx_msg void OnViewSmaller();
	afx_msg void OnViewSelect();
	afx_msg void OnViewBigger();
	afx_msg void OnViewOptimal();
	afx_msg void OnViewPop();
	afx_msg void OnViewMoveRight();
	afx_msg void OnViewMoveLeft();
	afx_msg void OnViewMoveUp();
	afx_msg void OnViewMoveDown();
	afx_msg void OnUpdateViewPerspective(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewTop(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewFront(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewRight(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewLeft(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewBack(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewSmaller(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewSelect(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewBigger(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewOptimal(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewPop(CCmdUI * pCmdUI);
	afx_msg void OnUpdateViewMove(CCmdUI * pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void b3DrawRect(b3_coord x1, b3_coord y1, b3_coord x2, b3_coord y2);
	void b3Update(b3_u32 update_mask);
	void b3Paint();

protected:
	void b3SetMagnification();
	void b3UnsetMagnification();

	friend class CB3Action;
	friend class CB3MoveAction;
	friend class CB3CameraRotateAction;
	friend class CB3ShapeAction;

	friend class CB3ActionMagnify;
	friend class CB3ActionObjectSelect;
	friend class CB3ActionObjectRotate;
	friend class CB3ActionObjectScale;
	friend class CB3ActionCameraMove;
	friend class CB3ActionCameraTurn;
	friend class CB3ActionCameraRotate;
	friend class CB3ActionCameraView;
};

#ifndef _DEBUG  // debug version in AppLinesView.cpp
inline CAppRenderDoc * CAppRenderView::GetDocument()
{
	return (CAppRenderDoc *)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPRENDERVIEW_H)
