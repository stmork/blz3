/*
**
**	$Filename:	b3CompareTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base and system unit tests
**
**      (C) Copyright 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "b3CompareTest.h"
#include <blz3/base/b3Array.h>

/*************************************************************************
**                                                                      **
**                        Array unit test                               **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3CompareTest);

#endif

class b3TestCmp : public b3Comparator<b3TestCmp>
{
	int m_Value;

public:
	b3TestCmp() = delete;

	explicit inline b3TestCmp(int value)
	{
		m_Value = value;
	}

	inline int b3Cmp(const b3TestCmp * compare) const override
	{
		return m_Value - compare->m_Value;
	}
};

void b3CompareTest::test()
{
	const b3TestCmp l(1);
	const b3TestCmp m(2);
	const b3TestCmp r(3);
	const b3TestCmp q(r);

	CPPUNIT_ASSERT(l < m);
	CPPUNIT_ASSERT(l < r);
	CPPUNIT_ASSERT(m < r);

	CPPUNIT_ASSERT(b3TestCmp::b3Sort(&l, &r) < 0);
	CPPUNIT_ASSERT(b3TestCmp::b3Sort(&r, &l) > 0);
	CPPUNIT_ASSERT(b3TestCmp::b3Sort(&r, &q) == 0);
}
