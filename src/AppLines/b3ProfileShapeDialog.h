/*
**
**	$Filename:	b3ProfileShapeDialog.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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
#include "b3Profile.h"

class CB3ProfileShapeDialog : public CB3ShapeDialog  
{
public:
	b3Profile    *m_Profile;

public:
	              CB3ProfileShapeDialog(UINT IDD,CWnd* pParent = NULL);
	virtual void  b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CB3ProfileShapeDialog)
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ProfileShapeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CB3ProfileShapeDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_B3PROFILESHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_)
