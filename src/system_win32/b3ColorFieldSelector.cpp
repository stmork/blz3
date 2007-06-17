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
