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

#ifndef B3_BASE_DISPLAY_H
#define B3_BASE_DISPLAY_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Mem.h"
#include "blz3/base/b3List.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        typedefs...                                   **
**                                                                      **
*************************************************************************/

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
protected:
	b3_res                m_xs,m_ys;
	b3_res                m_depth;
	b3_pkd_color         *m_Buffer;
	b3Mutex               m_Mutex;
	b3Tx                 *m_Tx;

public:
						  b3Display();
	                      b3Display(const char *title);
						  b3Display(b3Tx *image);
	                      b3Display(b3_res xSize,b3_res ySize,const char *title = null);
	virtual              ~b3Display();
	virtual void          b3GetRes(b3_res &xSize,b3_res &ySize);
	virtual void          b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color pixel);
	virtual b3_pkd_color  b3GetPixel(b3_coord x,b3_coord y);
	virtual void          b3PutRow(b3Row *row);
	virtual b3_bool       b3IsCancelled(b3_coord x,b3_coord y);
	virtual void          b3Wait();

private:
	        void          b3Init(b3_res xSize,b3_res ySize,const char *title);
};

class b3Row : public b3Link<b3Row>, public b3Mem
{
protected:
	b3_res        m_xSize;

public:
	b3_coord      m_y;
	b3_pkd_color *m_buffer;

public:
	         b3Row(b3_coord y,b3_res xSize);
	         b3Row(b3_coord y,b3_res xSize,b3_pkd_color *buffer);
	virtual ~b3Row() {}
};

#endif
