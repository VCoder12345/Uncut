#include "TlController.h"

TlController::TlController(Uncut& window, TlModel* model) : model(model), tlView(window.getTlView())
{
	tlView->setModel(model);
	
	model->addTrack();
	model->addTrack();
	model->addTrack();

	model->addClip(new ClipData(nullptr, "", 0, 5.0, 0.0), 1);
	model->addClip(new ClipData(nullptr, "", 0, 2.0, 0.0), 2);
	model->addClip(new ClipData(nullptr, "", 0, 1.0, 0.0), 0);
	model->addClip(new ClipData(nullptr, "", 0, 4.0, 0.0), 2);

	connect(tlView, &TlView::requestMovingClip, this, &TlController::onMovingClipRequested);
	connect(tlView, &TlView::clipItemMoved, this, &TlController::onClipItemMoved);
	connect(tlView, &TlView::requestClipSelect, this, &TlController::onClipSelectRequested);
	connect(tlView, &TlView::newClipFromDrag, this, &TlController::onNewClipFromDrag);
}




void TlController::onMovingClipRequested(ClipItem* clipItem, QPointF newPos)
{
	clipItem->setPos(snapToTracks(newPos).first);
}

void TlController::onClipItemMoved(ClipItem* item, double newPos)
{
	model->moveClip(item->data, newPos, snapToTracks(item->pos()).second);
}



void TlController::onClipSelectRequested(ClipData* data)
{
	model->select(data);
}

void TlController::onNewClipFromDrag(const QMimeData* mimeData)
{
	QByteArray itemData = mimeData->data("application/x-libitemdata");
	QDataStream dataStream(&itemData, QIODevice::ReadOnly);

	QString filePath;
	dataStream >> filePath;

	
}

std::pair<QPointF, int> TlController::snapToTracks(const QPointF& pos)
{
	qreal py = pos.y();
	int track = -1;
	for (int i = 0; i < model->tracks.size(); ++i) {
		qreal y = model->tracks[i]->getY();
		if (py >= y && py < y + model->tracks[i]->getHeight()) {
			py = y;
			track = i;
			break;
		}
	}

	return { QPointF(pos.x(), py), track };
}


