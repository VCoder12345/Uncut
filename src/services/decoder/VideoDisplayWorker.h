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
    VideoDisplayWorker(VideoStreamBuffer* vbuffer, SDL_AudioStream* stream, AudioDecodeWorker* audioDec)
        : vbuffer(vbuffer), stream(stream), audioDec(audioDec) {}

    void interrupt() {
        interrupted.store(true);
    }
public slots:
    void startDisplaying() {

        qDebug() << "[displayer] let's start displaying stuff";
        std::shared_ptr<VideoFrame> frame = vbuffer->dequeue();
        if (!frame) return;

        while (!interrupted.load() && !vbuffer->isFinished()) {
            double audioPts = audioClock();

            // Drop frames that are too far behind
            while (!interrupted.load() && frame && frame->pts < audioPts - 0.03) {  // 30ms late? Drop it.
                //qDebug() << "Dropping late frame at " << frame->pts << " (audio: " << audioPts << ")";
                frame = vbuffer->dequeue();
            }

            if (!frame) break;

            // Wait until it's time to show the frame
            if (frame->pts <= audioPts + 0.005) { // Allow small lead
                // qDebug() << "Displaying frame " << frame->pts << " (audio: " << audioPts << ")";
                emit displayFrame(frame);
                frame = vbuffer->dequeue();
            }
            else {
                // Frame is early – sleep a tiny bit, or spin if close
                unsigned long wait = std::ceil((frame->pts - audioPts) * 1000.0);
                if (wait > 10) wait = 10;
                QThread::msleep(wait);
            }
        }

        qDebug() << "[displayer] finished my job";
    }

    double audioClock() {
        return audioDec->getAudioClock() - (double)(SDL_GetAudioStreamQueued(stream))/(44100 * 2 * 2);
    }
    

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