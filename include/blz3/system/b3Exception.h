/*
**
**	$Filename:  b3List.h $
**	$Release:   Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer: Steffen A. Mork $
**
**	Blizzard III - double linked list routines (proto types)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_EXCEPTION_H
#define B3_BASE_EXCEPTION_H

template <class T> class b3Exception
{
protected:
	T m_ErrorCode;

public:
	b3Exception(T error)
	{
		m_ErrorCode = error;
	}

	T b3GetError()
	{
		return m_ErrorCode;
	}
};

#endif
