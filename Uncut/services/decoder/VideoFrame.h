#pragma once

#include <cstdint>
#include <qimage.h>

class VideoFrame {
public:
    uint8_t* data;
    double pts;
    int width, height;

    VideoFrame(uint8_t* data, int width, int height, double pts)
        : data(data), width(width), height(height), pts(pts) {
    }

    QImage toImage() {
        return QImage(data, width, height, QImage::Format_RGB888);
    }
};