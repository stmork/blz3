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

#define B3_RANDOM_DEFAULT_SEED 0x4711

template<class T>class B3_PLUGIN b3Random
{
public:
	b3Random<T>(){}
	virtual T b3Rand() = 0;
	virtual T b3Rand(const T mult) = 0;
};

template<class T>class B3_PLUGIN b3PseudoRandom : public b3Random<T>
{
	long int m_Seed;

public:
	inline b3PseudoRandom<T>(const int seed = B3_RANDOM_DEFAULT_SEED) : b3Random<T>()
	{
		m_Seed = seed;
	}

	inline void b3SetSeed(const int seed = B3_RANDOM_DEFAULT_SEED)
	{
		m_Seed = seed;
	}

	inline virtual T b3Rand()
	{
		m_Seed = (m_Seed * 7141 + 54773)  % 259200;
		
		return static_cast<T>(m_Seed) / 259200;
	}

	inline virtual T b3Rand(const T mult)
	{
		m_Seed = (m_Seed * 7141 + 54773)  % 259200;
		
		return static_cast<T>(m_Seed) * mult / 259200;
	}
};

template<class T> class B3_PLUGIN b3Rand48 : public b3Random<T>
{
public:
	inline b3Rand48<T>(const int seed = B3_RANDOM_DEFAULT_SEED) : b3Random<T>()
	{
		unsigned short conv = seed & 0xffff;

		seed48(&conv);
	}

	inline void b3SetSeed(const int seed = B3_RANDOM_DEFAULT_SEED)
	{
		unsigned short conv = seed & 0xffff;

		seed48(&conv);
	}

	inline virtual T b3Rand()
	{
		return static_cast<T>(drand48());
	}

	inline virtual T b3Rand(const T mult)
	{
		return static_cast<T>(drand48() * mult);
	}
};

#endif
