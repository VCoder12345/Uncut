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
    AudioDecodeWorker(QString filePath, SDL_AudioStream* sdlStream, size_t MAX_BUFFER)
        : filePath(filePath), sdlStream(sdlStream), MAX_BUFFER(MAX_BUFFER) {
    }

    ~AudioDecodeWorker() {
        cleanup();
    }

    void interrupt() {
        interrupted.store(true);
    }

public slots:
    void startDecoding() {
        decodeAudioFile();
    }

    double getAudioClock() {
        return audioClock.load(std::memory_order_relaxed);
    }

    void pause() {
        SDL_PauseAudioStreamDevice(sdlStream);
        qDebug() << "[audio] audio playback paused";
    }

    void resume() {
        SDL_ResumeAudioStreamDevice(sdlStream);
        qDebug() << "[audio] resume playback";
    }

signals:
    void finished();

private:
    QString filePath;
    SDL_AudioStream* sdlStream;
    std::atomic<double> audioClock{ 0.0 };
    AudioDecoder decoder;
    const size_t MAX_BUFFER;
    Stream stream;
    std::atomic<bool> interrupted{ false };
    
    void cleanup() {
        decoder.cleanup();
        decoder.closeStream(stream);

        //qDebug() << "audio decoder clean";
    }

    void decodeAudioFile() {
        // Do FFmpeg decoding like before
       // Whenever you get PCM data:
        qDebug() << "[audio] start decoding";

        if (auto streamResult = decoder.openStream(filePath.toStdString().c_str())) {
            stream = streamResult.value();
            
            std::unique_ptr<AudioFrame> frame;
            while (!interrupted.load() && (frame = decoder.nextAudioFrame(stream))) {

                while (!interrupted.load() && SDL_GetAudioStreamQueued(sdlStream) > MAX_BUFFER) {
                    QThread::msleep(10);
                    QCoreApplication::processEvents();
                }

                SDL_PutAudioStreamData(sdlStream, (char*)frame->data, frame->size);
                audioClock.store(frame->pts);
                QCoreApplication::processEvents();
            }

            

            cleanup();

            qDebug() << "[audio] finished decoding";
        }
    }
};