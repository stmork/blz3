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

#include "blz3/image/b3Tx.h"
#include "blz3/base/b3World.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	{ B3_MK_ERRNO(B3_TX_MEMORY,                     'TX'),  IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_WORLD_OPEN,                    'BLZ'), IDS_ERR_OPEN },
	{ B3_MK_ERRNO(B3_WORLD_READ,                    'BLZ'), IDS_ERR_READ },
	{ B3_MK_ERRNO(B3_WORLD_MEMORY,                  'BLZ'), IDS_ERR_MEMORY },
	{ B3_MK_ERRNO(B3_WORLD_PARSE,                   'BLZ'), IDS_ERR_PARSE },
	{ B3_MK_ERRNO(B3_WORLD_WRITE,                   'BLZ'), IDS_ERR_WRITE },
	{ B3_MK_ERRNO(B3_WORLD_STORAGE_NOT_IMPLEMENTED, 'BLZ'), IDS_ERR_STORAGE },
	{ B3_MK_ERRNO(B3_WORLD_OUT_OF_ORDER,            'BLZ'), IDS_ERR_ORDER },
	{ B3_MK_ERRNO(B3_WORLD_CLASSTYPE_UNKNOWN,       'BLZ'), IDS_ERR_CLASSTYPE_UNKNOWN }
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
