/*
**
**	$Filename:	b3ExceptionLogger.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting exception error codes into human readable messages
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"
#include "b3ExceptionLogger.h"

#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Print.h"
#include "blz3/system/b3Date.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        CB3ExceptionLogger implementation             **
**                                                                      **
*************************************************************************/

CString                        CB3ExceptionLogger::m_LocalMessageString;
b3HashMap<b3_errno,UINT>       CB3ExceptionLogger::m_LocalMessageMap;
b3HashContainer<b3_errno,UINT> CB3ExceptionLogger::m_LocalMessages[] =
{
	{ B3_MK_ERRNO(B3_MEM_MEMORY,                    'MEM'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_DISPLAY_MEMORY,                'DSP'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_NOISE_MEMORY,                  'NOI'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_DISPLAY_NO_COLORMAP,           'DSP'), IDS_ERR_NO_COLORMAP },
	{ B3_MK_ERRNO(B3_DISPLAY_OPEN,                  'DSP'), IDS_PRGERR_DISPLAY_OPEN },
	{ B3_MK_ERRNO(B3_WORLD_OPEN,                    'BLZ'), IDS_ERR_OPEN },
	{ B3_MK_ERRNO(B3_WORLD_READ,                    'BLZ'), IDS_ERR_READ },
	{ B3_MK_ERRNO(B3_WORLD_MEMORY,                  'BLZ'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_WORLD_PARSE,                   'BLZ'), IDS_ERR_PARSE },
	{ B3_MK_ERRNO(B3_WORLD_WRITE,                   'BLZ'), IDS_ERR_WRITE },
	{ B3_MK_ERRNO(B3_WORLD_NOT_REGISTERED,          'BLZ'), IDS_ERR_STORAGE },
	{ B3_MK_ERRNO(B3_WORLD_OUT_OF_ORDER,            'BLZ'), IDS_ERR_ORDER },
	{ B3_MK_ERRNO(B3_WORLD_CLASSTYPE_UNKNOWN,       'BLZ'), IDS_ERR_CLASSTYPE_UNKNOWN },
	{ B3_MK_ERRNO(B3_WORLD_IMPORT,                  'BLZ'), IDS_ERR_IMPORT },
	{ B3_MK_ERRNO(B3_TX_MEMORY,                      'TX'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_TX_NOT_FOUND,					 'TX'), IDS_ERR_NOT_FOUND },
	{ B3_MK_ERRNO(B3_TX_NOT_SAVED,					 'TX'), IDS_ERR_NOT_SAVED },
	{ B3_MK_ERRNO(B3_TX_UNSUPP,						 'TX'), IDS_ERR_UNPACK },
	{ B3_MK_ERRNO(B3_TX_ERR_PACKING,				 'TX'), IDS_ERR_PACKING },
	{ B3_MK_ERRNO(B3_TX_UNCOMPL,					 'TX'), IDS_ERR_UNCOMPL },
	{ B3_MK_ERRNO(B3_TX_ERR_HEADER,					 'TX'), IDS_ERR_HEADER },
	{ B3_MK_ERRNO(B3_TX_UNKNOWN_FILETYPE,			 'TX'), IDS_ERR_FILETYPE },
	{ B3_MK_ERRNO(B3_TX_UNKNOWN_DATATYPE,			 'TX'), IDS_PRGERR_DATATYPE },
	{ B3_MK_ERRNO(B3_TX_ILLEGAL_DATATYPE,			 'TX'), IDS_ERR_COLORDEPTH },
	{ B3_MK_ERRNO(B3_PRINT_NO_MEMORY,               'PRT'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_BSPLINE_TOO_MUCH_CONTROLS,		'SPL'), IDS_PRGERR_BSPLINE_TOO_MUCH_CONTROLS },
	{ B3_MK_ERRNO(B3_BSPLINE_TOO_FEW_CONTROLS,		'SPL'), IDS_PRGERR_BSPLINE_TOO_FEW_CONTROLS },
	{ B3_MK_ERRNO(B3_BSPLINE_TOO_FEW_MAXKNOTS,		'SPL'), IDS_PRGERR_BSPLINE_TOO_FEW_MAXKNOTS },
	{ B3_MK_ERRNO(B3_BSPLINE_TOO_FEW_MAXCONTROLS,	'SPL'), IDS_PRGERR_BSPLINE_TOO_FEW_MAXCONTROLS },
	{ B3_MK_ERRNO(B3_BSPLINE_MISSING_KNOTS,         'SPL'), IDS_PRGERR_BSPLINE_MISSING_KNOTS },
	{ B3_MK_ERRNO(B3_BSPLINE_CLOSED,                'SPL'), IDS_PRGERR_BSPLINE_CLOSED },
	{ B3_MK_ERRNO(B3_BSPLINE_TOO_LOW_MULTIPLICATION,'SPL'), IDS_PRGERR_BSPLINE_TOO_LOW_MULTIPLICATION },
	{ B3_MK_ERRNO(B3_ARRAY_OUT_OF_BOUNDS,           'ARR'),	IDS_PRGERR_OUT_OF_BOUNDS },
	{ B3_MK_ERRNO(B3_ARRAY_INVALID_INCREMENT,       'ARR'),	IDS_PRGERR_INVALID_INCREMENT },
	{ B3_MK_ERRNO(B3_HASH_DUPLICATE_KEY,            'HSH'), IDS_PRGERR_HASH_DUPLICATE_KEY },
	{ B3_MK_ERRNO(B3_HASH_INVALID,				    'HSH'), IDS_PRGERR_HASH_INVALID },
	{ B3_MK_ERRNO(B3_FILE_NOT_FOUND,                'FIL'), IDS_ERR_FILE_NOT_FOUND },
	{ B3_MK_ERRNO(B3_FILE_NOT_READ,                 'FIL'), IDS_ERR_FILE_NOT_READ },
	{ B3_MK_ERRNO(B3_FILE_NOT_WRITTEN,              'FIL'), IDS_ERR_FILE_NOT_WRITTEN },
	{ B3_MK_ERRNO(B3_FILE_MEMORY,                   'FIL'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_FILE_NOT_OPEN,                 'FIL'), IDS_PRGERR_FILE_NOT_OPEN },
	{ B3_MK_ERRNO(B3_DIR_NOT_FOUND,                 'DIR'), IDS_ERR_DIR_NOT_FOUND },
	{ B3_MK_ERRNO(B3_DATE_ILLEGAL,                  'DAT'), IDS_PRGERR_ILLEGAL_DATE },
	{ B3_MK_ERRNO(B3_DATE_ILLEGAL_MODE,             'DAT'), IDS_PRGERR_ILLEGAL_DATEMODE }
};

CB3ExceptionLogger::CB3ExceptionLogger()
{
	m_LocalMessageMap.b3Init(m_LocalMessages,sizeof(m_LocalMessages) / sizeof(b3HashContainer<b3_errno,UINT>));
	b3ExceptionBase::b3SetMsgFunc(&b3GetMessage);
}

const char *CB3ExceptionLogger::b3GetMessage(const b3_errno ErrNo)
{
	UINT *MsgID = m_LocalMessageMap.b3Find(ErrNo);

	if (MsgID != null)
	{
		m_LocalMessageString.LoadString(*MsgID);
	}
	else
	{
		m_LocalMessageString.Format(IDS_ERR_UNKNOWN,ErrNo);
	}
	return m_LocalMessageString;
}
