/*
**
**	$Filename:	b3PickInfoObject.cpp $ 
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setup pick points depending on shapes
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "b3PickInfoObject.h"
#include "b3PickVector.h"

/*************************************************************************
**                                                                      **
**                        b3PickInfo implementation                     **
**                                                                      **
*************************************************************************/

b3PickInfoObject::b3PickInfoObject(b3RenderView *render_view)
{
	m_RenderView = render_view;
}

void b3PickInfoObject::b3AddPickPoint(
	b3_vector  *point,
	const char *title)
{
	b3Pick *pick = new b3PickPoint(m_RenderView,point,title);

	b3Append(pick);
}

void b3PickInfoObject::b3AddPickDir(
	b3_vector  *point,
	b3_vector  *dir,
	const char *title)
{
	b3Pick *pick = new b3PickDir(m_RenderView,point,dir,title);

	b3Append(pick);
}
