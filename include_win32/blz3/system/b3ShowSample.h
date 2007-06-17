/*
**
**	$Filename:	b3ShowSample.h $  
**	$Release:	Dortmund 2004 $
**	$Revision: 1341 $
**	$Date: 2006-03-19 15:47:18 +0100 (So, 19 Mrz 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying a sampled image inside a control
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_SHOWSAMPLE_H
#define B3_SYSTEM_SHOWSAMPLE_H

#include "blz3/system/b3ShowImage.h"
#include "blz3/system/b3Display.h"
#include "blz3/image/b3Sampler.h"

class B3_PLUGIN CB3ShowSample : public CB3ShowImage
{
	DECLARE_DYNAMIC(CB3ShowSample)

public:
	     CB3ShowSample();
	void b3Init();
	void b3Update(b3Sampler *sampler);

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CB3ShowSample)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
