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

#ifdef HAVE_VIDEO_ENCODER
extern "C"
{
#	include <libswscale/swscale.h>
#	include <libavcodec/avcodec.h>
#	include <libavformat/avformat.h>
#	include <libavutil/opt.h>
}

class b3EncoderFrameBuffer
{
	AVFrame * m_Frame = nullptr;

public:
	explicit b3EncoderFrameBuffer(const b3Tx * tx, int format)
	{
		m_Frame = av_frame_alloc();
		m_Frame->format = format;
		m_Frame->width  = tx->xSize;
		m_Frame->height = tx->ySize;
		av_frame_get_buffer(m_Frame, 0);
	}

	virtual ~b3EncoderFrameBuffer()
	{
		av_frame_free(&m_Frame);
	}

	inline operator AVFrame * ()
	{
		return m_Frame;
	}

	inline AVFrame * operator -> ()
	{
		return m_Frame;
	}
};

class b3EncoderPacket
{
	AVPacket  m_Packet;

public:
	b3EncoderPacket()
	{
		av_init_packet(&m_Packet);
		m_Packet.data = nullptr;
		m_Packet.size = 0;
	}

	virtual ~b3EncoderPacket()
	{
		av_packet_unref(&m_Packet);
	}

	inline operator AVPacket * ()
	{
		return &m_Packet;
	}

	inline void key()
	{
		m_Packet.flags |= AV_PKT_FLAG_KEY;
	}
};

class b3CodecRegister
{
	b3CodecRegister()
	{
		av_register_all();
		avcodec_register_all();
		av_log_set_callback(b3Log);
	}

	static void b3Log(void *, int level, const char * fmt, va_list vargs)
	{
		char message[8192];
		b3_log_level b3_level;

		switch(level)
		{
		case AV_LOG_QUIET:
			b3_level = B3LOG_NONE;
			break;

		case AV_LOG_DEBUG:
			b3_level = B3LOG_DEBUG;
			break;

		case AV_LOG_VERBOSE:
		case AV_LOG_TRACE:
			b3_level = B3LOG_FULL;
			break;

		default:
			b3_level = B3LOG_NORMAL;
			break;
		}

		vsnprintf(message, sizeof(message), fmt, vargs);
		b3PrintF(b3_level, "%s\n", message);
	}

public:
	static b3CodecRegister & b3Instance()
	{
		static b3CodecRegister instance;

		return instance;
	}
};

class b3MovieEncoder
{
	SwsContext       *       m_SwsCtx        = nullptr;
	AVOutputFormat     *     m_OutputFormat  = nullptr;
	AVFormatContext     *    m_FormatContext = nullptr;
	AVCodec         *        m_Codec         = nullptr;
	AVStream        *        m_Stream        = nullptr;
	AVCodecContext     *     m_CodecContext  = nullptr;

	b3EncoderFrameBuffer     m_RgbFrame;
	b3EncoderFrameBuffer     m_YuvFrame;
	AVRational               m_FramesPerSecond;

