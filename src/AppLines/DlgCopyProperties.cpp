/*
**
**	$Filename:	DlgCopyProperties.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select shape property copy modes
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
#include "DlgCopyProperties.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3BBox.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**
**	Revision 1.2  2004/07/02 19:28:03  sm
**	- Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**	- Recoupled b3Scene include from CApp*Doc header files to allow
**	  faster compilation.
**	- Removed intersection counter completely because of a mysterious
**	  destruction problem of b3Mutex.
**	
**	Revision 1.1  2004/05/07 16:30:33  sm
**	- Bug #13 fixed. The BBox hierarchy is recounted on every
**	  object edit finish.
**	- Wooden materials contain a dark and a separate light
**	  material. Changes were also made in Lines.
**	- Introduced shape property copy including all materials,
**	  bumps and conditions. Multiple copy modes are possible.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        DlgCopyProperties implementation              **
**                                                                      **
*************************************************************************/

CDlgCopyProperties::CDlgCopyProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCopyProperties::IDD, pParent)
{
	CB3App *app = CB3GetApp();

	//{{AFX_DATA_INIT(CDlgCopyProperties)
	m_ModeMat = 0;
	m_ShapeEqualMat = TRUE;
	m_ShapeActiveMat = TRUE;
	m_ModeBump = 3;
	m_ShapeEqualBump = FALSE;
	m_ShapeActiveBump = TRUE;
	m_ModeCond = 3;
	m_ShapeEqualCond = FALSE;
	m_ShapeActiveCond = TRUE;
	//}}AFX_DATA_INIT

	m_ModeBump        = app->GetProfileInt(CB3ClientString(),"copy properties.mode bump",m_ModeBump);
	m_ModeCond        = app->GetProfileInt(CB3ClientString(),"copy properties.mode cond",m_ModeCond);
	m_ModeMat         = app->GetProfileInt(CB3ClientString(),"copy properties.mode mat", m_ModeMat);
	m_ShapeEqualBump  = app->GetProfileInt(CB3ClientString(),"copy properties.equal bump",m_ShapeEqualBump);
	m_ShapeEqualCond  = app->GetProfileInt(CB3ClientString(),"copy properties.equal cond",m_ShapeEqualCond);
	m_ShapeEqualMat   = app->GetProfileInt(CB3ClientString(),"copy properties.equal mat", m_ShapeEqualMat);
	m_ShapeActiveBump = app->GetProfileInt(CB3ClientString(),"copy properties.active bump",m_ShapeActiveBump);
	m_ShapeActiveCond = app->GetProfileInt(CB3ClientString(),"copy properties.active cond",m_ShapeActiveCond);
	m_ShapeActiveMat  = app->GetProfileInt(CB3ClientString(),"copy properties.active mat", m_ShapeActiveMat);
}


void CDlgCopyProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCopyProperties)
	DDX_Radio(pDX, IDC_BUMP_NONE, m_ModeBump);
	DDX_Check(pDX, IDC_BUMP_SHAPE_EQUAL, m_ShapeEqualBump);
	DDX_Radio(pDX, IDC_COND_NONE, m_ModeCond);
	DDX_Check(pDX, IDC_COND_SHAPE_EQUAL, m_ShapeEqualCond);
	DDX_Radio(pDX, IDC_MAT_NONE, m_ModeMat);
	DDX_Check(pDX, IDC_MAT_SHAPE_EQUAL, m_ShapeEqualMat);
	DDX_Check(pDX, IDC_BUMP_ACTIVE, m_ShapeActiveBump);
	DDX_Check(pDX, IDC_COND_ACTIVE, m_ShapeActiveCond);
	DDX_Check(pDX, IDC_MAT_ACTIVE, m_ShapeActiveMat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCopyProperties, CDialog)
	//{{AFX_MSG_MAP(CDlgCopyProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCopyProperties message handlers

