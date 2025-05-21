#pragma once

#include "VideoFrame.h"
#include <queue>
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qdebug.h>
#include <memory>
#include <vector>

class VideoStreamBuffer {
public:
    VideoStreamBuffer(size_t maxSize) : maxSize(maxSize) {
        this->buffer = new std::shared_ptr<VideoFrame> [maxSize];
    }

    ~VideoStreamBuffer() {
        delete[] buffer;
    }

    void appendData(std::shared_ptr<VideoFrame> vframe) {
        QMutexLocker locker(&mutex);
        while (bufferSize >= maxSize) {
            waitCond.wait(&mutex);
        }


        buffer[tailPointer] = vframe;

        
        tailPointer = (tailPointer + 1) % maxSize;
        ++bufferSize;

        waitCond.wakeAll();
    }

    size_t size() const {
        QMutexLocker locker(&mutex);
        return bufferSize;
    }

    void setEOF() {
        QMutexLocker locker(&mutex);
        reached_eof = true;
    }

    std::shared_ptr<VideoFrame> dequeue() {
        QMutexLocker locker(&mutex);
        while (bufferSize == 0 && !reached_eof) {
            waitCond.wait(&mutex);
        }

        if (bufferSize == 0 && reached_eof) return nullptr;

        std::shared_ptr<VideoFrame> front = buffer[headPointer];
        headPointer = (headPointer + 1) % maxSize;
        --bufferSize;

        waitCond.wakeAll();

        return front;
    }

    bool isFinished() const {
        QMutexLocker locker(&mutex);
        
        return bufferSize == 0 && reached_eof;
    }

    void clear() {
        QMutexLocker locker(&mutex);
        bufferSize = 0;
        headPointer = 0;
        tailPointer = 0;
        reached_eof = false;
        waitCond.wakeAll();
    }


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