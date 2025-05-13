#pragma once
#include "FFMPEGDecoder.h"
#include <memory>

class AudioFrame {
public:
    uint8_t* data;
    int size;

    AudioFrame(uint8_t* data, int size) : data(data), size(size) {}
};

class AudioDecoder :
    public FFMPEGDecoder
{
public:
    std::unique_ptr<AudioFrame> nextAudioFrame(const Stream& stream);
    std::optional<Stream> openStream(const char* filePath);
    void cleanup() override;

private:
    uint8_t* outBuffer;
    SwrContext* swrCtx;
    AVChannelLayout outLayout;
};

