/*
**
**	$Filename:	b3Print.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - print control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_PRINT_H
#define B3_SYSTEM_PRINT_H

#include "blz3/b3Config.h"
#include "blz3/system/b3View.h"
#include "blz3/system/b3Exception.h"

enum b3_print_error
{
	B3_PRINT_ERROR = -1,
	B3_PRINT_OK    = 0,
	B3_PRINT_NO_MEMORY
};

typedef b3Exception<b3_print_error,'PRT'> b3PrintException;

class CB3Print
{
	CB3View  *m_Print;
	b3Tx     *m_Tx;
	b3Tx     *m_TxBuffer;
	b3Tx     *m_PrtLine;
	b3_bool   m_NeedGreyScale;

	// size of one strip in original image
	b3_res    m_xSize;
	b3_res    m_ySize;

	// number of pages to print
	b3_count  m_NumPages;

	// number of strips per page
	b3_count  m_StripsPerPage;

	// number of whole strips
	b3_count  m_Strips;

	// height of one strip of print image
	b3_res    m_StripHeight;

	// resolution of print page
	b3_res    m_PrtWidth;
	b3_res    m_PrtHeight;

	// resolution of rescaled strip
	b3_res    m_ScaleWidth;
	b3_res    m_ScaleHeight;

public:
	      CB3Print(b3Tx *,b3_res xSize,b3_res ySize,CDC *pDC,CPrintInfo *pInfo,b3_bool FitOnPage=false);
		  CB3Print(CDC *pDC,CPrintInfo *pInfo,b3_bool FitOnPage = true);
	     ~CB3Print();
	void  b3PrintPage(CDC *pDC,CPrintInfo *pInfo);
	void  b3PrintPage(CDC *srcDC,CDC *pDC,CPrintInfo *pInfo,b3Tx *pTX);
};

#endif
