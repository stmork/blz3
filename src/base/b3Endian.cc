/*
**
**	$Filename:	b3Endian.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Endian specific access routines
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

#include "blz3/base/b3Endian.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/10/12 18:43:21  sm
**	- Endian conversion added
**
**	
*/

/*************************************************************************
**                                                                      **
**                        get data types in processor manner.           **
**                                                                      **
*************************************************************************/

b3_u16 b3Endian::b3Get16(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
	{
		Value =                (long)Pointer[1];
		Value = (Value << 8) | (long)Pointer[0];
	}
	else
	{
		Value =                (long)Pointer[0];
		Value = (Value << 8) | (long)Pointer[1];
	}
	return Value;
}

b3_u32 b3Endian::b3Get32 (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
	{
		Value =                (b3_u32)Pointer[3];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[0];
	}
	else 
	{
		Value =                (b3_u32)Pointer[0];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[3];
	}
	return Value;
}

/*************************************************************************
**                                                                      **
**                  get values in Motorola(R) order                     **
**                                                                      **
*************************************************************************/

b3_u16 b3Endian::b3GetMot16(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	Value =                (b3_u16)Pointer[0];
	Value = (Value << 8) | (b3_u16)Pointer[1];

	return Value;
}

b3_u32 b3Endian::b3GetMot32 (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	Value =                (b3_u32)Pointer[0];
	Value = (Value << 8) | (b3_u32)Pointer[1];
	Value = (Value << 8) | (b3_u32)Pointer[2];
	Value = (Value << 8) | (b3_u32)Pointer[3];

	return Value;
}

/*************************************************************************
**                                                                      **
**                    get values in Intel(R) order                      **
**                                                                      **
*************************************************************************/

b3_u16 b3Endian::b3GetIntel16(void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	Value =                (b3_u16)Pointer[1];
	Value = (Value << 8) | (b3_u16)Pointer[0];

	return Value;
}

b3_u32 b3Endian::b3GetIntel32 (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	Value =                (b3_u32)Pointer[3];
	Value = (Value << 8) | (b3_u32)Pointer[2];
	Value = (Value << 8) | (b3_u32)Pointer[1];
	Value = (Value << 8) | (b3_u32)Pointer[0];

	return Value;
}

/*************************************************************************
**                                                                      **
**                  Change endian type of data types                    **
**                                                                      **
*************************************************************************/

b3_size b3Endian::b3ChangeEndian16 (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;

	B3_SWAP(Pointer[0],Pointer[1]);
	return 2;
}

b3_size b3Endian::b3ChangeEndian32 (void *Ptr)
{
	b3_u08 *Pointer = (b3_u08 *)Ptr;

	B3_SWAP(Pointer[0],Pointer[3]);
	B3_SWAP(Pointer[1],Pointer[2]);
	return 4;
}
