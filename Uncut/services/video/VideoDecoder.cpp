#include "VideoDecoder.h"
#include <qdebug.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}



bool VideoDecoder::getVideoInfo(const char* filePath, VideoInfo& videoInfo)
{
    videoInfo.firstFrame = nullptr;
    AVFormatContext* pFormatCtx = NULL;
    if (avformat_open_input(&pFormatCtx, filePath, NULL, NULL) < 0) {
        qWarning() << "[video] Could not open file " << filePath;
        return false;
    }

    videoInfo.duration = pFormatCtx->duration;

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        qWarning() << "[video] Could not find stream information of " << filePath;
        return false;
    }

    int videoStream = -1;

    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStream < 0) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1) {
        qWarning() << "[video] Could not find video stream";

        return false;
    }
    else {
        const AVCodec* codec = NULL;
        codec = avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id);
        if (codec == NULL) {
            qWarning() << "[video] Unsupported codec";
            return false;
        }

        AVCodecContext* codecCtx = NULL;
        codecCtx = avcodec_alloc_context3(codec);
        if (avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[videoStream]->codecpar) < 0) {
            qWarning() << "[video] Could not copy codec context";
            return false;
        }

        if (avcodec_open2(codecCtx, codec, NULL) < 0) {
            qWarning() << "[video] Unsupported codec";
            return false;
        }

        uint8_t* buffer[4];
        int lineSize[4];

        struct SwsContext* sws_ctx;

        sws_ctx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24,
            0, 0, 0, 0);
        av_image_alloc(buffer, lineSize, codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24, 16);


        AVPacket* pkt = av_packet_alloc();
        if (pkt == NULL) {
            qWarning() << "[video] Could not allocate AVPacket";
            return false;
        }

        AVFrame* frame = NULL;
        frame = av_frame_alloc();
        if (!frame) {
            qWarning() << "[video] Could not allocate AVFrame";
            return false;
        }

        double pts;
        double tb;

        bool foundFrame = false;

        while (av_read_frame(pFormatCtx, pkt) >= 0 && !foundFrame) {
            if (pkt->stream_index == videoStream) {
                if (avcodec_send_packet(codecCtx, pkt) < 0) {
                    qWarning() << "[video] Error submitting a packet for decoding ";
                    return false;
                }

                AVRational timeBase = pFormatCtx->streams[pkt->stream_index]->time_base;
                while (avcodec_receive_frame(codecCtx, frame) >= 0 && !foundFrame) {
                    sws_scale(sws_ctx, (const uint8_t* const*)frame->data, frame->linesize, 0, frame->height, buffer, lineSize);

                    if (pkt->dts != AV_NOPTS_VALUE) {
                        pts = frame->best_effort_timestamp;
                    }
                    else {
                        pts = 0;
                    }
                    tb = av_q2d(timeBase);
                    pts *= tb;
                    int size = frame->width * frame->height * 3;
                    uint8_t* fdata = new uint8_t[size];
                    std::copy(buffer[0], buffer[0] + size, fdata);
                    videoInfo.firstFrame = new VideoFrame(fdata, frame->width, frame->height, pts);
                    foundFrame = true;
                }


            }

            av_packet_unref(pkt);
        }

        qDebug() << "[video] finished decoding";

        avcodec_free_context(&codecCtx);
        av_freep(&buffer[0]);
        sws_freeContext(sws_ctx);


    }






    avformat_close_input(&pFormatCtx);


    if (videoInfo.firstFrame == nullptr) {
        qWarning() << "[video] Couldn't find any frame";
        return false;
    }

    return true;
}
