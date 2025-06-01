#pragma once
#include <model/timeline/TlModel.h>
#include <view/Uncut.h>
#include <view/timeline/TlView.h>
#include <QObject>

class TlController : public QObject
{
	Q_OBJECT
public:
	TlController(Uncut& window, TlModel* model);

public slots:
	void onMouseMoved(QMouseEvent* event);
	void onMousePressed(QMouseEvent* event);
	void onMouseReleased(QMouseEvent* event);

private:
	TlView* tlView;
	TlModel* model;
	ClipItem* slcClipItem = nullptr;
	QPointF	dragOffset;
	int slcTrack;


	QPointF snapToTracks(const QPointF& pos);
};

