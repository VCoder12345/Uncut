#include "AudioStream.h"
#include <QDebug>

bool AudioStream::openAudioStream(const char* filePath, double startPts)
{
	if (openStream(filePath, AVMEDIA_TYPE_AUDIO))
	{
		outBuffer = (uint8_t*)av_malloc(192000);

		av_channel_layout_default(&outLayout, 2); // stereo output

		swrCtx = swr_alloc();
		if (!swrCtx)
		{
			qWarning() << "[audio] Failed to allocate SwrContext";
			return false;
		}

		int err = swr_alloc_set_opts2(&swrCtx,
		                              &outLayout, AV_SAMPLE_FMT_S16, 44100, // output
		                              &codecCtx->ch_layout, codecCtx->sample_fmt,
		                              codecCtx->sample_rate, // input
		                              0, nullptr);

		if (err < 0)
		{
			qWarning() << "[audio] swr_alloc_set_opts2 failed";
			return false;
		}

		swr_init(swrCtx);

		return true;
	}

	return false;
}

AudioStream::~AudioStream()
{
	if (swrCtx)
	{
		swr_free(&swrCtx);
		av_free(outBuffer);
		av_channel_layout_uninit(&outLayout);
	}
}

std::unique_ptr<AudioFrame> AudioStream::nextAudioFrame()
{
	if (auto result = nextFrame())
	{
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
