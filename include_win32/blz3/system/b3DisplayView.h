/*
**
**      $Filename:	b3DisplayView.h   $
**      $Release:	Dortmund 2001, 2002 $
**      $Revision:	1.07 
**      $Date$
**      $Developer:	Steffen A. Mork $
**      $Id$
**
**      Blizzard III - Display Hardware Routines
**
**      (C) Copyright 2001, 2002  Steffen A. Mork
**          All Rights Reserved
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

class b3DisplayView : public b3Display
{
	char               *m_Title;

	// Some X values
	b3_count            m_RowCounter;
	CB3ScrollView      *m_View;
	b3Document         *m_Doc;

public:
	                    b3DisplayView(CB3ScrollView *view,const char *title = null);
	                    b3DisplayView(CB3ScrollView *view,b3_res xSize,b3_res ySize,const char *title = null);
	                   ~b3DisplayView();
	void                b3PutRow(b3Row *row);
	void                b3PutTx(b3Tx *tx);
	void                b3Wait();

private:
	       void         b3Open(CB3ScrollView *view,b3_res xSize,b3_res ySize);
};

#endif
