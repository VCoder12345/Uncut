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


struct Stream
{
	AVFormatContext* formatCtx = nullptr;
	const AVCodec* codec = nullptr;
	AVCodecContext* codecCtx = nullptr;
	int index;
	AVPacket* pkt = nullptr;
	AVFrame* frame = nullptr;
	bool initDecoding = false;
	bool firstDecode = true;
};

struct DecodeFrame
{
	double pts;
	const AVFrame* frame;
};


class FFMPEGDecoder
{
public:
	static std::optional<Stream> openStream(const char* filePath, int type);
	static void closeStream(Stream& stream);
	static std::optional<DecodeFrame> nextFrame(Stream& stream);

};
