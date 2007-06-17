/*
**
**	$Filename:	b3View.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - viewing routines (proto types)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_VIEW_H
#define B3_SYSTEM_VIEW_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Bitmap.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III viewing base                     **
**                        class (for CB3ScrollView)                     **
**                                                                      **
*************************************************************************/

class CB3View : public CB3BitmapDDB
{
	// resulution of view
	b3_res        xPos;
	b3_res        yPos;
	b3_f64        m_xScale,m_yScale;

public:
	              CB3View();
	             ~CB3View();
	void          b3UpdateView  (b3Tx *image,CScrollView *pView,b3_coord yStart = 0,b3_coord yEnd = 0);
	void          b3DrawView    (CDC *srcDC,b3_coord xOrig = 0,b3_coord yOrig = 0);
	void          b3SetPos      (b3_coord xPos,b3_coord yPos);
	b3_count      b3Draw    (CDC *dstDC,
		b3_res   xDstSize = 0,b3_res   yDstSize = 0,
		b3_coord xOrig    = 0,b3_coord yOrig    = 0);
	b3_count      b3Print   (CDC *displayDC,CDC *printDC,
		b3_res xDstSize = 0,b3_res yDstSize = 0);
};

#endif // B3VIEW_H
