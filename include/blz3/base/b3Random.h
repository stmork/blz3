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
	int m_Seed;

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
		
		return (T)m_Seed / 259200.0;
	}

	inline virtual T b3Rand(const T mult)
	{
		m_Seed = (m_Seed * 7141 + 54773)  % 259200;
		
		return (T)m_Seed * mult / 259200.0;
	}
};

#endif
