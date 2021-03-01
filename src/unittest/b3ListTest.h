/*
**
**	$Filename:	b3ListTest.h $
**	$Release:	Dortmund 2006, 2016 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for doubly linked lists.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_LIST_TEST_H
#define B3_LIST_TEST_H

#include "blz3/base/b3List.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

#define TEST_CLASS      (static_cast<b3_u32>(0x47110000))
#define TEST_TYPE       (static_cast<b3_u32>(0x00000815))
#define TEST_CLASS_TYPE (TEST_CLASS | TEST_TYPE)

class b3TestElement : public b3Link<b3TestElement>
{
	int element;

public:
	b3TestElement(int element = 0) : b3Link<b3TestElement>(sizeof(b3TestElement), TEST_CLASS_TYPE)
	{
		this->element = element;
	}

	inline void operator=(const int & value)
	{
		this->element = value;
	}

	inline int b3GetElement() const
	{
		return this->element;
	}
};

template<typename T> class b3ListTest : public CppUnit::TestFixture
{
	b3Base<T> base;
	b3Base<T> src;
	b3Base<T> dst;
	T         a, b, c, d, e, f;

	CPPUNIT_TEST_SUITE(b3ListTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
		a = 1;
		b = 2;
		c = 3;
		d = 4;
		e = 5;
	}

	void tearDown()
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void test()
	{
		CPPUNIT_ASSERT_NO_THROW(base.b3InitBase(TEST_CLASS_TYPE));
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), base.First);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), base.Last);
		CPPUNIT_ASSERT_EQUAL(TEST_CLASS_TYPE, base.b3GetClass());
		CPPUNIT_ASSERT(base.b3IsEmpty());

		CPPUNIT_ASSERT_NO_THROW(src.b3InitBase(TEST_CLASS_TYPE));
		CPPUNIT_ASSERT_NO_THROW(src.b3Append(&a));
		CPPUNIT_ASSERT_NO_THROW(src.b3Append(&b));
		CPPUNIT_ASSERT_NO_THROW(src.b3Append(&c));
		CPPUNIT_ASSERT(!src.b3IsEmpty());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(3), src.b3GetCount());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(0), dst.b3GetCount());

		CPPUNIT_ASSERT_EQUAL(
			static_cast<b3_link_state>(
				B3_NODE_NOT_NULL | B3_NODE_FIRST | B3_NODE_NOT_LAST | B3_NODE_NOT_PREV | B3_NODE_SUCC),
			src.b3State(&a));
		CPPUNIT_ASSERT_EQUAL(
			static_cast<b3_link_state>(
				B3_NODE_NOT_NULL | B3_NODE_NOT_FIRST | B3_NODE_NOT_LAST | B3_NODE_PREV | B3_NODE_SUCC),
			src.b3State(&b));
		CPPUNIT_ASSERT_EQUAL(
			static_cast<b3_link_state>(
				B3_NODE_NOT_NULL | B3_NODE_NOT_FIRST | B3_NODE_LAST | B3_NODE_PREV | B3_NODE_NOT_SUCC),
			src.b3State(&c));
		CPPUNIT_ASSERT_EQUAL(B3_NODE_NULL, src.b3State(nullptr));

		CPPUNIT_ASSERT_NO_THROW(dst.b3InitBase(TEST_CLASS_TYPE));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Append(&d));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Move(&src));
		CPPUNIT_ASSERT(src.b3IsEmpty());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(0), src.b3GetCount());
		CPPUNIT_ASSERT(!dst.b3IsEmpty());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(4), dst.b3GetCount());

		CPPUNIT_ASSERT_EQUAL(TEST_CLASS,      a.b3GetClass());
		CPPUNIT_ASSERT_EQUAL(TEST_TYPE,       a.b3GetType());
		CPPUNIT_ASSERT_EQUAL(TEST_CLASS_TYPE, a.b3GetClassType());

		CPPUNIT_ASSERT_NO_THROW(dst.b3RemoveAll());
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Succ);

		CPPUNIT_ASSERT_NO_THROW(src.b3First(&a)); // a
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Succ);
		CPPUNIT_ASSERT_EQUAL(&a, src.First);
		CPPUNIT_ASSERT_EQUAL(&a, src.Last);

		CPPUNIT_ASSERT_NO_THROW(src.b3Append(&b)); // a b
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Prev);
		CPPUNIT_ASSERT_EQUAL(&b,                        a.Succ);
		CPPUNIT_ASSERT_EQUAL(&a,                        b.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Succ);
		CPPUNIT_ASSERT_EQUAL(&a,                        src.First);
		CPPUNIT_ASSERT_EQUAL(&b,                        src.Last);

		CPPUNIT_ASSERT_NO_THROW(src.b3Insert(nullptr, &c)); // c a b
		CPPUNIT_ASSERT_EQUAL(&c,                        a.Prev);
		CPPUNIT_ASSERT_EQUAL(&b,                        a.Succ);
		CPPUNIT_ASSERT_EQUAL(&a,                        b.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Prev);
		CPPUNIT_ASSERT_EQUAL(&a,                        c.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), d.Succ);
		CPPUNIT_ASSERT_EQUAL(&c, src.First);
		CPPUNIT_ASSERT_EQUAL(&b, src.Last);

		CPPUNIT_ASSERT_NO_THROW(src.b3Insert(&a, &d)); // c a d b
		CPPUNIT_ASSERT_EQUAL(&c,                        a.Prev);
		CPPUNIT_ASSERT_EQUAL(&d,                        a.Succ);
		CPPUNIT_ASSERT_EQUAL(&d,                        b.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Prev);
		CPPUNIT_ASSERT_EQUAL(&a,                        c.Succ);
		CPPUNIT_ASSERT_EQUAL(&a,                        d.Prev);
		CPPUNIT_ASSERT_EQUAL(&b,                        d.Succ);
		CPPUNIT_ASSERT_EQUAL(&c,                        src.First);
		CPPUNIT_ASSERT_EQUAL(&b,                        src.Last);

		CPPUNIT_ASSERT_NO_THROW(src.b3Remove(&a)); // c d b
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), a.Succ);
		CPPUNIT_ASSERT_EQUAL(&d,                        b.Prev);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), b.Succ);
		CPPUNIT_ASSERT_EQUAL(static_cast<T *>(nullptr), c.Prev);
		CPPUNIT_ASSERT_EQUAL(&d,                        c.Succ);
		CPPUNIT_ASSERT_EQUAL(&c,                        d.Prev);
		CPPUNIT_ASSERT_EQUAL(&b,                        d.Succ);
		CPPUNIT_ASSERT_EQUAL(&c,                        src.First);
		CPPUNIT_ASSERT_EQUAL(&b,                        src.Last);

		b3_count items = 0;
		for (const T & item : src)
		{
			b3PrintF(B3LOG_NORMAL, "%p\n", &item);

			CPPUNIT_ASSERT_EQUAL(TEST_CLASS,      item.b3GetClass());
			CPPUNIT_ASSERT_EQUAL(TEST_TYPE,       item.b3GetType());
			CPPUNIT_ASSERT_EQUAL(TEST_CLASS_TYPE, item.b3GetClassType());

			items++;
		}
		CPPUNIT_ASSERT_EQUAL(src.b3GetCount(), items);

		items = 0;
		for (auto it = src.begin(); it != src.end(); ++it)
		{
			CPPUNIT_ASSERT(it);
			CPPUNIT_ASSERT_EQUAL(TEST_CLASS,      it->b3GetClass());
			CPPUNIT_ASSERT_EQUAL(TEST_TYPE,       it->b3GetType());
			CPPUNIT_ASSERT_EQUAL(TEST_CLASS_TYPE, it->b3GetClassType());

			items++;
		}
		CPPUNIT_ASSERT_EQUAL(src.b3GetCount(), items);

		items = 0;
		for (auto it = src.rbegin(); it != src.rend(); ++it)
		{
			CPPUNIT_ASSERT(it);
			CPPUNIT_ASSERT_EQUAL(TEST_CLASS,      it->b3GetClass());
			CPPUNIT_ASSERT_EQUAL(TEST_TYPE,       it->b3GetType());
			CPPUNIT_ASSERT_EQUAL(TEST_CLASS_TYPE, it->b3GetClassType());

			items++;
		}
		CPPUNIT_ASSERT_EQUAL(src.b3GetCount(), items);

		CPPUNIT_ASSERT(src.begin() != src.end());
		CPPUNIT_ASSERT(src.rbegin() != src.rend());

		b3Base<b3TestElement>::iterator         first = src.begin();
		b3Base<b3TestElement>::reverse_iterator last  = src.rbegin();

		b3TestElement * f = first;
		b3TestElement * l = last;

		CPPUNIT_ASSERT_EQUAL(f, src.First);
		CPPUNIT_ASSERT_EQUAL(l, src.Last);

		CPPUNIT_ASSERT_NO_THROW(src.b3Sort(sorter));
		CPPUNIT_ASSERT_EQUAL(&b, src.First);
		CPPUNIT_ASSERT_EQUAL(&d, src.Last);
		CPPUNIT_ASSERT_EQUAL(c.Prev, src.First);
		CPPUNIT_ASSERT_EQUAL(c.Succ, src.Last);
	}

private:
	static int sorter(const T * aPtr, const T * bPtr)
	{
		return aPtr->b3GetElement() - bPtr->b3GetElement();
	}
};

#endif
#endif
