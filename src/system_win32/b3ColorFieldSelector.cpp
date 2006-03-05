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

#include "b3SystemInclude.h"
#include "blz3/system/b3ColorFieldSelector.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.1  2006/03/03 21:54:06  sm
**	- Moved color field selector into system.
**	
**	Revision 1.4  2003/03/04 20:37:36  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.3  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
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

void CB3ColorFieldSelector::b3Init(b3Color *color,CWnd *parent)
{
	m_Color  = color;
	m_Parent = parent;
	b3SetColor(*color);
}

b3_bool CB3ColorFieldSelector::b3Select()
{
	b3_bool result;

	result = b3Select(*m_Color,m_Parent);
	if (result)
	{
		b3SetColor(*m_Color);
	}
	return result;
}

b3_bool CB3ColorFieldSelector::b3Select(b3Color &color,CWnd *parent)
{
	CColorDialog dlg(color.b3GetColorref(),CC_FULLOPEN,parent);
	b3_bool      result;

	result = dlg.DoModal() == IDOK;
	if (result)
	{
		color.b3SetColorref(dlg.GetColor());
	}
	return result;
}
