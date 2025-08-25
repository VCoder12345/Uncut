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
  VideoDecodeWorker(QString path, VideoStreamBuffer *videoBuffer);

  ~VideoDecodeWorker();

  void cleanup();

  void interrupt();

public slots:
  void startDecoding();

signals:
    void finished();

private:
    QString filePath;
    VideoStreamBuffer* videoBuffer;
    VideoStream stream;
    std::atomic<bool> interrupted{ false };

    void decodeVideoFile();
};
