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

#include "blz3/b3Config.h"
#include "blz3/system/b3TimeAbstract.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/11/16 14:24:00  sm
**	- Added a CPU benchmark
**	- Removed system dependend #IF from raytracing
**
**	Revision 1.1  2002/08/11 11:03:40  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**	
**	
*/

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
	b3PrintF(level," Time needed: %s\n",b3PrintTime(buffer,m_rTime));
	b3PrintF(level," User time:   %s\n",b3PrintTime(buffer,m_uTime));
	b3PrintF(level," System time: %s\n",b3PrintTime(buffer,m_sTime));
	b3PrintF(level," Load:        %3.2f%%\n",b3GetUsage() * 100.0);
}

char *b3TimeSpanAbstract::b3PrintTime(char *buffer,b3_f64 value)
{
	b3_s32 time_needed = (b3_s32)floor(value * 1000.0 + 0.5);

	sprintf(buffer,"%3d:%02d,%02d",
		 time_needed / 60000,
		(time_needed /  1000) % 60,
		 time_needed %  1000);
	return buffer;
}
