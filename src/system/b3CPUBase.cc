/*
**
**	$Filename:	b3CPUBase.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III CPU info base class
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

#include <thread>

#include "b3SystemIndInclude.h"
#include "blz3/system/b3CPUBase.h"

/*************************************************************************
**                                                                      **
**                        CPU info base class                           **
**                                                                      **
*************************************************************************/

b3CPUBase::b3CPUBase() :
	cpu_count(std::thread::hardware_concurrency())
{
}
