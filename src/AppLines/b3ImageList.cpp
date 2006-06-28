/*
**
**	$Filename:	b3ImageList.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Image list for div. dialogs
**
**	(C) Copyright 2002  Steffen A. Mork
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

#include "b3ImageList.h"
#include "DlgShape1.h"
#include "DlgShape2.h"
#include "DlgShape3.h"
#include "DlgTorus.h"
#include "DlgCSG1.h"
#include "DlgCSG3.h"
#include "DlgCSGTorus.h"
#include "DlgSelectProfile.h"

/*************************************************************************
**                                                                      **
**                        CB3ImageList implementation                   **
**                                                                      **
*************************************************************************/

int CB3ImageList::m_ResIcon[] =
{
	IDI_BBOX_EMPTY,
	IDI_BBOX_SUB,
	IDI_BBOX_SHAPES,
	IDI_BBOX_SHAPES_SUB,
	IDI_BBOX_WORLD,
	IDI_BBOX_EMPTY,
	IDI_BBOX_SUB,
	IDI_BBOX_S_SHAPES,
	IDI_BBOX_S_SHAPES_SUB,
	IDI_BBOX_WORLD,
	IDI_SHAPE_AREA,
	IDI_SHAPE_DISK,
	IDI_SHAPE_SPHERE,
	IDI_SHAPE_CYLINDER,
	IDI_SHAPE_CONE,
	IDI_SHAPE_ELLIPSOID,
	IDI_SHAPE_BOX,
	IDI_SHAPE_TORUS,
	IDI_SHAPE_TRIANGLES,
	IDI_SHAPE_BSPLINE_ROT,
	IDI_SHAPE_BSPLINE_AREA,
	IDI_SHAPE_BSPLINE_CYLINDER,
	IDI_SHAPE_BSPLINE_RING,

	// CSG shapes
	IDI_CSG_SPHERE,
	IDI_CSG_CYLINDER,
	IDI_CSG_CONE,
	IDI_CSG_ELLIPSOID,
	IDI_CSG_BOX,
	IDI_CSG_TORUS
};

int CB3ImageList::m_ResString[] =
{
	IDS_UNKNOWN,
	IDS_BBOX,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_UNKNOWN,
	IDS_SHAPE_AREA,
	IDS_SHAPE_DISK,
	IDS_SHAPE_SPHERE,
	IDS_SHAPE_CYLINDER,
	IDS_SHAPE_CONE,
	IDS_SHAPE_ELLIPSOID,
	IDS_SHAPE_BOX,
	IDS_SHAPE_TORUS,
	IDS_SHAPE_TRIANGLES,
	IDS_SHAPE_BSPLINE_ROT,
	IDS_SHAPE_BSPLINE_AREA,
	IDS_SHAPE_BSPLINE_CYLINDER,
	IDS_SHAPE_BSPLINE_RING,

	// CSG shapes
	IDS_CSG_SPHERE,
	IDS_CSG_CYLINDER,
	IDS_CSG_CONE,
	IDS_CSG_ELLIPSOID,
	IDS_CSG_BOX,
	IDS_CSG_TORUS
};

b3_u32 CB3ImageList::m_ClassTypes[] =
{
	0,
	BBOX,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,

	// Simple shapes
	AREA,
	DISK,
	SPHERE,
	CYLINDER,
	CONE,
	ELLIPSOID,
	BOX,
	TORUS,
	TRIANGLES,
	SPLINE_ROT,
	SPLINES_AREA,
	SPLINES_CYL,
	SPLINES_RING,

	// CSG shapes
	CSG_SPHERE,
	CSG_CYLINDER,
	CSG_CONE,
	CSG_ELLIPSOID,
	CSG_BOX,
	CSG_TORUS
};

b3ItemEditCall CB3ImageList::m_EditCalls[] =
{
	null,
	null,
	null,
	null,
	null,
	null,
	null,
	null,
	null,
	null,

	// Simple Shapes
	CDlgShape2::b3Edit,
	CDlgShape2::b3Edit,
	CDlgShape1::b3Edit,
	CDlgShape3::b3Edit,
	CDlgShape3::b3Edit,
	CDlgShape3::b3Edit,
	CDlgShape3::b3Edit,
	CDlgTorus::b3Edit,
	CDlgSelectProfile::b3Edit,
	CDlgSelectProfile::b3Edit,
	CDlgSelectProfile::b3Edit,
	CDlgSelectProfile::b3Edit,
	CDlgSelectProfile::b3Edit,

	// CSG shapes
	CDlgCSG1::b3Edit,
	CDlgCSG3::b3Edit,
	CDlgCSG3::b3Edit,
	CDlgCSG3::b3Edit,
	CDlgCSG3::b3Edit,
	CDlgCSGTorus::b3Edit
};

