#pragma once
#include <memory>

#include "FFMPEGStream.h"

class AudioFrame
{
public:
	uint8_t* data;
	int size;
	double pts;

	AudioFrame(uint8_t* data, int size, double pts) : data(data), size(size), pts(pts)
	{
	}
};

class AudioStream : protected FFMPEGStream
{
public:
	bool openAudioStream(const char* filePath, double startPts = 0.0);

	uint8_t* outBuffer = nullptr;
	SwrContext* swrCtx = nullptr;
	AVChannelLayout outLayout;


	~AudioStream() override;

	std::unique_ptr<AudioFrame> nextAudioFrame();
};
