/*
**
**	$Filename:	b3Display.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Controlling a color display
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard includes                             **
**                                                                      **
*************************************************************************/

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Display.h"
#include "blz3/base/b3Aux.h"
#include "blz3/image/b3Tx.h"

#define no_SYNC
#define no_DEBUG

#ifdef _DEBUG
#define LOW_RES
#endif

#define READY 128

/*************************************************************************
**                                                                      **
**                        color display routines                        **
**                                                                      **
*************************************************************************/

b3Display::b3Display(b3Tx * tx)
{
	m_xMax   = tx->xSize;
	m_yMax   = tx->ySize;
	m_depth  = tx->depth;
	m_Buffer = tx->b3GetHdrData();
	m_Tx     = tx;
	m_OwnTx  = false;
}

b3Display::b3Display()
{
	b3_coord xSize;
	b3_coord ySize;

#ifdef LOW_RES
	xSize = 240;
	ySize = 180;
#else
	xSize = 768;
	ySize = 576;
#endif
	b3Init(xSize, ySize, "");
}

b3Display::b3Display(const char * title)
{
	b3_coord xSize;
	b3_coord ySize;

#ifdef LOW_RES
	xSize = 240;
	ySize = 180;
#else
	xSize = 768;
	ySize = 576;
#endif
	b3Init(xSize, ySize, title);
}

b3Display::b3Display(const b3_res xSize, const b3_res ySize, const char * title)
{
	b3Init(xSize, ySize, title);
}

void b3Display::b3Init(const b3_res xSize, const b3_res ySize, const char * title)
{
	b3PrintF(B3LOG_FULL, "Opening display \"%s\" of size %u,%u\n",
		title,
		xSize, ySize);

	m_OwnTx = true;
	m_xMax  = xSize;
	m_yMax  = ySize;
	m_depth = 128;
	m_Tx    = new b3Tx();
	m_Tx->b3AllocTx(xSize, ySize, m_depth);
	m_Buffer = m_Tx->b3GetHdrData();
}

b3Display::~b3Display()
{
	b3PrintF(B3LOG_FULL, "Closing display...\n");
	if ((m_OwnTx) && (m_Tx != nullptr))
	{
		delete m_Tx;
	}
}


void b3Display::b3PutRow(const b3Row * row)
{
	b3_coord   y = row->m_y;
	b3_color * src, *dst;

	B3_ASSERT(m_Buffer != nullptr);
	src = row->m_buffer;
	dst = &m_Buffer[y * m_xMax];
	if (src != dst)
	{
		b3ColorMemCopy(dst, src, m_xMax);
	}
}

void b3Display::b3PutTx(b3Tx * tx)
{
	b3_coord y;

	for (y = 0; y < m_yMax; y++)
	{
		tx->b3GetRow(&m_Buffer[y * m_xMax], y);
	}
}

b3_bool b3Display::b3SaveImage(const char * filename, b3TxExif * exif)
{
	return m_Tx->b3SaveImage(filename, exif) == B3_OK;
}
