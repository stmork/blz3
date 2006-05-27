/*
**
**	$Filename:	PageScaling.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Scaling properties
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

#include "AppLinesInclude.h"

#include "PageScaling.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2006/05/27 13:32:22  sm
**	- Added CB3Dialog base class for simple dialogs.
**	- Adjusted all tool dialog base classes for better oAW MDA generation
**
**	Revision 1.5  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.4  2005/04/27 13:55:01  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**	
**	Revision 1.3  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.2  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.1  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CPageScaling implementation                   **
**                                                                      **
*************************************************************************/


CPageScaling::CPageScaling() : CB3PropertyPage(CPageScaling::IDD)
{
	//{{AFX_DATA_INIT(CPageScaling)
	m_ScaleMode = -1;
	//}}AFX_DATA_INIT
}

CPageScaling::~CPageScaling()
{
}

void CPageScaling::DoDataExchange(CDataExchange* pDX)
{
	if (!pDX->m_bSaveAndValidate)
	{
		m_ScaleMode = m_Scaling->b3GetScaleMode();
	}
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageScaling)
	DDX_Control(pDX, IDC_SCALE_X,   m_xScaleCtrl);
	DDX_Control(pDX, IDC_SCALE_Y,   m_yScaleCtrl);
	DDX_Control(pDX, IDC_SCALE_Z,   m_zScaleCtrl);
	DDX_Radio(pDX,   IDC_SCALE_BOX_POLAR, m_ScaleMode);
	//}}AFX_DATA_MAP
	m_ScaleCtrl.b3DDX(pDX);
	if (pDX->m_bSaveAndValidate)
	{
		m_Scaling->b3SetScaleMode(m_ScaleMode);
	}
}


BEGIN_MESSAGE_MAP(CPageScaling, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageScaling)
	ON_EN_KILLFOCUS(IDC_SCALE_X, OnEdit)
	ON_EN_KILLFOCUS(IDC_SCALE_Y, OnEdit)
	ON_EN_KILLFOCUS(IDC_SCALE_Z, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_POLAR, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_OBJECT_POLAR, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_BOX_POLAR, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_IPOINT, OnEdit)
	ON_BN_CLICKED(IDC_SCALE_IPOINT_ORIGINAL, OnEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageScaling message handlers

void CPageScaling::b3PreInitDialog()
{
	m_ScaleCtrl.b3Init(&m_Scaling->m_Scale,&m_xScaleCtrl,&m_yScaleCtrl,&m_zScaleCtrl);
}

void CPageScaling::b3PostInitDialog()
{
	m_xScaleCtrl.b3SetRange(0.0001,10000);
	m_yScaleCtrl.b3SetRange(0.0001,10000);
	m_zScaleCtrl.b3SetRange(0.0001,10000);
}
