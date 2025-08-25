#pragma once
#include "FFMPEGDecoder.h"
#include <memory>

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

struct AudioStream : public Stream
{
	AudioStream(const Stream& stream) : Stream(stream)
	{
	}

	AudioStream() {}

	uint8_t* outBuffer = nullptr;
	SwrContext* swrCtx = nullptr;
	AVChannelLayout outLayout;
};

class AudioDecoder :
	public FFMPEGDecoder
{
public:
	static std::unique_ptr<AudioFrame> nextAudioFrame(AudioStream& stream);
	static std::optional<AudioStream> openStream(const char* filePath);
	static void closeStream(AudioStream& stream);

private:
};
