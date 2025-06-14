/*
**
**	$Filename:	b3Random.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Some fast pseudo random numbers.
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_BASE_RANDOM_H
#define B3_BASE_RANDOM_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        Some head/node defines                        **
**                                                                      **
*************************************************************************/

// TODO: Reimplement as a class.
template<typename T> inline T b3SimpleRandom(const T x)
{
	return T(x * drand48());
}

#define B3_IRAN(x) b3SimpleRandom<long>(x)
#define B3_FRAN(x) b3SimpleRandom<double>(x)

template<class T>class B3_PLUGIN b3Random
{
protected:
	static constexpr int B3_RANDOM_DEFAULT_SEED = 0x4711;

public:
	constexpr b3Random<T>() = default;

	virtual T b3Rand() = 0;
	virtual T b3Rand(const T mult) = 0;
};

template<class T>class B3_PLUGIN b3PseudoRandom : public b3Random<T>
{
	long int m_Seed;

public:
	explicit inline b3PseudoRandom<T>(const int seed = b3Random<T>::B3_RANDOM_DEFAULT_SEED) :
		b3Random<T>()
	{
		m_Seed = seed;
	}

	inline void b3SetSeed(const int seed = b3Random<T>::B3_RANDOM_DEFAULT_SEED)
	{
		m_Seed = seed;
	}

	virtual T b3Rand() override
	{
		m_Seed = (m_Seed * 7141 + 54773)  % 259200;

		return static_cast<T>(m_Seed) / 259200;
	}

	virtual T b3Rand(const T mult) override
	{
		m_Seed = (m_Seed * 7141 + 54773)  % 259200;

		return static_cast<T>(m_Seed) * mult / 259200;
	}
};

//#define __rand_C 16807
//#define __rand_A 2147483647.0

template<class T> class B3_PLUGIN b3Rand48 : public b3Random<T>
{
	static constexpr b3_u32 __rand_C =      16807;
	static constexpr b3_f64 __rand_A = 2147483647.0;

	b3_f64 m_Seed;

public:
	explicit constexpr b3Rand48<T>(const int seed = b3Random<T>::B3_RANDOM_DEFAULT_SEED) :
		b3Random<T>()
	{
		unsigned short conv = seed & 0xffff;

		b3Seed48(&conv);
	}

	constexpr void b3SetSeed(const int seed = b3Random<T>::B3_RANDOM_DEFAULT_SEED)
	{
		unsigned short conv = seed & 0xffff;

		b3Seed48(&conv);
	}

	virtual T b3Rand() override
	{
		return static_cast<T>(b3DRand48());
	}

	virtual T b3Rand(const T mult) override
	{
		return static_cast<T>(b3DRand48() * mult);
	}

private:
	constexpr b3_f64 b3DRand48()
	{
		const b3_u32 ki = static_cast<b3_u32>((__rand_C * m_Seed) / __rand_A);

		m_Seed = __rand_C * m_Seed - ki * __rand_A;
		return m_Seed / (__rand_A - 1);
	}

	constexpr void b3Seed48(const unsigned short * seed)
	{
		B3_ASSERT(*seed != 0);
		m_Seed = *seed;
	}
};

#endif
