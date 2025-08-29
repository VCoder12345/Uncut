#include "VideoStream.h"

bool VideoStream::openVideoStream(const char* filePath, double startPts)
{
	if (openStream(filePath, AVMEDIA_TYPE_VIDEO, startPts))
	{
		swsCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
		                        codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24,
		                        0, 0, 0, 0);
		av_image_alloc(buffer, lineSize, codecCtx->width, codecCtx->height,
		               AV_PIX_FMT_RGB24, 16);

		return true;
	}

	return false;
}

std::shared_ptr<VideoFrame> VideoStream::nextVideoFrame()
{
	if (auto result = nextFrame())
	{
		DecodeFrame decodeFrame(result.value());
		const AVFrame* frame = decodeFrame.frame;
		sws_scale(swsCtx, (const uint8_t* const*)frame->data, frame->linesize, 0, frame->height, buffer,
		          lineSize);
		int fw = decodeFrame.frame->width;
		int fh = decodeFrame.frame->height;
		int size = fw * fh * 3;
		uint8_t* fdata = new uint8_t[size];
		std::copy(buffer[0], buffer[0] + size, fdata);
		return std::make_shared<VideoFrame>(fdata, fw, fh, size, decodeFrame.pts);
	}
	return nullptr;
}

std::optional<VideoInfo> VideoStream::getVideoInfo(const QString& filePath)
{
	VideoStream stream;
	if (stream.openVideoStream(filePath.toStdString().c_str()))
	{
		VideoInfo info;
		info.duration = stream.formatCtx->duration;

		if (std::shared_ptr<VideoFrame> videoFrame = stream.nextVideoFrame())
		{
			info.firstFrame = videoFrame;

			return info;
		}
	}

	return std::nullopt;
}

VideoStream::~VideoStream()
{
	if (swsCtx)
	{
		av_freep(&buffer[0]);
		sws_freeContext(swsCtx);
		swsCtx = nullptr;
	}
}
