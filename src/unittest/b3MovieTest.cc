/*
**
**	$Filename:	b3MovieTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for movie streaming.
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

#include "b3MovieTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MovieTest);

#include "blz3/image/b3Tx.h"
#include "blz3/image/b3Encoder.h"

void b3MovieTest::setUp()
{
	const char * filename = "Data1.bwd";

#ifdef HAVE_VIDEO_ENCODER
	b3CodecRegister::b3Instance();
#endif

	b3SetupImages();

	world.b3Read(filename);
	scene = (b3Scene *)world.b3GetFirst();
	scene->b3Reorg();
	scene->b3SetupVertexMemory(&context);
	scene->b3SetFilename(filename);
	animation  = scene->b3GetAnimation(false);
	distribute = scene->b3GetDistributed(false);

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3MovieTest::tearDown()
{
	world.b3Free();
	scene      = nullptr;
	animation  = nullptr;
	distribute = nullptr;
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3MovieTest::test()
{
	CPPUNIT_ASSERT(scene != nullptr);
	CPPUNIT_ASSERT(animation != nullptr);
	CPPUNIT_ASSERT(distribute != nullptr);

	// Deactivate motion blur.
	distribute->m_Type = 0;
	CPPUNIT_ASSERT(!distribute->b3IsActive());
	CPPUNIT_ASSERT(!distribute->b3IsMotionBlur());

	// Define FPS!
#ifndef USE_GCOV
	animation->m_FramesPerSecond = 25;
#else
	animation->m_FramesPerSecond =  4;
#endif

	b3Display      display(640, 480);
#ifdef HAVE_VIDEO_ENCODER
	b3MovieEncoder encoder("test-animation.mp4", display, animation->m_FramesPerSecond);
#endif

	// Dry run animation.
	scene->b3ResetAnimation();
	const b3_f64   step  = 1.0 / animation->m_FramesPerSecond;

	b3_count frame = 0;
	for (b3_f64 t = animation->m_Start; t <= animation->m_End; t += step)
	{
		char imagename[B3_FILESTRINGLEN];

		scene->b3SetAnimation(t);
		scene->b3Raytrace(&display);
#ifdef HAVE_VIDEO_ENCODER
		CPPUNIT_ASSERT(encoder.b3AddFrame(display));
#endif

		snprintf(imagename, sizeof(imagename), "test-animation-%04d.jpg", frame++);
		display.b3SaveImage(imagename);
	}
}

void b3MovieTest::testEmpty()
{
#ifdef HAVE_VIDEO_ENCODER
	b3Tx      tx;

	CPPUNIT_ASSERT(tx.b3AllocTx(640, 480, 128));
	b3MovieEncoder encoder("test-video-empty.mp4", &tx, 25);

	encoder.b3AddFrame(&tx);
#endif
}

void b3MovieTest::testUneven()
{
#ifdef HAVE_VIDEO_ENCODER
	b3Tx tx;

	tx.b3AllocTx(19, 31, 32);
	CPPUNIT_ASSERT_THROW(b3MovieEncoder encoder("test-video-uneven.mp4", &tx, 25), b3TxException);
#endif
}

void b3MovieTest::testDiffTx()
{
#ifdef HAVE_VIDEO_ENCODER
	b3Tx tx;

	CPPUNIT_ASSERT(tx.b3AllocTx(640, 480, 128));

	b3MovieEncoder encoder("test-video-difftx.mp4", &tx, 25);
	CPPUNIT_ASSERT(tx.b3AllocTx(320,320,32));
	CPPUNIT_ASSERT(!encoder.b3AddFrame(&tx));
#endif
}

void b3MovieTest::testAudio()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video-audio.mp4", true));
}

void b3MovieTest::testAvi()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.avi"));
}

void b3MovieTest::testM2v()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.m2v"));
}

void b3MovieTest::testMpg()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.mpg"));
}

void b3MovieTest::testMp4()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.mp4"));
}

void b3MovieTest::testMkv()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.mkv"));
}

void b3MovieTest::testMov()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.mov"));
}

void b3MovieTest::testWebm()
{
	CPPUNIT_ASSERT_NO_THROW(createEmptyVideo("test-video.webm"));
}

void b3MovieTest::createEmptyVideo(const char * filename, const bool use_audio)
{
#ifdef HAVE_VIDEO_ENCODER
	b3_res frames = FRAMES_PER_SECONDS * DURATION;
	b3Tx   tx;

	tx.b3Copy(&m_TxTrueColor);
	b3MovieEncoder encoder(filename, &tx, FRAMES_PER_SECONDS, use_audio);

	for (int i = 0; i <= frames; i++)
	{
		encoder.b3AddFrame(&tx);
	}
#else
	(void)filename;
	(void)use_audio;
#endif
}

#endif
