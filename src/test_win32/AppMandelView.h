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
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAppMandelView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
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
