/*
**
**	$Filename:	b3ProfileShapeDialog.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 308 $
**	$Date: 2002-03-10 21:34:18 +0100 (So, 10 Mrz 2002) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for shapes which can be created via profiles
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3PROFILESHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_)
#define AFX_B3PROFILESHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "b3ShapeDialog.h"

class b3Profile;

class CB3ProfileShapeDialog : public CB3ShapeDialog  
{
protected:
	CB3PosGroup   m_BaseGroup;
	b3_vector     m_Base;

public:
	b3Profile    *m_Profile;

public:
	              CB3ProfileShapeDialog(UINT IDD,CWnd* pParent = NULL);
	virtual      ~CB3ProfileShapeDialog();
	virtual void  b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CB3ProfileShapeDialog)
	CB3FloatEdit	m_xBaseCtrl;
	CB3FloatEdit	m_yBaseCtrl;
	CB3FloatEdit	m_zBaseCtrl;
	int		m_Align;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ProfileShapeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void b3Init();

	// Generated message map functions
	//{{AFX_MSG(CB3ProfileShapeDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_B3PROFILESHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_)
