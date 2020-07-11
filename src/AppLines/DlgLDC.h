/*
**
**	$Filename:	DlgLDC.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Light properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGLDC_H__0234CD51_EAFC_11D5_9CFB_080020FDDE74__INCLUDED_)
#define AFX_DLGLDC_H__0234CD51_EAFC_11D5_9CFB_080020FDDE74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLDC.h : header file
//

#include "blz3/raytrace/b3Light.h"
#include "blz3/system/b3Dialog.h"
#include "blz3/system/b3ColorField.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "b3ControlLDC.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLDC dialog

class CDlgLDC : public CB3Dialog
{
	// Construction
	b3_u32     m_ShadingClassType;
	b3Scene  * m_LightScene;

public:
	b3Light  * m_Light;
	CDlgLDC(b3_u32 class_type, CWnd * pParent = NULL); // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgLDC)
	enum { IDD = IDD_LDC };
	CB3ControlLDC	m_CtrlLDC;
	CB3ControlLDC	m_CtrlDiagram;
	CB3ShowRaytrace	m_CtrlPreview;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLDC)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult);
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLDC)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLDC_H__0234CD51_EAFC_11D5_9CFB_080020FDDE74__INCLUDED_)
