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

public slots:
	void onMovingClipRequested(ClipItem* clipItem, QPointF newPos);
	void onClipItemMoved(ClipItem* item, double newPos);
	void onClipSelectRequested(ClipData* data);
	void onNewClipFromDrag(const QMimeData* mimeData);

private:
	TlView* tlView;
	TlModel* model;


	std::pair<QPointF, int> snapToTracks(const QPointF& pos);
};

