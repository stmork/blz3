/*
**
**	$Filename:	b3MovieTest.cc $
**	$Release:	Dortmund 2024 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of a MP4/h.264 video encoder.
**
**      (C) Copyright 2001 - 2024  Steffen A. Mork
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

#include "blz3/base/b3ConstantMap.h"
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
			unsigned m_mpg1 : 1;
			unsigned m_mpg2 : 1;
			unsigned m_h264 : 1;
			unsigned m_hevc : 1;
			unsigned m_vp8  : 1;
			unsigned m_vp9  : 1;
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
	void b3PrepareOutputFormats();
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

	b3CodecFrame               m_RgbFrame;
	b3CodecFrame               m_YuvFrame;
	b3CodecFrame               m_AudioFrame;

	static const unsigned      m_kbit_rate = 900;
	static const AVPixelFormat m_SrcFormat;
	static const AVPixelFormat m_DstFormat;

	b3_res                     m_xSize;
	b3_res                     m_ySize;

	static const b3ConstantMap<AVMediaType> m_MediaMap;

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
	 * @param use_audio True if an silent audio stream should be included.
	 */
	explicit b3MovieEncoder(
		const char * filename,
		const b3Tx * tx,
		const b3_res frames_per_second = 25,
		const bool   use_audio         = false);

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
	 * This method prepares and opens a media stream. Since the b3EncoderStream
	 * and b3EncoderFrame classes both don't have error handling this is done
	 * inside this method.
	 *
	 * @param stream The stream to prepare and open.
	 * @throws b3TxException If the stream cannot prepared or opened.
	 */
	void b3PrepareStream(b3EncoderStream * stream);

	/**
	 * This method adds a video frame to the video stream using the
	 * b3SendFrame() method.
	 *
	 * @param tx The b3Tx class containing the image to send.
	 * @return True on success.
	 */
	bool b3AddVideoFrame(const b3Tx * tx);

	/**
	 * This method adds an empty audio frame to the audio stream using the
	 * b3SendFrame() method.
	 *
	 * @return True on success.
	 */
	bool b3AddAudioFrame();

	/**
	 * This method simply sends an AVFrame to a given media stream.
	 *
	 * @param stream The stream to send the frame.
	 * @param frame The frame to send.
	 * @return True on success.
	 */
	int  b3SendFrame(b3EncoderStream * stream, AVFrame * frame = nullptr);

	/**
	 * This method encodes delayed packes at the end of the video stream.
	 */
	void b3Finish();

	/**
	 * This method frees all resources allocated by this video encoder.
	 */
	void b3Free();

	/**
	 * This Method prints a purpose and depending on the error code its
	 * error message as clear text. The log level depends on the given error
	 * code. Non negative error codes leeds into successful debug logging
	 * whereas negative error codes leeds into failed normal logging. The
	 * method may throw an b3TxException on failed state.
	 *
	 * @param description The purpose of action done.
	 * @param error The error code of that action.
	 * @param throw_exception If an exception should be thrown in failed state.
	 */
	void b3PrintErr(
		const char * description,
		const int    error,
		const bool   throw_exception = true);
};

#endif
#endif