	static const unsigned    m_kbit_rate = 900;
	unsigned                 m_iFrame    =   0;
	b3_res                   m_xSize;
	b3_res                   m_ySize;

public:
	explicit b3MovieEncoder(
		const char * filename,
		const b3Tx * tx,
		const b3_res frames_per_second) :
		m_RgbFrame(tx, AV_PIX_FMT_RGB24),
		m_YuvFrame(tx, AV_PIX_FMT_YUV420P)
	{
		int err;

		m_xSize = tx->xSize;
		m_ySize = tx->ySize;
		m_FramesPerSecond.num =  1;
		m_FramesPerSecond.den = frames_per_second;

		m_SwsCtx = sws_getContext(
				m_xSize, m_ySize, AV_PIX_FMT_RGB24,
				m_xSize, m_ySize, AV_PIX_FMT_YUV420P,
				SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

		m_OutputFormat = av_guess_format(nullptr, filename, nullptr);
		err = avformat_alloc_output_context2(&m_FormatContext, m_OutputFormat, nullptr, filename);
		if (err != 0)
		{
			b3PrintF(B3LOG_NORMAL, "Error allocation context: %d!\n", err);
		}
		m_Codec  = avcodec_find_encoder(m_OutputFormat->video_codec);
		m_Stream = avformat_new_stream(m_FormatContext, m_Codec);
		m_CodecContext   = avcodec_alloc_context3(m_Codec);

		m_Stream->codecpar->codec_id   = m_OutputFormat->video_codec;
		m_Stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
		m_Stream->codecpar->width      = m_xSize;
		m_Stream->codecpar->height     = m_ySize;
		m_Stream->codecpar->format     = AV_PIX_FMT_YUV420P;
		m_Stream->codecpar->bit_rate   = m_kbit_rate * 1000;
		m_Stream->codecpar->profile    = FF_PROFILE_H264_BASELINE;
		m_Stream->time_base            = m_FramesPerSecond;

		avcodec_parameters_to_context(m_CodecContext, m_Stream->codecpar);
		m_CodecContext->time_base    = m_FramesPerSecond;
		m_CodecContext->max_b_frames =  2;
		m_CodecContext->gop_size     = 12;
		if (m_Stream->codecpar->codec_id == AV_CODEC_ID_H264)
		{
			av_opt_set(m_CodecContext, "tune", "zerolatency", 0);
			av_opt_set(m_CodecContext, "preset", "ultrafast", 0);
			av_opt_set(m_CodecContext, "crf", "20", 0);
		}
		if (m_FormatContext->oformat->flags & AVFMT_GLOBALHEADER)
		{
//			cctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		}
		avcodec_parameters_from_context(m_Stream->codecpar, m_CodecContext);

		err = avcodec_open2(m_CodecContext, m_Codec, nullptr);
		if (err >= 0)
		{
			if ((m_OutputFormat->flags & AVFMT_NOFILE) == 0)
			{
				err = avio_open(&m_FormatContext->pb, filename, AVIO_FLAG_WRITE);
				if (err >= 0)
				{
					err = avformat_write_header(m_FormatContext, nullptr);
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

		av_dump_format(m_FormatContext, 0, filename, 1);
	}

	virtual ~b3MovieEncoder()
	{
		b3Finish();
		if (!(m_OutputFormat->flags & AVFMT_NOFILE))
		{
			avio_close(m_FormatContext->pb);
		}
		b3Free();
	}

	void b3AddFrame(const b3Tx * tx)
	{
		// The AVFrame data will be stored as RGBRGBRGB... row-wise,
		// from left to right and from top to bottom.
		for (b3_res y = 0; y < m_ySize; y++)
		{
			b3_pkd_color row[m_xSize];

			tx->b3GetRow(row, y);
			for (b3_res x = 0; x < m_xSize; x++)
			{
				// rgbpic->linesize[0] is equal to width.
				m_RgbFrame->data[0][y * m_RgbFrame->linesize[0] + 3 * x + 0] = (row[x] & 0xff0000) >> 16;
				m_RgbFrame->data[0][y * m_RgbFrame->linesize[0] + 3 * x + 1] = (row[x] & 0x00ff00) >>  8;
				m_RgbFrame->data[0][y * m_RgbFrame->linesize[0] + 3 * x + 2] = (row[x] & 0x0000ff);
			}
		}

		// Not actually scaling anything, but just converting
		// the RGB data to YUV and store it in yuvpic.
		sws_scale(m_SwsCtx,
			m_RgbFrame->data, m_RgbFrame->linesize, 0, m_ySize,
			m_YuvFrame->data, m_YuvFrame->linesize);

		// The PTS of the frame are just in a reference unit,
		// unrelated to the format we are using. We set them,
		// for instance, as the corresponding frame number.
		m_YuvFrame->pts = m_iFrame++;
		if (avcodec_send_frame(m_CodecContext, m_YuvFrame) >= 0)
		{
			b3EncoderPacket pkt;

			if (avcodec_receive_packet(m_CodecContext, pkt) >= 0)
			{
				pkt.key();
				av_interleaved_write_frame(m_FormatContext, pkt);
			}
		}
	}

private:
	void b3Finish()
	{
		int result = 0;
		do
		{
			b3EncoderPacket pkt;

			avcodec_send_frame(m_CodecContext, nullptr);
			result = avcodec_receive_packet(m_CodecContext, pkt);
			if (result == 0)
			{
				av_interleaved_write_frame(m_FormatContext, pkt);
			}
		}
		while (result == 0);
		av_write_trailer(m_FormatContext);
	}

	void b3Free()
	{
		// Freeing all the allocated memory:
		avcodec_free_context(&m_CodecContext);
		avformat_free_context(m_FormatContext);
		sws_freeContext(m_SwsCtx);
	}
};

#endif

void b3MovieTest::setUp()
{
	const char * filename = "Data1.bwd";

	b3CodecRegister::b3Instance();

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
		encoder.b3AddFrame(display);
#endif

		snprintf(imagename, sizeof(imagename), "test-animation-%04d.jpg", frame++);
		display.b3SaveImage(imagename);
	}
}

#endif
