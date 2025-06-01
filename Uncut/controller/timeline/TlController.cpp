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

	connect(tlView, &TlView::mouseMoved, this, &TlController::onMouseMoved);
	connect(tlView, &TlView::mousePressed, this, &TlController::onMousePressed);
	connect(tlView, &TlView::mouseReleased, this, &TlController::onMouseReleased);
}

void TlController::onMousePressed(QMouseEvent* event)
{
	ClipItem* clipItem = dynamic_cast<ClipItem*>(tlView->itemAt(event->pos()));
	
	if (clipItem) {
		slcClipItem = clipItem->shallowCopy(); 
		slcClipItem->setOpacity(0.5);
		tlView->scene->addItem(slcClipItem);

		dragOffset = clipItem->pos() - tlView->mapToScene(event->pos());
		slcTrack = clipItem->data->trackIndex;
		model->select(clipItem->data);
	}
	
}

void TlController::onMouseReleased(QMouseEvent* event)
{
	if (slcClipItem) {
		double newPos = slcClipItem->x() / (double)tlView->wps;
		model->moveClip(slcClipItem->data, newPos, slcTrack);

		qDebug() << slcClipItem->data->pos << slcClipItem->data->trackIndex;
		tlView->scene->removeItem(slcClipItem);
		delete slcClipItem;
		slcClipItem = nullptr;
	}
}

QPointF TlController::snapToTracks(const QPointF& pos)
{
	qreal py = pos.y();
	for (int i = 0; i < model->tracks.size(); ++i) {
		qreal y = model->tracks[i]->getY();
		if (py >= y && py < y + model->tracks[i]->getHeight()) {
			py = y;
			slcTrack = i;
			break;
		}
	}

	return QPointF(pos.x(), py);
}

void TlController::onMouseMoved(QMouseEvent* event) {
	if (slcClipItem) {
		QPointF newPos =  tlView->mapToScene(event->pos()) + QPointF(dragOffset.x(), 0);
		slcClipItem->setPos(snapToTracks(newPos));
	}
}

