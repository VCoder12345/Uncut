#include "VideoDecodeWorker.h"

VideoDecodeWorker::VideoDecodeWorker(const QString& path,
                                     VideoStreamBuffer* videoBuffer)
	: filePath(path), videoBuffer(videoBuffer)
{
}

VideoDecodeWorker::~VideoDecodeWorker() { cleanup(); }

void VideoDecodeWorker::cleanup()
{
	stream.reset();
}

void VideoDecodeWorker::interrupt() { interrupted.store(true); }

void VideoDecodeWorker::startDecoding() { decodeVideoFile(); }

void VideoDecodeWorker::decodeVideoFile()
{
	qDebug() << "[video] start decoding";
	stream = std::make_unique<VideoStream>();

	if (stream->openVideoStream(filePath.toStdString().c_str()))
	{
		std::shared_ptr<VideoFrame> frame;
		while (!interrupted.load() && ((frame = stream->nextVideoFrame())))
		{
			videoBuffer->appendData(frame);
		}

		videoBuffer->setEOF();

		cleanup();

		qDebug() << "[video] finished decoding";
	}
}
