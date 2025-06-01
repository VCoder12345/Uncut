#include "ClipItem.h"
#include <QPainter>

ClipItem::ClipItem(ClipData* data, int wps, int height, double opacity) : data(data), wps(wps), height(height), opacity(opacity)
{
	setZValue(5);
}

ClipItem::~ClipItem()
{}

QRectF ClipItem::boundingRect() const {
	double duration = data->duration();
	return QRectF(0, 0, duration * wps, height);
}

void ClipItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->setOpacity(opacity);
	painter->fillRect(boundingRect(), QBrush(Qt::blue));


	if (data->selected) {
		painter->setPen(QPen(Qt::red));
		painter->drawRect(boundingRect());
	}
}
