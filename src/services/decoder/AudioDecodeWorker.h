#pragma once

#include <QObject>
#include <qdebug.h>
#include <SDL3/SDL_audio.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#include <qthread.h>
#include <atomic>
#include "AudioDecoder.h"
#include <QString>
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qcoreapplication.h>

class AudioDecodeWorker : public QObject {
    Q_OBJECT
public:
  AudioDecodeWorker(QString filePath, SDL_AudioStream *sdlStream,
                    size_t MAX_BUFFER);

  ~AudioDecodeWorker();

  void interrupt();

public slots:
  void startDecoding();

  double getAudioClock();

  void pause();

  void resume();

signals:
    void finished();

private:
    QString filePath;
    SDL_AudioStream* sdlStream;
    std::atomic<double> audioClock{ 0.0 };
    const size_t MAX_BUFFER;
    AudioStream stream;
    std::atomic<bool> interrupted{ false };

    void cleanup();

    void decodeAudioFile();
};
