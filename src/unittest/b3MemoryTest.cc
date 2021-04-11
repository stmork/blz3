/*
**
**	$Filename:	b3MemoryTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for memory management.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3MemoryTest.h"
#include "b3TestMacros.h"

#include "blz3/system/b3Memory.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MemoryTest);

void b3MemoryTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);

	mem.b3Free();
	for (b3_loop i = 0; i < MEM_MIN; i++)
	{
		b3_u08  v;
#if 0
		v         = B3_IRAN(256);
#else
		v         = i;
#endif
		buffer[i] = (b3_u08)(v & 0xff);
	}
	ptr1 = nullptr;
	ptr2 = nullptr;
}

void b3MemoryTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);

	if (ptr1 == nullptr)
	{
		return;
	}
	for (b3_loop i = 0; i < MEM_MIN; i++)
	{
		if ((i & 7) == 0)
		{
			b3PrintF(B3LOG_NORMAL, "\n%04x: ", i);
		}
		b3PrintF(B3LOG_NORMAL, " %02x-%02x", buffer[i], ((b3_u08 *)ptr1)[i]);
	}
	b3PrintF(B3LOG_NORMAL, "\n");
	mem.b3Free();
}

void b3MemoryTest::testSimple()
{
	b3Mem      pool;
	b3_color * ptr = nullptr;
	void   *   v   = nullptr;
	void   *   n   = nullptr;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0, pool.b3Count());

	CPPUNIT_ASSERT_NO_THROW(ptr = pool.b3TypedAlloc<b3_color>(ARRAY_COUNT));
	CPPUNIT_ASSERT(ptr != nullptr);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 1, pool.b3Count());

	CPPUNIT_ASSERT_NO_THROW(v = pool.b3Alloc(ARRAY_COUNT));
	CPPUNIT_ASSERT(v != nullptr);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 2, pool.b3Count());

	CPPUNIT_ASSERT_NO_THROW(n = pool.b3Realloc(v, ARRAY_COUNT >> 1));
	CPPUNIT_ASSERT(n != nullptr);
	CPPUNIT_ASSERT(v == n);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 2, pool.b3Count());

	CPPUNIT_ASSERT_NO_THROW(n = pool.b3Realloc(v, ARRAY_COUNT));
	CPPUNIT_ASSERT(n != nullptr);
	CPPUNIT_ASSERT(v == n);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 2, pool.b3Count());

	CPPUNIT_ASSERT_NO_THROW(n = pool.b3Realloc(v, ARRAY_COUNT << 1));
	CPPUNIT_ASSERT(n != nullptr);
	CPPUNIT_ASSERT(v != n);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 2, pool.b3Count());

	CPPUNIT_ASSERT_TYPED_EQUAL(void *,nullptr, pool.b3Alloc(0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 2, pool.b3Count());

	CPPUNIT_ASSERT_NO_THROW(pool.b3Dump());

	v = n;
	CPPUNIT_ASSERT_NO_THROW(n = pool.b3Realloc(v, 0));
	CPPUNIT_ASSERT(n == nullptr);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 1, pool.b3Count());

	CPPUNIT_ASSERT(pool.b3Free(ptr));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0, pool.b3Count());

	CPPUNIT_ASSERT(!pool.b3Free(ptr));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0, pool.b3Count());

	CPPUNIT_ASSERT(pool.b3Free());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0, pool.b3Count());
}

void b3MemoryTest::testMemory()
{
	b3_loop  i;
	b3_u08 * ptr;
	b3_u08   mask = 0;

	ptr1 = mem.b3Alloc(MEM_MIN);
	CPPUNIT_ASSERT(ptr1 != nullptr);

	ptr2 = mem.b3Alloc(MEM_MIN);
	CPPUNIT_ASSERT(ptr2 != nullptr);

	CPPUNIT_ASSERT(mem.b3Free(ptr1));
	CPPUNIT_ASSERT(!mem.b3Free(nullptr));
	CPPUNIT_ASSERT(mem.b3Free());

	ptr1 = mem.b3Realloc(nullptr,  MEM_MIN * 2);
	CPPUNIT_ASSERT(ptr1 != nullptr);

	memcpy(ptr1, buffer, MEM_MIN);

	ptr2 = mem.b3Realloc(ptr1,  MEM_MIN);
	CPPUNIT_ASSERT_EQUAL(ptr1, ptr2);
	CPPUNIT_ASSERT(ptr2 != nullptr);

	ptr1 = mem.b3Realloc(ptr2, MEM_MIN * MEM_HIGH_MULT);
	CPPUNIT_ASSERT(ptr1 != ptr2);
	CPPUNIT_ASSERT(ptr1 != nullptr);
	CPPUNIT_ASSERT(memcmp(buffer, ptr1, MEM_MIN)  == 0);

	ptr = static_cast<b3_u08 *>(ptr1);
	for (i = MEM_MIN; i < (MEM_MIN * MEM_HIGH_MULT); i++)
	{
		mask |= ptr[i];
	}
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u08, mask, 0);

	ptr2 = mem.b3Realloc(ptr1,     0);
	CPPUNIT_ASSERT(ptr1 != ptr2);
	CPPUNIT_ASSERT_TYPED_EQUAL(void *, nullptr, ptr2);
	ptr1 = nullptr;
}

void b3MemoryTest::testMultiple()
{
	void * array[ARRAY_COUNT];
	b3Mem  pool;

	// Allocate a lot of memory chunks
	for (b3_count i = 0; i < ARRAY_COUNT; i++)
	{
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, i, pool.b3Count());
		array[i] = pool.b3Alloc(B3_IRAN(ARRAY_COUNT) + 1);
	}

	// Permute pointers
	for (b3_count i = 0; i < 100000; i++)
	{
		b3_index a = B3_IRAN(ARRAY_COUNT);
		b3_index b = B3_IRAN(ARRAY_COUNT);
		void  *  aux;

		aux      = array[a];
		array[a] = array[b];
		array[b] = aux;
	}

	// Free memory chunks and verify.
	for (b3_count i = 0; i < ARRAY_COUNT; i++)
	{
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, ARRAY_COUNT - i, pool.b3Count());
		CPPUNIT_ASSERT(pool.b3Free(array[i]));
		CPPUNIT_ASSERT(!pool.b3Free(array[i]));
	}
}

#endif
