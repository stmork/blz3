#if !defined(AFX_B3IMAGEBUTTON_H__FDF44B83_040C_11D6_9D01_080020FDDE74__INCLUDED_)
#define AFX_B3IMAGEBUTTON_H__FDF44B83_040C_11D6_9D01_080020FDDE74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3ImageButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CB3ImageButton window

class CB3ImageButton : public CButton
{
	bool m_Pressed;

// Construction
public:
	CB3ImageButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ImageButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CB3ImageButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ImageButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3IMAGEBUTTON_H__FDF44B83_040C_11D6_9D01_080020FDDE74__INCLUDED_)
