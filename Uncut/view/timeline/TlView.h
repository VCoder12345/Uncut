#pragma once

#include <QGraphicsView>
#include "ClipItem.h"
#include "TrackItem.h"
#include <model/timeline/TrackData.h>
#include <model/timeline/TlModel.h>
#include <QMimeData>

class TlView  : public QGraphicsView
{
	Q_OBJECT

public:
	int wps = 100;
	QGraphicsScene* scene;
	qreal defaultTrackHeight = 100;


	TlView(QWidget *parent);
	~TlView();

	void setModel(TlModel* model);

signals:
	void mouseMoved(QMouseEvent* event);
	void mousePressed(QMouseEvent* event);
	void mouseReleased(QMouseEvent* event); 
	void clipItemMoved(ClipData* data, double newPos, int newTrack);
	void requestClipSelect(ClipData* data);
	void newClipFromDrag(const QMimeData* mimeData);

public slots:
	void onClipAdded(ClipData* data, int trackIndex);
	void onClipsSelected();


protected:
	void resizeEvent(QResizeEvent* event) override;
	void scrollContentsBy(int dx, int dy) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	std::vector<ClipItem*> clips;
	std::vector<TrackItem*> tracks;
	TlModel* model = nullptr;
	ClipItem* slcClipItem = nullptr;
	QPointF	dragOffset;


	void updateTrackWidths();
	void addTrack(TrackData* data);
	TrackItem* lastTrack();
	void updateClipPositions();
	std::pair<QPointF, int> snapToTracks(const QPointF& pos);
};
