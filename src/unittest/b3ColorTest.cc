/*
**
**	$Filename:	b3ColorTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base and system unit tests
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

#include "b3ColorTest.h"
#include "b3TestMacros.h"

#include "blz3/base/b3Render.h"

#include <cstddef>

/*************************************************************************
**                                                                      **
**                        Color unit test                               **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ColorTest);

void b3ColorTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3ColorTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3ColorTest::testColor()
{
	b3_color   offset_color;

	CPPUNIT_ASSERT_EQUAL(b3Color::A * sizeof (offset_color.a), offsetof(b3_color, a));
	CPPUNIT_ASSERT_EQUAL(b3Color::R * sizeof (offset_color.r), offsetof(b3_color, r));
	CPPUNIT_ASSERT_EQUAL(b3Color::G * sizeof (offset_color.g), offsetof(b3_color, g));
	CPPUNIT_ASSERT_EQUAL(b3Color::B * sizeof (offset_color.b), offsetof(b3_color, b));

	//	printf("color: %p %p %08x\n", &color, &color[b3Color::A],reinterpret_cast<int>(&color[b3Color::A]));
#if defined(BLZ3_USE_SSE) && defined(SSE_ALIGNED)
	b3_f32  *  fPtr = &color[b3Color::A];
#if 0
	color[b3Color::A] = 0;
	color[b3Color::R] = 1;
	color[b3Color::G] = 2;
	color[b3Color::B] = 3;

	printf("this: %p\n", this);
	for (int q = 0; q < 4; q++)
	{
		printf("%d: %f %p\n", q, fPtr[q], &fPtr[q]);
	}
#endif

	b3_offset  offset = reinterpret_cast<b3_offset>(fPtr) & 0xf;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_offset, 0, offset);
#endif

	CPPUNIT_ASSERT_EQUAL(
				&offset_color,
				b3ColorBase::b3Init(&offset_color, 0.25f, 0.5f, 0.75f, 1.0f));
	color = offset_color;
	testColor(0.25f, 0.5f, 0.75f, 1.0f);

	color = b3Color(0.5f);
	testColor(0.5f, 0.5f, 0.5f, 0.0f);

	color = b3Color(0.25f, 0.75f);
	testColor(0.25f, 0.25f, 0.25f, 0.75f);

	color = B3_WHITE;
	testColor(1.0f, 1.0f, 1.0f, 0.0f);

	color = B3_RED;
	testColor(1.0f, 0.0f, 0.0f, 0.0f);

	color = B3_GREEN;
	testColor(0.0f, 1.0f, 0.0f, 0.0f);

	color = B3_BLUE;
	testColor(0.0f, 0.0f, 1.0f, 0.0f);

	color = static_cast<b3_u16>(0x0f0f);
	testColor(1.0f, 0.0f, 1.0f, 0.0f);

	color.b3Init();
	testColor(0.0f, 0.0f, 0.0f, 0.0f);

	color.b3InitFactor(0.25);
	testColor(0.25f, 0.25f, 0.25f, 0.25f);

	color.b3Init(0.25, 0.5);
	testColor(0.25f, 0.25f, 0.25f, 0.5f);

	color.b3Init(0.25, 0.5, 0.75, 1.0);
	testColor(0.25f, 0.5f, 0.75f, 1.0f);

	color.b3SetAlpha(0.0);
	testColor(0.25f, 0.5f, 0.75f, 0.0f);

	color = b3Color::b3Mix(
			b3Color(B3_WHITE), b3Color(B3_RED), static_cast<b3_f64>(0.25));
	testColor(1.0f, 0.75f, 0.75f, 0.0f);

	color = b3Color::b3Mix(
			b3Color(B3_WHITE), b3Color(B3_RED), static_cast<b3_f32>(0.25));
	testColor(1.0f, 0.75f, 0.75f, 0.0f);

	color = b3Color::b3Mix(
			b3Color(B3_WHITE), b3Color(B3_RED), b3Color(0.25, 0.5, 1.0, 0.0));
	testColor(1.0f, 0.5f, 0.0f, 0.0f);

	color.b3Init(1.0f, 2.0f, 3.0f, 4.0f);
	color.b3Sat();
	testColor(1.0f, 1.0f, 1.0f, 1.0f);

	color.b3Init(0.5f, 1.0f, 1.5f, 2.0f);
	color.b3Sat();
	testColor(0.5f, 1.0f, 1.0f, 1.0f);

	color.b3Init(-1.0, 2.0, -3.0, 4.0);
	color.b3Min();
	testColor(0.0f, 2.0f, 0.0f, 4.0f);

	color.b3Init(-1.0, 2.0, -3.0, 4.0);
	color.b3Min(0.5f);
	testColor(0.5f, 2.0f, 0.5f, 4.0f);

	ac.b3Init(0.25f, 0.5f,  1.0f, 0.75f);
	ac.b3Sat();
	color = ac;
	testColor(0.25f, 0.5f,  1.0f, 0.75f);

	bc.b3Init(0.25f, 0.25f, 0.5f, 0.25f);
	bc.b3Sat();
	color = bc;
	testColor(0.25f, 0.25f, 0.5f, 0.25f);

	color.b3Init(-1.0, 2.0, -3.0, 4.0);
	color.b3Abs();
	testColor(1.0, 2.0, 3.0, 4.0);

	color = ac + bc;
	testColor(0.5f, 0.75f, 1.5f, 1.0f);

	color = ac - bc;
	testColor(0.0f, 0.25f, 0.5f, 0.5f);

	color = ac * bc;
	testColor(0.0625f, 0.125f, 0.5f, 0.1875f);

	color = ac * 2.5;
	testColor(0.625f, 1.25f, 2.5f, 1.875f);

	color = ac * 2.5f;
	testColor(0.625f, 1.25f, 2.5f, 1.875f);

	color = ac / 0.25;
	testColor(1.0f, 2.0f, 4.0f, 3.0f);

	color = ac / 0.25f;
	testColor(1.0f, 2.0f, 4.0f, 3.0f);

	color = ac / static_cast<b3_count>(4);
	testColor(0.0625f, 0.125f, 0.25f, 0.1875f);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x30102040, color);

	CPPUNIT_ASSERT(ac >= bc);
	CPPUNIT_ASSERT(bc <= ac);
	CPPUNIT_ASSERT(ac != bc);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xbf4080ff, ac);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x40404080, bc);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16, 0xb48f, ac);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16, 0x4448, bc);

	bc /= 2.0;
	CPPUNIT_ASSERT(ac  > bc);
	CPPUNIT_ASSERT(bc <  ac);
	CPPUNIT_ASSERT(ac != bc);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xbf4080ff, ac);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x20202040, bc);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16, 0xb48f, ac);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16, 0x2224, bc);

	CPPUNIT_ASSERT_NO_THROW(color.b3Dump());
}

void b3ColorTest::testPkdColor()
{
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_WHITE,       b3Color::b3MakePkdColor(0xff, 0xff, 0xff));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_LIGHT_GREY,  b3Color::b3MakePkdColor(0xc0, 0xc0, 0xc0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_GREY,        b3Color::b3MakePkdColor(0x80, 0x80, 0x80));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_DARK_GREY,   b3Color::b3MakePkdColor(0x40, 0x40, 0x40));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_BLACK,       b3Color::b3MakePkdColor(0x00, 0x00, 0x00));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_RED,         b3Color::b3MakePkdColor(0xff, 0x00, 0x00));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_GREEN,       b3Color::b3MakePkdColor(0x00, 0xff, 0x00));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_BLUE,        b3Color::b3MakePkdColor(0x00, 0x00, 0xff));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_YELLOW,      b3Color::b3MakePkdColor(0xff, 0xff, 0x00));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_MAGENTA,     b3Color::b3MakePkdColor(0xff, 0x00, 0xff));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_CYAN,        b3Color::b3MakePkdColor(0x00, 0xff, 0xff));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_MARKER,      b3Color::b3MakePkdColor(0xff, 0x11, 0x44));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, B3_TRANSPARENT, b3Color::b3MakePkdColor(0x00, 0x00, 0x00, 0xff));
}

void b3ColorTest::testOpenGlColor()
{
#ifdef BLZ3_USE_OPENGL
	b3Color src;
	GLfloat dst_float[4];
	GLubyte dst_ubyte[4];

	src.b3Init(0.25, 0.5, 0.75, 0.125);

	b3RenderContext::b3ColorToGL(src, dst_float);
	CPPUNIT_ASSERT_EQUAL(0.25f,  dst_float[0]);
	CPPUNIT_ASSERT_EQUAL(0.5f,   dst_float[1]);
	CPPUNIT_ASSERT_EQUAL(0.75f,  dst_float[2]);
	CPPUNIT_ASSERT_EQUAL(0.875f, dst_float[3]);

	b3RenderContext::b3ColorToGL(src, dst_ubyte);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0x3f,  dst_ubyte[0]);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0x7f,  dst_ubyte[1]);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0xbf,  dst_ubyte[2]);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0xe0,  dst_ubyte[3]);

	b3RenderContext::b3PkdColorToGL(0x204080c0, dst_float);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25f,  dst_float[0], 1.0 / 255.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5f,   dst_float[1], 1.0 / 255.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75f,  dst_float[2], 1.0 / 255.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.875f, dst_float[3], 1.0 / 255.0);

	b3RenderContext::b3PkdColorToGL(0x12345678, dst_ubyte);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0x34,  dst_ubyte[0]);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0x56,  dst_ubyte[1]);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0x78,  dst_ubyte[2]);
	CPPUNIT_ASSERT_TYPED_EQUAL(GLubyte, 0xed,  dst_ubyte[3]);

	const b3_pkd_color result = b3RenderContext::b3GLToPkdColor(dst_float);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x4080c0, result);
#endif
}

void b3ColorTest::testSaturation()
{
	b3Color test_color;

	test_color.b3Init(2.0, 0.5, -1.5);

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xff8000, test_color);
}

void b3ColorTest::testColor(
	const b3_f32 r,
	const b3_f32 g,
	const b3_f32 b,
	const b3_f32 a)
{
	b3_f32 ta = color[b3Color::A];
	b3_f32 tr = color[b3Color::R];
	b3_f32 tg = color[b3Color::G];
	b3_f32 tb = color[b3Color::B];

	CPPUNIT_ASSERT_EQUAL(b3Color(r, g, b, a), color);
	CPPUNIT_ASSERT_EQUAL(a, ta);
	CPPUNIT_ASSERT_EQUAL(r, tr);
	CPPUNIT_ASSERT_EQUAL(g, tg);
	CPPUNIT_ASSERT_EQUAL(b, tb);
}

#endif
