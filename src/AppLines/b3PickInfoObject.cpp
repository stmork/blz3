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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.3  2003/02/25 19:26:07  sm
**	- Fixed missing grid update on pick move
**	
**	Revision 1.2  2003/02/24 17:32:38  sm
**	- Added further picking support.
**	- Fixed geometry update delay.
**	
**	Revision 1.1  2003/02/23 21:15:41  sm
**	- First shape picking
**	
*/

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
