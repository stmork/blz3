/*
**
**	$Filename:	DlgDistributed.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Distributed properties
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "DlgDistributed.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.6  2006/02/19 18:58:47  sm
**	- Some dialog inheritance corrections.
**	
**	Revision 1.5  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.4  2002/08/21 20:13:32  sm
**	- Introduced distributed raytracing with all sampling methods
**	  and filter computations. This made some class movements
**	  inside files necessary. The next step would be to integrate
**	  motion blur.
**	
**	Revision 1.3  2001/11/25 12:25:31  sm
**	- Completing some dialogs:
**	  o super sampling
**	  o distributed raytracing
**	
**	Revision 1.2  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.1  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgDistributed implementation                **
**                                                                      **
*************************************************************************/

CDlgDistributed::CDlgDistributed(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgDistributed::IDD)
{
	//{{AFX_DATA_INIT(CDlgDistributed)
	m_ActDistributed = FALSE;
	m_ActMotionBlur = FALSE;
	m_PixelFilter = -1;
	m_TimeFilter = -1;
	m_SamplingMethod = -1;
	//}}AFX_DATA_INIT
}


void CDlgDistributed::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDistributed)
	DDX_Control(pDX, IDC_SPP_SPIN, m_CtrlSPP);
	DDX_Control(pDX, IDC_SPF_SPIN, m_CtrlSPF);
	DDX_Check(pDX, IDC_ACT_DISTRIBUTED, m_ActDistributed);
	DDX_Check(pDX, IDC_ACT_MOTION_BLUR, m_ActMotionBlur);
	DDX_Radio(pDX, IDC_PFLTR_BOX, m_PixelFilter);
	DDX_Radio(pDX, IDC_TFLTR_BOX, m_TimeFilter);
	DDX_Radio(pDX, IDC_SMP_REGULAR, m_SamplingMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDistributed, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgDistributed)
	ON_BN_CLICKED(IDC_ACT_DISTRIBUTED, OnActivation)
	ON_BN_CLICKED(IDC_ACT_MOTION_BLUR, OnActivation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDistributed message handlers

BOOL CDlgDistributed::OnInitDialog() 
{
	b3_s32 flags = SAMPLE_GET_FLAGS(m_Distributed);
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ActDistributed = m_Distributed->b3IsActive();
	m_ActMotionBlur  = flags & SAMPLE_MOTION_BLUR;
	m_SamplingMethod = SAMPLE_GET_TYPE(m_Distributed) >> 8;
	m_PixelFilter    = m_Distributed->m_PixelAperture;
	m_TimeFilter     = m_Distributed->m_FrameAperture;

	UpdateData(FALSE);
	m_CtrlSPP.b3SetRange(1,  10);
	m_CtrlSPF.b3SetRange(1,1000);
	m_CtrlSPP.b3SetPos(m_Distributed->m_SamplesPerPixel);
	m_CtrlSPF.b3SetPos(m_Distributed->m_SamplesPerFrame);
	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgDistributed::PreTranslateMessage(MSG* pMsg) 
{
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CDlgDistributed::b3UpdateUI()
{
	GetDlgItem(IDC_ACT_MOTION_BLUR)->EnableWindow(m_ActDistributed);

	// Sampling methods
	GetDlgItem(IDC_SMP_REGULAR)->EnableWindow(m_ActDistributed);
	GetDlgItem(IDC_SMP_RANDOM)->EnableWindow(m_ActDistributed);
	GetDlgItem(IDC_SMP_JITTER)->EnableWindow(m_ActDistributed);
	GetDlgItem(IDC_SMP_SEMI)->EnableWindow(m_ActDistributed);
	GetDlgItem(IDC_SMP_SEPARATED)->EnableWindow(m_ActDistributed);

	// Samples per pixel
	GetDlgItem(IDC_SPP)->EnableWindow(m_ActDistributed);
	m_CtrlSPP.EnableWindow(m_ActDistributed);

	// Samples per frame
	GetDlgItem(IDC_SPF)->EnableWindow(m_ActDistributed && m_ActMotionBlur);
	m_CtrlSPF.EnableWindow(m_ActDistributed && m_ActMotionBlur);

	// Pixel aperture
	GetDlgItem(IDC_PFLTR_BOX)->EnableWindow(m_ActDistributed);
	GetDlgItem(IDC_PFLTR_GAUSS)->EnableWindow(m_ActDistributed);

	// Frame aperture
	GetDlgItem(IDC_TFLTR_BOX)->EnableWindow(m_ActDistributed && m_ActMotionBlur);
	GetDlgItem(IDC_TFLTR_GAUSS)->EnableWindow(m_ActDistributed && m_ActMotionBlur);
	GetDlgItem(IDC_TFLTR_SHUTTER)->EnableWindow(m_ActDistributed && m_ActMotionBlur);
}

void CDlgDistributed::OnActivation() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	b3UpdateUI();
}

void CDlgDistributed::OnOK() 
{
	// TODO: Add extra validation here
	b3_u32 flags = 0;

	UpdateData(TRUE);
	if (m_ActDistributed) flags |= SAMPLE_SUPERSAMPLE;
	if (m_ActMotionBlur)  flags |= SAMPLE_MOTION_BLUR;

	SAMPLE_SET_FLAGS(m_Distributed,flags);
	SAMPLE_SET_TYPE(m_Distributed,m_SamplingMethod << 8);
	m_Distributed->m_SamplesPerPixel = m_CtrlSPP.b3GetPos();
	m_Distributed->m_SamplesPerFrame = m_CtrlSPF.b3GetPos();
	m_Distributed->m_PixelAperture   = (b3_filter)m_PixelFilter;
	m_Distributed->m_FrameAperture   = (b3_filter)m_TimeFilter;
	CPropertyPage::OnOK();
}
