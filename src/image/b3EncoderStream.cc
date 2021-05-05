/*
**
**  $Filename:  b3Encoder.cc $
**  $Release:   Dortmund 2021 $
**  $Revision$
**  $Date$
**  $Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of an encoder streams.
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

#include "b3EncoderStream.h"

#ifdef HAVE_VIDEO_ENCODER

#include "blz3/image/b3EncoderBuffer.h"

/*************************************************************************
**                                                                      **
**                        AV stream/codec implementation                **
**                                                                      **
*************************************************************************/

b3EncoderStream::b3EncoderStream(
	AVFormatContext * format_context,
	const char    *   filename,
	const b3_res      frames_per_second B3_UNUSED,
	const AVCodecID   preferred_codec_id,
	const AVMediaType media_type)
{
	if (avformat_query_codec(format_context->oformat, preferred_codec_id, FF_COMPLIANCE_VERY_STRICT) > 0)
	{
		m_CodecId = preferred_codec_id;
	}
	else
	{
		m_CodecId = av_guess_codec(format_context->oformat, nullptr, filename, nullptr, media_type);
	}

	m_Codec        = avcodec_find_encoder(m_CodecId);
	if (m_Codec == nullptr)
	{
		B3_THROW(b3TxException, B3_TX_STREAMING_ERROR);
	}
	m_Stream       = avformat_new_stream(format_context, m_Codec);
	m_CodecContext = avcodec_alloc_context3(m_Codec);

	// Populate extradata if encoder has support.
	if (format_context->oformat->flags & AVFMT_GLOBALHEADER)
	{
		m_CodecContext->flags         |= CODEC_FLAG_GLOBAL_HEADER;
	}
}

b3EncoderStream::~b3EncoderStream()
{
	avcodec_close(m_CodecContext);
	avcodec_free_context(&m_CodecContext);
	m_Codec  = nullptr;
	m_Stream = nullptr;
}

int64_t b3EncoderStream::b3FrameNo(const unsigned increment)
{
	const int64_t frame_no = m_FrameNo;

	m_FrameNo += increment;
	return frame_no;
}

int64_t b3EncoderStream::b3Pts(const unsigned increment)
{
	m_Pts =
		(m_CodecContext->time_base.num * m_Stream->time_base.den * m_FrameNo) /
		(m_CodecContext->time_base.den * m_Stream->time_base.num);
	m_FrameNo += increment;

	return m_Pts;
}

void b3EncoderStream::b3Rescale(AVPacket * packet) const
{
	av_packet_rescale_ts(packet, m_CodecContext->time_base, m_Stream->time_base);
}

int64_t b3EncoderStream::b3Rescale() const
{
	return av_rescale_q(m_FrameNo, m_CodecContext->time_base, m_Stream->time_base);
}

b3_f64 b3EncoderStream::b3Time() const
{
	b3_f64 result = m_FrameNo;

	result *= m_CodecContext->time_base.num;
	result /= m_CodecContext->time_base.den;

	return result;
}

int b3EncoderStream::b3Prepare()
{
	return avcodec_parameters_from_context(m_Stream->codecpar, m_CodecContext);
}

int b3EncoderStream::b3Open()
{
	return avcodec_open2(m_CodecContext, m_Codec, nullptr);
}

/*************************************************************************
**                                                                      **
**                        AV stream/codec implementation (Audio)        **
**                                                                      **
*************************************************************************/

b3AudioStream::b3AudioStream(
	AVFormatContext    *   format_context,
	const char      *      filename,
	const b3_res           frames_per_second,
	b3EncoderFrameBuffer & buffer) :
	b3EncoderStream(format_context, filename, frames_per_second, AV_CODEC_ID_AAC, AVMEDIA_TYPE_AUDIO)
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

	m_FrameDuration.num = m_CodecContext->frame_size /
		(av_get_bytes_per_sample (m_CodecContext->sample_fmt) *
			m_CodecContext->channels);
	m_FrameDuration.den = m_CodecContext->sample_rate;
	m_CodecContext->time_base             = m_FrameDuration;
	m_CodecContext->framerate.num         = m_FrameDuration.den;
	m_CodecContext->framerate.den         = m_FrameDuration.num;
}

int b3AudioStream::b3SuggestSampleRate()
{
	int sample_rate = SAMPLE_RATE;

	if (m_Codec->supported_samplerates != nullptr)
	{
		sample_rate = std::max(sample_rate, *m_Codec->supported_samplerates);

		for (const int * p = m_Codec->supported_samplerates; *p != 0; p++)
		{
			sample_rate = std::min(sample_rate, *p);
		}
	}
	return sample_rate;
}

/*************************************************************************
**                                                                      **
**                        AV stream/codec implementation (Video)        **
**                                                                      **
*************************************************************************/

b3VideoStream::b3VideoStream(
	AVFormatContext  *  format_context,
	const char     *    filename,
	const b3_res        frames_per_second,
	const b3_res        xSize,
	const b3_res        ySize,
	const AVPixelFormat pixel_format) :
	b3EncoderStream(format_context, filename, frames_per_second,
		AV_CODEC_ID_PROBE, AVMEDIA_TYPE_VIDEO)
{
	m_FrameDuration.num =  1;
	m_FrameDuration.den = frames_per_second;

	// Prepare codec context.
	m_CodecContext->width                  = xSize;
	m_CodecContext->height                 = ySize;
	m_CodecContext->pix_fmt                = pixel_format;
	m_CodecContext->sample_aspect_ratio    = av_make_q(1, 1);
	m_CodecContext->bit_rate               = xSize * ySize * 3;
	m_CodecContext->time_base              = m_FrameDuration;
	m_CodecContext->framerate.num          = m_FrameDuration.den;
	m_CodecContext->framerate.den          = m_FrameDuration.num;
	m_CodecContext->max_b_frames           =  2;
	m_CodecContext->gop_size               = 10;
	m_CodecContext->strict_std_compliance  = FF_COMPLIANCE_VERY_STRICT;

	// Prepare color values
	m_CodecContext->color_primaries        = AVCOL_PRI_BT709;
	m_CodecContext->color_trc              = AVCOL_TRC_BT709;
	m_CodecContext->colorspace             = AVCOL_SPC_BT709;
	m_CodecContext->color_range            = AVCOL_RANGE_MPEG;
	m_CodecContext->chroma_sample_location = AVCHROMA_LOC_CENTER;

	// Prepare codec specific parameters.
	switch (m_CodecId)
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
		m_CodecContext->profile   = FF_PROFILE_H264_MAIN;
		break;

	case AV_CODEC_ID_HEVC:
		av_opt_set(m_CodecContext, "preset", "medium", 0);
		av_opt_set(m_CodecContext, "tune", "zerolatency", 0);
		av_opt_set(m_CodecContext, "crf", "28", 0);
		m_CodecContext->profile   = FF_PROFILE_HEVC_MAIN;
		break;

	case AV_CODEC_ID_VP9:
		m_CodecContext->profile = FF_PROFILE_VP9_0;
		break;

	default:
		// Intentionally do nothing!
		break;
	}

	// Prepare stream.
	m_Stream->sample_aspect_ratio = m_CodecContext->sample_aspect_ratio;
	m_Stream->avg_frame_rate      = m_CodecContext->framerate;
}

#endif
