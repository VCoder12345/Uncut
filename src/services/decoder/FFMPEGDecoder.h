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




struct Stream {
	AVFormatContext* formatCtx;
	const AVCodec* codec;
	AVCodecContext* codecCtx;
	int index;
};

struct DecodeFrame {
	double pts;
	const AVFrame* frame;
};


class FFMPEGDecoder
{
public:
	std::optional<Stream> openStream(const char* filePath, int type);
	void closeStream(Stream& stream);
	std::optional<DecodeFrame> nextFrame(const Stream& stream);
	virtual void cleanup();

	bool isInitialised();

private:
	bool initDecoding = false;
	AVPacket* pkt = nullptr;
	AVFrame* frame = nullptr;
};



