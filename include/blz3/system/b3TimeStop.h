/*
**
**	$Filename:	b3TimeStop.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Time stopping via C'tor and D'tor
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_TIMESTOP_H
#define B3_SYSTEM_TIMESTOP_H

#include "blz3/system/b3Time.h"

class B3_PLUGIN b3TimeStop : protected b3Time
{
	      b3_f64        m_Start;
	const b3_log_level  m_Level;
	const char         *m_Title;

public:
	inline b3TimeStop(const char *title, const b3_log_level level = B3LOG_FULL)
		: m_Level(level)
	{
		B3_ASSERT(title != null);

		m_Title = title;
		m_Start = b3Now();

		b3PrintF(m_Level, "TIME START <%s>\n", m_Title);
	}

	inline void b3TimePos(const char *text = null)
	{
		b3PrintF(m_Level, "TIME CONT <%s> duration %2.3f\n",
			m_Title, b3Now() - m_Start);
	}

	inline virtual ~b3TimeStop()
	{
		b3PrintF(m_Level, "TIME STOP <%s> duration %2.3f\n",
			m_Title, b3Now() - m_Start);
	}
};

#endif
