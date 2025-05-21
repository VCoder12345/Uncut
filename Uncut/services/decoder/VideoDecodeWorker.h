#pragma once

#include <QObject>
#include <qdebug.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

#include <qthread.h>
#include "VideoFrame.h"
#include "VideoStreamBuffer.h"
#include "VideoDecoder.h"


class VideoDecodeWorker : public QObject {
    Q_OBJECT
public:
    VideoDecodeWorker(QString path, VideoStreamBuffer* videoBuffer)
        : filePath(path), videoBuffer(videoBuffer) {
    }

    ~VideoDecodeWorker() {
        cleanup();
    }

    void cleanup() {
        //qDebug() << "video decoder clean";
        decoder.cleanup();
        decoder.closeStream(stream);
    }

    void interrupt() {
        interrupted.store(true);
    }

public slots:
    void startDecoding() {
        decodeVideoFile();
    }

   

signals:
    void finished();

private:
    QString filePath;
    VideoStreamBuffer* videoBuffer;
    VideoDecoder decoder;
    Stream stream;
    std::atomic<bool> interrupted{ false };

    void decodeVideoFile() {
        qDebug() << "[video] start decoding";

        
        if (auto streamResult = decoder.openStream(filePath.toStdString().c_str())) {
            stream = streamResult.value();

            std::shared_ptr<VideoFrame> frame;
            while (!interrupted.load() && (frame = decoder.nextVideoFrame(stream))) {
                videoBuffer->appendData(frame);
            }

            videoBuffer->setEOF();

            cleanup();

            qDebug() << "[video] finished decoding";
        }
    }
};