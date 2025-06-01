#include "PreviewController.h"


void PreviewController::onLibItemSelected(int oldSelected, int selected, const LibItemData* data) {
    playVideo(data->filePath);
}

PreviewController::PreviewController(Uncut& window, LibModel* libModel) : previewView(window.getPreviewView())
{
    connect(libModel, &LibModel::itemSelected, this, &PreviewController::onLibItemSelected);
    connect(window.getPlayBtn(), &QPushButton::clicked, this, &PreviewController::onPlayBtnClicked);

    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec spec;
    spec.channels = 2;
    spec.format = SDL_AUDIO_S16;
    spec.freq = 44100;

	audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

    vBuffer = new VideoStreamBuffer(100);
}

PreviewController::~PreviewController()
{
    deleteThreads();
    SDL_DestroyAudioStream(audioStream);
    delete vBuffer;
}

void PreviewController::onPlayBtnClicked()
{
    if (paused) {
        paused = false;
        emit resumePlaying();
    }
    else {
        paused = true;
        emit suspendPlaying();
    }
    
}


void PreviewController::deleteThreads()
{
    if(audioWorker)
        audioWorker->interrupt();

    if(videoDecodeWorker)
        videoDecodeWorker->interrupt();
    
    if(videoDisplayWorker)
        videoDisplayWorker->interrupt();

    vBuffer->interrupt();

    if (audioThread) {
        audioThread->quit();
        audioThread->wait();
        delete audioThread;
    }
    if (videoDecodeThread) {
        videoDecodeThread->quit();
        videoDecodeThread->wait();
        delete videoDecodeThread;
    }
    if (videoDisplayThread) {
        videoDisplayThread->quit();
        videoDisplayThread->wait();
        delete videoDisplayThread;
    }

    if (audioWorker) {
        audioWorker->deleteLater();
    }
    if (videoDecodeWorker) {
        videoDecodeWorker->deleteLater();
    }
    if (videoDisplayWorker) {
        videoDisplayWorker->deleteLater();
    }
}

void PreviewController::playVideo(const QString& filePath)
{
    SDL_PauseAudioStreamDevice(audioStream);
    paused = false;
    deleteThreads();
    
    
    SDL_ClearAudioStream(audioStream);
    vBuffer->clear();


    audioThread = new QThread();
    videoDecodeThread = new QThread();
    videoDisplayThread = new QThread();

    audioWorker = new AudioDecodeWorker(filePath, audioStream, 96000);
    audioWorker->moveToThread(audioThread);
    connect(audioThread, &QThread::started, audioWorker, &AudioDecodeWorker::startDecoding);
    connect(audioWorker, &AudioDecodeWorker::finished, audioThread, &QThread::quit);
    connect(audioWorker, &AudioDecodeWorker::finished, audioWorker, &AudioDecodeWorker::deleteLater);
    connect(audioThread, &QThread::finished, audioThread, &QThread::deleteLater);
    connect(this, &PreviewController::suspendPlaying, audioWorker, &AudioDecodeWorker::pause);
    connect(this, &PreviewController::resumePlaying, audioWorker, &AudioDecodeWorker::resume);

    videoDecodeWorker = new VideoDecodeWorker(filePath, vBuffer);
    videoDecodeWorker->moveToThread(videoDecodeThread);
    connect(videoDecodeThread, &QThread::started, videoDecodeWorker, &VideoDecodeWorker::startDecoding);
    connect(videoDecodeWorker, &VideoDecodeWorker::finished, videoDecodeThread, &QThread::quit);
    connect(videoDecodeWorker, &VideoDecodeWorker::finished, videoDecodeWorker, &VideoDecodeWorker::deleteLater);
    connect(videoDecodeThread, &QThread::finished, videoDecodeThread, &QThread::deleteLater);

    videoDisplayWorker = new VideoDisplayWorker(vBuffer, audioStream, audioWorker);
    videoDisplayWorker->moveToThread(videoDisplayThread);
    connect(videoDisplayThread, &QThread::started, videoDisplayWorker, &VideoDisplayWorker::startDisplaying);
    connect(videoDisplayWorker, &VideoDisplayWorker::finished, videoDisplayThread, &QThread::quit);
    connect(videoDisplayWorker, &VideoDisplayWorker::finished, videoDisplayWorker, &VideoDisplayWorker::deleteLater);
    connect(videoDisplayThread, &QThread::finished, videoDisplayThread, &QThread::deleteLater);
    connect(videoDisplayWorker, &VideoDisplayWorker::displayFrame, previewView, &PreviewView::drawFrame);

    videoDecodeThread->start();
    audioThread->start();
    videoDisplayThread->start();

    SDL_ResumeAudioStreamDevice(audioStream);
}
