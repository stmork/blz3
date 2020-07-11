/*
**
**	$Filename:	b3Endian.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3BaseInclude.h"
#include "blz3/base/b3Endian.h"

union b3_change_buffer
{
	b3_u08   m_iBuffer[8];
	b3_u16   m_sBuffer[4];
	b3_u32   m_lBuffer[2];
	b3_f32   m_fBuffer[2];
	b3_f64   m_dBuffer[1];
};

/*************************************************************************
**                                                                      **
**                        b3EndianChanger class                         **
**                                                                      **
*************************************************************************/

class b3EndianChanger
{
	b3_change_buffer m_Changer;

public:
	b3EndianChanger(void * ptr, b3_size size)
	{
		b3_u08 * cPtr = (b3_u08 *)ptr;

		for (b3_size i = 0; i < size; i++)
		{
			m_Changer.m_iBuffer[i] = *cPtr++;
		}
	}

	inline void b3Change16()
	{
		B3_SWAP(m_Changer.m_iBuffer[0], m_Changer.m_iBuffer[1]);
	}

	inline void b3Change32()
	{
		B3_SWAP(m_Changer.m_iBuffer[0], m_Changer.m_iBuffer[3]);
		B3_SWAP(m_Changer.m_iBuffer[1], m_Changer.m_iBuffer[2]);
	}

	inline void b3Change64()
	{
		B3_SWAP(m_Changer.m_iBuffer[0], m_Changer.m_iBuffer[3]);
		B3_SWAP(m_Changer.m_iBuffer[1], m_Changer.m_iBuffer[2]);
		B3_SWAP(m_Changer.m_iBuffer[4], m_Changer.m_iBuffer[7]);
		B3_SWAP(m_Changer.m_iBuffer[5], m_Changer.m_iBuffer[6]);
	}

	inline const b3_bool b3IsIntel()
	{
		return b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN;
	}

	inline const b3_bool b3IsMotorola()
	{
		return b3Runtime::b3GetCPUType() == B3_BIG_ENDIAN;
	}

	inline const b3_u16 b3GetU16()
	{
		return m_Changer.m_sBuffer[0];
	}

	inline const b3_u32 b3GetU32()
	{
		return m_Changer.m_lBuffer[0];
	}

	inline const b3_f32 b3GetF32()
	{
		return m_Changer.m_fBuffer[0];
	}

	inline const b3_f64 b3GetF64()
	{
		return m_Changer.m_dBuffer[0];
	}
};

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 21:22:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2003/02/02 14:22:32  sm
**	- Added TGF import facility.
**
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

const b3_u16 b3Endian::b3Get16(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;
	b3_u16  Value;

	if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
	{
		Value = (long)Pointer[1];
		Value = (Value << 8) | (long)Pointer[0];
	}
	else
	{
		Value = (long)Pointer[0];
		Value = (Value << 8) | (long)Pointer[1];
	}
	return Value;
}

const b3_u32 b3Endian::b3Get32(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;
	b3_u32  Value;

	if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
	{
		Value = (b3_u32)Pointer[3];
		Value = (Value << 8) | (b3_u32)Pointer[2];
		Value = (Value << 8) | (b3_u32)Pointer[1];
		Value = (Value << 8) | (b3_u32)Pointer[0];
	}
	else
	{
		Value = (b3_u32)Pointer[0];
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

const b3_u16 b3Endian::b3GetMot16(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u16));

	if (changer.b3IsIntel())
	{
		changer.b3Change16();
	}
	return changer.b3GetU16();
}

const b3_u32 b3Endian::b3GetMot32(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u32));

	if (changer.b3IsIntel())
	{
		changer.b3Change32();
	}
	return changer.b3GetU32();
}

const b3_f32 b3Endian::b3GetMotFloat(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_f32));

	if (changer.b3IsIntel())
	{
		changer.b3Change32();
	}
	return changer.b3GetF32();
}

const b3_f64 b3Endian::b3GetMotDouble(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_f64));

	if (changer.b3IsIntel())
	{
		changer.b3Change64();
	}
	return changer.b3GetF64();
}

/*************************************************************************
**                                                                      **
**                    get values in Intel(R) order                      **
**                                                                      **
*************************************************************************/

const b3_u16 b3Endian::b3GetIntel16(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u16));

	if (changer.b3IsMotorola())
	{
		changer.b3Change16();
	}
	return changer.b3GetU16();
}

const b3_u32 b3Endian::b3GetIntel32(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u32));

	if (changer.b3IsMotorola())
	{
		changer.b3Change32();
	}
	return changer.b3GetU32();
}

const b3_f32 b3Endian::b3GetIntelFloat(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_f32));

	if (changer.b3IsMotorola())
	{
		changer.b3Change32();
	}
	return changer.b3GetF32();
}

const b3_f64 b3Endian::b3GetIntelDouble(void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_f64));

	if (changer.b3IsMotorola())
	{
		changer.b3Change64();
	}
	return changer.b3GetF64();
}

/*************************************************************************
**                                                                      **
**                  Change endian type of data types                    **
**                                                                      **
*************************************************************************/

const b3_size b3Endian::b3ChangeEndian16(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;

	B3_SWAP(Pointer[0], Pointer[1]);
	return 2;
}

const b3_size b3Endian::b3ChangeEndian32(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;

	B3_SWAP(Pointer[0], Pointer[3]);
	B3_SWAP(Pointer[1], Pointer[2]);
	return 4;
}

const b3_size b3Endian::b3ChangeEndian64(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;

	b3ChangeEndian32(&Pointer[0]);
	b3ChangeEndian32(&Pointer[4]);
	return 8;
}
