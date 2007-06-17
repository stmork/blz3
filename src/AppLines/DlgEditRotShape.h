/*
**
**	$Filename:	DlgEditRotShape.h $
**	$Release:	Dortmund 2007 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for rotation shapes.
**
**	(C) Copyright 2007  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLG_EDIT_ROTSHAPE_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
#define AFX_DLG_EDIT_ROTSHAPE_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShape3.h : header file
//

#include "AppLines.h"
#include "b3SpanningShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEditRotShape dialog

class CDlgEditRotShape : public CB3SpanningShapeDialog
{
// Construction
protected:
	CB3PosGroup m_Base;
	CB3DirGroup m_Dir1;

public:
	CDlgEditRotShape(CWnd* pParent = NULL);   // standard constructor
	CDlgEditRotShape(UINT id,CWnd* pParent = NULL);

	static  int  b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create=true);
	virtual void b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CDlgEditRotShape)
	enum { IDD = IDD_EDIT_ROTSHAPE };
	CB3FloatEdit	m_xBaseCtrl;
	CB3FloatEdit	m_yBaseCtrl;
	CB3FloatEdit	m_zBaseCtrl;
	CB3FloatEdit	m_xDir1Ctrl;
	CB3FloatEdit	m_yDir1Ctrl;
	CB3FloatEdit	m_zDir1Ctrl;
	CB3FloatEdit	m_lenDir1Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditRotShape)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnChangedBase();
	afx_msg void OnChangedDir1();
	afx_msg void OnChangedLen1();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();
	virtual void        b3SetDirMode(int dirmode);
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CDlgEditRotShape)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EDIT_ROTSHAPE_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
