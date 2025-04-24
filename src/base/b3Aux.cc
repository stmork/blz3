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
#include "blz3/system/b3Locale.h"
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
	const b3_coord y2) :
	m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{
}

bool b3Rect::b3UpdateBound(const b3Rect * rect)
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

/*************************************************************************
**                                                                      **
**                        String tool                                   **
**                                                                      **
*************************************************************************/

int b3StringTool::b3CaseCompare(
	const std::string & left,
	const std::string & right)
{
	try
	{
		b3Locale     locale;
		std::wstring u16_left  = locale.b3FromBytes(left);
		std::wstring u16_right = locale.b3FromBytes(right);
		b3_size      i = 0;
		int          diff;

		i = 0;
		do
		{
			diff = std::tolower(u16_left[i], locale) - std::tolower(u16_right[i], locale);
			if ((u16_left[i] == 0) || (u16_right[i] == 0))
			{
				return diff;
			}
			i++;
		}
		while (diff == 0);

		return diff;
	}
	catch (std::range_error & e)
	{
		return left.compare(right);
	}
}

std::string b3StringTool::b3ToLower(const std::string & input)
{
	b3Locale     locale;
	std::wstring result = locale.b3FromBytes(input);

	for (b3_size i = 0; i < result.size(); i++)
	{
		result[i] = std::tolower(result[i], locale);
	}

	return locale.b3ToBytes(result);
}

std::string b3StringTool::b3ToUpper(const std::string & input)
{
	b3Locale     locale;
	std::wstring result = locale.b3FromBytes(input);

	for (b3_size i = 0; i < result.size(); i++)
	{
		result[i] = std::toupper(result[i], locale);
	}

	return locale.b3ToBytes(result);
}
