// AppMandelView.h : interface of the CAppMandelView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPMANDELVIEW_H__DF6683BF_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
#define AFX_APPMANDELVIEW_H__DF6683BF_729E_11D5_A544_0050BF4EB3F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/system/b3ScrollView.h"

class CAppMandelView : public CB3ScrollView
{
protected: // create from serialization only
	CAppMandelView();
	DECLARE_DYNCREATE(CAppMandelView)

// Attributes
public:
	CAppMandelDoc* GetDocument();

// Operations
public:
	void b3BestFit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppMandelView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppMandelView();
	virtual b3Document *b3GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAppMandelView)
	afx_msg void OnUnfiltered();
	afx_msg void OnFiltered();
	afx_msg void OnOriginal();
	afx_msg void OnWidth();
	afx_msg void OnHeight();
	afx_msg void OnFull();
	afx_msg void OnMore();
	afx_msg void OnLess();
	afx_msg void OnMagnify();
	afx_msg void OnUpdateUnfiltered(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiltered(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOriginal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWidth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHeight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFull(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMore(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLess(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMagnify(CCmdUI* pCmdUI);
	//}}AFX_MSG
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AppMandelView.cpp
inline CAppMandelDoc* CAppMandelView::GetDocument()
   { return (CAppMandelDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPMANDELVIEW_H__DF6683BF_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
