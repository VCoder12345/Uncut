#include "FFMPEGStream.h"
#include <QDebug>

FFMPEGStream::~FFMPEGStream()
{
	if (codecCtx)
		avcodec_free_context(&codecCtx);

	if (formatCtx)
		avformat_close_input(&formatCtx);

	if (frame)
		av_frame_free(&frame);

	if (pkt)
		av_packet_free(&pkt);

	initDecoding = false;
}

bool FFMPEGStream::openStream(const char* filePath, const int type, double startPts)
{
	formatCtx = nullptr;
	qDebug() << "opening file at " << filePath;
	if (avformat_open_input(&formatCtx, filePath, nullptr, nullptr) < 0)
	{
		qWarning() << "[ffmpeg] Could not open file " << filePath;
		return false;
	}


	if (avformat_find_stream_info(formatCtx, nullptr) < 0)
	{
		qWarning() << "[ffmpeg] Could not find index-information of " << filePath;
		return false;
	}

	index = -1;

	for (int i = 0; i < formatCtx->nb_streams; ++i)
	{
		if (formatCtx->streams[i]->codecpar->codec_type == type && index < 0)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		qWarning() << "[ffmpeg] Could not find indexof type ";

		return false;
	}
	else
	{
		codec = nullptr;
		codec = avcodec_find_decoder(formatCtx->streams[index]->codecpar->codec_id);
		if (codec == nullptr)
		{
			qWarning() << "[ffmpeg] Unsupported codec";
			return false;
		}

		codecCtx = nullptr;
		codecCtx = avcodec_alloc_context3(codec);
		if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[index]->codecpar) <
			0)
		{
			qWarning() << "[ffmpeg] Could not copy codec context";
			return false;
		}

		if (avcodec_open2(codecCtx, codec, nullptr) < 0)
		{
			qWarning() << "[ffmpeg] Unsupported codec";
			return false;
		}

		pkt = av_packet_alloc();
		if (pkt == nullptr)
		{
			qWarning() << "[ffmpeg] Could not allocate AVPacket";
			return false;
		}

		frame = nullptr;
		frame = av_frame_alloc();
		if (!frame)
		{
			qWarning() << "[ffmpeg] Could not allocate AVFrame";
			return false;
		}

		if (startPts > 1e-6)
		{
			// Convert seconds to microseconds
			int64_t seekTarget = static_cast<int64_t>(startPts * AV_TIME_BASE);

			// Convert microseconds to stream time base
			seekTarget = av_rescale_q(seekTarget, AV_TIME_BASE_Q,
			                          formatCtx->streams[index]->time_base);

			// Seek to the target frame
			if (av_seek_frame(formatCtx, index, seekTarget, AVSEEK_FLAG_BACKWARD) < 0)
			{
				qWarning() << "[ffmpeg] Seek failed";
				return false;
			}
			avcodec_flush_buffers(codecCtx);
		}
	}

	return true;
}

std::optional<DecodeFrame> FFMPEGStream::nextFrame()
{
	bool firstIter = true;

	if (!firstDecode)
	{
		av_frame_unref(frame);
	}

	do
	{
		if (!firstDecode && pkt->stream_index == index)
		{
			if (!firstIter && avcodec_send_packet(codecCtx, pkt) < 0)
			{
				qWarning() << "[video] Error submitting a packet for decoding ";
				return std::nullopt;
			}

			if (avcodec_receive_frame(codecCtx, frame) >= 0)
			{
				const AVRational timeBase = formatCtx->streams[pkt->stream_index]->time_base;
				double pts;
				if (pkt->dts != AV_NOPTS_VALUE)
				{
					pts = frame->best_effort_timestamp;
				}
				else
				{
					pts = 0;
				}
				const double tb = av_q2d(timeBase);
				pts *= tb;
				DecodeFrame decodeFrame;
				decodeFrame.pts = pts;
				decodeFrame.frame = frame;

				return decodeFrame;
			}
		}

		if (!firstDecode)
		{
			av_packet_unref(pkt);
		}


		firstIter = false;
		firstDecode = false;
	} while (av_read_frame(formatCtx, pkt) >= 0);

	return std::nullopt;
}
