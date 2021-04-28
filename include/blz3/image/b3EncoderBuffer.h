/*
**
**  $Filename:  b3EncoderBuffer.cc $
**  $Release:   Dortmund 2021 $
**  $Revision$
**  $Date$
**  $Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of an encoder buffer/packet.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_IMAGE_ENCODER_BUFFER_H
#define B3_IMAGE_ENCODER_BUFFER_H

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
 * This method represents a single AV frame. It is needed as input for an
 * encoding job. Mostly the RGB video input has to converted into YUV format and
 * then encoded as h.264.
 */
class b3EncoderFrameBuffer
{
	AVFrame * m_Frame      = nullptr;
	int       m_BufferSize = 0;
	int       m_SampleSize = 0;

public:
	explicit b3EncoderFrameBuffer();

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

	void b3InitAudio(const AVCodecContext * codec_context, const b3_res frames_per_second);

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
	 * This cast operator simply dereferences a AV frame pointer for convenience.
	 */
	inline AVPacket * operator -> ()
	{
		return &m_Packet;
	}

	/**
	 * This method marks the AV packet as a key frame.
	 */
	inline void key();
};

#endif
#endif
