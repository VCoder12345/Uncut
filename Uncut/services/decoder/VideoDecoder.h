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

class VideoDecoder : public FFMPEGDecoder
{
public:
	std::shared_ptr<VideoFrame> nextVideoFrame(const Stream& stream);
	std::optional<Stream> openStream(const char* filePath);
	void cleanup() override;
	std::optional<VideoInfo> getVideoInfo(const char* filePath);

private:
	uint8_t* buffer[4];
	struct SwsContext* sws_ctx;
	int lineSize[4];
};

