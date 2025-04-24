/*
**
**  $Filename:  b3EncoderBuffer.cc $
**  $Release:   Dortmund 2024 $
**  $Revision$
**  $Date$
**  $Developer:     Steffen A. Mork $
**
**  Blizzard III - Implementation of an encoder buffer/packet.
**
**      (C) Copyright 2001 - 2024  Steffen A. Mork
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

#include "blz3/image/b3EncoderBuffer.h"

#ifdef HAVE_VIDEO_ENCODER

/*************************************************************************
**                                                                      **
**                        AV frame buffer implementation                **
**                                                                      **
*************************************************************************/

b3CodecFrame::b3CodecFrame()
{
	m_Frame = av_frame_alloc();
}

void b3CodecFrame::b3InitAudio(
	const AVCodecContext * codec_context,
	const b3_res           frames_per_second)
{
#if LIBAVFORMAT_VERSION_MAJOR >= 60
	av_channel_layout_copy(&m_Frame->ch_layout, &codec_context->ch_layout);

	const int channels = m_Frame->ch_layout.nb_channels;
#else
	m_Frame->channel_layout = codec_context->channel_layout;
	m_Frame->channels       = av_get_channel_layout_nb_channels(m_Frame->channel_layout);

	const int channels = m_Frame->channels;

#endif
	m_Frame->format         = codec_context->sample_fmt;
	m_Frame->sample_rate    = codec_context->sample_rate;
	m_Frame->nb_samples     = codec_context->sample_rate * channels / frames_per_second;

	m_BufferSize = av_samples_get_buffer_size(nullptr,
			channels,
			codec_context->sample_rate / frames_per_second,
			codec_context->sample_fmt, 1);
	m_SampleSize = av_get_bytes_per_sample (codec_context->sample_fmt);

	av_frame_get_buffer(m_Frame, 0);
	memset(m_Frame->data[0], 0, m_Frame->linesize[0]);
}

b3CodecFrame::b3CodecFrame(const b3Tx * tx, int format)
{
	m_Frame = av_frame_alloc();
	m_Frame->format = format;
	m_Frame->width  = tx->xSize;
	m_Frame->height = tx->ySize;
	av_frame_get_buffer(m_Frame, 0);
	m_Frame->extended_data = m_Frame->data;
}

b3CodecFrame::~b3CodecFrame()
{
	av_frame_free(&m_Frame);
}

/*************************************************************************
**                                                                      **
**                        AV stream packet implementation               **
**                                                                      **
*************************************************************************/

b3CodecPacket::b3CodecPacket() :
	m_Packet(av_packet_alloc())
{
	B3_ASSERT(m_Packet->data == nullptr);
	B3_ASSERT(m_Packet->size == 0);
}

b3CodecPacket::~b3CodecPacket()
{
	av_packet_free(&m_Packet);
}

#endif
