// AppMandelDoc.h : interface of the CAppMandelDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPMANDELDOC_H__DF6683BD_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
#define AFX_APPMANDELDOC_H__DF6683BD_729E_11D5_A544_0050BF4EB3F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/system/b3Document.h"

class CAppMandelDoc : public CDocument, public b3Document
{
protected: // create from serialization only
	CAppMandelDoc();
	DECLARE_DYNCREATE(CAppMandelDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppMandelDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAppMandelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAppMandelDoc)
	afx_msg void OnReset();
	afx_msg void OnCompute();
	afx_msg void OnUpdateMandel(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	b3_f64    m_rStart;
	b3_f64    m_rEnd;
	b3_f64    m_iStart;
	b3_f64    m_iEnd;
	int       m_size;
	char     *m_args[6];
	char      m_text[4][32];
	b3Thread *m_MandelThread;

	static b3_u32 b3ComputingThread(void *ptr);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPMANDELDOC_H__DF6683BD_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
