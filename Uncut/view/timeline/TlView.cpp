#include "TlView.h"

#include <QMimeData>

#include "TrackItem.h"
#include <QMouseEvent>

TlView::TlView(QWidget *parent)
	: QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);
	setScene(scene);

	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setAcceptDrops(true);
} 

TlView::~TlView()
{
}

void TlView::setModel(TlModel* model)
{
	this->model = model;
	
	connect(model, &TlModel::clipAdded, this, &TlView::onClipAdded);
	connect(model, &TlModel::trackAdded, this, &TlView::addTrack);
	connect(model, &TlModel::clipsSelected, this, &TlView::onClipsSelected);
	connect(model, &TlModel::clipMoved, this, &TlView::updateClipPositions);
}

void TlView::onClipAdded(ClipData* data, int trackIndex) {
	TrackItem* track = tracks[trackIndex];
	ClipItem* clipItem = new ClipItem(data, wps, track->getClipHeight());
	clips.push_back(clipItem);

	scene->addItem(clipItem);
	updateClipPositions();
}


void TlView::onClipsSelected()
{
	for (ClipItem* item : clips) {
		item->update();
	}
}

void TlView::resizeEvent(QResizeEvent * event)
{
	QGraphicsView::resizeEvent(event);
	updateTrackWidths();
}

void TlView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	updateTrackWidths();
}

void TlView::mouseMoveEvent(QMouseEvent* event)
{
	if (slcClipItem) {
		QPointF newPos =  mapToScene(event->pos()) + QPointF(dragOffset.x(), 0);
		emit requestMovingClip(slcClipItem, newPos);
	}
	QGraphicsView::mouseMoveEvent(event);
}

void TlView::mousePressEvent(QMouseEvent* event)
{
	ClipItem* clipItem = dynamic_cast<ClipItem*>(itemAt(event->pos()));

	if (clipItem)
	{
		slcClipItem = clipItem->shallowCopy();
		slcClipItem->setOpacity(0.5);
		scene->addItem(slcClipItem);

		dragOffset = clipItem->pos() - mapToScene(event->pos());
		emit requestClipSelect(clipItem->data);
	}
	QGraphicsView::mousePressEvent(event);
}

void TlView::mouseReleaseEvent(QMouseEvent* event)
{
	if (slcClipItem) {
		double newPos = slcClipItem->x() / (double) wps;

		emit clipItemMoved(slcClipItem, newPos);

		scene->removeItem(slcClipItem);
		delete slcClipItem;
		slcClipItem = nullptr;
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void TlView::dragEnterEvent(QDragEnterEvent* event)
{
	QGraphicsView::dragEnterEvent(event);
	qDebug() << "hullo";
	if (event->mimeData()->hasFormat("application/x-libitemdata"))
	{
		event->acceptProposedAction();
		emit newClipFromDrag(event->mimeData());
	} else
	{
		event->ignore();
	}
}

void TlView::dragMoveEvent(QDragMoveEvent* event)
{
	QGraphicsView::dragMoveEvent(event);
	if (event->mimeData()->hasFormat("application/x-libitemdata"))
	{
		event->acceptProposedAction();
		emit newClipFromDrag(event->mimeData());
	} else
	{
		event->ignore();
	}
}

void TlView::dropEvent(QDropEvent* event)
{
	QGraphicsView::dropEvent(event);
	event->acceptProposedAction();
}

void TlView::updateTrackWidths()
{
	QRectF visibleRegion = mapToScene(viewport()->rect()).boundingRect();
	for (TrackItem* track : tracks) {
		track->updateWidth(visibleRegion.left(), visibleRegion.right());
	}
}

void TlView::addTrack(TrackData* data)
{
	TrackItem* trackItem = new TrackItem(data);
	tracks.push_back(trackItem);
	scene->addItem(trackItem);
	trackItem->setPos(0, data->getY());

	updateTrackWidths();
}

TrackItem* TlView::lastTrack()
{
	if (tracks.size() == 0) return nullptr;
	return tracks[tracks.size() - 1];
}

void TlView::updateClipPositions()
{
	for (ClipItem* clipItem : clips) {
		clipItem->setPos(wps * clipItem->data->pos, tracks[clipItem->data->trackIndex]->getY());
	}
}
