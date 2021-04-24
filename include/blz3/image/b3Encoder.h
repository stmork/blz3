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

/**
 * This method represents a single AV frame. It is needed as input for an
 * encoding job. Mostly the RGB input has to converted into YUV format and
 * then encoded as h.264.
 */
class b3EncoderFrameBuffer
{
	AVFrame * m_Frame = nullptr;

public:
	/**
	 * Thie constructor allocates an AV frame in the specified resolution
	 * given by the b3Tx instance and a color format.
	 *
	 * @param tx The b3Tx instance to get the frame resolution from.
	 * @param format The color format of the AV frame.
	 */
	explicit b3EncoderFrameBuffer(const b3Tx * tx, int format);

	/**
	 * The destructor frees the frame resource.
	 */
	virtual ~b3EncoderFrameBuffer();

	/**
	 * This cast operator simply returns a AV frame pointer for convenience.
	 */
	inline operator AVFrame * ()
	{
		return m_Frame;
	}

	/**
	 * This cast operator simply dereferences a AV frame pointer for convenience.
	 */
	inline AVFrame * operator -> ()
	{
		return m_Frame;
	}
};

/**
 * The AV packet contains the encoded video data.
 */
class b3EncoderPacket
{
	AVPacket  m_Packet;

public:
	/**
	 * The constructor initializes the AV packet for further usage.
	 */
	b3EncoderPacket();

	/**
	 * The destructor frees references to this AV packet.
	 */
	virtual ~b3EncoderPacket();

	/**
	 * This cast operator simply returns a AV packet pointer for convenience.
	 */
	inline operator AVPacket * ()
	{
		return &m_Packet;
	}

	/**
	 * This method marks the AV packet as a key frame.
	 */
	inline void key();
};

/**
 * This class provides an easy way to encode multiple single images into
 * one video encoded MP4 file.
 */
class b3MovieEncoder
{
	SwsContext        *        m_SwsCtx        = nullptr;
	AVOutputFormat      *      m_OutputFormat  = nullptr;
	AVFormatContext      *     m_FormatContext = nullptr;
	AVCodec          *         m_Codec         = nullptr;
	AVStream         *         m_Stream        = nullptr;
	AVCodecContext      *      m_CodecContext  = nullptr;

	b3EncoderFrameBuffer       m_RgbFrame;
	b3EncoderFrameBuffer       m_YuvFrame;
	AVRational                 m_FramesPerSecond;

	static const unsigned      m_kbit_rate = 900;
	static const AVPixelFormat m_SrcFormat;
	static const AVPixelFormat m_DstFormat;

	unsigned                   m_iFrame    =   0;
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

	void b3PrintErr(
		const char * description,
		const int    err,
		const bool   throw_exception = true);
};

#endif
#endif
