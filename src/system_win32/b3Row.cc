/*
**
**      $Filename:      b3Row.cpp $
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

#include "blz3/system/b3Display.h"

b3Row::b3Row(b3_coord y,b3_res xSize) :
	b3Link<b3Row>(sizeof(b3Row))
{
	this->xSize = xSize;
	this->y     = y;
	buffer      = (b3_pkd_color *)b3Alloc(sizeof(b3_pkd_color) * xSize);
}
