/*
**
**	$Filename:	b3ShowRaytrace.h $  
**	$Release:	Dortmund 2001 $
**	$Revision: 1341 $
**	$Date: 2006-03-19 15:47:18 +0100 (So, 19 Mrz 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying a raytraced image inside a control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_SHOWRAYTRACE_H
#define B3_SYSTEM_SHOWRAYTRACE_H

#include "blz3/system/b3ShowImage.h"
#include "blz3/system/b3Display.h"
#include "blz3/raytrace/b3Scene.h"

class B3_PLUGIN CB3ShowRaytrace : public CB3ShowImage
{
	DECLARE_DYNAMIC(CB3ShowRaytrace)

    b3Display *m_Display;

public:
	     CB3ShowRaytrace();
	void b3Update(b3Scene *scene);

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CB3ShowRaytrace)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
