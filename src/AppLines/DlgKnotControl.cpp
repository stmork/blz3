/*
**
**	$Filename:	DlgKnotControl.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - BSpline knot control
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "DlgKnotControl.h"

#define KNOT_SCALING 20

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/07/27 19:05:59  sm
**	- Some typo cleanups.
**
**	Revision 1.1  2004/07/03 13:49:30  sm
**	- Added spline knot control dialog which is not completed yet.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgKnotControl implementation                **
**                                                                      **
*************************************************************************/

CDlgKnotControl::CDlgKnotControl(b3Spline *spline,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKnotControl::IDD, pParent)
{
	m_Spline = spline;
	//{{AFX_DATA_INIT(CDlgKnotControl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_KnotDisplay.b3Init(m_Spline);
	m_KnotNumCtrl.b3SetRange(1,spline->knot_num);
	m_KnotNum = 1;
}


void CDlgKnotControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKnotControl)
	DDX_Control(pDX, IDC_KNOT_DISPLAY, m_KnotDisplay);
	DDX_Control(pDX, IDC_KNOT_POS, m_KnotCtrl);
	DDX_Control(pDX, IDC_KNOT_NUM_SPIN, m_KnotNumCtrl);
	//}}AFX_DATA_MAP
	m_KnotNumCtrl.b3DDX(pDX,m_KnotNum);
}


BEGIN_MESSAGE_MAP(CDlgKnotControl, CDialog)
	//{{AFX_MSG_MAP(CDlgKnotControl)
	ON_EN_KILLFOCUS(IDC_KNOT_NUM_EDIT, OnKnotNumEdit)
	ON_NOTIFY(WM_LBUTTONUP, IDC_KNOT_NUM_SPIN, OnKnotNumSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnotControl message handlers

BOOL CDlgKnotControl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_KnotCtrl.SetPageSize(KNOT_SCALING);
	m_KnotCtrl.SetTicFreq(5);
	b3SetKnotCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnotControl::b3SetKnotCtrl()
{
	b3_index  num = m_KnotNum - 1;
	b3_index  min,max;
	b3_knots  knots = m_Spline->knots;

	min = (num > 0 ? knots[num - 1] : 0);
	max = (num < (m_Spline->knot_num - 1) ? knots[num + 1] : knots[m_Spline->knot_num - 1]);

	m_KnotCtrl.SetSelection(min * KNOT_SCALING,max * KNOT_SCALING);
	m_KnotCtrl.SetPos(knots[num] * KNOT_SCALING);
	m_KnotCtrl.SetRange(0,knots[m_Spline->knot_num - 1] * KNOT_SCALING,TRUE);
	m_KnotDisplay.b3Update();
}

void CDlgKnotControl::OnKnotNumEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3SetKnotCtrl();
}

void CDlgKnotControl::OnKnotNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnKnotNumEdit();
	*pResult = 0;
}
