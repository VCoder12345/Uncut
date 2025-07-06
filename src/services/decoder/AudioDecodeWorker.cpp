#include "AudioDecodeWorker.h"

AudioDecodeWorker::AudioDecodeWorker(QString filePath,
                                     SDL_AudioStream *sdlStream,
                                     size_t MAX_BUFFER)
    : filePath(filePath), sdlStream(sdlStream), MAX_BUFFER(MAX_BUFFER) {}
AudioDecodeWorker::~AudioDecodeWorker() { cleanup(); }

void AudioDecodeWorker::interrupt() { interrupted.store(true); }

void AudioDecodeWorker::startDecoding() { decodeAudioFile(); }

double AudioDecodeWorker::getAudioClock() {
  return audioClock.load(std::memory_order_relaxed);
}

void AudioDecodeWorker::pause() {
  SDL_PauseAudioStreamDevice(sdlStream);
  qDebug() << "[audio] audio playback paused";
}

void AudioDecodeWorker::resume() {
  SDL_ResumeAudioStreamDevice(sdlStream);
  qDebug() << "[audio] resume playback";
}

void AudioDecodeWorker::cleanup() {
  decoder.cleanup();
  decoder.closeStream(stream);

  // qDebug() << "audio decoder clean";
}

void AudioDecodeWorker::decodeAudioFile() {
  // Do FFmpeg decoding like before
  // Whenever you get PCM data:
  qDebug() << "[audio] start decoding";

  if (auto streamResult = decoder.openStream(filePath.toStdString().c_str())) {
    stream = streamResult.value();

    std::unique_ptr<AudioFrame> frame;
    while (!interrupted.load() && (frame = decoder.nextAudioFrame(stream))) {

      while (!interrupted.load() &&
             SDL_GetAudioStreamQueued(sdlStream) > MAX_BUFFER) {
        QThread::msleep(10);
        QCoreApplication::processEvents();
      }

      SDL_PutAudioStreamData(sdlStream, (char *)frame->data, frame->size);
      audioClock.store(frame->pts);
      QCoreApplication::processEvents();
    }

    cleanup();

    qDebug() << "[audio] finished decoding";
  }
}

