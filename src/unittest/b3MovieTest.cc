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

extern "C"
{
#	include <x264.h>
#	include <libswscale/swscale.h>
#	include <libavcodec/avcodec.h>
#	include <libavutil/mathematics.h>
#	include <libavformat/avformat.h>
#	include <libavutil/opt.h>
}

class Packet
{
	AVPacket          pkt;

public:
	Packet()
	{
		av_init_packet(&pkt);
		pkt.data = nullptr;
		pkt.size = 0;
	}

	virtual ~Packet()
	{
		av_packet_unref(&pkt);
	}

	inline operator AVPacket * ()
	{
		return &pkt;
	}

	inline void key()
	{
		pkt.flags |= AV_PKT_FLAG_KEY;
	}
};

class MovieEncoder
{
	SwsContext    *   swsCtx  = nullptr;
	AVOutputFormat  * fmt     = nullptr;
	AVFormatContext * fmt_ctx = nullptr;
	AVCodec     *     codec   = nullptr;
	AVStream     *    stream  = nullptr;
	AVCodecContext  * cctx    = nullptr;

	AVFrame     *     rgbpic  = nullptr;
	AVFrame     *     yuvpic  = nullptr;
	AVRational        fps;

	static const unsigned    kbit_rate = 900;
	unsigned          iframe  = 0;
	b3_res            m_xSize;
	b3_res            m_ySize;

public:
	explicit MovieEncoder(const char * filename, const b3Tx * tx, const b3_res frames_per_second)
	{
		int err;

		m_xSize = tx->xSize;
		m_ySize = tx->ySize;
		fps.num =  1;
		fps.den = frames_per_second;

		av_register_all();
		swsCtx = sws_getContext(
				m_xSize, m_ySize, AV_PIX_FMT_RGB24,
				m_xSize, m_ySize, AV_PIX_FMT_YUV420P,
				SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

		fmt = av_guess_format(nullptr, filename, nullptr);
		err = avformat_alloc_output_context2(&fmt_ctx, fmt, nullptr, filename);
		if (err != 0)
		{
			b3PrintF(B3LOG_NORMAL, "Error allocation context: %d!\n", err);
		}
		codec  = avcodec_find_encoder(fmt->video_codec);
		stream = avformat_new_stream(fmt_ctx, codec);
		cctx   = avcodec_alloc_context3(codec);

		stream->codecpar->codec_id   = fmt->video_codec;
		stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
		stream->codecpar->width      = m_xSize;
		stream->codecpar->height     = m_ySize;
		stream->codecpar->format     = AV_PIX_FMT_YUV420P;
		stream->codecpar->bit_rate   = kbit_rate * 1000;
		stream->codecpar->profile    = FF_PROFILE_H264_BASELINE;
		stream->time_base            = fps;


		avcodec_parameters_to_context(cctx, stream->codecpar);
		cctx->time_base    = fps;
		cctx->max_b_frames =  2;
		cctx->gop_size     = 12;
		if (stream->codecpar->codec_id == AV_CODEC_ID_H264)
		{
			av_opt_set(cctx, "tune", "zerolatency", 0);
			av_opt_set(cctx, "preset", "ultrafast", 0);
			av_opt_set(cctx, "crf", "20", 0);
		}
		if (fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
		{
//			cctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		}
		avcodec_parameters_from_context(stream->codecpar, cctx);

		err = avcodec_open2(cctx, codec, nullptr);
		if (err >= 0)
		{
			if ((fmt->flags & AVFMT_NOFILE) == 0)
			{
				err = avio_open(&fmt_ctx->pb, filename, AVIO_FLAG_WRITE);
				if (err >= 0)
				{
					err = avformat_write_header(fmt_ctx, nullptr);
					if (err < 0)
					{
						b3PrintF(B3LOG_NORMAL, "Error writing header: %d!\n", err);
					}
				}
				else
				{
					b3PrintF(B3LOG_NORMAL, "Error opening file: %d!\n", err);
				}
			}
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Error opening codec: %d!\n", err);
		}

		av_dump_format(fmt_ctx, 0, filename, 1);

		// Preparing the containers of the frame data:
		// Allocating memory for each RGB frame, which will be lately converted to YUV.
		rgbpic = av_frame_alloc();
		rgbpic->format = AV_PIX_FMT_RGB24;
		rgbpic->width  = m_xSize;
		rgbpic->height = m_ySize;
		av_frame_get_buffer(rgbpic, 0);

		// Allocating memory for each conversion output YUV frame.
		yuvpic = av_frame_alloc();
		yuvpic->format = AV_PIX_FMT_YUV420P;
		yuvpic->width  = m_xSize;
		yuvpic->height = m_ySize;
		av_frame_get_buffer(yuvpic, 0);
	}

	virtual ~MovieEncoder()
	{
		b3Finish();
		if (!(fmt->flags & AVFMT_NOFILE))
		{
			avio_close(fmt_ctx->pb);
		}
		b3Free();
	}

	void add(const b3Tx * tx)
	{
		// The AVFrame data will be stored as RGBRGBRGB... row-wise,
		// from left to right and from top to bottom.
		for (b3_res y = 0; y < m_ySize; y++)
		{
			for (b3_res x = 0; x < m_xSize; x++)
			{
				const b3_pkd_color rgb = tx->b3GetValue(x, y);

				// rgbpic->linesize[0] is equal to width.
				rgbpic->data[0][y * rgbpic->linesize[0] + 3 * x + 0] = (rgb & 0xff0000) >> 16;
				rgbpic->data[0][y * rgbpic->linesize[0] + 3 * x + 1] = (rgb & 0x00ff00) >>  8;
				rgbpic->data[0][y * rgbpic->linesize[0] + 3 * x + 2] = (rgb & 0x0000ff);
			}
		}

		// Not actually scaling anything, but just converting
		// the RGB data to YUV and store it in yuvpic.
		sws_scale(swsCtx, rgbpic->data, rgbpic->linesize, 0,
			m_ySize, yuvpic->data, yuvpic->linesize);

		// The PTS of the frame are just in a reference unit,
		// unrelated to the format we are using. We set them,
		// for instance, as the corresponding frame number.
		yuvpic->pts = iframe++;
		if (avcodec_send_frame(cctx, yuvpic) >= 0)
		{
			Packet pkt;

			if (avcodec_receive_packet(cctx, pkt) >= 0)
			{
				pkt.key();
				av_interleaved_write_frame(fmt_ctx, pkt);
			}
		}
	}

private:
	void b3Finish()
	{
		int result = 0;
		do
		{
			Packet pkt;

			avcodec_send_frame(cctx, nullptr);
			result = avcodec_receive_packet(cctx, pkt);
			if (result == 0)
			{
				av_interleaved_write_frame(fmt_ctx, pkt);
			}
		}
		while (result == 0);
		av_write_trailer(fmt_ctx);
	}

	void b3Free()
	{
		// Freeing all the allocated memory:
		av_frame_free(&rgbpic);
		av_frame_free(&yuvpic);
		avcodec_free_context(&cctx);
		avformat_free_context(fmt_ctx);
		sws_freeContext(swsCtx);
	}
};

void b3MovieTest::setUp()
{
	const char * filename = "Data1.bwd";

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
	animation->m_FramesPerSecond = 25;

	b3Display    display(640, 480);
	MovieEncoder encoder("test-animation.mp4", display, animation->m_FramesPerSecond);

	// Dry run animation.
	scene->b3ResetAnimation();
	const b3_f64   step  = 1.0 / animation->m_FramesPerSecond;

	b3_count frame = 0;
	for (b3_f64 t = animation->m_Start; t <= animation->m_End; t += step)
	{
		char imagename[B3_FILESTRINGLEN];

		scene->b3SetAnimation(t);
		scene->b3Raytrace(&display);
		encoder.add(display);

		snprintf(imagename, sizeof(imagename), "test-animation-%04d.jpg", frame++);
		display.b3SaveImage(imagename);
	}
}

#endif
