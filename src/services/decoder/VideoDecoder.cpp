#include "VideoDecoder.h"
#include <QDebug>

std::shared_ptr<VideoFrame> VideoDecoder::nextVideoFrame(VideoStream& stream)
{
	if (auto result = nextFrame(stream))
	{
		DecodeFrame decodeFrame(result.value());
		const AVFrame* frame = decodeFrame.frame;

		sws_scale(stream.swsCtx, (const uint8_t* const*)frame->data, frame->linesize, 0, frame->height, stream.buffer,
		          stream.lineSize);

		int fw = decodeFrame.frame->width;
		int fh = decodeFrame.frame->height;
		int size = fw * fh * 3;
		uint8_t* fdata = new uint8_t[size];
		std::copy(stream.buffer[0], stream.buffer[0] + size, fdata);
		return std::make_unique<VideoFrame>(fdata, fw, fh, size, decodeFrame.pts);
	}

	return nullptr;
}

std::optional<VideoStream> VideoDecoder::openStream(const char* filePath)
{
	if (auto streamResult = FFMPEGDecoder::openStream(filePath, AVMEDIA_TYPE_VIDEO))
	{
		VideoStream vStream(streamResult.value());
		vStream.swsCtx = sws_getContext(vStream.codecCtx->width, vStream.codecCtx->height, vStream.codecCtx->pix_fmt,
		                                vStream.codecCtx->width, vStream.codecCtx->height, AV_PIX_FMT_RGB24,
		                                0, 0, 0, 0);
		av_image_alloc(vStream.buffer, vStream.lineSize, vStream.codecCtx->width, vStream.codecCtx->height,
		               AV_PIX_FMT_RGB24, 16);

		return vStream;
	}
	else
	{
		return std::nullopt;
	}
}

std::optional<VideoInfo> VideoDecoder::getVideoInfo(const QString& filePath)
{
	if (auto streamResult = openStream(filePath.toStdString().c_str()))
	{
		VideoStream& stream = streamResult.value();
		VideoInfo info;
		info.duration = stream.formatCtx->duration;

		std::shared_ptr<VideoFrame> videoFrame = nextVideoFrame(stream);

		if (videoFrame)
		{
			info.firstFrame = videoFrame;
		}

		closeStream(stream);

		if (info.firstFrame)
		{
			return info;
		}
	}

	return std::nullopt;
}

void VideoDecoder::closeStream(VideoStream& stream)
{
	FFMPEGDecoder::closeStream(stream);

	if (stream.swsCtx)
	{
		av_freep(&stream.buffer[0]);
		sws_freeContext(stream.swsCtx);
	}
}
