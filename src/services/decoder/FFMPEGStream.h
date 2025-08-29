#pragma once

#include <qimage.h>
#include <optional>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

#include <functional>

#define STREAM_TYPE_VIDEO AVMEDIA_TYPE_VIDEO
#define STREAM_TYPE_AUDIO AVMEDIA_TYPE_AUDIO

struct DecodeFrame
{
	double pts;
	const AVFrame* frame;
};

class FFMPEGStream
{
public:
	bool openStream(const char* filePath, const int type, double startPts = 0.0);

	AVFormatContext* formatCtx = nullptr;
	const AVCodec* codec = nullptr;
	AVCodecContext* codecCtx = nullptr;
	int index = 0;
	AVPacket* pkt = nullptr;
	AVFrame* frame = nullptr;
	bool initDecoding = false;
	bool firstDecode = true;


	virtual ~FFMPEGStream();

	std::optional<DecodeFrame> nextFrame();

	};
