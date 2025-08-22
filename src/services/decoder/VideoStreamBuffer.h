#pragma once

#include "VideoFrame.h"
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qdebug.h>
#include <memory>

class VideoStreamBuffer {
public:
  VideoStreamBuffer(size_t maxSize);

  ~VideoStreamBuffer();

  void appendData(std::shared_ptr<VideoFrame> vframe);

  size_t size() const;

  void setEOF();

  std::shared_ptr<VideoFrame> dequeue();

  bool isFinished() const;

  void clear();

  void interrupt();

private:
    std::shared_ptr<VideoFrame>* buffer;
	mutable QMutex mutex;
    bool reached_eof = false;
    mutable QWaitCondition waitCond;
    const size_t maxSize;
    size_t bufferSize = 0;
    size_t headPointer = 0;
    size_t tailPointer = 0;
};
