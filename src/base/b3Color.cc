/*
**
**	$Filename:	b3Color.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - color constants
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Color.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/05/28 19:35:39  sm
**	- Added Mork shader enhancement as new extra shader.
**
**	Revision 1.2  2003/05/17 21:09:48  sm
**	- Added feeder scanning calibration
**	
**	Revision 1.1  2003/03/08 16:08:14  sm
**	- Optimized b3_pkd_color <-> b3Color conversion.
**	
*/

/*************************************************************************
**                                                                      **
**                        color variables                               **
**                                                                      **
*************************************************************************/

b3_f32 B3_ALIGN_16 b3Color::m_Limit_m000[4] =
{
	0.0f,
	0.0f,
	0.0f,
	0.0f
};

b3_f32 B3_ALIGN_16 b3Color::m_Limit_m001[4] =
{
	1.0f,
	1.0f,
	1.0f,
	1.0f
};

b3_f32 B3_ALIGN_16 b3Color::m_Limit_m255[4] =
{
	255.0f,
	255.0f,
	255.0f,
	255.0f
};

b3_f32 B3_ALIGN_16 b3Color::m_Limit_d255[4] =
{
	0.0039215686f,
	0.0039215686f,
	0.0039215686f,
	0.0039215686f
};
