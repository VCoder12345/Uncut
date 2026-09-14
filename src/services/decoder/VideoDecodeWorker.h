#pragma once

#include <QObject>
#include <qdebug.h>

#include "VideoStream.h"

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


class VideoDecodeWorker : public QObject {
    Q_OBJECT
public:
  VideoDecodeWorker(const QString& path, VideoStreamBuffer *videoBuffer);

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
    std::unique_ptr<VideoStream> stream = nullptr;
    std::atomic<bool> interrupted{ false };

    void decodeVideoFile();
};
