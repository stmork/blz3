/*
**
**      $Filename:	b3Display.h   $
**      $Release:	Dortmund 2001 $
**      $Revision:	1.07 
**      $Date$
**      $Developer:	Steffen A. Mork $
**      $Id$
**
**      Blizzard III - Display Hardware Routines
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DISPLAY_H
#define B3_SYSTEM_DISPLAY_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "stdafx.h"
#include "blz3/b3Types.h"
#include "blz3/base/b3List.h"
#include "blz3/system/b3Mem.h"

/*************************************************************************
**                                                                      **
**                        typedefs...                                   **
**                                                                      **
*************************************************************************/

typedef long          b3_res;
typedef b3_u32        b3_pkd_color;
typedef long          b3_coord;

typedef enum
{
	B3_DISPLAY_ERROR = -1,
	B3_DISPLAY_OK    =  0,
	B3_DISPLAY_MEMORY,
	B3_DISPLAY_NO_COLORMAP,
	B3_DISPLAY_OPEN
} b3_display_error;

class b3DisplayException
{
protected:
	b3_display_error error;

public:
	b3DisplayException(b3_display_error error)
	{
		this->error = error;
	}

	b3_display_error b3GetError()
	{
		return error;
	}
};

class b3Row;

class b3Display : public b3Mem
{
	b3_res              m_xMax,m_yMax; // This is the max. visible size
	b3_res              m_depth;
	char               *m_Title;

	// Some position values
	b3_res              m_xs,m_ys; // This is the size we really use
	b3_pkd_color       *m_Buffer;

	// Some X values
	b3_bool             m_Opened;
	b3_bool             m_Closed;
	b3Mutex             m_Mutex;

	CScrollView        *m_View;

public:
	                    b3Display(const char *title = null);
	                    b3Display(b3_res xSize,b3_res ySize,const char *title = null);
	                   ~b3Display();
	void                b3GetRes(b3_res &xSize,b3_res &ySize);
	void                b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color pixel);
	b3_pkd_color        b3GetPixel(b3_coord x,b3_coord y);
	void                b3PutRow(b3Row *row);
	b3_bool             b3IsCancelled(b3_coord x,b3_coord y);
	void                b3Wait();

private:
	       void         b3Init();
	       void         b3Open(b3_res xSize,b3_res ySize);
	       void         b3Close();
};

class b3Row : public b3Link<b3Row>, public b3Mem
{
protected:
	b3_coord      y;
	b3_res        xSize;
	b3_pkd_color *buffer;

public:
	b3Row(b3_coord y,b3_res xSize);

	friend void b3Display::b3PutRow(b3Row *row);
};

#endif
