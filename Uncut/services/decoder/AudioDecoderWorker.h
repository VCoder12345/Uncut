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

class AudioDecoderWorker : public QObject {
    Q_OBJECT
public:
    AudioDecoderWorker(const char* path, SDL_AudioStream* sdlStream)
        : filePath(path), sdlStream(sdlStream) {
    }

    //uint64_t getTotalBytesWritten() {
    //    return totalBytesRead.load(std::memory_order_relaxed);
    //}

public slots:
    void startDecoding() {
        decodeAudioFile();
    }

    double getAudioClock() {
        return audioClock.load(std::memory_order_relaxed);
    }

signals:
    void finished();

private:
    const char* filePath;
    SDL_AudioStream* sdlStream;
    //std::atomic<uint64_t> totalBytesRead{ 0 };
    uint64_t bytes_written;
    std::atomic<double> audioClock{ 0.0 };

    int decodeAudioFile() {
        // Do FFmpeg decoding like before
       // Whenever you get PCM data:
        qDebug() << "[audio] start decoding";

        AVFormatContext* pFormatCtx = NULL;
        if (avformat_open_input(&pFormatCtx, filePath, NULL, NULL) < 0) {
            qWarning() << "[audio] Could not open file " << filePath;
            return -1;
        }



        if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
            qWarning() << "[audio] Could not find stream information of " << filePath;
            return -1;
        }

        int audio_stream = -1;

        for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
            if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream < 0) {
                audio_stream = i;
                break;
            }
        }

        if (audio_stream == -1) {
            qWarning() << "[audio] Could not find audio stream";

            return -1;
        }
        else {
            const AVCodec* codec = NULL;
            codec = avcodec_find_decoder(pFormatCtx->streams[audio_stream]->codecpar->codec_id);
            if (codec == NULL) {
                qWarning() << "[audio] Unsupported codec";
                return -1;
            }

            AVCodecContext* codecCtx = NULL;
            codecCtx = avcodec_alloc_context3(codec);
            if (avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[audio_stream]->codecpar) < 0) {
                qWarning() << "[audio] Could not copy codec context";
                return -1;
            }

            if (avcodec_open2(codecCtx, codec, NULL) < 0) {
                qWarning() << "[audio] Unsupported codec";
                return -1;
            }
            /*AVChannelLayout inLayout;
            av_channel_layout_copy(&inLayout, &codecCtx->ch_layout);*/

            AVChannelLayout outLayout;
            av_channel_layout_default(&outLayout, 2); // stereo output

            SwrContext* swrCtx = swr_alloc();
            if (!swrCtx) {
                qWarning() << "[audio] Failed to allocate SwrContext";
                return -1;
            }

            int err = swr_alloc_set_opts2(&swrCtx,
                &outLayout, AV_SAMPLE_FMT_S16, 44100,        // output
                &codecCtx->ch_layout, codecCtx->sample_fmt, codecCtx->sample_rate,  // input
                0, nullptr);

            if (err < 0) {
                qWarning() << "[audio] swr_alloc_set_opts2 failed";
                return -1;
            }

            swr_init(swrCtx);

            AVPacket* pkt = av_packet_alloc();
            if (pkt == NULL) {
                qWarning() << "[audio] Could not allocate AVPacket";
                return -1;
            }

            AVFrame* frame = NULL;
            frame = av_frame_alloc();
            if (!frame) {
                qWarning() << "[audio] Could not allocate AVFrame";
                return -1;
            }

            uint8_t* outBuffer = (uint8_t*)av_malloc(192000);
            double pts, tb;

            while (av_read_frame(pFormatCtx, pkt) >= 0) {
                if (pkt->stream_index == audio_stream) {
                    if (avcodec_send_packet(codecCtx, pkt) < 0) {
                        qWarning() << "[audio] Error submitting a packet for decoding ";
                        return -1;
                    }


                    AVRational timeBase = pFormatCtx->streams[pkt->stream_index]->time_base;
                    while (avcodec_receive_frame(codecCtx, frame) >= 0) {
                        int outSamples = swr_convert(swrCtx,
                            &outBuffer, 192000 / 2,
                            (const uint8_t**)frame->data, frame->nb_samples
                        );

                        if (pkt->dts != AV_NOPTS_VALUE) {
                            pts = frame->best_effort_timestamp;
                        }
                        else {
                            pts = 0;
                        }
                        tb = av_q2d(timeBase);
                        pts *= tb;

                        int dataSize = outSamples * 2 * 2; // samples * channels * 16-bit
                        SDL_PutAudioStreamData(sdlStream, (char*)outBuffer, dataSize);
                        //totalBytesRead.fetch_add(dataSize, std::memory_order_relaxed);
                        //bytes_written += dataSize;
                        audioClock.store(pts);
                    }


                }

                av_packet_unref(pkt);
            }

            avcodec_free_context(&codecCtx);
            //av_channel_layout_uninit(&inLayout);
            av_channel_layout_uninit(&outLayout);
            swr_free(&swrCtx);
            av_free(outBuffer);

        }






        avformat_close_input(&pFormatCtx);


        qDebug() << "[audio] fertig mit decoding";

        return 0;
    }
};