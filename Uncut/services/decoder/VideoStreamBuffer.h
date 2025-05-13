#pragma once

#include "VideoFrame.h"
#include <queue>
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qdebug.h>

class VideoStreamBuffer {
public:


    ~VideoStreamBuffer() {
        while (!buffer.empty()) {
            delete buffer.front();
            buffer.pop();
        }
    }

    void appendData(VideoFrame* vframe) {
        QMutexLocker locker(&mutex);
        buffer.push(vframe);
        waitCond.wakeAll();
    }

    size_t size() const {
        QMutexLocker locker(&mutex);
        return buffer.size();
    }

    void setEOF() {
        QMutexLocker locker(&mutex);
        reached_eof = true;
    }

    VideoFrame* dequeue() {
        QMutexLocker locker(&mutex);
        while (buffer.size() == 0 && !reached_eof) {
            waitCond.wait(&mutex);
        }

        if (buffer.size() == 0 && reached_eof) return nullptr;

        VideoFrame* head = buffer.front();
        buffer.pop();

        return head;
    }

    bool isFinished() const {
        QMutexLocker locker(&mutex);
        
        return buffer.size() == 0 && reached_eof;
    }

private:
	std::queue<VideoFrame*> buffer;
	mutable QMutex mutex;
    bool reached_eof = false;
    mutable QWaitCondition waitCond;
};