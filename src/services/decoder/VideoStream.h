#pragma once
#include "FFMPEGStream.h"
#include "VideoFrame.h"
#include <memory>
#include <optional>

class VideoInfo
{
public:
	std::shared_ptr<VideoFrame> firstFrame;

	int64_t duration;
};

class VideoStream : protected FFMPEGStream
{
public:
	uint8_t* buffer[4];
	struct SwsContext* swsCtx = nullptr;
	int lineSize[4];

	bool openVideoStream(const char* filePath, double startPts = 0.0);

	std::shared_ptr<VideoFrame> nextVideoFrame();

	static std::optional<VideoInfo> getVideoInfo(const QString& filePath);


	~VideoStream() override;
};
