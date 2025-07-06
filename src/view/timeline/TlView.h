#pragma once

#include <QGraphicsView>
#include "ClipItem.h"
#include "TrackItem.h"
#include <model/timeline/TrackData.h>
#include <model/timeline/TlModel.h>
#include <QMimeData>

#include "services/exchange/ExchangeService.h"

class TlView  : public QGraphicsView
{
	Q_OBJECT

public:
	double wps = 100;
	QGraphicsScene* scene;
	qreal defaultTrackHeight = 100;
	ExchangeService exchangeService;


	TlView(QWidget *parent);
	~TlView();

	void setModel(TlModel* model);
	ClipItem* addClipToScene(ClipData* data, int trackIndex);

signals:
	void mouseMoved(QMouseEvent* event);
	void mousePressed(QMouseEvent* event);
	void mouseReleased(QMouseEvent* event); 
	void clipItemMoved(ClipData* data, double newPos, int newTrack, bool clipExists);
	void requestClipSelect(ClipData* data);
	void newClipFromDrag(QDragEnterEvent* event);
	void clipImported(ClipData* data, int trackIndex);
	void deleteKeyPressed();

public slots:
	void onClipAdded(ClipData* data, int trackIndex);
	void onClipsSelected();
	void addTrack(TrackData* data);
	void onLastTrackRemoved();
	void onClipRemoved(ClipData* data);


protected:
	void resizeEvent(QResizeEvent* event) override;
	void scrollContentsBy(int dx, int dy) override;
	void moveSelec(const QPoint& mousePos);
	void mouseMoveEvent(QMouseEvent* event) override;
	void putSelec(bool clipExists);
	void cleanupSelec();
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	void dragLeaveEvent(QDragLeaveEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

private:
	QHash<ClipData*, ClipItem*> clips;
	std::vector<TrackItem*> tracks;
	TlModel* model = nullptr;
	ClipItem* slcClipItem = nullptr;
	QPointF	dragOffset;


	void updateTrackWidths();
	TrackItem* lastTrack();
	void updateClipPos(ClipItem* clipItem);
	void updateClipPositions();
	std::pair<QPointF, int> snapToTracks(const QPointF& pos);
};
