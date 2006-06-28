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

#include "b3BaseInclude.h"
#include "blz3/base/b3Color.h"

/*************************************************************************
**                                                                      **
**                        color variables                               **
**                                                                      **
*************************************************************************/

const b3_f32 B3_ALIGN_16 b3Color::m_Limit_d015[4] =
{
	0.06666666666666666f,
	0.06666666666666666f,
	0.06666666666666666f,
	0.06666666666666666f
};

const b3_f32 B3_ALIGN_16 b3Color::m_Limit_d255[4] =
{
	0.0039215686f,
	0.0039215686f,
	0.0039215686f,
	0.0039215686f
};

#ifdef BLZ3_USE_SSE

const b3_u32 B3_ALIGN_16 b3Color::m_AbsMask[4] = { 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff };

#else

const b3_f32 B3_ALIGN_16 b3Color::m_Limit_m000[4] =
{
	0.0f,
	0.0f,
	0.0f,
	0.0f
};

const b3_f32 B3_ALIGN_16 b3Color::m_Limit_m001[4] =
{
	1.0f,
	1.0f,
	1.0f,
	1.0f
};

const b3_f32 B3_ALIGN_16 b3Color::m_Limit_m255[4] =
{
	255.0f,
	255.0f,
	255.0f,
	255.0f
};

const b3_f32 B3_ALIGN_16 b3Color::m_Limit_m015[4] =
{
	15.0f,
	15.0f,
	15.0f,
	15.0f
};

#endif
