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
**                        AV MP4/h.264 encoder implementation           **
**                                                                      **
*************************************************************************/

const AVPixelFormat b3MovieEncoder::m_SrcFormat = AV_PIX_FMT_BGRA;
const AVPixelFormat b3MovieEncoder::m_DstFormat = AV_PIX_FMT_YUV420P;

class b3MovieStream
{
public:
	AVCodecID        codec_id       = AV_CODEC_ID_PROBE;
	AVCodec     *    m_Codec        = nullptr;
	AVStream    *    m_Stream       = nullptr;
	AVCodecContext * m_CodecContext = nullptr;

public:
	b3MovieStream(AVFormatContext * format_context, const char * filename, AVMediaType media_type)
	{
		codec_id = av_guess_codec(format_context->oformat, nullptr, filename, nullptr, media_type);

		m_Codec        = avcodec_find_encoder(codec_id);
		m_Stream       = avformat_new_stream(format_context, m_Codec);
		m_CodecContext = avcodec_alloc_context3(m_Codec);
	}

	virtual ~b3MovieStream()
	{
		avcodec_free_context(&m_CodecContext);
		m_Codec  = nullptr;
		m_Stream = nullptr;
	}

	inline operator AVCodecContext * ()
	{
		return m_CodecContext;
	}
};

class b3AudioStream : public b3MovieStream
{
public:
	b3AudioStream(AVFormatContext * format_context, const char * filename) :
		b3MovieStream(format_context, filename, AVMEDIA_TYPE_AUDIO)
	{
		m_CodecContext->sample_rate = 44100 / 25;
		m_CodecContext->sample_fmt  = AV_SAMPLE_FMT_S16;
		m_CodecContext->channels    = 2;
		m_CodecContext->channel_layout = 2;
	}
};

class b3VideoStream : public b3MovieStream
{
	AVRational                 m_FrameDuration;

public:
	b3VideoStream(
		AVFormatContext * format_context,
		const char * filename,
		const b3_res m_xSize,
		const b3_res m_ySize,
		const AVPixelFormat pixel_format,
		const b3_res frames_per_second) :
		b3MovieStream(format_context, filename, AVMEDIA_TYPE_VIDEO)
	{
		m_FrameDuration.num =  1;
		m_FrameDuration.den = frames_per_second;

		// Prepare codec context.
		m_CodecContext->width                 = m_xSize;
		m_CodecContext->height                = m_ySize;
		m_CodecContext->pix_fmt               = pixel_format;
		m_CodecContext->sample_aspect_ratio   = av_make_q(1, 1);
		m_CodecContext->color_range           = AVCOL_RANGE_MPEG;
		m_CodecContext->bit_rate              = m_xSize * m_ySize * 3;
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

b3MovieEncoder::b3MovieEncoder(const char * filename, const b3Tx * tx, const b3_res frames_per_second) :
	m_RgbFrame(tx, m_SrcFormat),
	m_YuvFrame(tx, m_DstFormat)
{
	int error = 0;

	m_xSize = tx->xSize;
	m_ySize = tx->ySize;
	m_FrameDuration.num =  1;
	m_FrameDuration.den = frames_per_second;

	m_SwsCtx = sws_getContext(
			m_xSize, m_ySize, m_SrcFormat,
			m_xSize, m_ySize, m_DstFormat,
			SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

	error = avformat_alloc_output_context2(&m_FormatContext, nullptr, nullptr, filename);
	b3PrintErr("Format context allocation", error);

	m_OutputFormat = m_FormatContext->oformat;
//	m_AudioStream  = new b3AudioStream(m_FormatContext, filename);
	m_VideoStream  = new b3VideoStream(m_FormatContext, filename, m_xSize, m_ySize, m_DstFormat, frames_per_second);

	if (m_VideoStream != nullptr)
	{
		error = avcodec_parameters_from_context(m_VideoStream->m_Stream->codecpar, *m_VideoStream);
		b3PrintErr("Converting codec context parameter", error);

		error = avcodec_open2(*m_VideoStream, m_VideoStream->m_Codec, nullptr);
		b3PrintErr("Codec opening", error);

		av_dump_format(m_FormatContext, m_VideoStream->m_Stream->index, filename, 1);
	}

	if (m_AudioStream != nullptr)
	{
		error = avcodec_parameters_from_context(m_AudioStream->m_Stream->codecpar, *m_AudioStream);
		b3PrintErr("Converting codec context parameter", error);

		error = avcodec_open2(*m_AudioStream, m_AudioStream->m_Codec, nullptr);
		b3PrintErr("Codec opening", error);

		av_dump_format(m_FormatContext, m_AudioStream->m_Stream->index, filename, 1);
	}

	if ((m_OutputFormat->flags & AVFMT_NOFILE) == 0)
	{
		error = avio_open(&m_FormatContext->pb, filename, AVIO_FLAG_WRITE);
		b3PrintErr("File opening", error);
	}

	error = avformat_write_header(m_FormatContext, nullptr);
	b3PrintErr("Header writing", error);
}

b3MovieEncoder::~b3MovieEncoder()
{
	b3Finish();
	if (!(m_OutputFormat->flags & AVFMT_NOFILE))
	{
		avio_close(m_FormatContext->pb);
	}
	b3Free();
}

bool b3MovieEncoder::b3AddFrame(const b3Tx * tx)
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

	// The PTS of the frame are just in a reference unit,
	// unrelated to the format we are using. We set them,
	// for instance, as the corresponding frame number.
	m_YuvFrame->pts = m_VideoStream->m_Stream->time_base.den * m_iFrame++ /
		(m_FrameDuration.den * m_VideoStream->m_Stream->time_base.num);

	int error = avcodec_send_frame(*m_VideoStream, m_YuvFrame);
	if (error >= 0)
	{
		b3EncoderPacket pkt;

		error = avcodec_receive_packet(*m_VideoStream, pkt);
		if ((error == AVERROR(EAGAIN)) || (error == AVERROR_EOF))
		{
			return true;
		}
		if (error >= 0)
		{
			error = av_interleaved_write_frame(m_FormatContext, pkt);
		}
	}

	b3PrintErr("Frame writing", error, false);
	return error >= 0;
}

void b3MovieEncoder::b3Finish()
{
	int result = 0;
	int error  = 0;

	do
	{
		b3EncoderPacket pkt;

		avcodec_send_frame(*m_VideoStream, nullptr);
		result = avcodec_receive_packet(*m_VideoStream, pkt);
		if (result == 0)
		{
			error = av_interleaved_write_frame(m_FormatContext, pkt);
			b3PrintErr("Frame writing", error, false);
		}
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
	m_AudioStream  = nullptr;
	m_VideoStream  = nullptr;
	m_OutputFormat = nullptr;
	m_SwsCtx       = nullptr;
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
