/*
**
**	$Filename:	b3SelectColor.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting color
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

#include "b3SelectColor.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**
**	Revision 1.1  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3SelectTexture implementation               **
**                                                                      **
*************************************************************************/

CB3ColorFieldSelector::CB3ColorFieldSelector()
{
	m_Color  = null;
	m_Parent = null;
}

void CB3ColorFieldSelector::b3Init(b3_color *color,CWnd *parent)
{
	m_Color  = color;
	m_Parent = parent;
	b3SetColor(b3Color::b3GetColor(color));
}

b3_bool CB3ColorFieldSelector::b3Select()
{
	b3_bool result;

	result = b3Select(m_Color,m_Parent);
	if (result)
	{
		b3SetColor(b3Color::b3GetColor(m_Color));
	}
	return result;
}

b3_bool CB3ColorFieldSelector::b3Select(b3_color *color,CWnd *parent)
{
	CColorDialog dlg(b3Color::b3GetColorref(color),CC_FULLOPEN,parent);
	b3_bool      result;

	result = dlg.DoModal() == IDOK;
	if (result)
	{
		b3Color::b3GetColorref(color,dlg.GetColor());
	}
	return result;
}
