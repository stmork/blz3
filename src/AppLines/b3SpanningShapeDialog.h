/*
**
**	$Filename:	b3SpanningShapeDialog.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for simple shapes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3SPANNINGSHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_)
#define AFX_B3SPANNINGSHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "b3ShapeDialog.h"

class CB3SpanningShapeDialog : public CB3ShapeDialog  
{
public:
	              CB3SpanningShapeDialog(UINT IDD,CWnd* pParent = NULL);
	virtual void  b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CB3SpanningShapeDialog)
	int		m_DirMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3SpanningShapeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3SetDirMode(int dirmode);
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CB3SpanningShapeDialog)
	afx_msg void OnDirModeChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_B3SPANNINGSHAPEDIALOG_H__DF9ADEB0_AD46_4347_BAC6_2CFB5E14CA51__INCLUDED_)
