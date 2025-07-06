#include "FFMPEGDecoder.h"
#include <qdebug.h>



std::optional<Stream> FFMPEGDecoder::openStream(const char* filePath, int streamType)
{
	Stream stream;
	stream.formatCtx = NULL;
	qDebug() << "opening file at " << filePath;
	if (avformat_open_input(&stream.formatCtx, filePath, NULL, NULL) < 0) {
		qWarning() << "[ffmpeg] Could not open file " << filePath;
		return std::nullopt;
	}


	if (avformat_find_stream_info(stream.formatCtx, NULL) < 0) {
		qWarning() << "[ffmpeg] Could not find stream.indexinformation of " << filePath;
		return std::nullopt;
	}

	stream.index = -1;

	for (int i = 0; i < stream.formatCtx->nb_streams; ++i) {
		if (stream.formatCtx->streams[i]->codecpar->codec_type == streamType && stream.index < 0) {
			stream.index = i;
			break;
		}
	}

	if (stream.index == -1) {
		qWarning() << "[ffmpeg] Could not find stream.indexof type ";

		return std::nullopt;
	}
	else {
		stream.codec = NULL;
		stream.codec = avcodec_find_decoder(stream.formatCtx->streams[stream.index]->codecpar->codec_id);
		if (stream.codec == NULL) {
			qWarning() << "[ffmpeg] Unsupported stream.codec";
			return std::nullopt;
		}

		stream.codecCtx = NULL;
		stream.codecCtx = avcodec_alloc_context3(stream.codec);
		if (avcodec_parameters_to_context(stream.codecCtx, stream.formatCtx->streams[stream.index]->codecpar) < 0) {
			qWarning() << "[ffmpeg] Could not copy stream.codec context";
			return std::nullopt;
		}

		if (avcodec_open2(stream.codecCtx, stream.codec, NULL) < 0) {
			qWarning() << "[ffmpeg] Unsupported stream.codec";
			return std::nullopt;
		}
	}

	return stream;
}

void FFMPEGDecoder::closeStream(Stream& stream)
{
	if(stream.codecCtx)
		avcodec_free_context(&stream.codecCtx);

	if(stream.formatCtx)
		avformat_close_input(&stream.formatCtx);
}

std::optional<DecodeFrame> FFMPEGDecoder::nextFrame(const Stream& stream)
{
	bool firstDecode = false;
	if (!initDecoding) {
		pkt = av_packet_alloc();
		if (pkt == NULL) {
			qWarning() << "[video] Could not allocate AVPacket";
			return std::nullopt;
		}

		frame = NULL;
		frame = av_frame_alloc();
		if (!frame) {
			qWarning() << "[video] Could not allocate AVFrame";
			return std::nullopt;
		}

		initDecoding = true;
		firstDecode = true;
	}
	

	bool firstIter = true;

	if (!firstDecode) {
		av_frame_unref(frame);
	}

	do {
		if (!firstDecode && pkt->stream_index == stream.index) {
			if (!firstIter && avcodec_send_packet(stream.codecCtx, pkt) < 0) {
				qWarning() << "[video] Error submitting a packet for decoding ";
				return std::nullopt;
			}

			if (avcodec_receive_frame(stream.codecCtx, frame) >= 0) {
				AVRational timeBase = stream.formatCtx->streams[pkt->stream_index]->time_base;
				double pts, tb;
				if (pkt->dts != AV_NOPTS_VALUE) {
					pts = frame->best_effort_timestamp;
				}
				else {
					pts = 0;
				}
				tb = av_q2d(timeBase);
				pts *= tb;
				DecodeFrame decodeFrame;
				decodeFrame.pts = pts;
				decodeFrame.frame = frame;

				return decodeFrame;
			}
		}

		if (!firstDecode) {
			av_packet_unref(pkt);
		}
			

		firstIter = false;
		firstDecode = false;
	} while (av_read_frame(stream.formatCtx, pkt) >= 0);
	
	return std::nullopt;
}



void FFMPEGDecoder::cleanup()
{
	if(frame)
		av_frame_free(&frame);

	if(pkt)
		av_packet_free(&pkt);

	initDecoding = false;
}

bool FFMPEGDecoder::isInitialised()
{
	return initDecoding;
}











