/*
**
**	$Filename:	b3ShapeDialog.h $
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

#if !defined(AFX_B3SHAPEDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_)
#define AFX_B3SHAPEDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// b3ShapeDialog.h : header file
//

#include "AppLines.h"
#include "b3VectorEdit.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/base/b3Matrix.h"

typedef int (*b3ItemEditCall)(b3Item *item,b3_bool create);

/////////////////////////////////////////////////////////////////////////////
// CB3ShapeDialog dialog

class CB3ShapeDialog : public CPropertyPage
{
// Construction
public:
	b3ShapeRenderObject *m_Shape;
	b3_bool              m_Creation;

public:
	     CB3ShapeDialog(UINT IDD,CWnd* pParent = NULL);   // standard constructor

	virtual void b3PostProcess();
	static  int  b3Edit(CB3ShapeDialog *page,b3Item *item,b3_bool create);

// Dialog Data
	//{{AFX_DATA(CB3ShapeDialog)
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ShapeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();
	virtual const char *b3GetSection();

	// Generated message map functions
	//{{AFX_MSG(CB3ShapeDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3SHAPEDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_)
