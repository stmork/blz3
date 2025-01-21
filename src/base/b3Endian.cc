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

#include "blz3/system/b3Runtime.h"
#include "blz3/base/b3Endian.h"

/*************************************************************************
**                                                                      **
**                        b3EndianChanger class                         **
**                                                                      **
*************************************************************************/

class b3EndianChanger
{
	union b3_change_buffer
	{
		b3_u08   m_iBuffer[8];
		b3_u16   m_sBuffer[4];
		b3_u32   m_lBuffer[2];
		b3_f32   m_fBuffer[2];
		b3_f64   m_dBuffer[1];
	}  m_Changer;

public:
	b3EndianChanger(const void * ptr, b3_size size)
	{
		const b3_u08 * cPtr = (const b3_u08 *)ptr;

		for (b3_size i = 0; i < size; i++)
		{
			m_Changer.m_iBuffer[i] = *cPtr++;
		}
	}

	inline void b3Change16()
	{
		std::swap(m_Changer.m_iBuffer[0], m_Changer.m_iBuffer[1]);
	}

	inline void b3Change32()
	{
		std::swap(m_Changer.m_iBuffer[0], m_Changer.m_iBuffer[3]);
		std::swap(m_Changer.m_iBuffer[1], m_Changer.m_iBuffer[2]);
	}

	inline void b3Change64()
	{
		std::swap(m_Changer.m_iBuffer[0], m_Changer.m_iBuffer[3]);
		std::swap(m_Changer.m_iBuffer[1], m_Changer.m_iBuffer[2]);
		std::swap(m_Changer.m_iBuffer[4], m_Changer.m_iBuffer[7]);
		std::swap(m_Changer.m_iBuffer[5], m_Changer.m_iBuffer[6]);
	}

	inline static constexpr b3_bool b3IsIntel()
	{
		return b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN;
	}

	inline static constexpr b3_bool b3IsMotorola()
	{
		return b3Runtime::b3GetCPUType() == B3_BIG_ENDIAN;
	}

	inline b3_u16 b3GetU16() const
	{
		return m_Changer.m_sBuffer[0];
	}

	inline b3_u32 b3GetU32() const
	{
		return m_Changer.m_lBuffer[0];
	}

	inline b3_f32 b3GetF32() const
	{
		return m_Changer.m_fBuffer[0];
	}

	inline b3_f64 b3GetF64() const
	{
		return m_Changer.m_dBuffer[0];
	}
};

/*************************************************************************
**                                                                      **
**                  get values in Motorola(R) order                     **
**                                                                      **
*************************************************************************/

b3_u16 b3Endian::b3GetMot16(const void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u16));

	if (changer.b3IsIntel())
	{
		changer.b3Change16();
	}
	return changer.b3GetU16();
}

b3_u32 b3Endian::b3GetMot32(const void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u32));

	if (changer.b3IsIntel())
	{
		changer.b3Change32();
	}
	return changer.b3GetU32();
}

b3_f32 b3Endian::b3GetMotFloat(const void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_f32));

	if (changer.b3IsIntel())
	{
		changer.b3Change32();
	}
	return changer.b3GetF32();
}

b3_f64 b3Endian::b3GetMotDouble(const void * ptr)
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

b3_u16 b3Endian::b3GetIntel16(const void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u16));

	if (changer.b3IsMotorola())
	{
		changer.b3Change16();
	}
	return changer.b3GetU16();
}

b3_u32 b3Endian::b3GetIntel32(const void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_u32));

	if (changer.b3IsMotorola())
	{
		changer.b3Change32();
	}
	return changer.b3GetU32();
}

b3_f32 b3Endian::b3GetIntelFloat(const void * ptr)
{
	b3EndianChanger changer(ptr, sizeof(b3_f32));

	if (changer.b3IsMotorola())
	{
		changer.b3Change32();
	}
	return changer.b3GetF32();
}

b3_f64 b3Endian::b3GetIntelDouble(const void * ptr)
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

b3_size b3Endian::b3ChangeEndian16(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;

	std::swap(Pointer[0], Pointer[1]);
	return sizeof(b3_u16);
}

b3_size b3Endian::b3ChangeEndian32(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;

	std::swap(Pointer[0], Pointer[3]);
	std::swap(Pointer[1], Pointer[2]);
	return sizeof(b3_u32);
}

b3_size b3Endian::b3ChangeEndian64(void * Ptr)
{
	b3_u08 * Pointer = (b3_u08 *)Ptr;

	b3ChangeEndian32(&Pointer[0]);
	b3ChangeEndian32(&Pointer[4]);
	return sizeof(b3_u64);
}
