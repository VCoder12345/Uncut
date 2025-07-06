#include "AudioDecoder.h"
#include <qdebug.h>

std::unique_ptr<AudioFrame> AudioDecoder::nextAudioFrame(const Stream& stream)
{
    if (!isInitialised()) {
        outBuffer = (uint8_t*)av_malloc(192000);

        av_channel_layout_default(&outLayout, 2); // stereo output

        swrCtx = swr_alloc();
        if (!swrCtx) {
            qWarning() << "[audio] Failed to allocate SwrContext";
            return nullptr;
        }

        int err = swr_alloc_set_opts2(&swrCtx,
            &outLayout, AV_SAMPLE_FMT_S16, 44100,        // output
            &stream.codecCtx->ch_layout, stream.codecCtx->sample_fmt, stream.codecCtx->sample_rate,  // input
            0, nullptr);

        if (err < 0) {
            qWarning() << "[audio] swr_alloc_set_opts2 failed";
            return nullptr;
        }

        swr_init(swrCtx);
    }

    if (auto result = nextFrame(stream)) {
        DecodeFrame decoderFrame = result.value();

        int outSamples = swr_convert(swrCtx,
            &outBuffer, 192000 / 2,
            (const uint8_t**)decoderFrame.frame->data, decoderFrame.frame->nb_samples
        );
        int dataSize = outSamples * 2 * 2; // samples * channels * 16-bit


        return std::make_unique<AudioFrame>(outBuffer, dataSize, decoderFrame.pts);
    }

    return nullptr;
}

std::optional<Stream> AudioDecoder::openStream(const char* filePath)
{
    return FFMPEGDecoder::openStream(filePath, AVMEDIA_TYPE_AUDIO);
}

void AudioDecoder::cleanup()
{
    FFMPEGDecoder::cleanup();

    if (isInitialised()) {
		swr_free(&swrCtx);
		av_free(outBuffer);
		av_channel_layout_uninit(&outLayout);
    }
   }
