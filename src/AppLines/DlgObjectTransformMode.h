/*
**
**	$Filename:	DlgObjectTransformMode.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for selecting transform when
**	               loading/replacing an object.
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGOBJECTTRANSFORMMODE_H__005AFCBA_94E6_4A01_A224_21A983212790__INCLUDED_)
#define AFX_DLGOBJECTTRANSFORMMODE_H__005AFCBA_94E6_4A01_A224_21A983212790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjectTransformMode.h : header file
//


enum b3_object_transform_mode
{
	OBJECT_TRANSFORM_CANCEL  = -1,
	OBJECT_TRANSFORM_KEEP    =  0,
	OBJECT_TRANSFORM_DELETE,
	OBJECT_TRANSFORM_RESET,
	OBJECT_TRANSFORM_USE_OLD
};

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectTransformMode dialog

class CDlgObjectTransformMode : public CDialog
{
	const char *m_RegistryKey;

// Construction
public:
	CDlgObjectTransformMode(const char *key,int id,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjectTransformMode)
	int		m_TransformMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjectTransformMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjectTransformMode)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	inline b3_object_transform_mode b3GetMode()
	{
		DoModal();
		return (b3_object_transform_mode)m_TransformMode;
	}
};

class CDlgObjectTransformReplace : public CDlgObjectTransformMode
{
public:
	CDlgObjectTransformReplace(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjectTransformReplace)
	enum { IDD = IDD_OBJECT_REPLACE };
	//}}AFX_DATA

	static b3_object_transform_mode b3Dialog()
	{
		CDlgObjectTransformReplace dlg;

		return dlg.b3GetMode();
	}
};

class CDlgObjectTransformLoad : public CDlgObjectTransformMode
{
public:
	CDlgObjectTransformLoad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjectTransformLoad)
	enum { IDD = IDD_OBJECT_LOAD };
	//}}AFX_DATA

	static b3_object_transform_mode b3Dialog()
	{
		CDlgObjectTransformLoad dlg;

		return dlg.b3GetMode();
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJECTTRANSFORMMODE_H__005AFCBA_94E6_4A01_A224_21A983212790__INCLUDED_)
