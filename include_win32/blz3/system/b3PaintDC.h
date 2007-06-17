/*
**
**	$Filename:	b3PaintDC.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1457 $
**	$Date: 2006-08-22 15:30:01 +0200 (Di, 22 Aug 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - A thread safe CPaintDC
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_PAINTDC_H
#define B3_SYSTEM_PAINTDC_H

#pragma once
#include "afxwin.h"

/**
 * This class provides a multi threaded CPaintDC
 */
class B3_PLUGIN CB3PaintDC : public CPaintDC
{
public:
	/**
	 * This constructor computes a new CWnd object from a given
	 * HWND via another CWnd object. This is necessary since a
	 * CWnd object is not valid through the trhreads boundary.
	 *
	 * @param wnd The CWnd object which contains a valid HWND.
	 */
	inline CB3PaintDC(CWnd *wnd) : CPaintDC(CWnd::FromHandle(wnd->m_hWnd))
	{
	}

	virtual ~CB3PaintDC(void)
	{
	}
};

#endif
