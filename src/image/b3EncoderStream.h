/*
**
**  $Filename:  b3EncoderStream.cc $
**  $Release:   Dortmund 2024 $
**  $Revision$
**  $Date$
**  $Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of an encoder streams.
**
**      (C) Copyright 2001 - 2024  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_IMAGE_ENCODER_STREAM_H
#define B3_IMAGE_ENCODER_STREAM_H

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

class b3CodecFrame;

/*************************************************************************
**                                                                      **
**                        AV stream/codec implementation                **
**                                                                      **
*************************************************************************/

/**
 * This method encapsulates the handling of a codec environment. It handles
 * the following libav entities:
 * <ul>
 * <li>AVCodec</li>
 * <li>AVCodecContext</li>
 * <li>AVStream</li>
 * </ul>
 */
class b3EncoderStream
{
	int64_t          m_FrameNo      = 0;
	int64_t          m_Pts          = 0;

protected:
	AVRational       m_FrameDuration { 0, 1 };
	AVCodecID        m_CodecId      = AV_CODEC_ID_PROBE;
	const AVCodec  * m_Codec        = nullptr;
	AVCodecContext * m_CodecContext = nullptr;
	AVStream    *    m_Stream       = nullptr;

public:
	/**
	 * This constructor initializes the codec and stream handling of one
	 * single media type.
	 *
	 * @param format_context The description of the container format.
	 * @param filename The container filename.
	 * @param preferred_codec_id The codecs' preferred ID.
	 * @param media_type The media type (audio or video) to use.
	 */
	b3EncoderStream(
		AVFormatContext * format_context,
		const char    *   filename,
		const AVCodecID   preferred_codec_id,
		const AVMediaType media_type);

	virtual ~b3EncoderStream();

	/**
	 * This method returns the internal frame counter and increments the frame
	 * counter by the given value.
	 *
	 * @param increment The increment of the frame counter.
	 * @return The frame counter before incrementing.
	 */
	[[nodiscard]]
	int64_t b3FrameNo(const unsigned increment = 1);

	/**
	 * This method computes the streams PTS and DTS using the
	 * av_packet_rescale_ts() method.
	 *
	 * @param packet The packet to rescale.
	 */
	void b3Rescale(const b3CodecPacket & packet) const;

	/**
	 * This method computes the streams PTS using the av_rescale_q()
	 * method and the internal frame counter.
	 *
	 * @return The actual frame PTS.
	 */
	[[nodiscard]]
	int64_t b3Rescale() const;

	/**
	 * This method returns the timepoint of the actual frame to be handled
	 * using the internal frame counter.
	 *
	 * @return The time point of the actual state.
	 */
	[[nodiscard]]
	b3_f64  b3Time() const;

	/**
	 * This method prepares the stream environment configured in the codec
	 * context structure.
	 *
	 * @return AV error code which should be consumed by the b3PrintErr()
	 * method.
	 */
	[[nodiscard]]
	int     b3Prepare();

	/**
	 * This method opens a codec context from the selected codec.
	 *
	 * @return AV error code which should be consumed by the b3PrintErr()
	 * method.
	 */
	[[nodiscard]]
	int     b3Open();

	/**
	 * This operator casts this instance to an AVCodecContext pointer.
	 */
	[[nodiscard]]
	inline operator AVCodecContext * () const
	{
		return m_CodecContext;
	}

	/**
	 * This method returns the stream index inside the container.
	 *
	 * @return The container stream index.
	 */
	[[nodiscard]]
	inline int b3GetIndex() const
	{
		return m_Stream->index;
	}

	/**
	 * This method returns the AV media type enumeration.
	 *
	 * @return The represented media type.
	 */
	[[nodiscard]]
	inline AVMediaType b3GetMediaType() const
	{
		return m_CodecContext->codec_type;
	}
};

/**
 * This class is a specialization of the b3EncoderStream class for audio
 * usage.
 */
class b3AudioStream : public b3EncoderStream
{
	static const unsigned SAMPLE_RATE = 8000;

public:
	/**
	 * This constructor initializes an audio media stream.
	 *
	 * @param format_context The description of the container format.
	 * @param filename The container filename.
	 * @param frames_per_second The video frames per second.
	 * @param buffer b3EncoderFrameBuffer AVFrame buffer to initialize.
	 */
	b3AudioStream(
		AVFormatContext * format_context,
		const char    *   filename,
		const b3_res      frames_per_second,
		b3CodecFrame   &  buffer);

private:
	int b3SuggestSampleRate();
};

/**
 * This class is a specialization of the b3EncoderStream class for video
 * usage.
 */
class b3VideoStream : public b3EncoderStream
{
public:
	/**
	 * @brief b3VideoStream
	 *
	 * @param format_context The description of the container format.
	 * @param filename The container filename.
	 * @param frames_per_second The video frames per second.
	 * @param xSize The video width.
	 * @param ySize The video height.
	 * @param pixel_format The encoded video format.
	 */
	b3VideoStream(
		AVFormatContext  *  format_context,
		const char     *    filename,
		const b3_res        frames_per_second,
		const b3_res        xSize,
		const b3_res        ySize,
		const AVPixelFormat pixel_format);
};

#endif
#endif
