/*
**
**	$Filename:	DlgProperties.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Scene properties
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

#include "AppLines.h"
#include "DlgProperties.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/08/15 13:56:42  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**
**	Revision 1.2  2002/02/10 20:03:18  sm
**	- Added grid raster
**	- Changed icon colors of shapes
**	
**	Revision 1.1  2002/02/08 15:53:37  sm
**	- Cleaned up makefiles for Un*x
**	- New dialog for print buffer size.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgProperties implementation                 **
**                                                                      **
*************************************************************************/

CDlgProperties::CDlgProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProperties)
	//}}AFX_DATA_INIT
	m_App = CB3GetLinesApp();
}


void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperties)
	DDX_Control(pDX, IDC_ROW_REFRESH, m_RowRefreshCtrl);
	DDX_Control(pDX, IDC_PRT_BUFFER_SPIN, m_PrtBufferCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProperties, CDialog)
	//{{AFX_MSG_MAP(CDlgProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties message handlers

BOOL CDlgProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PrtBufferCtrl.b3SetRange(1,128);
	m_PrtBufferCtrl.b3SetPos(m_App->m_PrintBufferSize);

	m_RowRefreshCtrl.SetRange(1,32);
	m_RowRefreshCtrl.SetPos(m_App->m_RowRefreshCount);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProperties::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	m_App->m_PrintBufferSize = m_PrtBufferCtrl.b3GetPos();
	m_App->m_RowRefreshCount = m_RowRefreshCtrl.GetPos();
	m_App->WriteProfileInt(m_App->b3ClientName(),"print buffer size",m_App->m_PrintBufferSize);
	m_App->WriteProfileInt(m_App->b3ClientName(),"row refresh count",m_App->m_RowRefreshCount);
}
