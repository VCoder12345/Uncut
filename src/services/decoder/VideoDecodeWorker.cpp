#include "VideoDecodeWorker.h"
VideoDecodeWorker::VideoDecodeWorker(QString path,
                                     VideoStreamBuffer *videoBuffer)
    : filePath(path), videoBuffer(videoBuffer) {}

VideoDecodeWorker::~VideoDecodeWorker() { cleanup(); }

void VideoDecodeWorker::cleanup() {
  // qDebug() << "video decoder clean";
  VideoDecoder::closeStream(stream);
}

void VideoDecodeWorker::interrupt() { interrupted.store(true); }

void VideoDecodeWorker::startDecoding() { decodeVideoFile(); }

void VideoDecodeWorker::decodeVideoFile() {
  qDebug() << "[video] start decoding";

  if (auto streamResult = VideoDecoder::openStream(filePath.toStdString().c_str())) {
    stream = streamResult.value();

    std::shared_ptr<VideoFrame> frame;
    while (!interrupted.load() && (frame = VideoDecoder::nextVideoFrame(stream))) {
      videoBuffer->appendData(frame);
    }

    videoBuffer->setEOF();

    cleanup();

    qDebug() << "[video] finished decoding";
  }
}

