/*
**
**      $Filename:      b3Row.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Controlling one single row
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Display.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**
**	Revision 1.2  2001/07/08 12:30:07  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	
*/

b3Row::b3Row(b3_coord y,b3_res xSize) :
	b3Link<b3Row>(sizeof(b3Row))
{
	this->m_xSize = xSize;
	this->m_y     = y;
	this->m_buffer = (b3_pkd_color *)b3Alloc(sizeof(b3_pkd_color) * xSize);
}
