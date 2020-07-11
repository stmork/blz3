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

#include "blz3/base/b3World.h"
#include "b3ShapeDialog.h"

class CB3ImageList : public CImageList
{
	static int             m_ResIcon[];
	static int             m_ResString[];
	static b3_u32          m_ClassTypes[];
	static b3ItemEditCall  m_EditCalls[];

public:
	void            b3Create(b3_res size = 16);
	static long            b3ComputeImgNum(b3Item * item);
	static long            b3ComputeText(b3Item * item, CString & text);
	static b3ItemEditCall  b3GetEditCall(b3Item * item);
	static void            b3InsertListEntries(CListCtrl * list_ctrl, b3_u32 class_def, b3_u32 select = 0);
};

#endif
