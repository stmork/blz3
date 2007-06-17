/*
**
**	$Filename:	AppLinesView.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 498 $
**	$Date: 2003-03-04 21:37:39 +0100 (Di, 04 Mrz 2003) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for scenes
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Pick.h"
#include "blz3/base/b3Render.h"
#include "b3CameraVolume.h"
#include "b3PickVector.h"
#include "AppRenderView.h"

class CB3Action;

class CAppLinesView : public CAppRenderView
{
	b3PickBase      m_PickBaseLight;

protected:
	b3Scene        *m_Scene;

protected: // create from serialization only
	CAppLinesView();
	DECLARE_DYNCREATE(CAppLinesView)

public:
	static b3Color  m_GridColorUnit;
	static b3Color  m_GridColorMove;

// Attributes
public:
	CAppLinesDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesView)
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
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
	b3_bool   b3IsMouseActionAllowed();
	b3_bool   b3GetDimension(b3_f64 &xSize,b3_f64 &ySize,b3_f64 &unit,b3_f64 &measure);
	void      b3Draw(b3_res xSize,b3_res ySize,b3_f64 xOffset = 0.0,b3_f64 yOffset = 0.0);
	void      b3DrawDC(HDC hDC,b3_res xSize,b3_res ySize,b3_f64 xOffset = 0.0,b3_f64 yOffset = 0.0);
	void      b3DrawLegend(CDC *pDC);
	void      b3UpdateLight();

// Generated message map functions
protected:
	//{{AFX_MSG(CAppLinesView)
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
	afx_msg void OnUpdateObjSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjScale(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamTurn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCamView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLightTurn(CCmdUI* pCmdUI);
	afx_msg void OnViewToFulcrum();
	afx_msg void OnCameraNew();
	afx_msg void OnCameraDelete();
	afx_msg void OnCameraProperties();
	afx_msg void OnUpdateCameraDelete(CCmdUI* pCmdUI);
	afx_msg void OnCameraEnable();
	afx_msg void OnUpdateCameraEnable(CCmdUI* pCmdUI);
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	afx_msg void OnRotateLeft();
	afx_msg void OnRotateRight();
	afx_msg void OnUpdateMovement(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3Move(b3_action_mode mode);
	void b3Rotate(b3_action_mode mode);

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
	friend class CB3ActionCameraView;
	friend class CB3ActionLightTurn;
};

#ifndef _DEBUG  // debug version in AppLinesView.cpp
inline CAppLinesDoc* CAppLinesView::GetDocument()
   { return (CAppLinesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESVIEW_H)
