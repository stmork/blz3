/*
**
**	$Filename:	b3Aux.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some helpers
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

#include "b3BaseInclude.h"
#include "blz3/base/b3Aux.h"

/*************************************************************************
**                                                                      **
**                    Blizzard III rect class                           **
**                                                                      **
*************************************************************************/

b3Rect::b3Rect(
	const b3_coord x1,
	const b3_coord y1,
	const b3_coord x2,
	const b3_coord y2)
{
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
}

b3_res b3Rect::b3Width() const
{
	return m_x2 - m_x1;
}

b3_res b3Rect::b3Height() const
{
	return m_y2 - m_y1;
}

bool b3Rect::b3UpdateBound(b3Rect * rect)
{
	bool changed = false;

	if (rect->m_x1 < m_x1)
	{
		m_x1 = rect->m_x1;
		changed = true;
	}
	if (rect->m_y1 < m_y1)
	{
		m_y1 = rect->m_y1;
		changed = true;
	}
	if (rect->m_x2 > m_x2)
	{
		m_x2 = rect->m_x2;
		changed = true;
	}
	if (rect->m_y2 > m_y2)
	{
		m_y2 = rect->m_y2;
		changed = true;
	}
	return changed;
}

bool b3Rect::b3CheckBound(b3Rect * rect)
{
	bool changed = false;

	if (m_x1 < rect->m_x1)
	{
		m_x1 = rect->m_x1;
		changed = true;
	}
	if (m_y1 < rect->m_y1)
	{
		m_y1 = rect->m_y1;
		changed = true;
	}
	if (m_x2 > rect->m_x2)
	{
		m_x2 = rect->m_x2;
		changed = true;
	}
	if (m_y2 > rect->m_y2)
	{
		m_y2 = rect->m_y2;
		changed = true;
	}
	if (m_x1 > m_x2)
	{
		m_x1 = m_x2;
		changed = true;
	}
	if (m_y1 > m_y2)
	{
		m_y1 = m_y2;
		changed = true;
	}
	return changed;
}

/*************************************************************************
**                                                                      **
**                        String tool                                   **
**                                                                      **
*************************************************************************/

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> b3StringTool::m_Convert;
std::locale                                            b3StringTool::m_Locale("de_DE.UTF-8");

int b3StringTool::b3CaseCompare(
	const std::string & left,
	const std::string & right)
{
	std::wstring u16_left  = m_Convert.from_bytes(left);
	std::wstring u16_right = m_Convert.from_bytes(right);
	b3_index     i = 0;
	int          diff;

	i = 0;
	do
	{
		diff = std::tolower(u16_left[i], m_Locale) - std::tolower(u16_right[i], m_Locale);
		if ((u16_left[i] == 0) || (u16_right[i] == 0))
		{
			return diff;
		}
		i++;
	}
	while (diff == 0);

	return diff;

}

std::string b3StringTool::b3ToLower(const std::string & input)
{
	std::wstring result = m_Convert.from_bytes(input);

	for (unsigned i = 0; i < result.size(); i++)
	{
		result[i] = std::tolower(result[i], m_Locale);
	}

	return m_Convert.to_bytes(result);
}

std::string b3StringTool::b3ToUpper(const std::string & input)
{
	std::wstring result = m_Convert.from_bytes(input);

	std::transform(
		result.begin(), result.end(), result.begin(),
		[] (const wchar_t ch)
	{
		return std::toupper(ch, m_Locale);
	});

	return m_Convert.to_bytes(result);
}
