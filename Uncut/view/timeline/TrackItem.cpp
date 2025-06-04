#include "TrackItem.h"
#include <QPainter>

TrackItem::TrackItem(TrackData* data, qreal y, qreal height) : QGraphicsRectItem(), data(data), y(y), height(height) {}

TrackItem::~TrackItem()
{}

void TrackItem::updateWidth(qreal visibleLeft, qreal visibleRight)
{
	setRect(visibleLeft, 0, visibleRight - visibleLeft, height);
}

qreal TrackItem::getHeight()
{
	return height;
}

qreal TrackItem::getY() {
	return y;
}


