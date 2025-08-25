#pragma once
#include "FFMPEGDecoder.h"
#include "VideoFrame.h"
#include <memory>
#include <optional>

class  VideoInfo {
public:
	std::shared_ptr<VideoFrame> firstFrame;

	int64_t duration;

};

struct VideoStream : public Stream {
	uint8_t* buffer[4];
	struct SwsContext* swsCtx = nullptr;
	int lineSize[4];

	VideoStream(const Stream& stream) : Stream(stream) {}
	VideoStream() {}
};

class VideoDecoder : public FFMPEGDecoder
{
public:
	static std::shared_ptr<VideoFrame> nextVideoFrame(VideoStream& stream);
	static std::optional<VideoStream> openStream(const char* filePath);
	static std::optional<VideoInfo> getVideoInfo(const QString& filePath);
	static void closeStream(VideoStream& stream);

};

