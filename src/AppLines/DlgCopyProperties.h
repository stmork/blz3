/*
**
**	$Filename:	DlgCopyProperties.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1091 $
**	$Date: 2005-05-14 21:01:24 +0200 (Sa, 14 Mai 2005) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select shape property copy modes
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCOPYPROPERTIES_H__AEE5B96B_B30C_4D61_B6D7_F006F442C37E__INCLUDED_)
#define AFX_DLGCOPYPROPERTIES_H__AEE5B96B_B30C_4D61_B6D7_F006F442C37E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCopyProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgCopyProperties dialog

#include "blz3/base/b3World.h"

class b3Shape;
class b3BBox;
class b3CopyPropertyInfo;

class CDlgCopyProperties : public CDialog
{
// Construction
public:
	CDlgCopyProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCopyProperties)
	enum { IDD = IDD_COPY_PROPERTIES };
	int		m_ModeBump;
	BOOL	m_ShapeEqualBump;
	int		m_ModeCond;
	BOOL	m_ShapeEqualCond;
	int		m_ModeMat;
	BOOL	m_ShapeEqualMat;
	BOOL	m_ShapeActiveBump;
	BOOL	m_ShapeActiveCond;
	BOOL	m_ShapeActiveMat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCopyProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCopyProperties)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	b3_bool b3CopyProperties(b3CopyPropertyInfo *info,b3BBox *bbox, b3Shape *shape);

private:
	static b3_bool b3HandleHead(b3CopyPropertyInfo *info,
		b3Shape *srcShape,b3Shape *dstShape,
		b3Base<b3Item> *src,b3Base<b3Item> *dst,
		int mode,BOOL test_equal,BOOL test_active);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOPYPROPERTIES_H__AEE5B96B_B30C_4D61_B6D7_F006F442C37E__INCLUDED_)
