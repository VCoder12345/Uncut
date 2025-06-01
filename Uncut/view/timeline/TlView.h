#pragma once

#include <QGraphicsView>
#include "ClipItem.h"
#include "TrackItem.h"
#include <model/timeline/TrackData.h>
#include <model/timeline/TlModel.h>

class TlView  : public QGraphicsView
{
	Q_OBJECT

public:
	int wps = 100;
	QGraphicsScene* scene;

	TlView(QWidget *parent);
	~TlView();

	void setModel(TlModel* model);

signals:
	void mouseMoved(QMouseEvent* event);
	void mousePressed(QMouseEvent* event);
	void mouseReleased(QMouseEvent* event); 

public slots:
	void onClipAdded(ClipData* data, int trackIndex);
	void onClipsSelected();

protected:
	void resizeEvent(QResizeEvent* event) override;
	void scrollContentsBy(int dx, int dy) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	std::vector<ClipItem*> clips;
	std::vector<TrackItem*> tracks;
	TlModel* model = nullptr;

	void updateTrackWidths();
	void addTrack(TrackData* data);
	TrackItem* lastTrack();
	void updateClipPositions();

};
