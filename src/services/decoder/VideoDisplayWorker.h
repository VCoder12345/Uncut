#pragma once
#include <QObject>

#include "VideoStreamBuffer.h"
#include <QThread>
#include <qdebug.h>
#include <SDL3/SDL_audio.h>
#include "AudioDecodeWorker.h"
#include <atomic>

class VideoDisplayWorker: public QObject {
    Q_OBJECT
public:
  VideoDisplayWorker(VideoStreamBuffer *vbuffer, SDL_AudioStream *stream,
                     AudioDecodeWorker *audioDec);

  void interrupt();
public slots:
  void startDisplaying();

  double audioClock();

signals:
    void finished();
    void displayFrame(std::shared_ptr<VideoFrame> frame);

private:
    VideoStreamBuffer* vbuffer;
    QElapsedTimer timer;
    const qint64 bytesPerSecond = 176400;
    qint64 baseBytesPlayed = 0;
    SDL_AudioStream* stream;
    AudioDecodeWorker* audioDec;
    std::atomic<bool> interrupted{ false };
};
