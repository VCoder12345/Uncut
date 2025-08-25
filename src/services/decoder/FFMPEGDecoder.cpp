#include "FFMPEGDecoder.h"
#include <qdebug.h>


std::optional<Stream> FFMPEGDecoder::openStream(const char* filePath, int streamType)
{
	Stream stream;
	stream.formatCtx = nullptr;
	qDebug() << "opening file at " << filePath;
	if (avformat_open_input(&stream.formatCtx, filePath, nullptr, nullptr) < 0)
	{
		qWarning() << "[ffmpeg] Could not open file " << filePath;
		return std::nullopt;
	}


	if (avformat_find_stream_info(stream.formatCtx, nullptr) < 0)
	{
		qWarning() << "[ffmpeg] Could not find stream.index-information of " << filePath;
		return std::nullopt;
	}

	stream.index = -1;

	for (int i = 0; i < stream.formatCtx->nb_streams; ++i)
	{
		if (stream.formatCtx->streams[i]->codecpar->codec_type == streamType && stream.index < 0)
		{
			stream.index = i;
			break;
		}
	}

	if (stream.index == -1)
	{
		qWarning() << "[ffmpeg] Could not find stream.indexof type ";

		return std::nullopt;
	}
	else
	{
		stream.codec = nullptr;
		stream.codec = avcodec_find_decoder(stream.formatCtx->streams[stream.index]->codecpar->codec_id);
		if (stream.codec == nullptr)
		{
			qWarning() << "[ffmpeg] Unsupported stream.codec";
			return std::nullopt;
		}

		stream.codecCtx = nullptr;
		stream.codecCtx = avcodec_alloc_context3(stream.codec);
		if (avcodec_parameters_to_context(stream.codecCtx, stream.formatCtx->streams[stream.index]->codecpar) < 0)
		{
			qWarning() << "[ffmpeg] Could not copy stream.codec context";
			return std::nullopt;
		}

		if (avcodec_open2(stream.codecCtx, stream.codec, nullptr) < 0)
		{
			qWarning() << "[ffmpeg] Unsupported stream.codec";
			return std::nullopt;
		}

		stream.pkt = av_packet_alloc();
		if (stream.pkt == nullptr)
		{
			qWarning() << "[ffmpeg] Could not allocate AVPacket";
			return std::nullopt;
		}

		stream.frame = nullptr;
		stream.frame = av_frame_alloc();
		if (!stream.frame)
		{
			qWarning() << "[ffmpeg] Could not allocate AVFrame";
			return std::nullopt;
		}
	}

	return stream;
}

void FFMPEGDecoder::closeStream(Stream& stream)
{
	if (stream.codecCtx)
		avcodec_free_context(&stream.codecCtx);

	if (stream.formatCtx)
		avformat_close_input(&stream.formatCtx);

	if (stream.frame)
		av_frame_free(&stream.frame);

	if (stream.pkt)
		av_packet_free(&stream.pkt);

	stream.initDecoding = false;
}

/**
 * 
 * @param stream 
 * @return 
 */
std::optional<DecodeFrame> FFMPEGDecoder::nextFrame(Stream& stream)
{
	bool firstIter = true;

	if (!stream.firstDecode)
	{
		av_frame_unref(stream.frame);
	}

	do
	{
		if (!stream.firstDecode && stream.pkt->stream_index == stream.index)
		{
			if (!firstIter && avcodec_send_packet(stream.codecCtx, stream.pkt) < 0)
			{
				qWarning() << "[video] Error submitting a packet for decoding ";
				return std::nullopt;
			}

			if (avcodec_receive_frame(stream.codecCtx, stream.frame) >= 0)
			{
				AVRational timeBase = stream.formatCtx->streams[stream.pkt->stream_index]->time_base;
				double pts, tb;
				if (stream.pkt->dts != AV_NOPTS_VALUE)
				{
					pts = stream.frame->best_effort_timestamp;
				}
				else
				{
					pts = 0;
				}
				tb = av_q2d(timeBase);
				pts *= tb;
				DecodeFrame decodeFrame;
				decodeFrame.pts = pts;
				decodeFrame.frame = stream.frame;

				return decodeFrame;
			}
		}

		if (!stream.firstDecode)
		{
			av_packet_unref(stream.pkt);
		}


		firstIter = false;
		stream.firstDecode = false;
	}
	while (av_read_frame(stream.formatCtx, stream.pkt) >= 0);

	return std::nullopt;
}



