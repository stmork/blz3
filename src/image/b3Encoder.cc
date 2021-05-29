/*
**
**  $Filename:  b3Encoder.cc $
**  $Release:   Dortmund 2021 $
**  $Revision$
**  $Date$
**  $Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of a MP4/h.264 video encoder.
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

#include "blz3/image/b3Encoder.h"
#include "blz3/system/b3LogBase.h"
#include "blz3/system/b3Runtime.h"

#ifdef HAVE_VIDEO_ENCODER

#include "b3EncoderStream.h"

/*************************************************************************
**                                                                      **
**                        AV registration singleton                     **
**                                                                      **
*************************************************************************/

b3CodecRegister::b3CodecRegister()
{
#if LIBAVFORMAT_VERSION_MAJOR <= 57
	av_register_all();
#endif
#if LIBAVCODEC_VERSION_MAJOR <= 57
	avcodec_register_all();
#endif
	av_log_set_callback(b3Log);
}

void b3CodecRegister::b3Log(void *, int level, const char * fmt, va_list vargs)
{
	char         message[8192];
	b3_log_level b3_level;

	switch (level)
	{
	case AV_LOG_QUIET:
		b3_level = B3LOG_NONE;
		break;

	case AV_LOG_INFO:
	case AV_LOG_VERBOSE:
	case AV_LOG_DEBUG:
		b3_level = B3LOG_DEBUG;
		break;

	case AV_LOG_TRACE:
		b3_level = B3LOG_FULL;
		break;

	default:
		b3_level = B3LOG_NORMAL;
		break;
	}

	vsnprintf(message, sizeof(message), fmt, vargs);
	b3PrintF(b3_level, "%s", message);
}

b3CodecRegister & b3CodecRegister::b3Instance()
{
	static b3CodecRegister instance;

	return instance;
}

void b3CodecRegister::b3PrepareCodecs()
{
#ifdef HAVE_AV_CODEC_ITERATE
	const AVCodec * codec;
	void      *     opaque = nullptr;

	while ((codec = av_codec_iterate(&opaque)) != nullptr)
#else
	for (AVCodec * codec = av_codec_next(nullptr);
		codec != nullptr;
		codec = av_codec_next(codec))
#endif
	{
		if (av_codec_is_encoder(codec) && (codec->type == AVMEDIA_TYPE_VIDEO))
		{
			switch (codec->id)
			{
			case AV_CODEC_ID_MPEG1VIDEO:
				m_Encoderinfo.bits.m_mpg1 = true;
				break;

			case AV_CODEC_ID_MPEG2VIDEO:
				m_Encoderinfo.bits.m_mpg2 = true;
				break;

			case AV_CODEC_ID_H264:
				m_Encoderinfo.bits.m_h264 = true;
				break;

			case AV_CODEC_ID_HEVC:
				m_Encoderinfo.bits.m_hevc = true;
				break;

			case AV_CODEC_ID_VP8:
				m_Encoderinfo.bits.m_vp8 = true;
				break;

			case AV_CODEC_ID_VP9:
				m_Encoderinfo.bits.m_vp9 = true;
				break;

			default:
				// Intentionally do nothing.
				break;
			}

			b3PrintF(B3LOG_FULL, "  AV codec %s\n", codec->name);
		}
	}
}

