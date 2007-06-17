/*
**
**	$Filename:	b3DataSizeTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision: 1475 $
**	$Date: 2007-01-01 14:04:37 +0100 (Mo, 01 Jan 2007) $
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit test for basic data types.
**
**      (C) Copyright 2006  Steffen A. Mork
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
  
#include "b3DataSizeTest.h"

/*************************************************************************
**                                                                      **
**                        Unit test for basic data types                **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3DataSizeTest);

void b3DataSizeTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);

	b3_loop i;
	b3_u08  v;

	for (i = 0;i < MEM_MIN;i++)
	{
#if 0
		v         = B3_IRAN(256);
#else
		v         = i;
#endif
		buffer[i] = (b3_u08)(v & 0xff);
	}
	ptr1 = null;
	ptr2 = null;
}

void b3DataSizeTest::tearDown()
{	
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);

	if (ptr1 == null)
	{
		return;
	}
	for (b3_loop i = 0; i < MEM_MIN;i++)
	{
		if ((i & 7) == 0)
		{
			b3PrintF(B3LOG_NORMAL,"\n%04x: ",i);
		}
		b3PrintF(B3LOG_NORMAL," %02x-%02x",buffer[i],((b3_u08 *)ptr1)[i]);
	}
	b3PrintF(B3LOG_NORMAL,"\n");
}

void b3DataSizeTest::testDataSize()
{
	void *ptr = null;

	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), sizeof(b3_u08));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), sizeof(b3_u16));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), sizeof(b3_u32));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(8), sizeof(b3_u64));

	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), sizeof(b3_s08));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), sizeof(b3_s16));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), sizeof(b3_s32));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(8), sizeof(b3_s64));

	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), sizeof(b3_bool));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), sizeof(b3_f32));
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(8), sizeof(b3_f64));
	CPPUNIT_ASSERT(sizeof(b3_f96) >= 10);
	
	CPPUNIT_ASSERT(sizeof(b3_ptr) == sizeof(ptr));
}

void b3DataSizeTest::testMemory()
{
	b3_loop  i;
	b3_u08  *ptr;
	b3_u08   mask = 0;

	ptr1 = mem.b3Alloc (MEM_MIN);
	CPPUNIT_ASSERT(ptr1 != null);	

	ptr2 = mem.b3Alloc (MEM_MIN);
	CPPUNIT_ASSERT(ptr2 != null); 

	CPPUNIT_ASSERT(mem.b3Free(ptr1));	
	CPPUNIT_ASSERT(!mem.b3Free(null));
	CPPUNIT_ASSERT(mem.b3Free());

	ptr1 = mem.b3Realloc(null,  MEM_MIN * 2);
	CPPUNIT_ASSERT(ptr1 != null);

	memcpy(ptr1, buffer, MEM_MIN);

	ptr2 = mem.b3Realloc(ptr1,  MEM_MIN);
	CPPUNIT_ASSERT_EQUAL(ptr1, ptr2);
	CPPUNIT_ASSERT(ptr2 != null);

	ptr1 = mem.b3Realloc(ptr2,MEM_MIN * MEM_HIGH_MULT);
	CPPUNIT_ASSERT(ptr1 != ptr2);
	CPPUNIT_ASSERT(ptr1 != null);
	CPPUNIT_ASSERT(memcmp(buffer,ptr1,MEM_MIN)  == 0);

	ptr = static_cast<b3_u08 *>(ptr1);
	for (i = MEM_MIN;i < (MEM_MIN * MEM_HIGH_MULT);i++)  
	{
		mask |= ptr[i];
	}
	CPPUNIT_ASSERT_EQUAL(mask, static_cast<b3_u08>(0));
	
	ptr2 = mem.b3Realloc(ptr1,     0);
	CPPUNIT_ASSERT(ptr1 != ptr2);
	CPPUNIT_ASSERT_EQUAL(ptr2, static_cast<void *>(null));
	ptr1 = null;
}

void b3DataSizeTest::testSwap()
{
	b3_u32    v1,v2;

	v1 = 1; v2 = 2;
	B3_SWAP (v1,v2);
	CPPUNIT_ASSERT_EQUAL(v1, static_cast<b3_u32>(2));
	CPPUNIT_ASSERT_EQUAL(v2, static_cast<b3_u32>(1));

	v1 = 1; v2 = 2;
	B3_PSWAP (&v1,&v2);
	CPPUNIT_ASSERT_EQUAL(v1, static_cast<b3_u32>(2));
	CPPUNIT_ASSERT_EQUAL(v2, static_cast<b3_u32>(1));
}

#endif
