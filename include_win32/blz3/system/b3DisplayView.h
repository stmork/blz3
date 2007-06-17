/*
**
**	$Filename:	b3DisplayView.h   $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision:	1.07 
**	$Date: 2006-06-28 19:48:22 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Display Hardware Routines
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DISPLAYVIEW_H
#define B3_SYSTEM_DISPLAYVIEW_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3ScrollView.h"

/*************************************************************************
**                                                                      **
**                        typedefs...                                   **
**                                                                      **
*************************************************************************/

#define B3_DISPLAY_ROWREFRESHCOUNT 32

class b3DisplayView : public b3Display
{
	char               *m_Title;

	// Some X values
	b3_count            m_RowCounter;
	CB3ScrollView      *m_View;
	b3Document         *m_Doc;
	b3_count            m_RowRefreshCount;

public:
	                    b3DisplayView(CB3ScrollView *view,const char *title = null);
	                    b3DisplayView(CB3ScrollView *view,const b3_res xSize, const b3_res ySize,const char *title = null);
	                   ~b3DisplayView();
	void                b3PutRow(const b3Row *row);
	void                b3PutTx(b3Tx *tx);
	void                b3Wait();
	void                b3SetRowRefreshCount(b3_count refresh_count = B3_DISPLAY_ROWREFRESHCOUNT);

private:
	       void         b3Open(CB3ScrollView *view, const b3_res xSize, const b3_res ySize);
};

#endif
