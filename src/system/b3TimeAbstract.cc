/*
**
**	$Filename:	b3TimeAbstract.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - date routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemIndInclude.h"
#include "blz3/system/b3TimeAbstract.h"

/*************************************************************************
**                                                                      **
**                        b3TimeSpan routines                           **
**                                                                      **
*************************************************************************/

b3TimeSpanAbstract::b3TimeSpanAbstract()
{
	b3Clear();
}

void b3TimeSpanAbstract::b3Clear()
{
	m_uTime = 0;
	m_sTime = 0;
	m_rTime = 0;
}

b3_f64 b3TimeSpanAbstract::b3GetUsage()
{
	return (m_rTime > 0 ? (m_uTime + m_sTime) / m_rTime : 1.0);
}

void b3TimeSpanAbstract::b3Print(b3_log_level level)
{
	char buffer[32];

	b3PrintF(level,"Computation time:\n");
	b3PrintF(level," Time needed: %s\n",b3PrintTime(buffer,sizeof(buffer),m_rTime));
	b3PrintF(level," User time:   %s\n",b3PrintTime(buffer,sizeof(buffer),m_uTime));
	b3PrintF(level," System time: %s\n",b3PrintTime(buffer,sizeof(buffer),m_sTime));
	b3PrintF(level," Load:        %3.2f%%\n",b3GetUsage() * 100.0);
}

char *b3TimeSpanAbstract::b3PrintTime(char *buffer,b3_size size,b3_f64 value)
{
	b3_s32 time_needed = (b3_s32)floor(value * 1000.0 + 0.5);

	snprintf(buffer,size,"%3d:%02d,%02d",
		 time_needed / 60000,
		(time_needed /  1000) % 60,
		 time_needed %  1000);
	return buffer;
}
