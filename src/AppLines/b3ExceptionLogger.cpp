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

#include "b3ExceptionLogger.h"
#include "Resource.h"

#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Print.h"
#include "blz3/system/b3Date.h"
#include "blz3/image/b3Tx.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2003/06/01 12:35:43  sm
**	- added invalid increment exception to b3Array
**
**	Revision 1.8  2003/02/02 14:22:32  sm
**	- Added TGF import facility.
**	
**	Revision 1.7  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.6  2002/08/15 13:56:42  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.5  2002/08/09 14:05:51  sm
**	- Minor bug fixes.
**	
**	Revision 1.4  2002/08/09 13:20:18  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.3  2002/08/08 15:14:22  sm
**	- Some problems concerning b3Mem::b3Realloc fixed.
**	- Further error messages added.
**	
**	Revision 1.2  2002/08/07 17:25:01  sm
**	- Added new error messages
**	- Changed exception handling a little bit
**	
**	Revision 1.1  2002/08/07 14:26:23  sm
**	- Introduced mapping from Blizzard III error codes to human
**	  readable error messages supplied from Windows resources.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3ExceptionLogger implementation             **
**                                                                      **
*************************************************************************/

static CString                        LocalMessageString;
static b3HashMap<b3_errno,UINT>       LocalMessageMap;
static b3HashContainer<b3_errno,UINT> LocalMessages[] =
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
	{ B3_MK_ERRNO(B3_WORLD_STORAGE_NOT_IMPLEMENTED, 'BLZ'), IDS_ERR_STORAGE },
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
	LocalMessageMap.b3Init(LocalMessages,sizeof(LocalMessages) / sizeof(b3HashContainer<b3_errno,UINT>));
	b3ExceptionBase::b3SetMsgFunc(&b3GetMessage);
}

const char *CB3ExceptionLogger::b3GetMessage(const b3_errno ErrNo)
{
	UINT MsgID = LocalMessageMap.b3Find(ErrNo);

	if (MsgID != 0)
	{
		LocalMessageString.LoadString(MsgID);
	}
	else
	{
		LocalMessageString.Format(IDS_ERR_UNKNOWN,ErrNo);
	}
	return LocalMessageString;
}
