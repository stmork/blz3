/*
**
**	$Filename:	DlgObjectTransformMode.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for selecting transform when
**	               loading/replacing an object.
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
#include "DlgObjectTransformMode.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/05/28 15:32:06  sm
**	- Corrected class wizard file
**
**	Revision 1.1  2004/05/17 13:00:33  sm
**	- Fixed inverse/reverse handling of object editing.
**	- Added diverse handling vor object loading/replacing.
**		
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgObjectTransformMode implementation        **
**                                                                      **
*************************************************************************/

CDlgObjectTransformMode::CDlgObjectTransformMode(const char *key, int id,CWnd* pParent /*=NULL*/)
	: CDialog(id, pParent)
{
	m_RegistryKey = key;
	//{{AFX_DATA_INIT(CDlgObjectTransformMode)
	m_TransformMode = 0;
	//}}AFX_DATA_INIT
	m_TransformMode = AfxGetApp()->GetProfileInt(CB3ClientString(),m_RegistryKey,m_TransformMode);
}


void CDlgObjectTransformMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjectTransformMode)
	DDX_Radio(pDX, IDC_OBJ_KEEP, m_TransformMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjectTransformMode, CDialog)
	//{{AFX_MSG_MAP(CDlgObjectTransformMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectTransformMode message handlers

void CDlgObjectTransformMode::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	AfxGetApp()->WriteProfileInt(CB3ClientString(),m_RegistryKey,m_TransformMode);
}

void CDlgObjectTransformMode::OnCancel() 
{
	// TODO: Add extra validation here
	CDialog::OnCancel();

	m_TransformMode = OBJECT_TRANSFORM_CANCEL;
}

/*************************************************************************
**                                                                      **
**                        CDlgObjectTransformReplace implementation     **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CDlgObjectTransformReplace, CDlgObjectTransformMode)
	//{{AFX_MSG_MAP(CDlgObjectTransformReplace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgObjectTransformReplace::CDlgObjectTransformReplace(CWnd* pParent /*=NULL*/)
	: CDlgObjectTransformMode(
		"transform.object replace mode", 
		CDlgObjectTransformReplace::IDD,
		pParent)
{
	//{{AFX_DATA_INIT(CDlgObjectTransformReplace)
	//}}AFX_DATA_INIT
}

/*************************************************************************
**                                                                      **
**                        CDlgObjectTransformReplace implementation     **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CDlgObjectTransformLoad, CDlgObjectTransformMode)
	//{{AFX_MSG_MAP(CDlgObjectTransformLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgObjectTransformLoad::CDlgObjectTransformLoad(CWnd* pParent /*=NULL*/)
	: CDlgObjectTransformMode(
		"transform.object load mode", 
		CDlgObjectTransformLoad::IDD,
		pParent)
{
	//{{AFX_DATA_INIT(CDlgObjectTransformLoad)
	//}}AFX_DATA_INIT
}
