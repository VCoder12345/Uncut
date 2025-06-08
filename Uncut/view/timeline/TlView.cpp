#include "TlView.h"

#include <QMimeData>

#include "TrackItem.h"
#include <QMouseEvent>

TlView::TlView(QWidget* parent)
	: QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);
	setScene(scene);

	setAlignment(Qt::AlignLeft | Qt::AlignCenter);
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
	connect(model, &TlModel::lastTrackRemoved, this, &TlView::onLastTrackRemoved);
	connect(model, &TlModel::clipsSelected, this, &TlView::onClipsSelected);
	connect(model, &TlModel::clipMoved, this, &TlView::updateClipPositions);
}

ClipItem* TlView::addClipToScene(ClipData* data, int trackIndex)
{
	TrackItem* track = tracks[trackIndex];
	ClipItem* clipItem = new ClipItem(data, wps, track->getHeight());

	scene->addItem(clipItem);
	updateClipPos(clipItem);

	return clipItem;
}

void TlView::onClipAdded(ClipData* data, int trackIndex) {
	ClipItem* clipItem = addClipToScene(data, trackIndex);
	clips.push_back(clipItem);
}


void TlView::onClipsSelected()
{
	for (ClipItem* item : clips) {
		item->update();
	}
}

void TlView::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);
	updateTrackWidths();
}

void TlView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	updateTrackWidths();
}

void TlView::moveSelec(const QPoint& mousePos)
{
	QPointF newPos = mapToScene(mousePos) + QPointF(dragOffset.x(), 0);
	slcClipItem->setPos(snapToTracks(newPos).first);

}

void TlView::mouseMoveEvent(QMouseEvent* event)
{
	if (slcClipItem) {
		moveSelec(event->pos());
	}
	QGraphicsView::mouseMoveEvent(event);
}

void TlView::putSelec(bool clipExists)
{
	double newPos = slcClipItem->x() / wps;

	emit clipItemMoved(slcClipItem->data, newPos, snapToTracks(slcClipItem->pos()).second, clipExists);

	cleanupSelec();
}

void TlView::cleanupSelec()
{
	scene->removeItem(slcClipItem);
	delete slcClipItem;
	slcClipItem = nullptr;
}

void TlView::mousePressEvent(QMouseEvent* event)
{
	ClipItem* clipItem = dynamic_cast<ClipItem*>(itemAt(event->pos()));

	if (clipItem)
	{
		slcClipItem = clipItem->shallowCopy();
		slcClipItem->setOpacity(0.8);
		scene->addItem(slcClipItem);

		dragOffset = clipItem->pos() - mapToScene(event->pos());
		moveSelec(event->pos());
		emit requestClipSelect(clipItem->data);
	}
	QGraphicsView::mousePressEvent(event);
}

void TlView::mouseReleaseEvent(QMouseEvent* event)
{
	if (slcClipItem) {
		putSelec(true);
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void TlView::dragEnterEvent(QDragEnterEvent* event)
{
	QGraphicsView::dragEnterEvent(event);
	if (event->mimeData()->hasFormat("application/x-libitemdata"))
	{
		event->acceptProposedAction();
		QByteArray itemData = event->mimeData()->data("application/x-libitemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QString filePath;
		dataStream >> filePath;

		double clipPos = event->position().x() / wps;
		ClipData* data = new ClipData(nullptr, filePath, 0, 2, clipPos);
		slcClipItem = new ClipItem(data, wps, defaultTrackHeight, 0.8);
		scene->addItem(slcClipItem);
		slcClipItem->setPos(event->position());
		dragOffset = QPointF(0, 0);
		moveSelec(event->position().toPoint());
	}
	else
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
		moveSelec(event->position().toPoint());
	}
	else
	{
		event->ignore();
	}
}

void TlView::dropEvent(QDropEvent* event)
{
	QGraphicsView::dropEvent(event);
	event->acceptProposedAction();
	putSelec(false);
}

void TlView::dragLeaveEvent(QDragLeaveEvent* event)
{
	QGraphicsView::dragLeaveEvent(event);

	cleanupSelec();
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
	qreal y;
	if (tracks.size() > 0)
	{
		TrackItem* lastTrack = tracks.back();
		y = lastTrack->getY() - defaultTrackHeight;
	}
	else
	{
		y = 0;
	}
	TrackItem* trackItem = new TrackItem(data, y, defaultTrackHeight);
	tracks.push_back(trackItem);
	scene->addItem(trackItem);
	trackItem->setPos(0, y);

	updateTrackWidths();
}

void TlView::onLastTrackRemoved()
{
	TrackItem* lastTrackItem = tracks.back();
	scene->removeItem(lastTrackItem);
	tracks.pop_back();
	delete lastTrackItem;
}

TrackItem* TlView::lastTrack()
{
	if (tracks.size() == 0) return nullptr;
	return tracks[tracks.size() - 1];
}

void TlView::updateClipPos(ClipItem* clipItem)
{
	clipItem->setPos(wps * clipItem->data->pos, tracks[clipItem->data->trackIndex]->getY());
}

void TlView::updateClipPositions()
{
	for (ClipItem* clipItem : clips) {
		updateClipPos(clipItem);
	}
}

std::pair<QPointF, int> TlView::snapToTracks(const QPointF& pos)
{
	qreal py = pos.y();
	int track = -1;
	for (int i = 0; i < tracks.size(); ++i) {
		qreal y = tracks[i]->getY();
		if (py >= y && py < y + tracks[i]->getHeight()) {
			py = y;
			track = i;
			break;
		}
	}

	if (track < 0) {
		py = tracks.back()->getY() - defaultTrackHeight;
		track = tracks.size();
	}

	return { QPointF(pos.x(), py), track };
}
