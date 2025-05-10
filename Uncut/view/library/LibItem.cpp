#include "LibItem.h"


LibItem::LibItem(const LibItemData* data, int width, int height) : data(data), width(width), height(height)
{
}

LibItem::~LibItem()
{}

QRectF LibItem::boundingRect() const {
	return QRectF(0, 0, width, height);
}

void LibItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->drawImage(boundingRect(), data->previewImg);
}

void LibItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	QGraphicsItem::mousePressEvent(event);
	update();
}