void CB3ImageList::b3Create(b3_res size)
{
	int i;
	CWinApp *app = AfxGetApp();

	Create(size,size,ILC_COLOR32,30,8);
	for (i = 0;i < (sizeof(m_ResIcon) / sizeof(int));i++)
	{
		Add(app->LoadIcon(m_ResIcon[i]));
	}
}

long CB3ImageList::b3ComputeImgNum(b3Item *item)
{
	long imgnum = 0;

	if (item->b3GetClass() == CLASS_BBOX)
	{
		b3BBox *BBox = (b3BBox *)item;
		b3BBox *sub;

		sub = (b3BBox *)BBox->b3GetBBoxHead()->First;
		if ((sub != null) || (BBox->b3GetShapeHead()->First != null))
		{
			if (sub != null)        imgnum += 1;
			imgnum += 2;
			if (BBox->b3IsActive()) imgnum += 5;
		}
		return imgnum;
	}
	else switch(item->b3GetClassType())
	{
	case AREA:
		imgnum = 10;
		break;
	case DISK:
		imgnum = 11;
		break;
	case SPHERE:
		imgnum = 12;
		break;
	case CYLINDER:
		imgnum = 13;
		break;
	case CONE:
		imgnum = 14;
		break;
	case ELLIPSOID:
		imgnum = 15;
		break;
	case BOX:
		imgnum = 16;
		break;
	case TORUS:
		imgnum = 17;
		break;
	case TRIANGLES:
		imgnum = 18;
		break;
	case SPLINE_ROT:
		imgnum = 19;
		break;
	case SPLINES_AREA:
		imgnum = 20;
		break;
	case SPLINES_CYL:
		imgnum = 21;
		break;
	case SPLINES_RING:
		imgnum = 22;
		break;

	case CSG_SPHERE:
		imgnum = 23;
		break;
	case CSG_CYLINDER:
		imgnum = 24;
		break;
	case CSG_CONE:
		imgnum = 25;
		break;
	case CSG_ELLIPSOID:
		imgnum = 26;
		break;
	case CSG_BOX:
		imgnum = 27;
		break;
	case CSG_TORUS:
		imgnum = 28;
		break;

	default:
		imgnum = 0;
		break;
	}
	return imgnum;
}

long CB3ImageList::b3ComputeText(b3Item *item,CString &text)
{
	CString  mode;
	b3BBox  *bbox;
	long     imgnum = b3ComputeImgNum(item);

	switch(item->b3GetClass())
	{
	case CLASS_BBOX:
		bbox = (b3BBox *)item;
		text = bbox->b3GetName();
		break;

	case CLASS_SHAPE:
	case CLASS_CSG:
		if (item->b3GetClass() == CLASS_CSG)
		{
			b3CSGShape *csg = (b3CSGShape *)item;
			switch(csg->m_Operation)
			{
			case B3_CSG_UNION:
				mode.LoadString(IDS_CSG_MODE_UNION);
				break;
			case B3_CSG_INTERSECT:
				mode.LoadString(IDS_CSG_MODE_INTERSECT);
				break;
			case B3_CSG_SUB:
				mode.LoadString(IDS_CSG_MODE_SUB);
				break;
			}
		}
		else
		{
			mode.LoadString(IDS_CSG_MODE_NONE);
		}

		text.Format(m_ResString[imgnum],(const char *)mode);
		break;
	}
	return imgnum;
}

b3ItemEditCall CB3ImageList::b3GetEditCall(b3Item *item)
{
	b3_u32 ClassType = item->b3GetClassType();
	int    i;

	for (i = 0;i < (sizeof(m_ClassTypes) / sizeof(b3_u32));i++)
	{
		if (m_ClassTypes[i] == ClassType)
		{
			return m_EditCalls[i];
		}
	}
	return null;
}

void CB3ImageList::b3InsertListEntries(
	CListCtrl *list_ctrl,
	b3_u32     shape_class,
	b3_u32     select_class_type)
{
	LVITEM  item;
	CString text;
	int     i;

	memset(&item,0,sizeof(item));
	item.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
	for (i = 0;i < (sizeof(m_ClassTypes) / sizeof(b3_u32));i++)
	{
		if ((m_ClassTypes[i] & 0xffff0000) == shape_class)
		{
			text.Format(m_ResString[i],"");

			item.iImage  = i;
			item.lParam  = m_ClassTypes[i];
			item.state   = (m_ClassTypes[i] == select_class_type ? item.stateMask : 0);
			item.pszText = (char *)((const char *)text);
			list_ctrl->InsertItem(&item);
		}
	}
}
