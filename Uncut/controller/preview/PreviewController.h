#pragma once
#include <QObject>
#include <model/library/LibModel.h>
#include <view/preview/PreviewView.h>
#include "services/decoder/AudioDecodeWorker.h"
#include "services/decoder/VideoDecodeWorker.h"
#include "services/decoder/VideoDisplayWorker.h"
#include <QThread>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <view/Uncut.h>
#include <model/library/LibModel.h>

class PreviewController : public QObject
{
	Q_OBJECT

public:
	PreviewController(Uncut& window, LibModel* libModel);

	~PreviewController();
	
public slots:
	void onLibItemSelected(int oldSelected, int selected, const LibItemData* data);
	void onPlayBtnClicked();

signals:
	void suspendPlaying();
	void resumePlaying();

private:
	void deleteThreads();
	void playVideo(const QString& filePath);
	bool paused = true;
	PreviewView* previewView;
	QThread* audioThread = nullptr;
	QThread* videoDecodeThread = nullptr;
	QThread* videoDisplayThread = nullptr;
	AudioDecodeWorker* audioWorker = nullptr;
	VideoDecodeWorker* videoDecodeWorker = nullptr;
	VideoDisplayWorker* videoDisplayWorker = nullptr;
	SDL_AudioStream* audioStream = nullptr;
	VideoStreamBuffer* vBuffer = nullptr;
};

