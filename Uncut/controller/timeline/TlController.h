#pragma once
#include <model/timeline/TlModel.h>
#include <view/Uncut.h>
#include <view/timeline/TlView.h>
#include <QObject>
#include <QMimeData>

class TlController : public QObject
{
	Q_OBJECT
public:
	TlController(Uncut& window, TlModel* model);
	void removeUnusedTracks();

public slots:
	void onClipItemMoved(ClipData* data, double newPos, int newTrack, bool clipExists);
	void onClipSelectRequested(ClipData* data);
	void onDeleteKeyPressed();

private:
	TlView* tlView;
	TlModel* model;
};

