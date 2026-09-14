#include "VideoStreamBuffer.h"
VideoStreamBuffer::VideoStreamBuffer(size_t maxSize) : maxSize(maxSize) {
  this->buffer = new std::shared_ptr<VideoFrame>[maxSize];
}

VideoStreamBuffer::~VideoStreamBuffer() { delete[] buffer; }

void VideoStreamBuffer::appendData(std::shared_ptr<VideoFrame> vframe) {
  QMutexLocker locker(&mutex);
  while (bufferSize >= maxSize) {
    waitCond.wait(&mutex);
  }

  buffer[tailPointer] = vframe;

  tailPointer = (tailPointer + 1) % maxSize;
  ++bufferSize;

  waitCond.wakeAll();
}

size_t VideoStreamBuffer::size() const {
  QMutexLocker locker(&mutex);
  return bufferSize;
}

void VideoStreamBuffer::setEOF() {
  QMutexLocker locker(&mutex);
  reached_eof = true;
}

std::shared_ptr<VideoFrame> VideoStreamBuffer::dequeue() {
  QMutexLocker locker(&mutex);
  while (bufferSize == 0 && !reached_eof) {
    waitCond.wait(&mutex);
  }

  if (bufferSize == 0 && reached_eof)
    return nullptr;

  std::shared_ptr<VideoFrame> front = buffer[headPointer];
  buffer[headPointer] = nullptr;
  headPointer = (headPointer + 1) % maxSize;
  --bufferSize;

  waitCond.wakeAll();

  return front;
}

bool VideoStreamBuffer::isFinished() const {
  QMutexLocker locker(&mutex);

  return bufferSize == 0 && reached_eof;
}

void VideoStreamBuffer::clear() {
  QMutexLocker locker(&mutex);
  for (int i = headPointer; i < tailPointer; ++i) {
    buffer[i] = nullptr;
  }
  bufferSize = 0;
  headPointer = 0;
  tailPointer = 0;
  reached_eof = false;
  waitCond.wakeAll();
}

void VideoStreamBuffer::interrupt() { clear(); }

