#pragma once

#include <cstdint>
#include <qimage.h>
#include <QDebug>
#include <memory>

class VideoFrame {
public:
    uint8_t* data;
    double pts;
    int width, height;
    int dataSize;

    VideoFrame(uint8_t* data, int width, int height, int dataSize, const double pts)
        : data(data), pts(pts), width(width), height(height), dataSize(dataSize)
    {
    }

    ~VideoFrame() {
        delete[] data;
        data = nullptr;
    }

    QImage toImage() {
        return QImage(data, width, height, QImage::Format_RGB888);
    }

    uint8_t* copyData() const
    {
        uint8_t* copyData = new uint8_t[dataSize];
        std::memcpy(copyData, data, dataSize);
        return copyData;
    }

    std::shared_ptr<VideoFrame> copy() {
        return std::make_unique<VideoFrame>(copyData(), width, height, dataSize, pts);
    }
    
    QImage* toCopyImage() const
    {
        uint8_t* copiedData = copyData();

        QImageCleanupFunction cleanup = [](void* info) {
            delete[] static_cast<uint8_t*>(info);
        };

        return new QImage(copiedData, width, height, QImage::Format_RGB888, cleanup, copiedData);
    }
};