void b3CodecRegister::b3PrepareOutputFormats()
{
#ifdef HAVE_AV_MUXER_ITERATE
	const AVOutputFormat * oformat;
	void         *         opaque = nullptr;

	while ((oformat = av_muxer_iterate(&opaque)) != nullptr)
#else
	for (AVOutputFormat * oformat = av_oformat_next(nullptr);
		oformat != nullptr;
		oformat = av_oformat_next(oformat))
#endif
	{
		b3PrintF(B3LOG_FULL, "  AV output format: %s\n", oformat->long_name);

		if (oformat->video_codec == AV_CODEC_ID_H265)
		{
			b3PrintF(B3LOG_FULL, "     Container for h.265 found: %s\n", oformat->long_name);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        AV MP4/h.264 encoder implementation           **
**                                                                      **
*************************************************************************/

const AVPixelFormat b3MovieEncoder::m_SrcFormat = AV_PIX_FMT_RGB32;
const AVPixelFormat b3MovieEncoder::m_DstFormat = AV_PIX_FMT_YUV420P;

const b3ConstantMap<AVMediaType> b3MovieEncoder::m_MediaMap
{
	B3_CONSTANT(AVMEDIA_TYPE_VIDEO),
	B3_CONSTANT(AVMEDIA_TYPE_AUDIO),
	B3_CONSTANT(AVMEDIA_TYPE_DATA),
	B3_CONSTANT(AVMEDIA_TYPE_SUBTITLE),
	B3_CONSTANT(AVMEDIA_TYPE_ATTACHMENT)
};

/**
 *
 * @see https://superuser.com/questions/579008/add-1-second-of-silence-to-audio-through-ffmpeg
 * @code
ffmpeg -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -t 2 silence.ac3
@endcode
 *
 * @see https://superuser.com/questions/590201/add-audio-to-video-using-ffmpeg
 * @code
ffmpeg -i test-video.mp4 -i silence.ac3 -c:v copy output.mp4
@endcode
*/

b3MovieEncoder::b3MovieEncoder(
	const char  *  filename,
	const b3Tx  *  tx,
	const b3_res   frames_per_second,
	const bool     use_audio) :
	m_RgbFrame(tx, m_SrcFormat),
	m_YuvFrame(tx, m_DstFormat)
{
	int error = 0;

	m_xSize = tx->xSize;
	m_ySize = tx->ySize;

	m_SwsCtx = sws_getContext(
			m_xSize, m_ySize, m_SrcFormat,
			m_xSize, m_ySize, m_DstFormat,
			SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
	if (m_SwsCtx == nullptr)
	{
		b3PrintF(B3LOG_NORMAL, "Scaling context not available!\n");
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}

	error = avformat_alloc_output_context2(&m_FormatContext, nullptr, nullptr, filename);
	b3PrintErr("Format context allocation", error);

	m_VideoStream  = new b3VideoStream(m_FormatContext, filename, frames_per_second, m_xSize, m_ySize, m_DstFormat);
	if (use_audio)
	{
		m_AudioStream  = new b3AudioStream(m_FormatContext, filename, frames_per_second, m_AudioFrame);
	}

	b3PrepareStream(m_VideoStream);
	b3PrepareStream(m_AudioStream);

	if ((m_FormatContext->oformat->flags & AVFMT_NOFILE) == 0)
	{
		error = avio_open(&m_FormatContext->pb, filename, AVIO_FLAG_WRITE);
		b3PrintErr("File opening", error);
	}

	av_dict_set(&m_FormatContext->metadata, "Software", b3Runtime::b3GetProduct(), 0);
#ifdef B3_VERSION
	av_dict_set(&m_FormatContext->metadata, "Version", B3_VERSION, 0);
#endif
	av_dict_set(&m_FormatContext->metadata, "Author", "Steffen A. Mork", 0);
	av_dict_set(&m_FormatContext->metadata, "comment", b3Runtime::b3GetProduct(), 0);

	error = avformat_write_header(m_FormatContext, nullptr);
	b3PrintErr("Header writing", error);

	av_dump_format(m_FormatContext, 0, filename, 1);
}

b3MovieEncoder::~b3MovieEncoder()
{
	b3Finish();
	if (!(m_FormatContext->oformat->flags & AVFMT_NOFILE))
	{
		avio_close(m_FormatContext->pb);
	}
	b3Free();
}

bool b3MovieEncoder::b3AddFrame(const b3Tx * tx)
{
	bool success = false;

	if (m_VideoStream != nullptr)
	{
		success = b3AddVideoFrame(tx);

		if (m_AudioStream != nullptr)
		{
			b3PrintF(B3LOG_DEBUG, "at=%2.03f  vt=%2.03f\n",
				m_AudioStream->b3Time(), m_VideoStream->b3Time());

			while ((m_AudioStream->b3Time() < m_VideoStream->b3Time()) && success)
			{
				success = success && b3AddAudioFrame();
			}
		}
	}
	return success;
}

void b3MovieEncoder::b3PrepareStream(b3EncoderStream * stream)
{
	if (stream != nullptr)
	{
		int error = 0;

		error = stream->b3Open();
		b3PrintErr("Codec opening", error);

		error = stream->b3Prepare();
		b3PrintErr("Converting codec context parameter", error);
	}
}

bool b3MovieEncoder::b3AddVideoFrame(const b3Tx * tx)
{
	if ((tx->xSize != m_xSize) || (tx->ySize != m_ySize) || (m_YuvFrame == nullptr))
	{
		return false;
	}

	b3_tx_data     ptr  = m_RgbFrame->data[0];
	b3_pkd_color * lPtr = ptr;

	for (b3_res y = 0; y < m_ySize; y++)
	{
		tx->b3GetRow(lPtr, y);
		lPtr += m_xSize;
	}

	// Not actually scaling anything, but just converting
	// the RGB data to YUV and store it in yuvpic.
	sws_scale(m_SwsCtx,
		m_RgbFrame->data, m_RgbFrame->linesize, 0, m_ySize,
		m_YuvFrame->data, m_YuvFrame->linesize);

	int error = b3SendFrame(m_VideoStream, m_YuvFrame);

	b3PrintErr("Video frame writing", error, false);
	return (error >= 0) || (error == AVERROR(EAGAIN));
}

bool b3MovieEncoder::b3AddAudioFrame()
{
	if (m_AudioFrame == nullptr)
	{
		return false;
	}

	bzero(m_AudioFrame->data[0], m_AudioFrame->linesize[0]);
	int error = b3SendFrame(m_AudioStream, m_AudioFrame);

	b3PrintErr("Audio frame writing", error, false);
	return (error >= 0) || (error == AVERROR(EAGAIN));
}

int b3MovieEncoder::b3SendFrame(b3EncoderStream * stream, AVFrame * frame)
{
	if (stream == nullptr)
	{
		return AVERROR_STREAM_NOT_FOUND;
	}

	const int64_t     pts        = stream->b3Rescale();
	const b3_f64      t          = stream->b3Time();
	const AVMediaType media_type = stream->b3GetMediaType();
	int64_t           frame_no   = stream->b3FrameNo();

	if (frame != nullptr)
	{
		frame->pts = frame_no;
	}

	b3PrintF(B3LOG_DEBUG, "###############################################\n");
	b3PrintF(B3LOG_DEBUG, "%-20.20s ==== t=%2.03f pts: %6ld %p\n",
		m_MediaMap.b3Get(media_type).c_str(), t, pts, frame);

	int error = avcodec_send_frame(*stream, frame);
	b3PrintErr("Frame send to encoder", error, false);

	do
	{
		b3CodecPacket pkt;

		error = avcodec_receive_packet(*stream, pkt);
		b3PrintErr("Packet receive from encoder", error, false);
		if (error >= 0)
		{
			pkt->stream_index = stream->b3GetIndex();
			stream->b3Rescale(pkt);
			b3PrintF(B3LOG_DEBUG, "========================= no.: %4ld pts: %6ld  dts: %6ld  flags: %02x\n",
				frame_no, pkt->pts, pkt->dts, pkt->flags);
			error = av_interleaved_write_frame(m_FormatContext, pkt);
			b3PrintErr("Packet write", error, true);
		}
	}
	while (error == 0);

	return error;
}

void b3MovieEncoder::b3Finish()
{
	int result = 0;
	int error  = 0;

	do
	{
		result = b3SendFrame(m_VideoStream);
	}
	while (result == 0);
	do
	{
		result = b3SendFrame(m_AudioStream);
	}
	while (result == 0);

	error = av_write_trailer(m_FormatContext);
	b3PrintErr("Trailer writing", error, false);
}

void b3MovieEncoder::b3Free()
{
	// Freeing all the allocated memory:
	delete m_VideoStream;
	delete m_AudioStream;

	avformat_free_context(m_FormatContext);
	sws_freeContext(m_SwsCtx);
	m_FormatContext = nullptr;
	m_AudioStream   = nullptr;
	m_VideoStream   = nullptr;
	m_SwsCtx        = nullptr;
}

void b3MovieEncoder::b3PrintErr(
	const char * description,
	const int    error,
	const bool   throw_exception)
{
	char       message[256];
	const bool is_success = (error >= 0) || (error == AVERROR_EOF) || (error == AVERROR(EAGAIN));

	av_strerror(error, message, sizeof(message));
	b3PrintF(is_success ? B3LOG_DEBUG : B3LOG_NORMAL,
		"%s: %s (%d)\n", description, message, error);

	if (!is_success && (throw_exception))
	{
		b3Free();
		B3_THROW(b3TxException, B3_TX_STREAMING_ERROR);
	}
}

#endif