void CDlgCopyProperties::OnOK() 
{
	CB3App *app = CB3GetApp();

	// TODO: Add extra validation here
	CDialog::OnOK();

	app->WriteProfileInt(CB3ClientString(),"copy properties.mode bump",m_ModeBump);
	app->WriteProfileInt(CB3ClientString(),"copy properties.mode cond",m_ModeCond);
	app->WriteProfileInt(CB3ClientString(),"copy properties.mode mat", m_ModeMat);
	app->WriteProfileInt(CB3ClientString(),"copy properties.equal bump",m_ShapeEqualBump);
	app->WriteProfileInt(CB3ClientString(),"copy properties.equal cond",m_ShapeEqualCond);
	app->WriteProfileInt(CB3ClientString(),"copy properties.equal mat", m_ShapeEqualMat);
	app->WriteProfileInt(CB3ClientString(),"copy properties.active bump",m_ShapeActiveBump);
	app->WriteProfileInt(CB3ClientString(),"copy properties.active cond",m_ShapeActiveCond);
	app->WriteProfileInt(CB3ClientString(),"copy properties.active mat", m_ShapeActiveMat);
}

b3_bool CDlgCopyProperties::b3CopyProperties(b3BBox *bbox,b3Shape *shape)
{
	b3_bool  changed = false;
	b3Item  *item;

	B3_FOR_BASE(bbox->b3GetBBoxHead(),item)
	{
		changed |= b3CopyProperties((b3BBox *)item,shape);
	}

	B3_FOR_BASE(bbox->b3GetShapeHead(),item)
	{
		b3Shape *dstShape = (b3Shape *)item;

		changed |= b3HandleHead(shape,dstShape,shape->b3GetMaterialHead(), dstShape->b3GetMaterialHead(), m_ModeMat, m_ShapeEqualMat, m_ShapeActiveMat);
		changed |= b3HandleHead(shape,dstShape,shape->b3GetBumpHead(),     dstShape->b3GetBumpHead(),     m_ModeBump,m_ShapeEqualBump,m_ShapeActiveBump);
		changed |= b3HandleHead(shape,dstShape,shape->b3GetConditionHead(),dstShape->b3GetConditionHead(),m_ModeCond,m_ShapeEqualCond,m_ShapeActiveCond);
	}
	return changed;
}

b3_bool CDlgCopyProperties::b3HandleHead(
	b3Shape        *srcShape,
	b3Shape        *dstShape,
	b3Base<b3Item> *src,
	b3Base<b3Item> *dst,
	int mode,BOOL test_equal,BOOL test_active)
{
	b3Item  *srcItem,*dstItem;
	b3_bool  changed = false;

	if (src == dst)
	{
		return false;
	}

	if (test_equal)
	{
		if (srcShape->b3GetClassType() != dstShape->b3GetClassType())
		{
			return false;
		}
	}

	if (test_active)
	{
		if (!dstShape->b3IsActive())
		{
			return false;
		}
	}

	switch(mode)
	{
	case 3:
		// Replace always
		changed = true;
		break;

	case 2:
		// Is any source class type inside destination class?
		B3_FOR_BASE(src,srcItem)
		{
			B3_FOR_BASE(dst,dstItem)
			{
				if (srcItem->b3GetClassType() == dstItem->b3GetClassType())
				{
					changed = true;
				}
			}
		}
		break;

	case 1:
		// source must be equal to dst
		if (src->b3GetCount() == dst->b3GetCount())
		{
			int count = 0;
			int equal = 0;

			for (srcItem = src->First,dstItem = dst->First;(srcItem != null) && (dstItem != null);srcItem = srcItem->Succ,dstItem = dstItem->Succ)
			{
				count++;
				if (srcItem->b3GetClassType() == dstItem->b3GetClassType())
				{
					equal++;
				}
			}
			if (count == equal)
			{
				changed = true;
			}
		}
		break;

	case 0:
	default:
		// Do nothing!
		break;
	}

	if (changed)
	{
		// Store original items
		srcShape->b3Store();

		// Delete old items
		dst->b3Free();

		// Clone items
		b3World::b3CloneBase(src,dst);
	}
	return changed;
}
