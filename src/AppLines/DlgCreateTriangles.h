/*
**
**	$Filename:	DlgCreateTriangles.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for creating triangles
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCREATETRIANGLES_H__681CB6E4_BEDE_4BB0_B647_D28615AFDE2D__INCLUDED_)
#define AFX_DLGCREATETRIANGLES_H__681CB6E4_BEDE_4BB0_B647_D28615AFDE2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateTriangles.h : header file
//

#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "b3ShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateTriangles dialog

class CDlgCreateTriangles : public CB3ShapeDialog
{
	b3_f64  m_xSize;
	b3_f64  m_ySize;
	b3_s32  m_xCount;
	b3_s32  m_yCount;
	// Construction
public:
	CDlgCreateTriangles(CWnd * pParent = NULL);  // standard constructor

	virtual void b3PostProcess();
	// Dialog Data
	//{{AFX_DATA(CDlgCreateTriangles)
	enum { IDD = IDD_CREATE_TRIANGLES };
	CStatic	m_LegendCtrl;
	CB3FloatEdit	m_xSizeCtrl;
	CB3FloatEdit	m_ySizeCtrl;
	CB3IntSpinButtonCtrl	m_xCountCtrl;
	CB3IntSpinButtonCtrl	m_yCountCtrl;
	BOOL	m_Phong;
	BOOL	m_xClosed;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateTriangles)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual void        b3Init();

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateTriangles)
	afx_msg void OnHorzClosed();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATETRIANGLES_H__681CB6E4_BEDE_4BB0_B647_D28615AFDE2D__INCLUDED_)
