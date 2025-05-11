#pragma once

#include <qimage.h>
#include "VideoFrame.h"


struct VideoInfo {
	VideoFrame* firstFrame;
	int64_t duration;
};

class VideoDecoder
{
public:
	bool getVideoInfo(const char* filePath, VideoInfo& videoInfo);
};

