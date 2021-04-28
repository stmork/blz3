/*
**
**	$Filename:	b3MovieTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of a MP4/h.264 video encoder.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_IMAGE_ENCODER_H
#define B3_IMAGE_ENCODER_H

#include "blz3/b3Config.h"

#ifdef HAVE_VIDEO_ENCODER

#include "blz3/image/b3Tx.h"
#include "blz3/image/b3EncoderBuffer.h"

extern "C"
{
#	include <libswscale/swscale.h>
#	include <libavcodec/avcodec.h>
#	include <libavformat/avformat.h>
#	include <libavutil/opt.h>
}

/**
 * This singleton registers all necessary video codecs and methods. It
 * provides a Blizzard III compatible logging.
 */
class b3CodecRegister
{
	union
	{
		struct b3_encoder_info
		{
			unsigned m_mpg: 1;
			unsigned m_h264: 1;
			unsigned m_hevc: 1;
			unsigned m_vp8: 1;
			unsigned m_vp9: 1;
		} bits;
		unsigned value = 0;
	} m_Encoderinfo;


	b3CodecRegister();

	static void b3Log(void *, int level, const char * fmt, va_list vargs);

public:
	/**
	 * This method returns the singleton registrar service.
	 *
	 * @return The registrar service.
	 */
	static b3CodecRegister & b3Instance();

	void b3PrepareCodecs();
};

class b3EncoderStream;
class b3AudioStream;
class b3VideoStream;

/**
 * This class provides an easy way to encode multiple single images into
 * one video encoded MP4 file.
 */
class b3MovieEncoder
{
	SwsContext        *        m_SwsCtx        = nullptr;
	AVFormatContext      *     m_FormatContext = nullptr;
	b3AudioStream       *      m_AudioStream   = nullptr;
	b3VideoStream       *      m_VideoStream   = nullptr;

	b3EncoderFrameBuffer       m_RgbFrame;
	b3EncoderFrameBuffer       m_YuvFrame;
	b3EncoderFrameBuffer       m_AudioFrame;

	static const unsigned      m_kbit_rate = 900;
	static const AVPixelFormat m_SrcFormat;
	static const AVPixelFormat m_DstFormat;

	b3_res                     m_xSize;
	b3_res                     m_ySize;

public:
	/**
	 * The constructor initializes an encoded video stream by getting the
	 * needed information from the given b3Tx instance.
	 *
	 * @param filename The filename of the encoded video stream. @note The
	 * filename extension determines the used codec container format.
	 * @param tx The b3Tx instance which contains the image resolution and
	 * on encoding the image pixel data.
	 * @param frames_per_second The frames per second to encode.
	 */
	explicit b3MovieEncoder(
		const char * filename,
		const b3Tx * tx,
		const b3_res frames_per_second = 25);

	/**
	 * The destructor finishes the video stream and frees all resources
	 * allocated by this video encoder.
	 */
	virtual ~b3MovieEncoder();

	/**
	 * This method adds a single image from a b3Tx instance to the encoded
	 * video stream.
	 *
	 * @param tx The b3Tx instance which contains the single image data.
	 * @returns True on success or false on error or different b3Tx resolution
	 * given during initialization.
	 */
	bool b3AddFrame(const b3Tx * tx);

private:
	/**
	 * This method encodes delayed packes at the end of the video stream.
	 */
	void b3Finish();

	/**
	 * This method frees all resources allocated by this video encoder.
	 */
	void b3Free();

	void b3PrepareStream(b3EncoderStream * stream);

	bool b3AddVideoFrame(const b3Tx * tx);
	bool b3AddAudioFrame();

	int  b3SendFrame(b3EncoderStream * stream, AVFrame * frame = nullptr);

	static inline bool b3Delayed(int error)
	{
		return (error == AVERROR(EAGAIN)) || (error == AVERROR_EOF);
	}

	void b3PrintErr(
		const char * description,
		const int    err,
		const bool   throw_exception = true);
};

#endif
#endif
