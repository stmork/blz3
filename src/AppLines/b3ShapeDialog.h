/*
**
**	$Filename:	b3ShapeDialog.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for quadric shapes
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
#include "blz3/system/b3FloatEdit.h"
#include "blz3/base/b3Matrix.h"

class CB3PosGroup
{
protected:
	CB3FloatEdit *m_xCtrl;
	CB3FloatEdit *m_yCtrl;
	CB3FloatEdit *m_zCtrl;
	b3_vector    *m_Vector;

public:
	void b3Init(b3_vector *vector,CB3FloatEdit *xCtrl,CB3FloatEdit *yCtrl,CB3FloatEdit *zCtrl)
	{
		m_Vector = vector;
		m_xCtrl  = xCtrl;
		m_yCtrl  = yCtrl;
		m_zCtrl  = zCtrl;
	}

	void b3Read(const char *title)
	{
		CString  heading;

		heading.Format("%s.x",title);
		m_Vector->x = b3ReadProfileFloat(heading,m_Vector->x);
		heading.Format("%s.y",title);
		m_Vector->y = b3ReadProfileFloat(heading,m_Vector->y);
		heading.Format("%s.z",title);
		m_Vector->z = b3ReadProfileFloat(heading,m_Vector->z);
	};

	void b3Write(const char *title)
	{
		CString  heading;

		heading.Format("%s.x",title);
		b3WriteProfileFloat(heading,m_Vector->x);
		heading.Format("%s.y",title);
		b3WriteProfileFloat(heading,m_Vector->y);
		heading.Format("%s.z",title);
		b3WriteProfileFloat(heading,m_Vector->z);
	};

	void b3Set(b3_bool init=false)
	{
		if (init)
		{
			m_xCtrl->b3SetDigits(5,3);
			m_yCtrl->b3SetDigits(5,3);
			m_zCtrl->b3SetDigits(5,3);
		}
		m_xCtrl->b3SetValue(m_Vector->x);
		m_yCtrl->b3SetValue(m_Vector->y);
		m_zCtrl->b3SetValue(m_Vector->z);
	}

	static b3_f64 b3ReadProfileFloat(const char *title,b3_f64 default_value)
	{
		CWinApp *app = AfxGetApp();
		CString  value;

		value.Format("%lf",default_value);
		return atof(app->GetProfileString(CB3ClientString(),title,value));
	}

	static b3_bool b3WriteProfileFloat(const char *title,b3_f64 default_value)
	{
		CWinApp *app = AfxGetApp();
		CString  value;

		value.Format("%lf",default_value);
		return app->WriteProfileString(CB3ClientString(),title,value);
	}

	void b3Update()
	{
		m_Vector->x = m_xCtrl->m_Value;
		m_Vector->y = m_yCtrl->m_Value;
		m_Vector->z = m_zCtrl->m_Value;
	}
};

class CB3DirGroup : public CB3PosGroup
{
protected:
	CB3FloatEdit *m_lenCtrl;
	b3_vector    *m_Base;

public:
	enum b3_dir_mode
	{
		B3_DIRMODE_DIR,
		B3_DIRMODE_POS
	};

public:
	void b3Init(b3_vector *base,b3_vector *vector,CB3FloatEdit *xCtrl,CB3FloatEdit *yCtrl,CB3FloatEdit *zCtrl,CB3FloatEdit *lenCtrl)
	{
		CB3PosGroup::b3Init(vector,xCtrl,yCtrl,zCtrl);
		m_lenCtrl = lenCtrl;
		m_Base    = base;
	}

	void b3Set(int mode,b3_bool init=false)
	{
		b3_vector pos;

		// Set edit characteristics
		if (init)
		{
			m_lenCtrl->b3SetMin(0.0001);
			m_lenCtrl->b3SetDigits(5,3);
			m_xCtrl->b3SetDigits(5,3);
			m_yCtrl->b3SetDigits(5,3);
			m_zCtrl->b3SetDigits(5,3);
		}

		// Init values
		switch ((b3_dir_mode)mode)
		{
		case B3_DIRMODE_DIR:
			m_xCtrl->b3SetValue(m_Vector->x);
			m_yCtrl->b3SetValue(m_Vector->y);
			m_zCtrl->b3SetValue(m_Vector->z);
			break;
		case B3_DIRMODE_POS:
			b3Vector::b3Add(m_Base,m_Vector,&pos);
			m_xCtrl->b3SetValue(pos.x);
			m_yCtrl->b3SetValue(pos.y);
			m_zCtrl->b3SetValue(pos.z);
			break;
		}
		m_lenCtrl->b3SetValue(b3Vector::b3Length(m_Vector));
	}

	void b3Update(int mode)
	{
		b3_vector pos;

		switch ((b3_dir_mode)mode)
		{
		case B3_DIRMODE_DIR:
			m_Vector->x = m_xCtrl->m_Value;
			m_Vector->y = m_yCtrl->m_Value;
			m_Vector->z = m_zCtrl->m_Value;
			break;
		case B3_DIRMODE_POS:
			pos.x = m_xCtrl->m_Value;
			pos.y = m_yCtrl->m_Value;
			pos.z = m_zCtrl->m_Value;
			b3Vector::b3Sub(&pos,m_Base,m_Vector);
			break;
		}
		m_lenCtrl->b3SetValue(b3Vector::b3Length(m_Vector));
	}

	void b3UpdateLen(int mode)
	{
		b3Vector::b3Normalize(m_Vector,m_lenCtrl->m_Value);
		b3Set(mode);
	}
};

typedef int (*b3ItemEditCall)(b3Item *item,b3_bool create);

/////////////////////////////////////////////////////////////////////////////
// CB3ShapeDialog dialog

class CB3ShapeDialog : public CDialog
{
// Construction
public:
	b3_bool   m_Creation;
	CB3ShapeDialog(UINT IDD,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CB3ShapeDialog)
	int		m_DirMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ShapeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void b3SetDirMode(int dirmode);

	// Generated message map functions
	//{{AFX_MSG(CB3ShapeDialog)
	afx_msg void OnDirModeChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3SHAPEDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_)
