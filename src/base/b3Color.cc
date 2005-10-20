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
**	Revision 1.7  2005/10/20 19:55:17  sm
**	- Introduced SSE intrinsics.
**
**	Revision 1.6  2005/10/16 08:37:23  sm
**	- Fixed OpenEXR configure.
**	- Fixed bimg3 for HDR image output.
**	- Optimized IFF-ILBM image access.
**	
**	Revision 1.5  2005/08/02 15:02:50  smork
**	- Optimized b3Mix methods.
**	- Documentation.
**	
**	Revision 1.4  2004/06/05 08:07:05  sm
**	- Corrected b3Color for multiplying colors as filter.
**	
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

#ifndef HAVE_SSE
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

b3_f32 B3_ALIGN_16 b3Color::m_Limit_d015[4] =
{
	0.0666666666f,
	0.0666666666f,
	0.0666666666f,
	0.0666666666f
};

b3_f32 B3_ALIGN_16 b3Color::m_Limit_d255[4] =
{
	0.0039215686f,
	0.0039215686f,
	0.0039215686f,
	0.0039215686f
};
#endif
