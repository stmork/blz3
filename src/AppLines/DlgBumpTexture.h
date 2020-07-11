/*
**
**	$Filename:	DlgBumpTexture.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture bump mapping
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPTEXTURE_H__31B2166E_250C_43FA_9DCE_01249D2B1356__INCLUDED_)
#define AFX_DLGBUMPTEXTURE_H__31B2166E_250C_43FA_9DCE_01249D2B1356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpTexture.h : header file
//

#include "blz3/raytrace/b3BumpSampler.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "blz3/system/b3ShowSample.h"
#include "blz3/system/b3SimplePreviewDialog.h"
#include "b3ConditionControl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpTexture dialog

class CDlgBumpTexture : public CB3SimplePreviewDialog
{
	b3_stencil_bound    m_Bound;
	b3BBox       *      m_BBox;
	b3Shape      *      m_Shape;
	b3BumpTexture   *   m_Bump;
	b3BumpSampler   *   m_BumpSampler;

	b3ConditionControl  m_xStart;
	b3ConditionControl  m_xScale;
	b3ConditionControl  m_yStart;
	b3ConditionControl  m_yScale;

	// Construction
public:
	static b3_bool b3Edit(b3Item * item, void * ptr);
	static void    b3Register();
	CDlgBumpTexture(b3Item * item, CAppObjectDoc * pDoc, CWnd * pParent = NULL); // standard constructor
	~CDlgBumpTexture();

	// Dialog Data
	//{{AFX_DATA(CDlgBumpTexture)
	enum { IDD = IDD_BUMP_TEXTURE };
	CB3FloatSpinButtonCtrl	m_xStartCtrl;
	CB3FloatSpinButtonCtrl	m_xScaleCtrl;
	CB3IntSpinButtonCtrl	m_xTimesCtrl;
	CB3FloatSpinButtonCtrl	m_yStartCtrl;
	CB3FloatSpinButtonCtrl	m_yScaleCtrl;
	CB3IntSpinButtonCtrl	m_yTimesCtrl;
	CString	m_yScaleLegend;
	CString	m_yStartLegend;
	CString	m_xScaleLegend;
	CString	m_xStartLegend;
	int		m_Unit;
	CB3FloatSpinButtonCtrl	m_AmplitudeCtrl;
	CB3ShowImage	m_PreviewTexture;
	CB3ShowSample	m_PreviewBumpCtrl;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpTexture)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpTexture)
	afx_msg void OnUnitChanged();
	afx_msg void OnLimitChanged();
	afx_msg void OnChangeTexturePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPTEXTURE_H__31B2166E_250C_43FA_9DCE_01249D2B1356__INCLUDED_)
