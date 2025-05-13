#include "VideoDecoder.h"
#include <QDebug>

std::unique_ptr<VideoFrame> VideoDecoder::nextVideoFrame(const Stream& stream)
{

	if (!isInitialised()) {


		sws_ctx = sws_getContext(stream.codecCtx->width, stream.codecCtx->height, stream.codecCtx->pix_fmt, stream.codecCtx->width, stream.codecCtx->height, AV_PIX_FMT_RGB24,
			0, 0, 0, 0);
		av_image_alloc(buffer, lineSize, stream.codecCtx->width, stream.codecCtx->height, AV_PIX_FMT_RGB24, 16);
	}
	

	if (auto result = nextFrame(stream)) {
		DecodeFrame decodeFrame = result.value();
		const AVFrame* frame = decodeFrame.frame;

		sws_scale(sws_ctx, (const uint8_t* const*)frame->data, frame->linesize, 0, frame->height, buffer, lineSize);

		int fw = decodeFrame.frame->width;
		int fh = decodeFrame.frame->height;
		int size = fw * fh * 3;
		uint8_t* fdata = new uint8_t[size];
		std::copy(buffer[0], buffer[0] + size, fdata);
		return std::make_unique<VideoFrame>(fdata, fw, fh, decodeFrame.pts);

	}

	return nullptr;
}

std::optional<Stream> VideoDecoder::openStream(const char* filePath)
{
	return FFMPEGDecoder::openStream(filePath, AVMEDIA_TYPE_VIDEO);
}

void VideoDecoder::cleanup()
{
	FFMPEGDecoder::cleanup();

	av_freep(&buffer[0]);
	sws_freeContext(sws_ctx);
}

std::optional<VideoInfo> VideoDecoder::getVideoInfo(const char* filePath) {
	if (auto streamResult = openStream(filePath)) {
		Stream& stream = streamResult.value();
		VideoInfo info;
		info.duration = stream.formatCtx->duration;

		std::unique_ptr<VideoFrame> videoFrame = nextVideoFrame(stream);

		if (videoFrame) {
			info.firstFrame = std::move(videoFrame);
		}
		cleanup();

		closeStream(stream);

		if (info.firstFrame) {
			return info;
		}
	}

	return std::nullopt;
}

