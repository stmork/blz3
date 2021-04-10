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

void b3Color::b3Dump() const
{
	const std::string & text = *this;

	b3PrintF(B3LOG_NORMAL, "%s\n", text.c_str());
}

std::ostream & operator<< (std::ostream & out, const b3Color & color)
{
	const std::string & text = color;

	out << text;
	return out;
}
