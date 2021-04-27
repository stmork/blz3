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

#ifdef HAVE_VIDEO_ENCODER

/*************************************************************************
**                                                                      **
**                        AV registration singleton                     **
**                                                                      **
*************************************************************************/

b3CodecRegister::b3CodecRegister()
{
	av_register_all();
	avcodec_register_all();
	av_log_set_callback(b3Log);
}

void b3CodecRegister::b3Log(void *, int level, const char * fmt, va_list vargs)
{
	char message[8192];
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
	for (AVCodec * codec = av_codec_next(nullptr);
		codec != nullptr;
		codec = av_codec_next(codec))
	{
		if (av_codec_is_encoder(codec) && (codec->type == AVMEDIA_TYPE_VIDEO))
		{
			switch (codec->id)
			{
			case AV_CODEC_ID_MPEG1VIDEO:
				m_Encoderinfo.bits.m_mpg = true;
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

			b3PrintF(B3LOG_FULL, "  AV %s\n", codec->name);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        AV frame buffer implementation                **
**                                                                      **
*************************************************************************/

b3EncoderFrameBuffer::b3EncoderFrameBuffer()
{
	m_Frame = av_frame_alloc();
}

void b3EncoderFrameBuffer::b3InitAudio(
		const AVCodecContext * codec_context,
		const b3_res           frames_per_second)
{
	m_Frame->format         = codec_context->sample_fmt;
	m_Frame->sample_rate    = codec_context->sample_rate;
	m_Frame->channel_layout = codec_context->channel_layout;
	m_Frame->channels       = av_get_channel_layout_nb_channels(m_Frame->channel_layout);
	m_Frame->nb_samples     = codec_context->sample_rate * m_Frame->channels / frames_per_second;

	buffer_size = av_samples_get_buffer_size(nullptr,
			m_Frame->channels,
			codec_context->sample_rate / frames_per_second,
			codec_context->sample_fmt, 0);
	sample_size = av_get_bytes_per_sample (codec_context->sample_fmt);

	av_frame_get_buffer(m_Frame, 0);
	bzero(m_Frame->data[0], m_Frame->linesize[0]);
}

b3EncoderFrameBuffer::b3EncoderFrameBuffer(const b3Tx * tx, int format)
{
	m_Frame = av_frame_alloc();
	m_Frame->format = format;
	m_Frame->width  = tx->xSize;
	m_Frame->height = tx->ySize;
	av_frame_get_buffer(m_Frame, 0);
}

b3EncoderFrameBuffer::~b3EncoderFrameBuffer()
{
	av_frame_free(&m_Frame);
}

/*************************************************************************
**                                                                      **
**                        AV stream packet implementation               **
**                                                                      **
*************************************************************************/

b3EncoderPacket::b3EncoderPacket()
{
	av_init_packet(&m_Packet);
	m_Packet.data = nullptr;
	m_Packet.size = 0;
}

b3EncoderPacket::~b3EncoderPacket()
{
	av_packet_unref(&m_Packet);
}

void b3EncoderPacket::key()
{
	m_Packet.flags |= AV_PKT_FLAG_KEY;
}

/*************************************************************************
**                                                                      **
**                        AV stream/codec implementation                **
**                                                                      **
*************************************************************************/

class b3MovieStream
{
	unsigned                   m_FrameNo    =   0;

protected:
	AVRational                 m_FrameDuration;

public:
	AVCodecID        codec_id       = AV_CODEC_ID_PROBE;
	AVCodec     *    m_Codec        = nullptr;
	AVStream    *    m_Stream       = nullptr;
	AVCodecContext * m_CodecContext = nullptr;

public:
	b3MovieStream(
		AVFormatContext * format_context,
		const char    *   filename,
		const b3_res      frames_per_second,
		const AVMediaType media_type)
	{
		codec_id = av_guess_codec(format_context->oformat, nullptr, filename, nullptr, media_type);

		m_FrameDuration.num =  1;
		m_FrameDuration.den = frames_per_second;

		m_Codec        = avcodec_find_encoder(codec_id);
		if (m_Codec == nullptr)
		{
			B3_THROW(b3TxException, B3_TX_STREAMING_ERROR);
		}
		m_Stream       = avformat_new_stream(format_context, m_Codec);
		m_CodecContext = avcodec_alloc_context3(m_Codec);
	}

	virtual ~b3MovieStream()
	{
		avcodec_free_context(&m_CodecContext);
		m_Codec  = nullptr;
		m_Stream = nullptr;
	}

	int64_t b3Pts()
	{
		return m_Stream->time_base.den * m_FrameNo++ /
			(m_FrameDuration.den * m_Stream->time_base.num);
	}

	inline operator AVCodecContext * ()
	{
		return m_CodecContext;
	}
};

class b3AudioStream : public b3MovieStream
{
public:
	static const unsigned SAMPLE_RATE = 8000;

	b3AudioStream(
		AVFormatContext * format_context,
		const char    *   filename,
		const b3_res      frames_per_second,
		b3EncoderFrameBuffer & buffer) :
		b3MovieStream(format_context, filename, frames_per_second, AVMEDIA_TYPE_AUDIO)
	{
		m_CodecContext->sample_rate    = b3SuggestSampleRate();
		m_CodecContext->bit_rate       = 64000;
		m_CodecContext->sample_fmt     = m_Codec->sample_fmts[0];
		m_CodecContext->channel_layout = AV_CH_LAYOUT_MONO;
		m_CodecContext->channels       = av_get_channel_layout_nb_channels(
				m_CodecContext->channel_layout);
		m_CodecContext->frame_size     = av_samples_get_buffer_size(nullptr,
				m_CodecContext->channels,
				m_CodecContext->sample_rate / frames_per_second,
				m_CodecContext->sample_fmt, 0);

		buffer.b3InitAudio(m_CodecContext, frames_per_second);
	}

private:
	int b3SuggestSampleRate()
	{
		int sample_rate = SAMPLE_RATE;

		if (m_Codec->supported_samplerates != nullptr)
		{
			for (const int *p = m_Codec->supported_samplerates; *p != 0; p++)
			{
				sample_rate = std::min(sample_rate, *p);
			}
		}
		return sample_rate;
	}
};

class b3VideoStream : public b3MovieStream
{
public:
	b3VideoStream(
		AVFormatContext  *  format_context,
		const char     *    filename,
		const b3_res        frames_per_second,
		const b3_res        xSize,
		const b3_res        ySize,
		const AVPixelFormat pixel_format) :
		b3MovieStream(format_context, filename, frames_per_second, AVMEDIA_TYPE_VIDEO)
	{
		// Prepare codec context.
		m_CodecContext->width                 = xSize;
		m_CodecContext->height                = ySize;
		m_CodecContext->pix_fmt               = pixel_format;
		m_CodecContext->sample_aspect_ratio   = av_make_q(1, 1);
		m_CodecContext->color_range           = AVCOL_RANGE_MPEG;
		m_CodecContext->bit_rate              = xSize * ySize * 3;
		m_CodecContext->time_base             = m_FrameDuration;
		m_CodecContext->framerate.num         = m_FrameDuration.den;
		m_CodecContext->framerate.den         = m_FrameDuration.num;
		m_CodecContext->max_b_frames          =  1;
		m_CodecContext->gop_size              = 10;
		m_CodecContext->strict_std_compliance = FF_COMPLIANCE_VERY_STRICT;

		// Prepare codec specific parameters.
		switch (codec_id)
		{
		case AV_CODEC_ID_MPEG2VIDEO:
			m_CodecContext->profile = FF_PROFILE_MPEG2_MAIN;
			break;

		case AV_CODEC_ID_MPEG4:
			m_CodecContext->profile = FF_PROFILE_MPEG4_MAIN;
			break;

		case AV_CODEC_ID_H264:
			av_opt_set(m_CodecContext, "preset", "medium", 0);
			av_opt_set(m_CodecContext, "tune", "zerolatency", 0);
			av_opt_set(m_CodecContext, "crf", "23", 0);
			m_CodecContext->profile = FF_PROFILE_H264_MAIN;
			break;

		case AV_CODEC_ID_HEVC:
			av_opt_set(m_CodecContext, "preset", "medium", 0);
			av_opt_set(m_CodecContext, "tune", "zerolatency", 0);
			av_opt_set(m_CodecContext, "crf", "28", 0);
			m_CodecContext->profile = FF_PROFILE_HEVC_MAIN;
			break;

		case AV_CODEC_ID_VP9:
			m_CodecContext->profile = FF_PROFILE_VP9_0;
			break;

		default:
			// Intentionally do nothing!
			break;
		}

		// Prepare stream.
		m_Stream->avg_frame_rate = m_CodecContext->framerate;
	}
};

/*************************************************************************
**                                                                      **
**                        AV MP4/h.264 encoder implementation           **
**                                                                      **
*************************************************************************/

const AVPixelFormat b3MovieEncoder::m_SrcFormat = AV_PIX_FMT_BGRA;
const AVPixelFormat b3MovieEncoder::m_DstFormat = AV_PIX_FMT_YUV420P;

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

b3MovieEncoder::b3MovieEncoder(const char * filename, const b3Tx * tx, const b3_res frames_per_second) :
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

	error = avformat_alloc_output_context2(&m_FormatContext, nullptr, nullptr, filename);
	b3PrintErr("Format context allocation", error);

	m_VideoStream  = new b3VideoStream(m_FormatContext, filename, frames_per_second, m_xSize, m_ySize, m_DstFormat);
//	m_AudioStream  = new b3AudioStream(m_FormatContext, filename, frames_per_second, m_AudioFrame);

	b3PrepareStream(m_VideoStream);
	b3PrepareStream(m_AudioStream);

	if ((m_FormatContext->oformat->flags & AVFMT_NOFILE) == 0)
	{
		error = avio_open(&m_FormatContext->pb, filename, AVIO_FLAG_WRITE);
		b3PrintErr("File opening", error);
	}

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
	bool success = true;

	if (m_VideoStream != nullptr)
	{
		success = success && b3AddVideoFrame(tx);
	}
	if (m_AudioStream != nullptr)
	{
		success = success && b3AddAudioFrame();
	}
	return success;
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

void b3MovieEncoder::b3PrepareStream(b3MovieStream * stream)
{
	if (stream != nullptr)
	{
		int error = 0;

		error = avcodec_parameters_from_context(stream->m_Stream->codecpar, *stream);
		b3PrintErr("Converting codec context parameter", error);

		error = avcodec_open2(*stream, stream->m_Codec, nullptr);
		b3PrintErr("Codec opening", error);
	}
}

bool b3MovieEncoder::b3AddVideoFrame(const b3Tx * tx)
{
	if ((tx->xSize != m_xSize) || (tx->ySize != m_ySize) || (m_RgbFrame == nullptr))
	{
		return false;
	}

	b3_tx_data     ptr  = m_RgbFrame->data[0];
	b3_pkd_color * lPtr = ptr;

	for (b3_res y = 0; y < m_ySize; y++)
	{
		b3_pkd_color row[m_xSize];

		tx->b3GetRow(row, y);
		for (b3_res x = 0; x < m_xSize; x++)
		{
			*lPtr++ = row[x];
		}
	}

	// Not actually scaling anything, but just converting
	// the RGB data to YUV and store it in yuvpic.
	sws_scale(m_SwsCtx,
		m_RgbFrame->data, m_RgbFrame->linesize, 0, m_ySize,
		m_YuvFrame->data, m_YuvFrame->linesize);

	int error = b3SendFrame(m_VideoStream, m_YuvFrame);

	b3PrintErr("Video frame writing", error, false);
	return error >= 0;
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
	return error >= 0;
}

int b3MovieEncoder::b3SendFrame(b3MovieStream * stream, AVFrame * frame)
{
	if (stream == nullptr)
	{
		return AVERROR_STREAM_NOT_FOUND;
	}
	if (frame != nullptr)
	{
		frame->pts = stream->b3Pts();
	}

	int error = avcodec_send_frame(*stream, frame);
	b3PrintErr("Frame send to encoder", error, false);

//	if (error >= 0)
	{
		b3EncoderPacket pkt;

		error = avcodec_receive_packet(*stream, pkt);
		b3PrintErr("Frame received from encoder", error, false);
		if (b3Delayed(error))
		{
			return true;
		}
		if (error >= 0)
		{
			pkt->stream_index = stream->m_Stream->index;
			error = av_interleaved_write_frame(m_FormatContext, pkt);
		}
	}
	return error;
}

void b3MovieEncoder::b3PrintErr(
	const char * description,
	const int    err,
	const bool   throw_exception)
{
	char message[256];

	av_strerror(err, message, sizeof(message));
	b3PrintF(err >= 0 ? B3LOG_DEBUG : B3LOG_NORMAL,
		"%s: %s (%d)\n", description, message, err);

	if ((err < 0) && (throw_exception))
	{
		b3Free();
		B3_THROW(b3TxException, B3_TX_STREAMING_ERROR);
	}
}

#endif
