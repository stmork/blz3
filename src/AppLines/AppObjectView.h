/*
**
**	$Filename:	AppObjectView.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for objects
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "AppRenderView.h"
#include "blz3/raytrace/b3RenderView.h"
#include "b3CameraVolume.h"

class CB3Action;

class CAppObjectView : public CAppRenderView
{
protected: // create from serialization only
	b3BBox *m_BBox;
	b3_f64  m_xAngle;
	b3_f64  m_yAngle;
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
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
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
	//{{AFX_MSG(CAppObjectView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CB3ShapeAction;
};

#ifndef _DEBUG  // debug version in AppLinesView.cpp
inline CAppObjectDoc* CAppObjectView::GetDocument()
   { return (CAppObjectDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPOBJECTVIEW_H)
