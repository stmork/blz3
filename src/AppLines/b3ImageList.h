/*
**
**	$Filename:	b3ImageList.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Image list for div. dialogs
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGELIST_H
#define B3_IMAGELIST_H

#include "blz3/raytrace/b3Raytrace.h"

class CB3ImageList : public CImageList
{
	static int    m_ResIcon[];
	static int    m_ResString[];
	static b3_u32 m_ClassTypes[];

public:
	       void b3Create(b3_res size = 16);
	static long b3ComputeImgNum(b3BBox *BBox);
	static long b3ComputeImgNum(b3Shape *Shape,CString &text);
	static void b3InsertListEntries(CListCtrl *list_ctrl,b3_u32 class_def);
};

#endif
