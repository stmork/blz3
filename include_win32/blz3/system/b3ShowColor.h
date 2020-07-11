/*
**
**	$Filename:	b3ShowColor.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying an HSV color cycle
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_SHOWCOLOR_H
#define B3_SYSTEM_SHOWCOLOR_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Bitmap.h"

class CB3ShowColor : public CStatic
{
	CB3BitmapDDB  m_DDB;
	b3_res        m_xSize;
	b3_res        m_ySize;

public:
	b3_f64        h, s, v;
	b3_f64        niveau;
	CB3ShowColor();
	b3_bool       b3Update();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ShowColor)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
