#include "AudioDecoder.h"
#include <qdebug.h>

std::unique_ptr<AudioFrame> AudioDecoder::nextAudioFrame(AudioStream& stream)
{
	if (auto result = nextFrame(stream))
	{
		DecodeFrame decoderFrame = result.value();

		int outSamples = swr_convert(stream.swrCtx,
		                             &stream.outBuffer, 192000 / 2,
		                             (const uint8_t**)decoderFrame.frame->data, decoderFrame.frame->nb_samples
		);
		int dataSize = outSamples * 2 * 2; // samples * channels * 16-bit


		return std::make_unique<AudioFrame>(stream.outBuffer, dataSize, decoderFrame.pts);
	}

	return nullptr;
}

std::optional<AudioStream> AudioDecoder::openStream(const char* filePath)
{
	if (auto result = FFMPEGDecoder::openStream(filePath, AVMEDIA_TYPE_AUDIO))
	{
		AudioStream stream(result.value());
		stream.outBuffer = (uint8_t*)av_malloc(192000);

		av_channel_layout_default(&stream.outLayout, 2); // stereo output

		stream.swrCtx = swr_alloc();
		if (!stream.swrCtx)
		{
			qWarning() << "[audio] Failed to allocate SwrContext";
			return std::nullopt;
		}

		int err = swr_alloc_set_opts2(&stream.swrCtx,
		                              &stream.outLayout, AV_SAMPLE_FMT_S16, 44100, // output
		                              &stream.codecCtx->ch_layout, stream.codecCtx->sample_fmt,
		                              stream.codecCtx->sample_rate, // input
		                              0, nullptr);

		if (err < 0)
		{
			qWarning() << "[audio] swr_alloc_set_opts2 failed";
			return std::nullopt;
		}

		swr_init(stream.swrCtx);

		return stream;
	}

	return std::nullopt;
}

void AudioDecoder::closeStream(AudioStream& stream)
{
	FFMPEGDecoder::closeStream(stream);

	if (stream.swrCtx)
	{
		swr_free(&stream.swrCtx);
		av_free(stream.outBuffer);
		av_channel_layout_uninit(&stream.outLayout);
	}
}


