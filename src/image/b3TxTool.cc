/*
**
**	$Filename:	b3TxTool.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some big/little endian support
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/01 12:24:59  sm
**	Initial revision
**
**	Revision 1.3  2000/09/21 10:22:27  smork
**	- Setting Blizzard III projects to warning level 3: Found
**	  some uninitialized variables.
**	- changed b3Mutex from CMutex to CRITICAL_SECTION (thread
**	  synchronization only)
**	- introduced b3IPCMutex for process synchronization
**	
**	Revision 1.2  2000/08/14 11:11:23  smork
**	- Inserted change log
**	- Inserted author tag
**	- Cleaned up file header
**	
*/

/*************************************************************************
**                                                                      **
**                        get data types in processor manner.           **
**                                                                      **
*************************************************************************/

b3_u16 GetShort(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

#	if THISPROCESSOR == INTEL
		Value =                (b3_u16)Pointer[1];
		Value = (Value << 8) | (b3_u16)Pointer[0];
#	else 
		Value =                (b3_u16)Pointer[0];
		Value = (Value << 8) | (b3_u16)Pointer[1];
#	endif
	return Value;
}

b3_u32 GetLong (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

#	if THISPROCESSOR == INTEL
		Value =                (b3_u32)Pointer[3];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[0];
#	else 
		Value =                (b3_u32)Pointer[0];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[3];
#	endif
	return Value;
}

/*************************************************************************
**                                                                      **
**                  get values in Motorola(R) order                     **
**                                                                      **
*************************************************************************/

b3_u16 GetMotShort(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	Value =                (b3_u16)Pointer[0];
	Value = (Value << 8) | (b3_u16)Pointer[1];

	return (Value);
}

b3_u32 GetMotLong (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	Value =                (b3_u32)Pointer[0];
	Value = (Value << 8) | (b3_u32)Pointer[1];
	Value = (Value << 8) | (b3_u32)Pointer[2];
	Value = (Value << 8) | (b3_u32)Pointer[3];

	return (Value);
}

/*************************************************************************
**                                                                      **
**                    get values in Intel(R) order                      **
**                                                                      **
*************************************************************************/

b3_u16 GetIntelShort(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	Value =                (b3_u16)Pointer[1];
	Value = (Value << 8) | (b3_u16)Pointer[0];

	return (Value);
}

b3_u32 GetIntelLong (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	Value =                (unsigned long)Pointer[3];
	Value = (Value << 8) | (unsigned long)Pointer[2];
	Value = (Value << 8) | (unsigned long)Pointer[1];
	Value = (Value << 8) | (unsigned long)Pointer[0];

	return (Value);
}

/*************************************************************************
**                                                                      **
**                  Change endian type of data types                    **
**                                                                      **
*************************************************************************/

b3_size ChangeWord (void *Ptr)
{
	b3_u16 *Pointer = (b3_u16 *)Ptr;
	b3_u16	Value;

	Value = Pointer[0];
	Pointer[0] = (b3_u16)(((Value & 0xff00) >> 8) | ((Value & 0xff) << 8));
	return sizeof(b3_u16);
}

b3_size ChangeLong (void *Ptr)
{
	b3_u32 *Pointer = (b3_u32 *)Ptr;
	b3_u32  Value;

	Value = Pointer[0];
	Pointer[0] =
		((Value & 0xff000000) >> 24) |
		((Value & 0x00ff0000) >>  8) |
		((Value & 0x0000ff00) <<  8) |
		((Value & 0x000000ff) << 24);
	return sizeof(b3_u32);
}
