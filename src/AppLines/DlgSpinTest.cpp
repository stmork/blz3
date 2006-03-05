/*
**
**	$Filename:	DlgSpinTest.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for testing spin button cotrols
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "DlgSpinTest.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.1  2002/03/08 16:46:14  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgSpinTest implementation                   **
**                                                                      **
*************************************************************************/

CDlgSpinTest::CDlgSpinTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpinTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSpinTest)
	//}}AFX_DATA_INIT
	m_Float = 15;
	m_Int   = 15;
	m_Min   = 15;
	m_Max   = 15;
	m_FloatMinCtrl.b3SetMin(10);
	m_FloatMinCtrl.b3SetDigits(2,2);
	m_FloatMaxCtrl.b3SetMax(20);
	m_FloatMaxCtrl.b3SetDigits(2,2);
	m_FloatCtrl.b3SetRange(10,20);
	m_FloatCtrl.b3SetDigits(2,2);
	m_FloatCtrl.b3SetIncrement(0.1);
	m_IntCtrl.b3SetRange(10,20);
}


void CDlgSpinTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSpinTest)
	DDX_Control(pDX, IDC_FLOAT_EDIT_MIN, m_FloatMinCtrl);
	DDX_Control(pDX, IDC_FLOAT_EDIT_MAX, m_FloatMaxCtrl);
	DDX_Control(pDX, IDC_FLOAT_SPIN, m_FloatCtrl);
	DDX_Control(pDX, IDC_INT_SPIN, m_IntCtrl);
	//}}AFX_DATA_MAP
	m_FloatMinCtrl.b3DDX(pDX,m_Min);
	m_FloatMaxCtrl.b3DDX(pDX,m_Max);
	m_FloatCtrl.b3DDX(pDX,m_Float);
	m_IntCtrl.b3DDX(pDX,m_Int);
}


BEGIN_MESSAGE_MAP(CDlgSpinTest, CDialog)
	//{{AFX_MSG_MAP(CDlgSpinTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSpinTest message handlers

BOOL CDlgSpinTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSpinTest::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	b3PrintF(B3LOG_DEBUG,"m_Float: %3.3f\n",m_Float);
	b3PrintF(B3LOG_DEBUG,"m_Int:   %d\n",   m_Int);
	b3PrintF(B3LOG_DEBUG,"m_Min:   %3.3f\n",m_Min);
	b3PrintF(B3LOG_DEBUG,"m_Max:   %3.3f\n",m_Max);
}
