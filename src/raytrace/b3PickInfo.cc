/*
**
**	$Filename:	b3PickInfo.cc $ 
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

#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2003/02/23 21:15:41  sm
**	- First shape picking
**
*/

/*************************************************************************
**                                                                      **
**                        b3PickInfo implementation                     **
**                                                                      **
*************************************************************************/

b3PickInfo::b3PickInfo()
{
	m_Shape = null;
}

b3PickInfo::~b3PickInfo()
{
	b3Free();
}

b3_bool b3PickInfo::b3SetShape(b3Shape *shape)
{
	b3_bool result = false;

	if (m_Shape != shape)
	{
		b3Free();
		m_Shape = shape;
		if (m_Shape != null)
		{
			m_Shape->b3SetupPicking(this);
		}
		result = true;
	}
	return result;
}
