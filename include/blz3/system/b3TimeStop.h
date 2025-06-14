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

#pragma once

#ifndef B3_SYSTEM_TIMESTOP_H
#define B3_SYSTEM_TIMESTOP_H

#include "blz3/system/b3Time.h"

class B3_PLUGIN b3TimeStop : protected b3Time
{
	const b3_f64        m_Start;
	const b3_log_level  m_Level;
	const char     *    m_Title;

public:
	explicit inline b3TimeStop(
		const char    *    title,
		const b3_log_level level = B3LOG_FULL) : m_Start(b3Now()), m_Level(level)
	{
		B3_ASSERT(title != nullptr);

		m_Title = title;

		b3PrintF(m_Level, "TIME START <%s>-----\n", m_Title);
	}

	inline void b3TimePos(const char * text B3_UNUSED = nullptr)
	{
		b3PrintF(m_Level, "TIME CONT  <%s>      duration %2.6f\n",
			m_Title, b3Now() - m_Start);
	}

	virtual ~b3TimeStop()
	{
		b3PrintF(m_Level, "TIME STOP  <%s>----- duration %2.6f\n",
			m_Title, b3Now() - m_Start);
	}
};

#endif
